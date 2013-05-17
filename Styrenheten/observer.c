#include "observer.h"
#include "global.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/utils.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

#define TIMECONSTANT 13 // ms
#define INVERTTIMECONSTANT 80 //Dimension 1/s

#define HALFSQUAREWIDTH 80 //halva cm

void setRelativeX(int8_t val) //x som om roboten har riktning up
{
	switch(glob_logical_direction)
	{
		case(LOGICAL_DIR_UP):
			glob_x = val;
			break;
		case(LOGICAL_DIR_RIGHT):
			glob_y = -val;
			break;
		case(LOGICAL_DIR_DOWN):
			glob_x = -val;
			break;
		case(LOGICAL_DIR_LEFT):
			glob_y = val;
			break;
	}
}

void setRelativeY(int8_t value) //Y som om roboten har riktning upp
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

void observe()
{
	if(glob_curComm == FORWARD_COMMAND || glob_curComm == CUSTOM_STEERING_COMMAND)
	{
		straightObserver();
	}
	else if(glob_curComm == RIGHT_90_COMMAND || glob_curComm == LEFT_90_COMMAND || glob_curComm == RIGHT_45_COMMAND || glob_curComm == LEFT_45_COMMAND)
	{
		turnObserver();
	}
	else if(glob_curComm == TURN_FINE)
	{
		turnFine();
	}
	else if(glob_curComm == VIRTUAL_REVERSE_COMMAND || glob_curComm == NULL_COMMAND)
	{
	}
}

uint8_t getSensorLongOverNoiseForw(uint8_t value, uint8_t value_old)
{
	if(value < 20)
	{
		return 0;
	}
	if(value == 255)
	{
		return 0;	
	}
	else
	{
		return 1;
	}
}

