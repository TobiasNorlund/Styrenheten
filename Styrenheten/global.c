	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: global.c
	 * Skriven av: I. Junaeus, C. Karlsson Schmidt, M. Karlsson, J. Källström, 
	 *			   D. Molin, T. Norlund		
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Definierar alla de globala variabler som styrenheten använder sig av.
	 */	
	
#include "global.h"

/*
*	returnerar vad sensorn fram har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad sensorn bak har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den långa sensorn vänster har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den långa sensorn höger har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den korta främre sensorn till vänster har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den korta bakre sensorn till vänster har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den korta främre sensorn till höger har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad den korta bakre sensorn till höger har för värde givet om roboten har kört en virtuell vändning
*/
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

/*
*	returnerar vad sensorn fram har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorLongForwardOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longFront_old;
	}
	else
	{
		return glob_longRear_old;
	}
}

/*
*	returnerar vad sensorn bak har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorLongRearOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longRear_old;
	}
	else
	{
		return glob_longFront_old;
	}
}

/*
*	returnerar vad den långa sensorn vänster har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorLongLeftOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longLeft_old;
	}
	else
	{
		return glob_longRight_old;
	}
}

/*
*	returnerar vad den långa sensorn höger har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorLongRightOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_longRight_old;
	}
	else
	{
		return glob_longLeft_old;
	}
}

/*
*	returnerar vad den korta främre sensorn till vänster har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorShortLeftForwardOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortFrontLeft_old;
	}
	else
	{
		return glob_shortRearRight_old;
	}
}

/*
*	returnerar vad den korta bakre sensorn till vänster har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorShortLeftRearOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortRearLeft_old;
	}
	else
	{
		return glob_shortFrontRight_old;
	}
}

/*
*	returnerar vad den korta främre sensorn till höger har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorShortRightForwardOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortFrontRight_old;
	}
	else
	{
		return glob_shortRearLeft_old;
	}
}

/*
*	returnerar vad den korta bakre sensorn till höger har för värde givet om roboten har kört en virtuell vändning
*/
uint8_t getSensorShortRightRearOld()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		return glob_shortRearRight_old;
	}
	else
	{
		return glob_shortFrontLeft_old;
	}
}