
#include "resources.h"


//global variables
//screen width, height, and bit depth
unsigned int displayWidth = 1024;
unsigned int displayHeight = 768;
unsigned int displayBpp = 24;
float displayAspectRatio;
bool displayFullscreen = false;
bool hardwareCursor = true;


float gridStartX = -0.7f;
float gridStartY = -0.7f;

float incrementX = 0.15f;
float incrementY = 0.1f;

// array of a single font
// after a texture is generated and kerning info extracted it should all be
// accessible from here
c_font font[1];

// which font to render kerning info for
unsigned int kernChar = (unsigned int)'A';

unsigned int mouseTexture = 0;			// ogl texture index
unsigned int torqueCurveTexture = 0;	// ogl texture index

vector<c_vector2f> torqueCurve;			// dynamic array of 2d points that holds the data of the "curve" you draw
unsigned int closestPointToMouse = 0;





int inputMode = INPUT_MODE_RESIZE;
int torqueMode = TORQUEMODE_METRIC;



float pictureR = 0.8f;
float pictureL = -0.7f;
float pictureT = 0.8f;
float pictureB = -0.7f;


string enteredText;
float enteredTorque;	// native should be Nm
float enteredRpm;



//////////////////////////////////////////
// input
c_vector2f mousePos;	// floating point mouse position
c_button keyboardButtons[BUTTONS_KEYBOARD];		// undo is the last SDL key
c_button mouseButtons[BUTTONS_MOUSE];


void bindKeys()
{
	keyboardButtons[SDLK_ESCAPE].action = ACTION_QUIT;
	keyboardButtons[SDLK_w].action = ACTION_WRITE_FILE;

	keyboardButtons[SDLK_m].action = ACTION_TOGGLE_MODE;
	keyboardButtons[SDLK_u].action = ACTION_TOGGLE_TORQUE_UNITS;

	keyboardButtons[SDLK_EQUALS].action = ACTION_TORQUE_ZOOM_IN;
	keyboardButtons[SDLK_MINUS].action = ACTION_TORQUE_ZOOM_OUT;
}





c_vector2f vector2fSet(float x, float y)
{
	c_vector2f v;

	v.a[0] = x;
	v.a[1] = y;

	return v;
}















void insertPointInCurve(c_vector2f pos)
{
	//////////////////////////////////////////////
	// this works
	//torqueCurve.push_back(pos);


	//////////////////////////////////////////////
	// but this other way is probably better
	if (torqueCurve.size() == 0)
	{
		torqueCurve.push_back(pos);
		return;
	}

	if (pos.a[0] < torqueCurve[0].a[0])
	{
		torqueCurve.insert(torqueCurve.begin(),pos);
		return;
	}
	if (pos.a[0] > torqueCurve[torqueCurve.size()-1].a[0])
	{
		torqueCurve.push_back(pos);
		return;
	}

	vector<c_vector2f>::iterator it;
	unsigned int index = 0;

	for ( it=torqueCurve.begin() ; it<torqueCurve.end()-1 ; it++ )
	{
		if (pos.a[0] == torqueCurve[index].a[0])
		{
			torqueCurve[index].a[1] = pos.a[1];
			printf("reset point\n");
			break;
		}



		if ( torqueCurve[index].a[0] < pos.a[0] && pos.a[0] < torqueCurve[index+1].a[0] )
		{
			torqueCurve.insert(it+1,pos);
			break;
		}
		index++;
	}
}











void processEnteredText()
{
	c_vector2f point = vector2fSet(gridStartX, gridStartY);


	enteredTorque = atof(enteredText.c_str());

	unsigned int rpmPos = enteredText.find("@");

	if (rpmPos == 0)
	{
		enteredTorque = 0;
		enteredRpm = 0;
		enteredText.resize(0);
		return;
	}

	enteredRpm = atof(&enteredText.c_str()[rpmPos+1]);


	printf("entered torque: %f @ %f\n", enteredTorque, enteredRpm);



	point.a[0] = convert_RpmToXPos(enteredRpm);





	if ( strstr(enteredText.c_str(), "Nm") )
	{
		printf("Nm\n");

		if (torqueMode == TORQUEMODE_IMPERIAL) enteredTorque *= 0.7376f;


		point.a[1] = convert_TorqueToYPos(enteredTorque);
	}
	else if ( strstr(enteredText.c_str(), "ft-lbs") )
	{
		printf("ft-lbs\n");

		if (torqueMode == TORQUEMODE_METRIC) enteredTorque /= 0.7376f;

		point.a[1] = convert_TorqueToYPos(enteredTorque);
	}
	else if ( strstr(enteredText.c_str(), "kW") )
	{
		printf("kW\n");

		// (torque * rpm) / 5252 = power
		// torque = 5252 * power / rpm


		float tempTorque = 5252 * enteredTorque / enteredRpm;		// in this case, they enetered a power not a torque

		printf("5252 * %fkW / %frpm = torque: %fNm\n", enteredTorque, enteredRpm, tempTorque);

		if (torqueMode == TORQUEMODE_IMPERIAL) tempTorque /= 0.7376f;

		point.a[1] = convert_TorqueToYPos(tempTorque);

	}
	else if ( strstr(enteredText.c_str(), "bhp") )
	{
		printf("bhp\n");

		float tempTorque = 5252 * enteredTorque / enteredRpm;		// in this case, they enetered a power not a torque
		//tempTorque *= 0.73549875f;

		printf("5252 * %fbhp / %frpm = torque: %fft-lbs\n", enteredTorque, enteredRpm, tempTorque);

		if (torqueMode == TORQUEMODE_METRIC) tempTorque *= 0.7376f;

		point.a[1] = convert_TorqueToYPos(tempTorque);

	}
	else
	{
		printf("default\n");
		// i don't know, they've fucked it up, clear the string and call it a day
	}


	enteredText.resize(0);

	insertPointInCurve(point);
}












