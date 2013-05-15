	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: observer.c
	 * Skriven av: C. Karlsson, M. Karlsson, D. Molin
	 *			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Har hand om styrenhetens observatör. 
	 * Sätter de observatörens värden utefter sensorvärden.
	 */	

#include "observer.h"

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
	else if(glob_curComm == VIRTUAL_REVERSE_COMMAND || glob_curComm == NULL_COMMAND)
	{
	}
}

uint8_t getSensorLongOverNoise(uint8_t value)
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

uint16_t getSensorShortOverNoise(uint8_t value)
{
	if(value == 0)
	{
		return 0;
	}
	if(value == 150)
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
	while(1)
	{
		if(max(iter-glob_x, glob_x-iter) <= HALFSQUAREWIDTH)
		{
			return iter;
		}
		iter = iter+(HALFSQUAREWIDTH<<1);
	}
}

int16_t getShiftedSensorY(int16_t sensorVal)
{
	int16_t iter = sensorVal-(int8to16(HALFSQUAREWIDTH)<<2);
	while(1)
	{
		if(max(iter-glob_y, glob_y-iter) <= int8to16(HALFSQUAREWIDTH))
		{
			return iter;
		}
		iter = iter+(int8to16(HALFSQUAREWIDTH)<<1);
	}
}
#pragma GCC pop_options
//end turn off optimization 
void moveForward()
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
	int16_t LongFront=getSensorLongForward();	// Används ej
	int16_t LongRear=getSensorLongRear();		// Används ej
	int16_t LongLeft=getSensorLongLeft();
	int16_t LongRight=getSensorLongRight();
	
	int16_t ShortLeftFront=getSensorShortLeftForward();
	int16_t ShortLeftRear=getSensorShortLeftRear();
	int16_t ShortRightFront=getSensorShortRightForward();
	int16_t ShortRightRear=getSensorShortRightRear();
 	
	int16_t overNoiseLongFront= 0; //getSensorLongOverNoise(LongFront);
	int16_t overNoiseLongRear= 0; //getSensorLongOverNoise(LongRear);
	int16_t overNoiseLongLeft=getSensorLongOverNoise(LongLeft);
	int16_t overNoiseLongRight=getSensorLongOverNoise(LongRight);
	
	int16_t overNoiseShortLeftFront = getSensorShortOverNoise(ShortLeftFront);
	int16_t overNoiseShortLeftRear = getSensorShortOverNoise(ShortLeftRear);
	int16_t overNoiseShortRightFront = getSensorShortOverNoise(ShortRightFront);
	int16_t overNoiseShortRightRear = getSensorShortOverNoise(ShortRightRear);
	
	
	int16_t overXPosUncert = 10;
	//ta fram x i korridor // du är här. blir problem då en sensor säger att man är på posY -80 och en annan säger att man är på +80 du har inte tänkt på att långa x sensorer kan se flera rutor
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
	int16_t numerator = sum1+sum2+sum3+sum4+sum5+sum6+sum7;
	int16_t denominator = overNoiseLongLeft+overNoiseLongRight+overNoiseShortLeftFront+overNoiseShortLeftRear+overNoiseShortRightFront+overNoiseShortRightRear+overXPosUncert;
	int16_t divAns=numerator/denominator;
	setRelativeX(divAns);
	//ta fram y
	int16_t YLongForward = getShiftedSensorY((HALFSQUAREWIDTH-CHASSITOLONGFRONT)-(LongFront<<1)); // du är här. blir problem då en sensor säger att man är på posY -80 och en annan säger att man är på +80
	int16_t YLongBack = getShiftedSensorY((LongRear<<1)+CHASSITOLONGBACK-HALFSQUAREWIDTH);
	int16_t overYPosUncert = 10; //inkluderar osäkerhet i y pga hast.
	int16_t velocity = getVelocity();
	
	numerator = YLongForward*overNoiseLongFront+YLongBack*overNoiseLongRear+overYPosUncert*(getRelativeY()+((TIMECONSTANT*velocity)>>10)); //lägg till hastighet*TIMECONSTANT vid getRelativeY i beräkningarna TODO
	denominator = overNoiseLongFront+overNoiseLongRear+overYPosUncert;
	setRelativeY(numerator/denominator);
	if(getRelativeY() > HALFSQUAREWIDTH)
	{
		moveForward();
	}
	
	setTheta(ShortLeftFront, ShortLeftRear, ShortRightFront, ShortRightRear);
	return;
}
#pragma GCC pop_options
//end turn off optimization

void turnObserver()
{
	int16_t tempTheta = (glob_gyro*TIMECONSTANT)>>10;
	glob_theta = glob_theta + tempTheta;
}
