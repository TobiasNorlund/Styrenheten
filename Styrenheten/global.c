/*
 * global.c
 *
 * Created: 4/17/2013 9:45:13 AM
 *  Author: davmo049
 */ 

#include "global.h"


uint8_t getSensorLongForward()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longFront;
	}
	else
	{
		return glob_longRear;
	}
}

uint8_t getSensorLongRear()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longRear;
	}
	else
	{
		return glob_longFront;
	}
}

uint8_t getSensorLongLeft()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longLeft;
	}
	else
	{
		return glob_longRight;
	}
}

uint8_t getSensorLongRight()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longRight;
	}
	else
	{
		return glob_longLeft;
	}
}

uint8_t getSensorShortLeftForward()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortFrontLeft;
	}
	else
	{
		return glob_shortRearRight;
	}
}

uint8_t getSensorShortLeftRear()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortRearLeft;
	}
	else
	{
		return glob_shortFrontRight;
	}
}

uint8_t getSensorShortRightForward()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortFrontRight;
	}
	else
	{
		return glob_shortRearLeft;
	}
}

uint8_t getSensorShortRightRear()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortRearRight;
	}
	else
	{
		return glob_shortFrontLeft;
		
	}
}