




// class difinition for fonts
class c_font_glyph
{


private:
float _uv_top;
float _uv_left;
float _uv_bottom;
float _uv_right;

float _size_x;
float _size_y;
float _shift_y;      // how much to drop lower-case g's and other such glyphs
float _advance_x;    // this is similar to _size_x, but cursive fonts need to overlap so this should be used







// use of private member data is mostly just to make the variables mostly read-only
public:
float uv_top()      { return _uv_top; }
float uv_left()     { return _uv_left; }
float uv_bottom()   { return _uv_bottom; }
float uv_right()    { return _uv_right; }
float size_x()  { return _size_x; }
float size_y()  { return _size_y; }
float shift_y()     { return _shift_y; }
float advance_x()     { return _advance_x; }

void set_uv(unsigned int pixel_height, unsigned int texture_width, unsigned int texture_height, float top, float left, float bottom, float right)
{
    _uv_top = top;
    _uv_left = left;
    _uv_bottom = bottom;
    _uv_right = right;

    ////////////////////////////////////////////////////////////////
    // from all that we should be able to compute the glyph size
    // but wait - this is screwy because larger pixel heights will
    // increase the size of the glyphs in the texture without
    // proportionally increasing the size of the texture which is
    // always a power of 2 and always 1.0x1.0 in floats.
    // so we correct for size down below.
    _size_x = _uv_right - _uv_left;
    _size_y = _uv_bottom - _uv_top;

    ////////////////////////////////////////////////////////////////
    // size correction
    // because several pixel heights will fit in the same font
    // texture dimensions we will need to be able to query the pixel
    // height to correct seemingly eratic font sizes.  in theory one
    // should always render a font at the highest pixel size
    // possible for a given texture size - 'cause it means higher
    // resolution fonts with no memory overhead.
    float scx = (float)texture_width/(float)pixel_height;
    float scy = (float)texture_height/(float)pixel_height;
    _size_x *= scx;
    _size_y *= scy;
}

// can't get this to work properly - no idea what freetype is doing here
// doesn't look like it's worth the effort to figure out for a few reasons
// 1. manually calculating advance based on the glyph's size works and, imo, looks better
// 2. the proper way to calculate this is with kerning pairs.  where the advance is
//    set for any given pair of glyphs: "rt" might have a different advance than "rl"
void set_advance(unsigned int pixel_height, unsigned int texture_width, float advance_x)
{
    float scx = (float)texture_width/(float)pixel_height;

    _advance_x = advance_x / 64.0f;     // fractional pixel correction
    _advance_x *= scx;

    _advance_x /= 1500.0f;          // this is sorta close, but i have no idea why

}


void set_shift(unsigned int pixel_height, unsigned int texture_height, float y)
{
    float scy = (float)texture_height/(float)pixel_height;
    _shift_y = y * scy;
}

c_font_glyph()
{
    _uv_top = 0.0f;
    _uv_left = 0.0f;
    _uv_bottom = 0.0f;
    _uv_top = 0.0f;
    _shift_y = 0.0f;
}

};

class c_font
{
public:
char fontName[80];
char fontTexture[80];
unsigned int opengl_texture_id;


c_font_glyph glyph[256];

c_font()
{
    fontName[0] = '\x00';
    fontTexture[0] = '\x00';
    opengl_texture_id = 0;
}

};


