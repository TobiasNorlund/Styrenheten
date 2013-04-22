#define OK_SENSOR_VALUE(x) (((x) != 0)||((x) != 255))
#include <avr/pgmspace.h>


const uint8_t lookupShort[140] PROGMEM = {
	0,3,6,10,13,16,19,22,25,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,74,77,79,82,85,88,90,93,96,98,101,104,106,109,111,114,116,119,121,123,126,128,130,133,135,137,139,141,143,145,147,150,151,153,155,157,159,161,163,165,167,168,170,172,173,175,177,178,180,182,183,185,186,188,189,191,192,193,195,196,198,199,200,202,203,204,205,207,208,209,210,211,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,234,235,236,237,238,239,239,240,241,242,243,243,244,245,246,246,247,248,248,249,250,250
};


void calcAngle(int8_t dir)
{
	uint8_t sensorLongForward=getSensorLongForward();
	uint8_t sensorLongRear=getSensorLongRear();
	uint8_t sensorLongLeft=getSensorLongLeft();
	uint8_t sensorLongRight=getSensorLongRight();
	uint8_t sensorShortLeftForward=getSensorShortLeftForward();
	uint8_t sensorShortLeftRear=getSensorShortLeftRear();
	uint8_t sensorShortRightForward=getSensorShortRightForward();
	uint8_t sensorShortRightRear=getSensorShortRightRear();

	//Endast främre
	uint8_t frontAngleK=0;
	int8_t frontAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortLeftForward)&&OK_SENSOR_VALUE(sensorShortRightForward))
	{
		frontAngleK=calcKSensorShort(sensorShortLeftForward)*calcKSensorShort(sensorShortRightForward);
		frontAngleDiff=currentTheta-calcOpositeSensors(sensorShortRightForward,sensorShortLeftForward,dir);
	}

	//Endast långa
	uint8_t middleAngleK=0;
	int8_t middleAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorLongLeft)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		middleAngleK=calcKSensorLong(sensorLongLeft)*calcKSensorLong(sensorLongRight);
		middleAngleDiff=currentTheta-calcOpositeSensors(sensorLongRight,sensorLongLeft,dir);
	}
	
	//Endast bakre
	uint8_t backAngleK=0;
	int8_t backAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortLeftRear)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		backAngleK=calcKSensorShort(sensorShortLeftRear)*calcKSensorShort(sensorShortRightRear);
		backAngleDiff=currentTheta-calcOpositeSensors(sensorShortRightRear,sensorShortLeftRear,dir);
	}
	//Högra sidan, endast korta
	uint8_t right36AngleK=0;
	int8_t right36AngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorShortRightRear))
	{
		right36AngleK=calcKSensorShort(sensorShortRightForward)*calcKSensorShort(sensorShortRightRear);
		right36AngleDiff=currentTheta-calcSideSensors36(sensorShortRightForward,sensorShortRightRear,1);
	}

	//Högra sidan, främre kort & långa 
	uint8_t right19FAngleK=0;
	int8_t right19FAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortRightForward)&&OK_SENSOR_VALUE(sensorLongRight))
	{
		right19FAngleK=calcKSensorShort(sensorShortRightForward)*calcKSensorLong(sensorLongRight);
		right19FAngleDiff=currentTheta-calcSideSensors19(sensorShortRightForward,sensorLongRight,dir);
	}

	//Vänsta sidan, endast korta
	uint8_t left36AngleK=0;
	int8_t left36AngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorShortleftRear))
	{
		left36AngleK=calcKSensorShort(sensorShortleftForward)*calcKSensorShort(sensorShortleftRear);
		left36AngleDiff=currentTheta-calcSideSensors36(sensorShortleftForward,sensorShortleftRear,0);
	}

	//Vänstra sidan, främre kort & långa
	uint8_t left19FAngleK=0;
	int8_t left19FAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftForward)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		left19FAngleK=calcKSensorShort(sensorShortleftForward)*calcKSensorLong(sensorLongleft);
		left19FAngleDiff=currentTheta-calcSideSensors19(sensorShortleftForward,sensorLongleft,dir);
	}
	
	//Räkna ut från tidigare vinkelhastighet*tid
	uint8_t angleSpeedAngleK=0;
	int8_t angleSpeedAngleDiff=0;
	if(OK_SENSOR_VALUE(sensorShortleftRear)&&OK_SENSOR_VALUE(sensorLongleft))
	{
		angleSpeedAngleK=calcKOmega(omega);
		angleSpeedAngleDiff=omega*T;//TODO
	}
	
	uint8_t irSensorK = calcKirSensorK(currentTheta);

	int16_t taljare = irSensorK*(frontAngleK*frontAngleDiff + middleAngleK*middleAngleDiff + backAngleK*backAngleDiff + right36AngleDiff*right36AngleK + right19FAngleDiff*right19FAngleK +  left36AngleDiff*left36AngleK + left19FAngleDiff*left19FAngleK) + angleSpeedAngleDiff*angleSpeedAngleK;
	int16_t namnare = irSensorK*(frontAngleK + middleAngleK + backAngleK + right36AngleK + right19FAngleK + left36AngleK + left19FAngleK) + angleSpeedAngleK;

	int16_t newAngleDiff = taljare/namnare;
	newAngle=currentTheta-newAngleDiff;
}