void writeTorqueCurve()
{
	string filename = "torque_curve.txt";

	printf("\nwritting: %s\n", filename.c_str() );

	FILE *fp = fopen(filename.c_str(), "w+");




	for (unsigned int lv=0 ; lv<torqueCurve.size() ; lv++)
	{

		//c_vector2f rpmTorque = convertScreenPositionToRPMTorque(torqueCurve[lv]);
		float rpm = convert_XPosToRpm(torqueCurve[lv].a[0]);
		float torque = convert_YPosToTorque(torqueCurve[lv].a[1]);

		if (lv < 10)		// vdrift doesn't like numbers without leading zeros.
		{
			fprintf(fp, "torque-curve-0%u = %u, %u\n", lv, (unsigned int)rpm, (unsigned int)torque);
			printf("torque-curve-0%u = %u, %u.    ", lv, (unsigned int)rpm, (unsigned int)torque);
		}
		else
		{
			fprintf(fp, "torque-curve-%u = %u, %u\n", lv, (unsigned int)rpm, (unsigned int)torque);
			printf("torque-curve-%u = %u, %u.    ", lv, (unsigned int)rpm, (unsigned int)torque);
		}
		//fprintf(fp, "torque-curve-%u = %f, %f\n", lv, rpm, torque);
		//printf("torque-curve-%u = %f, %f.    ", lv, rpm, torque);

		printf("\n");
	}

	fclose(fp);
}



















