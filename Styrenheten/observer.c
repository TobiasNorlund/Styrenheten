#include "global.h"
#include "../../TSEA27-include/geometrics.h"
#include <avr/io.h>

void interrupt-beta()
{
	uint8_t time;
	
	void setXV(uint8_t shortForward, uint8_t shortRear)
	{
		xOld = x;
		x = (shortForward + shortRear())*(2 - theta^2)/4
		v = (x - xOld)/time;
		return;
	}	
	
	void setThetaOmegaLeft(uint8_t shortLeftForward, uint8_t shortLeftRear)
	{
		thetaOld = theta;
		theta = (shortLeftForward - shortLeftRear)/SHORT_SENSOR_DISTANCE;
		omega = (theta - thetaOld)/time;
		return;
	}
	
	void setThetaOmegaRight(uint8_t shortRightForward, uint8_t shortRightRear)
	{
		thetaOld = theta;
		theta = (shortRightForward - shortRightRear)/SHORT_SENSOR_DISTANCE;
		omega = (theta - thetaOld)/time;
		return;
	}
	
	void setObserver(int frontLeftSensor, int backLeftSensor, int frontRightSensor, int backRightSensor)
	{
		uint8_t shortLeftForward = getSensorShortLeftForward();
		uint8_t shortLeftRear = getSensorShortLeftRear();
		uint8_t shortRightForward = getSensorShortRightForward();
		uint8_t shortRightRear = getSensorShortRightRear();
		
		if((shortLeftForward + shortLeftRear)/2 < (shortRightForward + shortRightRear)/2))
		{
			setThetaOmegaLeft(shortLeftForward, shortLeftRear);
			setXV(shortLeftForward);
		}
		else
		{
			setThetaOmegaRight(shortRightForward, shortRightRear);
			setXV(shortRightForward, shortRightRear);
		}
		return;
	}
	
}