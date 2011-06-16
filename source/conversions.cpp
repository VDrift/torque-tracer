#include "resources.h"



float convert_XPosToRpm(float xpos)
{
	float rpm;

	rpm = (xpos - gridStartX) / incrementX;
	rpm *= 1000.0000000;	// x1000 for rpm

	return rpm;
}

float convert_RpmToXPos(float rpm)
{
	float xpos;

	xpos = gridStartX + rpm * incrementX * 0.001;

	return xpos;
}


float convert_YPosToTorque(float ypos)
{
	float torque;

	torque = (ypos - gridStartY) / incrementY;
	torque *= 50;		// x50 for torque

	return torque;
}


float convert_TorqueToYPos(float torque)
{
	float ypos = 0;


	ypos = torque * incrementY / 50;


	ypos += gridStartY;


	return ypos;
}