int main( int argc, char **argv )
{
	load_configuration("configuration.ini");


    SDL_Surface *surface;
    SDL_Event event;

    int videoFlags;
    videoFlags  = SDL_OPENGL;          // Enable OpenGL in SDL
    videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
    //videoFlags |= SDL_HWPALETTE;       // Store the palette in hardware

	if ( displayFullscreen )
		 videoFlags |= SDL_FULLSCREEN;


    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    //Quit( 1 );
	}


	if (hardwareCursor)
		SDL_ShowCursor(SDL_DISABLE);

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    surface = SDL_SetVideoMode( displayWidth, displayHeight, displayBpp, videoFlags );


	SDL_WM_SetCaption("Torque Tracer","icon");

	bindKeys();

    OpenGL_init();
    load_font("Vera.ttf", &font[0], 18);







	//////////////////////////////////////
	// torque curve initial points - for debugging
	/*torqueCurve.push_back( vector2fSet(-0.7, -0.7) );
	torqueCurve.push_back( vector2fSet(-0.7 + incrementX * 1, -0.7 + incrementY*2) );
	torqueCurve.push_back( vector2fSet(-0.7 + incrementX * 2, -0.7 + incrementY*3) );*/

	bool exitApplication = false;

    while ( true )
    {
    	if (exitApplication == true) break;

        SDL_PollEvent( &event );


		SDL_PumpEvents();
        sdl_mouseUpdate();


		if (event.type == SDL_QUIT)
			exitApplication = true;



		////////////////////////////////////////////
		// check keyboard keys
		unsigned char *keyState = (unsigned char *) SDL_GetKeyState(NULL);
		for ( unsigned int klv=0 ; klv<BUTTONS_KEYBOARD ; klv++)
		{
			if ( keyState[klv] && keyboardButtons[klv].pressedLf == false )
			{
				keyboardButtons[klv].pressedLf = true;

				if (keyboardButtons[klv].action == ACTION_QUIT) exitApplication = true;

				if (keyboardButtons[klv].action == ACTION_TOGGLE_MODE)
				{
					if (inputMode == INPUT_MODE_RESIZE)
						inputMode = INPUT_MODE_DRAW;
					else if (inputMode == INPUT_MODE_DRAW)
						inputMode = INPUT_MODE_ENTER_TEXT;
					else if (inputMode == INPUT_MODE_ENTER_TEXT)
						inputMode = INPUT_MODE_RESIZE;
					//printf("toggle mode\n");
				}

				if (keyboardButtons[klv].action == ACTION_TORQUE_ZOOM_IN)
				{
					if (incrementY < 6)
						incrementY *= 1.25;
				}
				if (keyboardButtons[klv].action == ACTION_TORQUE_ZOOM_OUT)
				{
					if (incrementY > 0.05)
						incrementY /= 1.25;
				}
				if (keyboardButtons[klv].action == ACTION_WRITE_FILE)
				{
					writeTorqueCurve();
				}
				if (keyboardButtons[klv].action == ACTION_TOGGLE_TORQUE_UNITS)
				{
					if (torqueMode == TORQUEMODE_METRIC)
						torqueMode = TORQUEMODE_IMPERIAL;
					else
						torqueMode = TORQUEMODE_METRIC;
				}


				if (inputMode == INPUT_MODE_ENTER_TEXT)
				{
					// this covers numbers
					if (  SDLK_0 <= klv && klv <= SDLK_9 )
						enteredText += klv ;
					if ( klv == SDLK_PERIOD)
						enteredText += ".";


					if ( klv == SDLK_n )
						enteredText += "Nm @";
					if ( klv == SDLK_f )
						enteredText += "ft-lbs @";
					if ( klv == SDLK_k )
						enteredText += "kW @";
					if ( klv == SDLK_h || klv == SDLK_b )
						enteredText += "bhp @";


					if ( klv == SDLK_BACKSPACE )
					{
						// if you're backspacing over an @ sign, just clear the whole string to start over
						if (enteredText[enteredText.size()-1] == '@')
							enteredText.resize(0);

						if ( enteredText.size() > 0)
							enteredText.resize(enteredText.size()-1);


					}


					if ( klv == SDLK_RETURN )
					{
						processEnteredText();

					}

				}




			}
			else if (!keyState[klv] && keyboardButtons[klv].pressedLf == true)
			{
				keyboardButtons[klv].pressedLf = false;
			}
		}



		///////////////////////////////////////
		// check mouse stuff
		for (unsigned int mlv=0 ; mlv<BUTTONS_MOUSE ; mlv++)
		{
			if ( inputMode == INPUT_MODE_DRAW)
			{
				if (mouseButtons[mlv].pressed && mouseButtons[mlv].pressedLf == false)
				{
					if ( mlv == 1)			// left click
					{
						// no negative values please
						if (mousePos.a[0] > gridStartX && mousePos.a[1] > gridStartY)
						{
							insertPointInCurve(mousePos);

						}
					}
					else if ( mlv == 3)			// right click
					{
						// erase on right-click

						//////////////////////////////////////////////
						// this works
						//if (torqueCurve.size() > 0)
						//	torqueCurve.pop_back();




						//////////////////////////////////////////////
						// but we should probably erase whatever were closest to
						if ( torqueCurve.size() == 0)
						{
							break;
						}
						if ( torqueCurve.size() == 1)
						{
							torqueCurve.pop_back();
						}
						if (closestPointToMouse == torqueCurve.size()-1)
						{
							torqueCurve.pop_back();
							break;
						}

						vector<c_vector2f>::iterator it;
						unsigned int index = 0;
						for ( it=torqueCurve.begin() ; it<torqueCurve.end()-1 ; it++ )
						{
							if (index == closestPointToMouse)
							{
								torqueCurve.erase(it,it+1);
								break;
							}
							index++;
						}


					}
				}
			}
			else if (inputMode == INPUT_MODE_RESIZE)
			{
				if ( mouseButtons[mlv].pressed)			// left click or right click
				{
					if ( (mousePos.a[0] > pictureL-0.1) && (mousePos.a[0] < pictureL+0.1) )
					{
						pictureL = mousePos.a[0];
						//printf("l-border\n");
					}

					if ( (mousePos.a[0] > pictureR-0.1) && (mousePos.a[0] < pictureR+0.1) )
					{
						pictureR = mousePos.a[0];
						//printf("r-border\n");
					}

					if ( (mousePos.a[1] > pictureT-0.1) && (mousePos.a[1] < pictureT+0.1) )
					{
						pictureT = mousePos.a[1];
						//printf("t-border\n");
					}

					if ( (mousePos.a[1] > pictureB-0.1) && (mousePos.a[1] < pictureB+0.1) )
					{
						pictureB = mousePos.a[1];
						//printf("b-border\n");
					}

				}
			}

		}



		// debug text
		for (unsigned int l=0 ; l<torqueCurve.size() ; l++ )
		{
			//printf("(%f,%f)\n", torqueCurve[l].a[0], torqueCurve[l].a[1]);
		}
		//printf("\n");



        OpenGL_render();
        SDL_GL_SwapBuffers();
    }

    printf("done.\n");


	// reset resolution
	SDL_Quit();


	/////////////////////////////////////
	// every generation of windows gets
	// progressively lamer.  xp requires
	// this to exit cleanly.
	// C'est la micro$oft
	#ifdef WIN32
		FatalExit(666);
	#endif


	return 0;
}






