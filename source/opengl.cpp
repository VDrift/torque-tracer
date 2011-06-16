#include "resources.h"



//////////////////////////////////////////////////////////////////////////////////////////
//
// character printing
//
void drawCharacter(char ch, float scale)
{
    float depth = 0;
    c_font *f = &font[0];
    c_font_glyph *glyph = &f->glyph[(unsigned int)ch];



    //float sc = f->sizeCorrection;

    //glColor3f( 1.0f, 1.0f, 1.0f );
    glBindTexture(GL_TEXTURE_2D, font[0].opengl_texture_id);

    glBegin( GL_QUADS );
        glTexCoord2f( glyph->uv_left(), glyph->uv_bottom() );
        glVertex3f(
            0,
            0-glyph->shift_y() * scale,
            depth );

        glTexCoord2f( glyph->uv_right(), glyph->uv_bottom() );
        glVertex3f(
            glyph->size_x() * scale,
            0-glyph->shift_y() * scale,
            depth );

        glTexCoord2f( glyph->uv_right(), glyph->uv_top() );
        glVertex3f(
            glyph->size_x() * scale,
            (glyph->size_y()-glyph->shift_y()) * scale,
            depth);

        glTexCoord2f( glyph->uv_left(), glyph->uv_top() );
        glVertex3f(
            0,
            (glyph->size_y()-glyph->shift_y()) * scale,
            depth);
    glEnd();

}





