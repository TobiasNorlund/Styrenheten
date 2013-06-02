#include "observer.h"
#include "global.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/utils.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

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

uint8_t getSensorLongOverNoiseForw(uint8_t value_new, uint8_t value_old)//20-150 cm 
{
	if(value_new < 25)
	{
		return 0;
	}
	if(value_new > 130)//80 + 40 - 8 = 112 cm två rutor bort
	{
		return 0;
	}
	if(value_new == 255)
	{
		return 0;	
	}
	else
	{
		return 1;
	}
}
//TODO är kanske denna som fångar upp brus, denna borde se väggen endå om den är två rutor bort
uint8_t getSensorLongOverNoiseHoriz(uint8_t value_new, uint8_t value_old)//20-150 cm 
{
	int16_t value_new_16 = int8to16(value_new);
	int16_t value_old_16 = int8to16(value_old);
	if(value_new < 25)
	{
		return 0;
	}
	if(value_new > 130)//80 + 40 - 8 = 112 cm två rutor bort
	{
		return 0;
	}
	if(max(value_new_16-value_old_16, value_new_16-value_old_16) > 30)//OBS. Filterar bara spikar. ej om det kommer längre avikelser
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//TODO är troligen denna som fångar upp brus
uint8_t getSensorShortOverNoise(uint8_t value_new, uint8_t value_old)//10-80 cm
{
	int16_t value_new_16 = int8to16(value_new);
	int16_t value_old_16 = int8to16(value_old);
	if(value_new < 20)//10 cm då 20/2 pga halva centimeter
	{
		return 0;
	}
	if(max(value_new_16-value_old_16, value_old_16-value_new_16) > 15)//OBS. Filterar bara spikar. ej om det kommer längre avikelser
	{
		return 0;
	}
	if(value_new > 120)//halva cm. 40 - 5 = 35 cm 
	{
		return 0;
	}
	else
	{
		return 10;//mycket vikt bör justeras efter vart vi kör, om vi har öppna ytor på sidorna lite, så fort vi kan komma nära en vägg öka.
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
	else if(OK_SENSOR_VALUE(glob_vLeft))
	{
		return glob_vLeft<<1;
	}
	else if(OK_SENSOR_VALUE(glob_vRight))
	{
		return glob_vRight<<1;
	}
	/*else
	{
		return glob_vRight+glob_vLeft;
	}*/
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
	
	//om vi är långt från vägg minska vikten på de korta sensorerna.
	if(LongLeft > 60)
	{
		sum3=(sum3>>1);//halvera vikten
		sum4=(sum3>>1);
		overNoiseShortLeftFront=(overNoiseShortLeftFront>>1);
		overNoiseShortLeftRear=(overNoiseShortLeftRear>>1);
	}
	if(LongRight > 60)
	{
		sum5=(sum5>>1);//halvera vikten
		sum6=(sum6>>1);
		overNoiseShortRightFront=(overNoiseShortRightFront>>1);
		overNoiseShortRightRear=(overNoiseShortRightRear>>1);
	}

	
	int16_t taljare = sum1+sum2+sum3+sum4+sum5+sum6+sum7;
	int16_t namnare = overNoiseLongLeft+overNoiseLongRight+overNoiseShortLeftFront+overNoiseShortLeftRear+overNoiseShortRightFront+overNoiseShortRightRear+overXPosUncert;
	int16_t newX=taljare/namnare;
	setRelativeX(newX);

	int16_t X_inertia = 40;
	int16_t taljare = sum1+sum2+sum3+sum4+sum5+sum6+sum7+getRelativeX()*X_inertia;
	int16_t namnare = overNoiseLongLeft+overNoiseLongRight+overNoiseShortLeftFront+overNoiseShortLeftRear+overNoiseShortRightFront+overNoiseShortRightRear+overXPosUncert+X_inertia;
	int16_t divAns=taljare/namnare;
	setRelativeX(divAns);

	//ta fram y
	int16_t YLongForward = getShiftedSensorY((HALFSQUAREWIDTH-CHASSITOLONGFRONT)-(LongFront<<1));
	int16_t YLongBack = getShiftedSensorY((LongRear<<1)+CHASSITOLONGBACK-HALFSQUAREWIDTH);
	
	glob_sum_y += getVelocity();
	int16_t relYnew = TIMECONSTANT*(glob_sum_y>>10);//int16_t relYnew = getRelativeY()+((TIMECONSTANT*velocity)>>10);
	
	int16_t overYPosUncert = 5; //inkluderar osï¿½kerhet i y pga hast.

	
	taljare = YLongForward*overNoiseLongFront+YLongBack*overNoiseLongRear+overYPosUncert*relYnew;
	namnare = overNoiseLongFront+overNoiseLongRear+overYPosUncert;
	
	int16_t newY = taljare/namnare;
	setRelativeY(newY);
	/*if(glob_sum_y>HALFSQUARESUMY)//if(glob_sum_y > (HALFSQUAREWIDTH<<10)/TIMECONSTANT)
	{
		//moveForwards();//TODO kolla varför denna var o kommenterad. Fungerade med den okomenterad fast en till nedanför
		cbWrite(&glob_debugMesssageBuffer, 23);
		cbWrite(&glob_debugMesssageBuffer, glob_mapX);
		cbWrite(&glob_debugMesssageBuffer, 24);
		cbWrite(&glob_debugMesssageBuffer, glob_mapY);
		glob_sum_y-=2*HALFSQUARESUMY;//2*(HALFSQUAREWIDTH<<10)/TIMECONSTANT;
	}
	
	else if(glob_sum_y<-HALFSQUARESUMY)//if(glob_sum_y < -(HALFSQUAREWIDTH<<10)/TIMECONSTANT)
	{
		glob_sum_y+=2*HALFSQUARESUMY;//2*(HALFSQUAREWIDTH<<10)/TIMECONSTANT;
	}
	*/
	if(getRelativeY() > HALFSQUAREWIDTH)
	{
		glob_sum_y=-HALFSQUARESUMY; //ignorera
		moveForwards();
		cbWrite(&glob_debugMesssageBuffer, 23);
		cbWrite(&glob_debugMesssageBuffer, glob_mapX);
		cbWrite(&glob_debugMesssageBuffer, 24);
		cbWrite(&glob_debugMesssageBuffer, glob_mapY);
	}
	
	//räkna ut täta.
	setTheta(ShortLeftFront, ShortLeftRear, ShortRightFront, ShortRightRear);
		
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
	//V?nsta sidan, endast korta
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
	
	if(glob_gyro < 4 && glob_gyro > -4)
	{
		return;
	}
	int16_t tempTheta = (glob_gyro*TIMECONSTANT)>>2;
	glob_SumTheta += tempTheta;
	glob_theta = glob_SumTheta>>8;
	
	//glob_theta = glob_theta +  (glob_gyro*TIMECONSTANT)>>10;
	//glob_syncSpike = 1;
}
