#include "observer.h"
#include <avr/io.h>
#include <avr/pgmspace.h>


void setRelativeX(uint8_t shortFront, uint8_t shortRear) //x som om roboten har riktning up
{
	uint8_t value = ((shortFront + shortRear)*(2 - glob_theta^2))>>2;
	switch(glob_logical_direction)
	{
		case(LOGICAL_DIR_UP):
			glob_x = value;
			break;
		case(LOGICAL_DIR_RIGHT):
			glob_y = -value;
			break;
		case(LOGICAL_DIR_DOWN):
			glob_x = -value;
			break;
		case(LOGICAL_DIR_LEFT):
		glob_y = value;
			break;
	}
	return;
}
/* 
int8_t getRelativeY(int8_t value) //Y som om roboten har riktning upp
{
	switch(glob_logical_direction)
	{
		case(LOGICAL_DIR_UP):
			glob_y = value;
			break;
		case(LOGICAL_DIR_RIGHT):
			glob_x = value;
			break;
		case(LOGICAL_DIR_DOWN):
			glob_y = -value;
			break;
		case(LOGICAL_DIR_LEFT):
			glob_x = -value;
			break;
	}
	return;
}
*/
/*
const uint8_t lookupShort[140] PROGMEM = {
	0,3,6,10,13,16,19,22,25,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,74,77,79,82,85,88,90,93,96,98,101,104,106,109,111,114,116,119,121,123,126,128,130,133,135,137,139,141,143,145,147,150,151,153,155,157,159,161,163,165,167,168,170,172,173,175,177,178,180,182,183,185,186,188,189,191,192,193,195,196,198,199,200,202,203,204,205,207,208,209,210,211,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,234,235,236,237,238,239,239,240,241,242,243,243,244,245,246,246,247,248,248,249,250,250
	};
*/


/*
void setThetaOmegaLeft(uint8_t shortLeftFront, uint8_t shortLeftRear)
{
	glob_thetaOld = glob_theta;
	uint8_t diff;
	if (shortLeftFront < shortLeftRear)
	{
		diff = shortLeftRear - shortLeftFront;
		glob_theta = pgm_read_byte(&(lookupShort[diff]))>>2;
	}
	else
	{
		diff = shortLeftFront - shortLeftRear;
		glob_theta = -(pgm_read_byte(&(lookupShort[diff]))>>2);	
	}
	glob_omega = (glob_theta - glob_thetaOld)>>TIME;
	return;
}
	
void setThetaOmegaRight(uint8_t shortRightFront, uint8_t shortRightRear)
{
	glob_thetaOld = glob_theta;
	uint8_t diff;
	if (shortLeftFront < shortLeftRear)
	{
		diff = shortLeftRear - shortLeftFront;
		glob_theta = -(pgm_read_byte(&(lookupShort[diff]))>>2);
	}
	else
	{
		diff = shortLeftFront - shortLeftRear;
		glob_theta = pgm_read_byte(&(lookupShort[diff]))>>2;
	}
	glob_omega = (glob_theta - glob_thetaOld)>>TIME;
	return;
}
*/

void setOmega()
{
	int16_t gyroValue = glob_gyro;
	if((gyroValue < 10) && (gyroValue > -10))
	{
		glob_omega = (glob_theta - glob_thetaOld)/TIME;
	}
	else
	{
		glob_omega = gyroValue;
	}
	return;
}


/**
 * Ska bli en helvetes massa if-satser beroende p�
 * vad man f�r f�r v�rden. Dvs reglera p� olika s�tt
 * beroende p� vilka sensordata vi f�r�+-
 .
 */
void setStraightObserver()
{
	uint8_t LongFront=getSensorLongForward();	// Anv�nds ej
	uint8_t LongRear=getSensorLongRear();		// Anv�nds ej
	uint8_t LongLeft=getSensorLongLeft();
	uint8_t LongRight=getSensorLongRight();
	uint8_t ShortLeftFront=getSensorShortLeftForward();
	uint8_t ShortLeftRear=getSensorShortLeftRear();
	uint8_t ShortRightFront=getSensorShortRightForward();
	uint8_t ShortRightRear=getSensorShortRightRear();
	
	/*
	if((shortLeftFront + shortLeftRear) < (shortRightFront + shortRightRear))
	{
		setThetaOmegaLeft(shortLeftFront, shortLeftRear);
		setXV(shortLeftFront,shortRightRear);
	}
	else
	{
		setThetaOmegaRight(shortRightFront, shortRightRear);
		setXV(shortRightFront, shortRightRear);
	}
	*/
	
	setTheta(LongFront, LongRear, LongLeft, LongRight, ShortLeftFront, ShortLeftRear, ShortRightFront, ShortRightRear);
	setOmega();
	
	if(OK_SENSOR_VALUE(ShortLeftFront)&&OK_SENSOR_VALUE(ShortLeftRear))
	{
		setRelativeX(ShortLeftFront, ShortLeftRear);
	}
	else if(OK_SENSOR_VALUE(ShortRightFront)&&OK_SENSOR_VALUE(ShortRightRear))
	{
		setRelativeX(ShortRightFront, ShortRightRear);
	}	
	return;
}
	
