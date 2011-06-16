

#include "resources.h"










// This function gets the first power of 2 >= the
//  int that we pass it.
inline int next_power_of_2 ( int a )
{
	int rval=2;
	while(rval<a) rval<<=1;
	return rval;
}


void freetype_genFontTexture_ascii(FT_Face face, c_font *gameFont, unsigned int pixel_height)
{

    const unsigned int glyphsToRender = 256;    // ASCII set, change at your peril
    GLubyte *pixel_data;

    unsigned int glyph_pixel_width = 0;
    unsigned int glyph_pixel_height = 0;

    unsigned int texture_width = 0;
    unsigned int texture_height = 0;

    FT_Glyph        glyphArray[glyphsToRender];
    FT_BitmapGlyph  bitmapGlyphArray[glyphsToRender];

    unsigned int lv;    // loop variable


    // first loop through glyphs to determine how big the bigest one is.
    for (lv=0 ; lv<glyphsToRender ; lv++)
    {
        int error;

        error = FT_Load_Glyph(face, FT_Get_Char_Index( face, lv), FT_LOAD_DEFAULT);
        if (error)
        {
            printf("FT_Load_Glyph [%u]  ", error);
        }
        error = FT_Get_Glyph( face->glyph, &glyphArray[lv] );
        if (error)
        {
            printf("FT_Get_Glyph [%u]  ", error);
        }

        FT_Glyph_To_Bitmap( &glyphArray[lv], ft_render_mode_normal, 0, 1 );

        bitmapGlyphArray[lv] = (FT_BitmapGlyph)glyphArray[lv];
        FT_Bitmap& bitmap = bitmapGlyphArray[lv]->bitmap;

        if ((unsigned int)bitmap.rows > glyph_pixel_height) glyph_pixel_height = bitmap.rows;
        if ((unsigned int)bitmap.width > glyph_pixel_width) glyph_pixel_width = bitmap.width;
    }

    // make sure each glyph is a power of 2
    // not 100% necessary, but if we want each glyph to have an equal percentage
    //  of texture space we better do it.
    glyph_pixel_width = next_power_of_2(glyph_pixel_width);
    glyph_pixel_height = next_power_of_2(glyph_pixel_height);


    // now all glyph specs are pretty much finalized (so lets brag about it)
    printf(" character size: %ux%u, ", glyph_pixel_width, glyph_pixel_height);
    texture_width = glyph_pixel_width * 16;
    texture_height = glyph_pixel_height * 16;
    printf("ascii texture size: %ux%u\n", texture_width, texture_height);

    // make some pixel data in ram (this will be the font texture)
    unsigned int bytesForImage = 2 * texture_width * texture_height;
    pixel_data = new GLubyte[bytesForImage];

    // zero out all the newly allocated data
    for (unsigned int clearloop=0 ; clearloop < bytesForImage ; clearloop++ )
    {
        pixel_data[clearloop] = 0;
    }

    unsigned int glyphX_start = 0;
    unsigned int glyphY_start = 0;

    // now to go through each glyph again
    for (lv=0 ; lv<glyphsToRender ; lv++ )
    {
        FT_Bitmap& bitmap = bitmapGlyphArray[lv]->bitmap;

        glyphX_start = lv % 16;
        glyphY_start = lv / 16;

        glyphX_start *= glyph_pixel_width;
        glyphY_start *= glyph_pixel_height;


        // draw glyph out in the correct spot on the 16x16 glyph texture
        for(unsigned int gx=0; gx< (unsigned int)bitmap.width; gx++)
        {
            for(unsigned int gy=0; gy< (unsigned int)bitmap.rows ; gy++)
            {
                unsigned char *byte_luminence;
                unsigned char *byte_alpha;

                // calculate which pixels to work on
                unsigned int pixel_mem_loc =
                    2 * (        // channels: luminence & alpha
                    texture_width *
                    (glyphY_start + gy) +
                    glyphX_start + gx
                    );

                byte_luminence = &pixel_data[  pixel_mem_loc  ];
                byte_alpha = &pixel_data[ pixel_mem_loc + 1 ];

                *byte_luminence = 255;      // or this if you'd rather do this to blacken the alpha edges:  bitmap.buffer[gx + bitmap.width * gy];
                *byte_alpha = bitmap.buffer[gx + bitmap.width * gy];
            }
        }


        // pixel coordinates for glyph
        unsigned int top = (lv / 16) * glyph_pixel_height;
        unsigned int left = (lv % 16) * glyph_pixel_width;
        unsigned int bottom = top + bitmap.rows;
        unsigned int right = left + bitmap.width;

        // texture coordinates for glyph
        float uv_top = (float)top / texture_height;
        float uv_left = (float)left / texture_width;
        float uv_bottom = (float)bottom / texture_height;
        float uv_right = (float)right / texture_width;


        // seems some letters get cut too short to see their nice anti-aliasing - should probably do this when rendering
        //  but then again it's probably a single pixel of low alpha that's getting cut so it may be best to do a single
        //  pixel pad here, on the left and right sides.
        uv_top -= 1 * (1.0f / texture_height);
        uv_left -= 1 * (1.0f / texture_width);
        uv_bottom += 1 * (1.0f / texture_height);
        uv_right += 1 * (1.0f / texture_width);


        //////////////////////////////////////////////////////////////////
        // actually assign this data to the class
        gameFont->glyph[lv].set_uv(pixel_height, texture_width, texture_height, uv_top, uv_left, uv_bottom, uv_right);


        // figure out how much to lower characters that hang below the line
        float shift_y = (float)bitmap.rows - (float)bitmapGlyphArray[lv]->top;
        shift_y /= texture_height;

        gameFont->glyph[lv].set_shift(pixel_height, texture_height, shift_y);


        // set the advance - not used yet
        //face->glyph->advance;
        float advance = glyphArray[lv]->advance.x;
        gameFont->glyph[lv].set_advance(pixel_height, texture_width, advance);


        // test to see if glyph uv coordinates are what they should be
        /*if (lv == (unsigned int)'g')
        {
            printf("%c - uv(t=%f, l=%f, b=%f, r=%f) - shift(y=%f) - advance(x=%f)\n",
                (char)lv,
                gameFont->glyph[lv].uv_top(),
                gameFont->glyph[lv].uv_left(),
                gameFont->glyph[lv].uv_bottom(),
                gameFont->glyph[lv].uv_right(),
                gameFont->glyph[lv].shift_y(),
                gameFont->glyph[lv].advance_x());

        }*/
    }




    glGenTextures(1, &gameFont->opengl_texture_id);
    glBindTexture( GL_TEXTURE_2D, gameFont->opengl_texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);



    //Here we actually create the texture itself, notice
    //that we are using GL_LUMINANCE_ALPHA to indicate that
    //we are using 2 channel data.

    //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pixel_data );

    // do that line above - or build some mipmaps for trilinear filtering
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, texture_width, texture_height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pixel_data );


    //printf("texture specs: %ux%u\n", wsf, height);

    // now that all of that the image has been loading into the
    //  graphics card, we can free up the copy we built in the ram
    delete pixel_data;
}


// this file includes freetype initialization routines.
// putting them into a seperate function would be best for
//  multiple font programs
// note: i get some strange results when using really low
//  pixel heights with true type fonts
void load_font (const char *fontFileName, c_font *gameFont, unsigned int pixel_height)
{
    printf("Loading Font: %s\n", fontFileName);


    // Initialize FreeType
    int error;
    FT_Library  library;
    FT_Face     face;

    error = FT_Init_FreeType( &library );
    if (error)
    {
        printf(" freetype: library problem=%u\n", error);
    }

    // load a font with FreeType
    error = FT_New_Face (library, fontFileName,0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        printf(" freetype: unknown file format\n");
    }
    else if (error)
    {
        printf(" freetype: face problem=%u\n", error);
    }


    // set how many pixels to use
    FT_Set_Char_Size( face, pixel_height*64, pixel_height*64, 96, 96);

    //gameFont->pixelHeight = pixel_height;
    //gameFont->sizeCorrection = 1.5f;

    // start making a texture
    // maybe later do something more (if i understand unicode)
    freetype_genFontTexture_ascii(face, gameFont, pixel_height);



    // freetype cleanup
    FT_Done_Face(face);
    FT_Done_FreeType(library);

}























