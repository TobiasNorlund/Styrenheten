/*
 * global.c
 *
 * Created: 4/17/2013 9:45:13 AM
 *  Author: davmo049
 */ 

#include "global.h"

Globals globals;

uint8_t getSensorLongForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_1;
	}
	else
	{
		return globals.avstandsensor_3;
	}
}

uint8_t getSensorLongBack()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_3;
	}
	else
	{
		return globals.avstandsensor_1;
	}
}

uint8_t getSensorLongLeft()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_4;
	}
	else
	{
		return globals.avstandsensor_2;
	}
}

uint8_t getSensorLongRight()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_2;
	}
	else
	{
		return globals.avstandsensor_4;
	}
}

uint8_t getSensorShortLeftForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_7;
	}
	else
	{
		return globals.avstandsensor_6;
	}
}

uint8_t getSensorShortLeftBack()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_8;
	}
	else
	{
		return globals.avstandsensor_5;
	}
}

uint8_t getSensorShortRightForward()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_5;
	}
	else
	{
		return globals.avstandsensor_8;
	}
}

uint8_t getSensorShortRightBack()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		return globals.avstandsensor_6;
	}
	else
	{
		return globals.avstandsensor_7;
	}
}