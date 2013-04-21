#include "global.h"
#include "observer.h"
#include "../../TSEA27-include/geometrics.h"
#include <avr/io.h>

#define TIME 4 //i ms, ange i 2^n = t (TIME = 4 -> 16ms)



void setXV(uint8_t shortForward, uint8_t shortRear)
{
	globals.xOld = globals.x;
	globals.x = ((shortForward + shortRear)*(2 - globals.theta^2))>>2; // >>2 Delar med 4
	globals.v = (globals.x - globals.xOld)>>TIME; 
	return;
}	
	
void setThetaOmegaLeft(uint8_t shortLeftForward, uint8_t shortLeftRear)
{
	globals.thetaOld = globals.theta;
	globals.theta = (shortLeftForward - shortLeftRear)/SHORT_SENSOR_DISTANCE;
	globals.omega = (globals.theta - globals.thetaOld)>>TIME;
	return;
}
	
void setThetaOmegaRight(uint8_t shortRightForward, uint8_t shortRightRear)
{
	globals.thetaOld = globals.theta;
	globals.theta = (shortRightForward - shortRightRear)/SHORT_SENSOR_DISTANCE;
	globals.omega = (globals.theta - globals.thetaOld)>>TIME;
	return;
}

void setFromFront(uint8_t shortLeftForward, uint8_t shortRightForward)
{
	//TODO
}


/**
 * Ska bli en helvetes massa if-satser beroende på
 * vad man får för värden. Dvs reglera på olika sätt
 * beroende på vilka sensordata vi får.
 */
void setObserver(int frontLeftSensor, int backLeftSensor, int frontRightSensor, int backRightSensor)
{
	uint8_t shortLeftForward = getSensorShortLeftForward();
	uint8_t shortLeftRear = getSensorShortLeftRear();
	uint8_t shortRightForward = getSensorShortRightForward();
	uint8_t shortRightRear = getSensorShortRightRear();
	
	if((shortLeftRear == 255) & (shortRightRear == 255) & (shortLeftRear =/ 255) & shortRightForward =/ 255))
	{
		//TODO
		
	}	
	else if((shortLeftForward + shortLeftRear)>>1 < (shortRightForward + shortRightRear)>>1)
	{
		setThetaOmegaLeft(shortLeftForward, shortLeftRear);
		setXV(shortLeftForward,shortRightRear);
	}
	else //Kan man skriva så?
	{
		setThetaOmegaRight(shortRightForward, shortRightRear);
		setXV(shortRightForward, shortRightRear);
	}
	return;
}
	
