#define OK_SENSOR_VALUE(x) (((x) != 0)||((x) != 255))
#include <avr/pgmspace.h>


/**
 * Används vid reglering med frontsensorer
 * Tar in ett värde dimension 0.5cm och ger ut ett värde i dimension 0.25 grader.
 */
const uint8_t lookupOpposite[180] PROGMEM = {
	0,26,36,44,51,57,62,67,71,75,79,83,86,89,93,96,98,101,104,107,109,111,114,116,118,121,123,125,127,129,131,132,134,136,138,139,141,143,144,146,147,149,150,152,153,155,156,158,159,160,161,163,164,165,166,168,169,170,171,172,173,174,176,177,178,179,180,181,182,183,184,185,186,187,187,188,189,190,191,192,193,194,194,195,196,197,198,199,199,200,201,202,202,203,204,205,205,206,207,207,208,209,209,210,211,211,212,213,213,214,215,215,216,216,217,218,218,219,219,220,221,221,222,222,223,223,224,224,225,226,226,227,227,228,228,229,229,230,230,231,231,232,232,233,233,233,234,234,235,235,236,236,237,237,237,238,238,239,239,240,240,240,241,241,242,242,242,243,243,244,244,244,245,245,246,246,246,247,247,247,248
}

/** 
 * Används då man använder front- och rear-sensor.
 * Slår upp värdet ifrån diff till ett vinkelvärde i dimension 0.25cm
 */
const uint8_t lookup36[140] PROGMEM = {
	0,3,6,10,13,16,19,22,25,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,74,77,79,82,85,88,90,93,96,98,101,104,106,109,111,114,116,119,121,123,126,128,130,133,135,137,139,141,143,145,147,150,151,153,155,157,159,161,163,165,167,168,170,172,173,175,177,178,180,182,183,185,186,188,189,191,192,193,195,196,198,199,200,202,203,204,205,207,208,209,210,211,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,234,235,236,237,238,239,239,240,241,242,243,243,244,245,246,246,247,248,248,249,250,250
}; 

/**
 * Används då man använder front- och mittsensor.
 * Slår uppp värdet ifrån diff till ett vinkelvärde i dimension 0.5cm
 */
const uint8_t lookup19[130] PROGMEM = {
	0,6,12,18,24,29,35,40,46,51,56,60,65,69,73,77,80,84,87,90,93,96,98,101,103,106,108,110,112,114,115,117,119,120,122,123,124,126,127,128,129,130,131,132,133,134,135,136,137,138,138,139,140,141,141,142,143,143,144,144,145,145,146,146,147,147,148,148,149,149,150,150,150,151,151,152,152,152,153,153,153,154,154,154,155,155,155,155,156,156,156,156,157,157,157,157,158,158,158,158,158,159,159,159,159,159,160,160,160,160,160,161,161,161,161,161,161,162,162,162,162,162,162,162,163,163,163,163,163,163
}	


/**
 * dir = 0 -> positivt theta
 * dir = 1 -> negativt theta
 */
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


int8_t calcOpositeSensors(uint8_t rightFrontSensor,uint8_t leftFrontSensor, int8_t dir)
{
	uint8_t sumOfSensors = rightFrontSensor + leftFrontSensor;
	if(dir == 0)
	{
		return (pgm_read_byte(&(lookupOpposite[sumOfSensors])))>>2;
	}
	else
	{
		return -((pgm_read_byte(&(lookupOpposite[sumOfSensors])))>>2);
	}
}


/**
 * int8_t side, 0 om vänster, 1 om höger
 */
int8_t calcSideSensors36(uint8_t frontDistance,uint8_t rearDistance, int8_t side)
{
	int8_t diff; //Dimension 0.5 cm
	
	//Reglerar mot vänster sida
	if(side == 0)	
	{
		if (frontDistance < rearDistance)
		{
			diff = rearDistance - frontDistance;
			return pgm_read_byte(&(lookup36[diff]))>>2; // >>2 = *0.25 Pga dimensionen är 0.25grader
		}
		else
		{
			diff = frontDistance - rearDistance;
			return -(pgm_read_byte(&(lookup36[diff]))>>2); // >>2 = *0.25 Pga dimensionen är 0.25grader
			
		}
	}	
	//Reglerar mot höger sida			
	else
	{
		if(frontDistance < rearDistance)
		{
			diff = rearDistance - frontDistance;
			return -(pgm_read_byte(&(lookup36[diff]))>>2); // >>2 = *0.25 Pga dimensionen är 0.25grader
		} 
		else
		{
			diff = frontDistance - rearDistance;
			return pgm_read_byte(&(lookup36[diff]))>>2; // >>2 = *0.25 Pga dimensionen är 0.25grader
		}
	}
}

int8_t calcSideSensors19(uint8_t frontDistance,uint8_t longDistance, int8_t side)
{
	uint_8t realFrontDistance = frontDistance>>1; //Ändrar dimensionen till hela cm
	uint8_t diff; //Dimension 1cm
	//Reglerar mot vänster sida
	if(side == 0)
	{
		if (realFrontDistance < rearDistance)
		{
			diff = rearDistance - realFrontDistance;
			return pgm_read_byte(&(lookup36[diff]))>>1; // >>1 = *0.5 Pga dimensionen är 0.5 grader
		}
		else
		{
			diff = realFrontDistance - rearDistance;
			return -(pgm_read_byte(&(lookup36[diff]))>>1); // >>1 = *0.5 Pga dimensionen är 0.5 grade
		}
	}
	//Reglerar mot höger sida
	else
	{
				if (realFrontDistance < rearDistance)
				{
					diff = rearDistance - realFrontDistance;
					return -(pgm_read_byte(&(lookup36[diff]))>>1); // >>1 = *0.5 Pga dimensionen är 0.5 grader
				}
				else
				{
					diff = realFrontDistance - rearDistance;
					return pgm_read_byte(&(lookup36[diff]))>>1; // >>1 = *0.5 Pga dimensionen är 0.5 grade
				}
	}
}
