

#include <math.h>


//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// SDL
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

//OpenGL Headers
#include <GL/gl.h>
#include <GL/glu.h>

// stl
#include <string>
#include <vector>
using namespace std;


#include "font_class.h"
#include "input_buttons.h"


class c_vector2f
{
public:
float a[2];


c_vector2f()
{
    a[0] = 0;
    a[1] = 0;
}
void set(float x, float y)
{
    a[0] = x;
    a[1] = y;
}

};

//////////////////////////
// prototypes
c_vector2f vector2fSet(float x, float y);
void textureLoad_sdl(string filename, unsigned int *ogl_id);

void sdl_mouseUpdate();

void OpenGL_init();
void OpenGL_render();
void load_font (const char *fontFileName, c_font *gameFont, unsigned int pixel_height);
void displayString(string str);

void load_configuration(string filename);






float convert_XPosToRpm(float xpos);
float convert_RpmToXPos(float rpm);
float convert_YPosToTorque(float ypos);
float convert_TorqueToYPos(float torque);

/////////////////////////
// externs

extern unsigned int displayWidth;
extern unsigned int displayHeight;
extern unsigned int displayBpp;
extern float displayAspectRatio;
extern bool displayFullscreen;
extern bool hardwareCursor;


extern float gridStartX;
extern float gridStartY;
extern float incrementX;
extern float incrementY;

extern c_font font[1];
extern unsigned int kernChar;

extern c_vector2f mousePos;

extern unsigned int mouseTexture;
extern unsigned int torqueCurveTexture;

extern vector<c_vector2f> torqueCurve;

extern float pictureL;
extern float pictureR;
extern float pictureT;
extern float pictureB;

extern float torqueRange;


extern unsigned int closestPointToMouse;


extern string enteredText;

/////////////////////////
// defines
extern int inputMode;
#define INPUT_MODE_DRAW		0
#define INPUT_MODE_RESIZE	1
#define INPUT_MODE_ENTER_TEXT		2


extern int torqueMode;
#define TORQUEMODE_METRIC		0
#define TORQUEMODE_IMPERIAL		1
