


#define ACTION_NULL			0
#define ACTION_QUIT			1
#define ACTION_WRITE_FILE	2
#define ACTION_TOGGLE_MODE	3
#define ACTION_TOGGLE_TORQUE_UNITS	4

#define ACTION_TORQUE_ZOOM_IN	5
#define ACTION_TORQUE_ZOOM_OUT	6

#define BUTTONS_KEYBOARD	SDLK_UNDO
#define BUTTONS_MOUSE		12			// idk, why not 12?



class c_button
{
public:
bool pressed;
bool pressedLf;		// pressed last frame
unsigned int action;

c_button()
{
	pressed = false;
	pressedLf = false;		// pressed last frame
	action = 0;
}
};
