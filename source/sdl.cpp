#include "resources.h"




extern c_button keyboardButtons[BUTTONS_KEYBOARD];		// undo is the last SDL key
extern c_button mouseButtons[BUTTONS_MOUSE];


void sdl_mouseUpdate()
{
    displayAspectRatio = (float)displayWidth / (float)displayHeight;


    int mouse_x, mouse_y;
    unsigned int mouseButtonState;



    mouseButtonState = SDL_GetMouseState(&mouse_x, &mouse_y);

    //printf("%u,%u -> ", mouse_x, mouse_y);

    // uint to float position;
    mousePos.a[0] = (float)mouse_x / (float)displayWidth;
    mousePos.a[1] = (float)mouse_y / (float)displayHeight;

    // correct to center of screen
    mousePos.a[0] -= 0.5;
    mousePos.a[1] -= 0.5;

    // mutliply by 2
    mousePos.a[0] *= 2.0;
    mousePos.a[1] *= 2.0;

    // correct aspect ratio
    mousePos.a[0] *= displayAspectRatio;

    // mouse y is inverted
    mousePos.a[1] *= -1;



	if (mouseButtonState&SDL_BUTTON(1))			// normally the left button
	{
		if (mouseButtons[1].pressed)
			mouseButtons[1].pressedLf = true;
		else
			mouseButtons[1].pressed = true;

	}
	else
	{
		mouseButtons[1].pressed = false;
		mouseButtons[1].pressedLf = false;
	}


	if (mouseButtonState&SDL_BUTTON(3))
	{
		if (mouseButtons[3].pressed)
			mouseButtons[3].pressedLf = true;
		else
			mouseButtons[3].pressed = true;
	}
	else
	{
		mouseButtons[3].pressed = false;
		mouseButtons[3].pressedLf = false;
	}



}











void textureLoad_sdl(string filename, unsigned int *ogl_id)
{
	// SDL_Image
	SDL_Surface *image = IMG_Load(filename.c_str());
	if(!image)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		return;
	}

	printf("Load: %s ", filename.c_str());
	unsigned int bpp = image->format->BitsPerPixel;

	printf(" \n");		// if you cprint("\n"); it doesn't see the \n code


	unsigned int type;

	switch (bpp)
	{
	case 24:
		type = GL_RGB;
		//type = GL_BGR;
		break;
	case 32:
		type = GL_RGBA;
		break;
	default:
		type = GL_RGBA;
		break;
	}

	glGenTextures(1, ogl_id);
	glBindTexture(GL_TEXTURE_2D, *ogl_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, type, image->w, image->h, 0, type, GL_UNSIGNED_BYTE, image->pixels);
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, image->w, image->h, type, GL_UNSIGNED_BYTE, image->pixels);


	SDL_FreeSurface(image);
	return;
}

