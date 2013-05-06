#include "observer.h"
#include "global.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/utils.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

#define CHASSITOSHORTSIDE 7 //halva cm 7 18 21
#define CHASSITOLONGSIDE 21 //halva cm
#define CHASSITOLONGBACK 18 //halva cm
#define CHASSITOLONGFRONT 18 //halva cm

#define TIMECONSTANT 50 // ms
#define INVERTTIMECONSTANT 20 //Dimension 1/s

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
		glob_omega = (glob_theta - glob_thetaOld)*INVERTTIMECONSTANT; //TODO
	}
	else
	{
		glob_omega = gyroValue;
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
		return 1;
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
	int16_t LongFront=getSensorLongForward();	// Används ej
	int16_t LongRear=getSensorLongRear();		// Används ej
	int16_t LongLeft=getSensorLongLeft();
	int16_t LongRight=getSensorLongRight();
	
	int16_t ShortLeftFront=getSensorShortLeftForward();
	int16_t ShortLeftRear=getSensorShortLeftRear();
	int16_t ShortRightFront=getSensorShortRightForward();
	int16_t ShortRightRear=getSensorShortRightRear();
 	
	int16_t overNoiseLongFront=getSensorLongOverNoise(LongFront);
	int16_t overNoiseLongRear=getSensorLongOverNoise(LongRear);
	int16_t overNoiseLongLeft=getSensorLongOverNoise(LongLeft);
	int16_t overNoiseLongRight=getSensorLongOverNoise(LongRight);
	
	int16_t overNoiseShortLeftFront = getSensorShortOverNoise(ShortLeftFront);
	int16_t overNoiseShortLeftRear = getSensorShortOverNoise(ShortLeftRear);
	int16_t overNoiseShortRightFront = getSensorShortOverNoise(ShortRightFront);
	int16_t overNoiseShortRightRear = getSensorShortOverNoise(ShortRightRear);
	
	
	int16_t overXPosUncert = 1;
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
	int16_t taljare = sum1+sum2+sum3+sum4+sum5+sum6+sum7;
	int16_t namnare = overNoiseLongLeft+overNoiseLongRight+overNoiseShortLeftFront+overNoiseShortLeftRear+overNoiseShortRightFront+overNoiseShortRightRear+overXPosUncert;
	int16_t divAns=taljare/namnare;
	setRelativeX(divAns);
	//ta fram y
	int16_t YLongForward = getShiftedSensorY((HALFSQUAREWIDTH-CHASSITOLONGFRONT)-(LongFront<<1)); // du är här. blir problem då en sensor säger att man är på posY -80 och en annan säger att man är på +80
	int16_t YLongBack = getShiftedSensorY((LongRear<<1)+CHASSITOLONGBACK-HALFSQUAREWIDTH);
	uint16_t overYPosUncert = 10; //inkluderar osäkerhet i y pga hast.
	int16_t velocity = getVelocity();
	
	taljare = YLongForward*overNoiseLongFront+YLongBack*overNoiseLongRear+overYPosUncert*(getRelativeY()+((TIMECONSTANT*velocity)>>10)); //lägg till hastighet*TIMECONSTANT vid getRelativeY i beräkningarna TODO
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
	
	setTheta(LongFront, LongRear, LongLeft, LongRight, ShortLeftFront, ShortLeftRear, ShortRightFront, ShortRightRear);
	
	setOmega();
	
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

void turnObserver()
{
	int16_t tempTheta = (glob_gyro*TIMECONSTANT)>>10;
	glob_theta = glob_theta + tempTheta;
}