void displayString(float xpos, float ypos, float scale, float rotation, string str)
{

    float adv = 0.0f;       // increment this based on characters printed

    //glTranslatef(-0.5f, -0.5f, 0.0f);
    glPushMatrix();
	glTranslatef(xpos, ypos, 0.0f);
	glRotatef(rotation, 0,0,1);


    for (unsigned int lv=0 ; lv<str.size() ; lv++)
    {
        char ch = str.at(lv);

        glTranslatef(adv, 0.0f, 0.0f);
        drawCharacter(ch, scale);

        adv = font[0].glyph[ (unsigned int)ch ].size_x()*scale + 0.005f;          // based on the calculated size of glyph

        if ( ch == '\x20' ) adv = 0.4f * scale + 0.005f;        // 'space' character has no size so arbitrarily set it to 0.4f

        //adv = font[0].glyph[ (unsigned int)ch ].advance_x()*scale + 0.005f;     // based on the advance parameter from freetype - currently broken

    }
    glPopMatrix();
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////













void OpenGL_drawMouse()
{
    float size = 0.05f;

    glBindTexture(GL_TEXTURE_2D, mouseTexture);
    //printf("bind: %u\n", mouseTexture);
    glColor3f( 1.0, 1.0, 1.0 );

    glTranslatef(mousePos.a[0], mousePos.a[1], 0);
    glBegin( GL_QUADS );

        glTexCoord2f(0,0);	glVertex3f( 0.0, 0.0, 0 );
        glTexCoord2f(0,1);	glVertex3f( 0.0, -size, 0 );
        glTexCoord2f(1,1);	glVertex3f( size, -size, 0);
        glTexCoord2f(1,0);	glVertex3f( size, 0.0, 0);
    glEnd();
}




void OpenGL_init()
{
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    float aspectRatio = (float)displayWidth / (float)displayHeight;

    glViewport( 0, 0, displayWidth, displayHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 90.0f, aspectRatio, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );


	//glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	textureLoad_sdl("mouse.png", &mouseTexture);
	textureLoad_sdl("curve.jpg", &torqueCurveTexture);
}


void OpenGL_drawPoint(c_vector2f point, float size)
{
	float hs = size * 0.5;

	glBegin(GL_QUADS);
		glVertex3f(point.a[0] - hs, point.a[1] + hs, 0);
		glVertex3f(point.a[0] - hs, point.a[1] - hs, 0);
		glVertex3f(point.a[0] + hs, point.a[1] - hs, 0);
		glVertex3f(point.a[0] + hs, point.a[1] + hs, 0);
	glEnd();
}




void OpenGL_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    glDisable(GL_DEPTH_TEST);

    glTranslatef(0,0,-1);

    /////////////////////////////////////////
    // background setup
    glPushMatrix();

    c_vector2f screenSize;
    screenSize.a[0] = 1.0 * displayAspectRatio;
    screenSize.a[1] = 1.0;

    //screenSize.a[0] *= 0.9;
    //screenSize.a[1] *= 0.9;


    /////////////////////////////////////////
    // field
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin( GL_QUADS );
        glColor3f( 0.2f, 0.2f, 0.2f );

        glVertex3f( screenSize.a[0], screenSize.a[1], 0 );
        glVertex3f( -screenSize.a[0], screenSize.a[1], 0 );

        glColor3f( 0.1f, 0.1f, 0.1f );

        glVertex3f( -screenSize.a[0], -screenSize.a[1], 0);
        glVertex3f( screenSize.a[0], -screenSize.a[1], 0 );
    glEnd();



	// image
	glBindTexture(GL_TEXTURE_2D, torqueCurveTexture);
    glColor3f( 0.8f, 0.8f, 0.8f );
    glBegin( GL_QUADS );
		glTexCoord2f(1.0f, 0.0f);		glVertex3f( pictureR, pictureT, 0 );			// rt
        glTexCoord2f(0.0f, 0.0f);		glVertex3f( pictureL, pictureT, 0 );			// lt
        glTexCoord2f(0.0f, 1.0f);		glVertex3f( pictureL, pictureB, 0);				// lb
        glTexCoord2f(1.0f, 1.0f);		glVertex3f( pictureR, pictureB, 0 );			// rb
    glEnd();



	glPushMatrix();
	//glLoadIdentity();


	glColor3f(1.0,1.0,1.0);


	if (inputMode == INPUT_MODE_RESIZE)
		displayString(-1.33f, 0.95f, 0.03f, 0, "(m)ode: resize picture");
	else if ( inputMode == INPUT_MODE_DRAW)
		displayString( -1.33f, 0.95f, 0.03f, 0, "(m)ode: draw curve");
	else if ( inputMode == INPUT_MODE_ENTER_TEXT)
	{
		displayString(-1.33f, 0.95f, 0.03f, 0, "(m)ode: input value");
		glColor3f(0.3f, 1.0f, 0.3f);
		displayString(-1.33f, 0.90f, 0.03f, 0, "type:");

		glColor3f(0.3f, 0.3f, 1.0f);
		displayString(-1.2f, 0.90f, 0.03f, 0, enteredText);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
		displayString(-1.33f, 0.95f, 0.03f, 0, "(m)ode: ???");

	if (torqueMode == TORQUEMODE_METRIC)
		displayString(-1.33f, 0.80f, 0.03f, 0, "(u)nits: metric");
	else
		displayString(-1.33f, 0.80f, 0.03f, 0, "(u)nits: imperial");

	displayString(-1.33f, 0.75f, 0.03f, 0, "torque range: =/-");



	displayString(-1.33f, 0.40f, 0.03f, 0, "(w)rite file");
	displayString(-1.33f, 0.35f, 0.03f, 0, "esc to quit ");






	float ts = 1.0f;			// torque scale


	float textScale = 0.03f;


	if (torqueMode == TORQUEMODE_METRIC)
	{

		glColor3f(0.2f, 1.0f, 0.2f);
		displayString(gridStartX-0.11, 0.2f, textScale, 90, "power (kW)");
		glColor3f(0.3f, 0.3f, 1.0f);
		displayString(gridStartX-0.11, -0.2f, textScale, 90, "torque (Nm)");
	}
	else			// foot pounds
	{

		glColor3f(0.2f, 1.0f, 0.2f);
		displayString(gridStartX-0.11, 0.2f, textScale, 90, "power (bhp)");
		glColor3f(0.3f, 0.3f, 1.0f);
		displayString(gridStartX-0.11, -0.2f, textScale, 90, "torque (ft-lbs)");
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	// torque numbers
	displayString(gridStartX-0.05, gridStartY-0.02, textScale, 0, "0");
	displayString(gridStartX-0.07, gridStartX-0.02+incrementY*1*ts, textScale, 0, "50");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*2*ts, textScale, 0, "100");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*3*ts, textScale, 0, "150");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*4*ts, textScale, 0, "200");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*5*ts, textScale, 0, "250");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*6*ts, textScale, 0, "300");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*7*ts, textScale, 0, "350");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*8*ts, textScale, 0, "400");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*9*ts, textScale, 0, "450");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*10*ts, textScale, 0, "500");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*11*ts, textScale, 0, "550");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*12*ts, textScale, 0, "600");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*13*ts, textScale, 0, "650");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*14*ts, textScale, 0, "700");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*15*ts, textScale, 0, "750");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*16*ts, textScale, 0, "800");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*17*ts, textScale, 0, "850");
	displayString(gridStartX-0.09, gridStartX-0.02+incrementY*18*ts, textScale, 0, "900");
	displayString(gridStartX-0.115, gridStartX-0.02+incrementY*19*ts, textScale, 0, "1000");

	// rpm numbers
	displayString(gridStartX-0.01, gridStartY-0.05, textScale, 0, "0");
	displayString(gridStartX-0.01 + incrementX*1,	gridStartY-0.05, textScale, -60, "1000");
	displayString(gridStartX-0.01 + incrementX*2,	gridStartY-0.05, textScale, -60, "2000");
	displayString(gridStartX-0.01 + incrementX*3,	gridStartY-0.05, textScale, -60, "3000");
	displayString(gridStartX-0.01 + incrementX*4,	gridStartY-0.05, textScale, -60, "4000");
	displayString(gridStartX-0.01 + incrementX*5,	gridStartY-0.05, textScale, -60, "5000");
	displayString(gridStartX-0.01 + incrementX*6,	gridStartY-0.05, textScale, -60, "6000");
	displayString(gridStartX-0.01 + incrementX*7,	gridStartY-0.05, textScale, -60, "7000");
	displayString(gridStartX-0.01 + incrementX*8,	gridStartY-0.05, textScale, -60, "8000");
	displayString(gridStartX-0.01 + incrementX*9,	gridStartY-0.05, textScale, -60, "9000");
	displayString(gridStartX-0.02 + incrementX*10,	gridStartY-0.05, textScale, -60, "10000");
	displayString(gridStartX-0.02 + incrementX*11,	gridStartY-0.05, textScale, -60, "11000");
	displayString(gridStartX-0.02 + incrementX*12,	gridStartY-0.05, textScale, -60, "12000");
	glPopMatrix();







    /////////////////////////////////////////
    // draw grid
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.05f, 0.05f, 0.05f);



	glTranslatef(gridStartX, gridStartY, 0.0);

	float gv = 0.05f; // gray value

	float gmn = 0.25f;	// gray min
	float gmx = 0.75f;	// gray max



	unsigned int sizeX = 12;
	unsigned int sizeY = 20;

    for (unsigned int lvy=0 ; lvy<sizeY ; lvy++)
    {
    	gv = lvy* 0.50 /(float)19 + 0.25;
		glColor3f(gv, gv, gv);

        glBegin(GL_LINES);
        glVertex3f(0, incrementY*lvy*ts, 0);
        glVertex3f(incrementX*sizeX, incrementY*lvy*ts, 0);
        glEnd();
        for (unsigned int lvx=0 ; lvx<sizeX+1 ; lvx++)
        {
            glBegin(GL_LINES);
			glColor3f(gmn, gmn, gmn);
            glVertex3f(incrementX*lvx, 0, 0);

            glColor3f(gmx, gmx, gmx);
            glVertex3f(incrementX*lvx, incrementY*(sizeY-1)*ts, 0);
            glEnd();
        }
    }
	glPopMatrix();




    /////////////////////////////////////////
    // draw current torque curve
	float hsize = 0.0025f;
	glColor3f(0.1f, 0.1f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	unsigned int tlv;
	for (tlv=0 ; tlv<torqueCurve.size() ; tlv++)
	{

		glVertex3f ( torqueCurve[tlv].a[0], torqueCurve[tlv].a[1]-hsize, 0 );
		glVertex3f ( torqueCurve[tlv].a[0], torqueCurve[tlv].a[1]+hsize, 0 );
	}
	glEnd();

	// if the triangle strip curve is too steep then we can't see it very well
	// so draw a line strip over top of it
	glBegin(GL_LINE_STRIP);
	for (tlv=0 ; tlv<torqueCurve.size() ; tlv++)
	{
		glVertex3f ( torqueCurve[tlv].a[0], torqueCurve[tlv].a[1], 0 );
	}
	glEnd();



	// draw a horsepower curve
	// (Torque x Engine speed) / 5,252 = Horsepower
	glColor3f(0.0f, 0.6f, 0.0f);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_TRIANGLE_STRIP);
	unsigned int hplv;
	for (hplv=0 ; hplv<torqueCurve.size() ; hplv++)
	{
		float rpm = (torqueCurve[hplv].a[0] - gridStartX) * 1000 / incrementX;
		//printf("rpm: %f\n", rpm);


		float torque = (torqueCurve[hplv].a[1] - gridStartY) * 50 / incrementY;
		//printf("torque: %f\n", torque);

		float power = torque * rpm / 5252;
		//printf("power: %f\n", power);

		float scaledPower = power / 50 * incrementY;
		scaledPower += gridStartY;

		glVertex3f ( torqueCurve[hplv].a[0], scaledPower-hsize, 0 );
		glVertex3f ( torqueCurve[hplv].a[0], scaledPower+hsize, 0 );
	}
	glEnd();

	// if the triangle strip curve is too steep then we can't see it very well
	// so draw a line strip over top of it
	glBegin(GL_LINE_STRIP);
	for (hplv=0 ; hplv<torqueCurve.size() ; hplv++)
	{
		float rpm = (torqueCurve[hplv].a[0] - gridStartX) * 1000 / incrementX;
		float torque = (torqueCurve[hplv].a[1] - gridStartY) * 50 / incrementY;
		float power = torque * rpm / 5252;
		float scaledPower = power / 50 * incrementY;
		scaledPower += gridStartY;

		glVertex3f ( torqueCurve[hplv].a[0], scaledPower, 0 );
	}
	glEnd();





	/////////////////////////////////////////////////////
	// figure out which point is closest to the mouse
	// and draw a dot there
	if (torqueCurve.size() > 0)
	{
		float dist = 1000;
		// find the point closest to the mouse
		for (unsigned int tlv=0 ; tlv<torqueCurve.size() ; tlv++)
		{
			float leg1 = torqueCurve[tlv].a[0] - mousePos.a[0];
			float leg2 = torqueCurve[tlv].a[1] - mousePos.a[1];
			float hyp = sqrt( leg1*leg1 + leg2*leg2);

			if (dist > hyp )
			{
				dist = hyp;
				closestPointToMouse = tlv;
			}

		}

		glColor3f(1.0f, 1.0f, 0.25f);
		OpenGL_drawPoint(torqueCurve[closestPointToMouse], 0.01f);
	}



	if (hardwareCursor)
		OpenGL_drawMouse();

}







