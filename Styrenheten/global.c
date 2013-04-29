/*
 * global.c
 *
 * Created: 4/17/2013 9:45:13 AM
 *  Author: davmo049
 */ 

#include "global.h"


uint8_t getSensorLongForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.longFront;
	}
	else
	{
		return globals.longRear;
	}
}

uint8_t getSensorLongRear()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.longRear;
	}
	else
	{
		return globals.longFront;
	}
}

uint8_t getSensorLongLeft()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.longLeft;
	}
	else
	{
		return globals.longRight;
	}
}

uint8_t getSensorLongRight()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.longRight;
	}
	else
	{
		return globals.longLeft;
	}
}

uint8_t getSensorShortLeftForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.shortRearRight;
	}
	else
	{
		return globals.shortFrontLeft;
	}
}

uint8_t getSensorShortLeftRear()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.shortRearLeft;
	}
	else
	{
		return globals.shortFrontRight;
	}
}

uint8_t getSensorShortRightForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.shortFrontRight;
	}
	else
	{
		return globals.shortRearLeft;
	}
}

uint8_t getSensorShortRightRear()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.shortFrontLeft;
	}
	else
	{
		return globals.shortRearRight;
	}
}