uint8_t getSensorLongOverNoiseHoriz(uint8_t value, uint8_t value_old)
{
	if(value < 20)
	{
		return 0;
	}
	if(max(value-value_old, value_old-value) > 40)
	{
		return 0;
	}
	if(value == 255)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t getSensorShortOverNoise(uint8_t value, uint8_t value_old)
{
	if(value < 5)
	{
		return 0;
	}
	if(max(value-value_old, value_old-value) > 20)
	{
		return 0;
	}
	if(value > 150)
	{
		return 0;
	}
	else
	{
		return 10;
	}
}

int16_t max(int16_t a, int16_t b)
{
	if(a > b)
	{
		return a;
	}
	return b;
}

//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
int16_t getShiftedSensorX(int16_t sensorVal)
{
	int16_t iter = sensorVal-(HALFSQUAREWIDTH<<2);
	int8_t relX = getRelativeX();
	while(1)
	{
		if(max(iter-relX, relX-iter) <= HALFSQUAREWIDTH)
		{
			return iter;
		}
		iter = iter+(HALFSQUAREWIDTH<<1);
	}
}

int16_t getShiftedSensorY(int16_t sensorVal)
{
	int16_t iter = sensorVal-(int8to16(HALFSQUAREWIDTH)<<2);
	int8_t relY = getRelativeY();
	while(1)
	{
		if(max(iter-relY, relY-iter) <= int8to16(HALFSQUAREWIDTH))
		{
			return iter;
		}
		iter = iter+(int8to16(HALFSQUAREWIDTH)<<1);
	}
}
#pragma GCC pop_options
//end turn off optimization 
void moveForwards()
{
	switch(glob_logical_direction)
	{
		case LOGICAL_DIR_UP:
			++glob_mapY;
			break;
		case LOGICAL_DIR_RIGHT:
			++glob_mapX;
			break;
		case LOGICAL_DIR_DOWN:
			--glob_mapY;
			break;
		default:
			--glob_mapX;
			break;
	}
	setRelativeY(getRelativeY()-(int8to16(HALFSQUAREWIDTH)<<1));
}
//turn off optimization
#pragma GCC push_options
#pragma GCC optimize ("O0")

int16_t getVelocity() //halva cm /s
{
	if(glob_vLeft == 255 && glob_vRight == 255)
	{
		return 0;
	}
	else if(glob_vLeft == 255)
	{
		return glob_vRight<<1;
	}
	else if(glob_vRight == 255)
	{
		return glob_vLeft<<1;
	}
	else
	{
		return glob_vRight+glob_vLeft;
	}
}

void straightObserver()
{
	int16_t LongFront=getSensorLongForward();
	int16_t LongRear=getSensorLongRear();
	int16_t LongLeft=getSensorLongLeft();
	int16_t LongRight=getSensorLongRight();
	
	int16_t ShortLeftFront=getSensorShortLeftForward();
	int16_t ShortLeftRear=getSensorShortLeftRear();
	int16_t ShortRightFront=getSensorShortRightForward();
	int16_t ShortRightRear=getSensorShortRightRear();
 	
	int16_t overNoiseLongFront= getSensorLongOverNoiseForw(LongFront, getSensorLongForwardOld());
	int16_t overNoiseLongRear= getSensorLongOverNoiseForw(LongRear, getSensorLongRearOld());
	int16_t overNoiseLongLeft=getSensorLongOverNoiseHoriz(LongLeft, getSensorLongLeftOld());
	int16_t overNoiseLongRight=getSensorLongOverNoiseHoriz(LongRight, getSensorLongRightOld());
	
	int16_t overNoiseShortLeftFront = getSensorShortOverNoise(ShortLeftFront, getSensorShortLeftForwardOld());
	int16_t overNoiseShortLeftRear = getSensorShortOverNoise(ShortLeftRear, getSensorShortLeftRearOld());
	int16_t overNoiseShortRightFront = getSensorShortOverNoise(ShortRightFront, getSensorShortRightForwardOld());
	int16_t overNoiseShortRightRear = getSensorShortOverNoise(ShortRightRear, getSensorShortRightRearOld());
	
	int16_t overXPosUncert = 10;
	int16_t XShortLeftFront = getShiftedSensorX(ShortLeftFront+CHASSITOSHORTSIDE-HALFSQUAREWIDTH);
	int16_t XShortLeftRear = getShiftedSensorX(ShortLeftRear+CHASSITOSHORTSIDE-HALFSQUAREWIDTH);
	int16_t XShortRightFront = getShiftedSensorX(HALFSQUAREWIDTH-ShortRightFront-CHASSITOSHORTSIDE);
	int16_t XShortRightRear = getShiftedSensorX(HALFSQUAREWIDTH-ShortRightRear-CHASSITOSHORTSIDE);
	
	int16_t XLongLeft = getShiftedSensorX((LongLeft<<1)+CHASSITOLONGSIDE-HALFSQUAREWIDTH);
	int16_t XLongRight = getShiftedSensorX(HALFSQUAREWIDTH-(LongRight<<1)-CHASSITOLONGSIDE);
	int16_t sum1 = XLongLeft*overNoiseLongLeft;
	int16_t sum2 = XLongRight*overNoiseLongRight;
	int16_t sum3 = XShortLeftFront*overNoiseShortLeftFront;
	int16_t sum4 = XShortLeftRear*overNoiseShortLeftRear;
	int16_t sum5 = XShortRightFront*overNoiseShortRightFront;
	int16_t sum6 = XShortRightRear*overNoiseShortRightRear;
	int16_t sum7 = overXPosUncert*int8to16(getRelativeX());
	int16_t taljare = sum1+sum2+sum3+sum4+sum5+sum6+sum7;
	int16_t namnare = overNoiseLongLeft+overNoiseLongRight+overNoiseShortLeftFront+overNoiseShortLeftRear+overNoiseShortRightFront+overNoiseShortRightRear+overXPosUncert;
	int16_t divAns=taljare/namnare;
	setRelativeX(divAns);
	//ta fram y
	int16_t YLongForward = getShiftedSensorY((HALFSQUAREWIDTH-CHASSITOLONGFRONT)-(LongFront<<1)); // du är här. blir problem då en sensor säger att man är på posY -80 och en annan säger att man är på +80
	int16_t YLongBack = getShiftedSensorY((LongRear<<1)+CHASSITOLONGBACK-HALFSQUAREWIDTH);
	int16_t velocity = getVelocity();
	int16_t relYnew = getRelativeY()+((TIMECONSTANT*velocity)>>10);
	int16_t overYPosUncert = 10; //inkluderar osäkerhet i y pga hast.
	
	taljare = YLongForward*overNoiseLongFront+YLongBack*overNoiseLongRear+overYPosUncert*relYnew;
	namnare = overNoiseLongFront+overNoiseLongRear+overYPosUncert;
	setRelativeY(taljare/namnare);
	if(getRelativeY() > HALFSQUAREWIDTH)
	{
		moveForwards();
	}
	
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
	
	setTheta(ShortLeftFront, ShortLeftRear, ShortRightFront, ShortRightRear);
	
	/*
	if(OK_SENSOR_VALUE(ShortLeftFront)&&OK_SENSOR_VALUE(ShortLeftRear))
	{
		setRelativeX(ShortLeftFront, ShortLeftRear);
	}
	else if(OK_SENSOR_VALUE(ShortRightFront)&&OK_SENSOR_VALUE(ShortRightRear))
	{
		setRelativeX(ShortRightFront, ShortRightRear);
	}
	*/	
	return;
}
#pragma GCC pop_options
//end turn off optimization

void turnFine()
{
	int16_t LongFront=getSensorLongForward();
	int16_t LongRear=getSensorLongRear();
	int16_t LongLeft=getSensorLongLeft();
	int16_t LongRight=getSensorLongRight();
	
	int16_t ShortLeftFront=getSensorShortLeftForward();
	int16_t ShortLeftRear=getSensorShortLeftRear();
	int16_t ShortRightFront=getSensorShortRightForward();
	int16_t ShortRightRear=getSensorShortRightRear();
	
	uint8_t right36AngleK=0;
	int8_t right36AngleDiff=0;
	if(OK_SENSOR_VALUE(ShortRightFront)&&OK_SENSOR_VALUE(ShortRightRear))
	{
		right36AngleK=calc36K(ShortRightFront, ShortRightRear);
		right36AngleDiff=calcSideSensors36(ShortRightFront,ShortRightRear,1) - glob_theta;
	}
	//Vänsta sidan, endast korta
	uint8_t left36AngleK=0;
	int8_t left36AngleDiff=0;
	if(OK_SENSOR_VALUE(ShortLeftFront)&&OK_SENSOR_VALUE(ShortLeftRear))
	{
		left36AngleK=calc36K(ShortLeftFront, ShortLeftRear);
		left36AngleDiff=calcSideSensors36(ShortLeftFront,ShortLeftRear,0) - glob_theta;
	}
	glob_thetaOld = glob_theta;
	int16_t thetaLeft = int8to16(calcSideSensors36(ShortLeftFront,ShortLeftRear,0))-90;
	int16_t thetaRight = int8to16(calcSideSensors36(ShortRightFront,ShortRightRear,1))-90;
	while(max(thetaLeft-glob_theta, glob_theta-thetaLeft) >= 45)
	{
		thetaLeft+= 90;
	}
	while(max(thetaRight-glob_theta, glob_theta-thetaRight) >= 45)
	{
		thetaRight+= 90;
	}
	if(left36AngleK == 0 && right36AngleK == 0)
	{
		int16_t tempTheta = (glob_gyro*TIMECONSTANT)>>10;
		glob_theta = (glob_theta+tempTheta);
	}
	int16_t taljare = thetaLeft*left36AngleK + thetaRight*right36AngleK;
	int16_t namnare = right36AngleK + left36AngleK;
	
	//int16_t newAngleDiff = taljare/namnare;
	//glob_theta = glob_theta+newAngleDiff;
	glob_theta = taljare/namnare;
}

void turnObserver()
{
	int16_t tempTheta = (glob_gyro*TIMECONSTANT)>>10;
	glob_theta = glob_theta + tempTheta;
}