//max vikt 5, 5*5*90*9=20250 dvs inte risk för overflow
uint8_t calcKSensorShort(uint8_t a)
{
	// 10-80 cm * 2 pga konstant i sensorenheten 80 / 4 = 20
	//10/4= 2.5 
	// 20-2.5 = 17.5
	// 17.5/2= 8.75
	// 8.75/2= 4.375 ca 5  dvs max 5 ut.
	return (20-(a>>3))>>2; //kom på något bra för att straffa om värdet är lågt.
}

uint8_t calcKSensorLong(uint8_t a)// 20-150 cm 150/8=18.75 CA 20
{
	// 20/8= 2.5
	// 20-2.5 = 17.5
	// 17.5/2= 8.75
	// 8.75/2= 4.375 ca 5 dvs max 5 ut.
	return (20-(a>>3))>>2; //kom på något bra för att straffa om värdet är lågt.
}
uint8_t calcKOmega(int8_t omega)
{
	return (omega*omega)>>2;//TODO fixa så ju högre vinkelhastighet, destå högre K
}

uint8_t calcKirSensorK(uint8_t angle)
{
	// 90 / 8 = 11.25
	// 90 / 16 = 5.625 
	return (90-angle)>>4;//TODO ju högre vinkel destå lägre K, fixa så den växer snabbare.
}


int8_t calcOpositeSensors(uint8_t r,uint8_t l, int8_t side)
{
	//lookup table

	return ;//times dir
}


/**
 * int8_t side, 0 om vänster, 1 om höger
 */
int8_t calcSideSensors36(uint8_t frontDistance,uint8_t rearDistance, int8_t side)
{
	int8_t diff; //Dimension 0.5 cm
	if(side == 0)	//Reglerar mot vänster sida
	{
		if (frontDistance < rearDistance)
		{
			diff = rearDistance - frontDistance;
			return pgm_read_byte(&(lookupShort[diff]))>>2;
		}
		else
		{
			diff = frontDistance - rearDistance;
			return -(pgm_read_byte(&(lookupShort[diff]))>>2);
			
		}
	}				
	else			//Reglerar mot höger sida
	{
		if(frontDistance < rearDistance)
		{
			diff = rearDistance - frontDistance;
			return -(pgm_read_byte(&(lookupShort[diff]))>>2);
		}
		else
		{
			diff = frontDistance - rearDistance;
			return pgm_read_byte(&(lookupShort[diff]))>>2;
		}
	}
}
	
int8_t calcSideSensors19(uint8_t frontDistance,uint8_t longDistance, int8_t side)
{
	//lookup table

	return ;//times dir
}
