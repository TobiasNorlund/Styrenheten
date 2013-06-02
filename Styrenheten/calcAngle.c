
#include <avr/pgmspace.h>
#include "calcAngle.h"

#define RIGHT_SIDE 1
#define LEFT_SIDE 0

/**
 * Används vid reglering med frontsensorer 
 * Tar in ett värde dimension 0.5cm och ger ut ett värde i dimension 0.25 grader.
 */
const uint8_t lookupOpposite[181] PROGMEM = {
	0,26,36,44,51,57,62,67,71,75,79,83,86,89,93,96,98,101,104,107,109,111,114,116,118,121,123,125,127,129,131,132,134,136,138,139,141,143,144,146,147,149,150,152,153,155,156,158,159,160,161,163,164,165,166,168,169,170,171,172,173,174,176,177,178,179,180,181,182,183,184,185,186,187,187,188,189,190,191,192,193,194,194,195,196,197,198,199,199,200,201,202,202,203,204,205,205,206,207,207,208,209,209,210,211,211,212,213,213,214,215,215,216,216,217,218,218,219,219,220,221,221,222,222,223,223,224,224,225,226,226,227,227,228,228,229,229,230,230,231,231,232,232,233,233,233,234,234,235,235,236,236,237,237,237,238,238,239,239,240,240,240,241,241,242,242,242,243,243,244,244,244,245,245,246,246,246,247,247,247,248
};

/** 
 * Används då man använder front- och rear-sensor.
 * Slår upp värdet ifrån diff till ett vinkelvärde i dimension 0.25 grader.
 */
const uint8_t lookup36[140] PROGMEM = {
	0,3,6,10,13,16,19,22,25,29,32,35,38,41,44,47,50,53,56,59,62,65,68,71,74,77,79,82,85,88,90,93,96,98,101,104,106,109,111,114,116,119,121,123,126,128,130,133,135,137,139,141,143,145,147,150,151,153,155,157,159,161,163,165,167,168,170,172,173,175,177,178,180,182,183,185,186,188,189,191,192,193,195,196,198,199,200,202,203,204,205,207,208,209,210,211,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,234,235,236,237,238,239,239,240,241,242,243,243,244,245,246,246,247,248,248,249,250,250
}; 

/**
 * Används då man använder front- och mittsensor.
 * Slår uppp värdet ifrån diff till ett vinkelvärde i dimension 0.5 grader.
 */
const uint8_t lookup19[130] PROGMEM = {
	0,6,12,18,24,29,35,40,46,51,56,60,65,69,73,77,80,84,87,90,93,96,98,101,103,106,108,110,112,114,115,117,119,120,122,123,124,126,127,128,129,130,131,132,133,134,135,136,137,138,138,139,140,141,141,142,143,143,144,144,145,145,146,146,147,147,148,148,149,149,150,150,150,151,151,152,152,152,153,153,153,154,154,154,155,155,155,155,156,156,156,156,157,157,157,157,158,158,158,158,158,159,159,159,159,159,160,160,160,160,160,161,161,161,161,161,161,162,162,162,162,162,162,162,163,163,163,163,163,163
};


void setTheta(uint8_t ShortLeftFront, uint8_t ShortLeftRear, uint8_t ShortRightFront, uint8_t ShortRightRear)
{
	calcThetaWheels();//update wheel sum
	int16_t thetaWheels = glob_omegaWheelSum>>2;// delar med 16, hjulbasen är 17 cm dock vill vi ha i 1/4 grader så delar bara med 4:a
	int16_t thetaWeightWheels = 1;//TODO tune vs thetaWeight tröghet för gamla värden
	//Högra sidan, endast korta
	uint8_t right36AngleK=0;
	
	if((getSensorShortOverNoise(ShortRightFront, getSensorShortRightForwardOld()) != 0)&&(getSensorShortOverNoise(ShortRightRear, getSensorShortRightRearOld()) != 0) && 35 > max(ShortRightFront-ShortRightRear, ShortRightFront-ShortRightRear))
	{
		right36AngleK = 10;
		thetaWeightWheels = 0;//reglera inte efter hjulen om vi har ok värden från sensorerna
	}
	
	//Vänsta sidan, endast korta
	uint8_t left36AngleK = 0;
	if((getSensorShortOverNoise(ShortLeftFront, getSensorShortLeftForwardOld()) != 0)&&(getSensorShortOverNoise(ShortLeftRear, getSensorShortLeftRearOld()) != 0) && 35 > max(ShortLeftFront-ShortLeftRear, ShortLeftRear-ShortLeftFront))
	{
		left36AngleK = 10;
		thetaWeightWheels = 0;//reglera inte efter hjulen om vi har ok värden från sensorerna
	}
	
	//om vi är långt från vägg minska vikten på de korta sensorerna.
	if(getSensorLongLeft() > 60)
	{
		left36AngleK=0;
	}
	if(getSensorLongRight() > 60)
	{
		right36AngleK=0;
	}
	
	//om vi långa är kortare än båda korta betyder det att vi har ett hörn.
	if(((getSensorLongLeft()+4)< (ShortLeftFront>>1)) && ((getSensorLongLeft()+4)< (ShortLeftRear>>1))&&OK_SENSOR_VALUE(getSensorLongLeft()))
	{
		left36AngleK=0;
	}
	if(((getSensorLongRight()+4) < (ShortRightFront>>1))&&((getSensorLongRight()+4) < (ShortRightRear))&&OK_SENSOR_VALUE(getSensorLongRight()))
	{
		right36AngleK=0;
	}


	glob_thetaOld = glob_theta;
	int16_t thetaWeight = 2;// tröghet för gamla värden
	int16_t thetaReg = int8to16(glob_theta)<<2;

	int16_t leftSide36 = calcSideSensors36(ShortLeftFront,ShortLeftRear,LEFT_SIDE);
	int16_t rightSide36 = calcSideSensors36(ShortRightFront,ShortRightRear,RIGHT_SIDE);
	
/*	if(max(leftSide36-thetaReg,thetaReg-leftSide36) > 5*4)
	{
		left36AngleK=0;
	}
	if(max(rightSide36-thetaReg,thetaReg-rightSide36) > 5*4)
	{
		right36AngleK=0;
	}*/
	//i 1/4 grader
	int16_t numerator = leftSide36*left36AngleK + rightSide36*right36AngleK + thetaWeight*thetaReg;// + thetaWheels*thetaWeightWheels;
	int16_t denominator = right36AngleK + left36AngleK + thetaWeight;// + thetaWeightWheels;
	

	int16_t newTheta = numerator/denominator;
	glob_theta = newTheta>>2;
	if(thetaWeightWheels==0)//uppdatera summan om vi har en bättre skattning från andra sensorer.
	{
		glob_omegaWheelSum=newTheta<<2;
	}
}
//turn off optimization
#pragma GCC push_options
#pragma GCC optimize ("O0")
void calcThetaWheels()
{
	if(OK_SENSOR_VALUE(glob_vLeft)&&OK_SENSOR_VALUE(glob_vRight)&&(glob_vRight > 60)&&(glob_vLeft > 60))
	{	
		glob_omegaWheelSum+=(glob_vRightSign*(int16_t)glob_vRight-glob_vLeftSign*(int16_t)glob_vLeft);//TODO fix if +-90*16 grader
	}
	
}
#pragma GCC pop_options
//end turn off optimization

//Värde mellan 0 och 25
uint8_t calcOppositeShortK(uint8_t leftShortSensor, uint8_t rightShortSensor)
{
	return (leftShortSensor + rightShortSensor - 130)>>3;
}

/**
 * Eftersom leftLongSensor&rightLongSensor högst kan ge 150. Borde då max return bli
 * (150+150-100)>>3 = 200/8 = 25
 */
uint8_t calcOppositeLongK(uint8_t leftLongSensor, uint8_t rightLongSensor)
{
	uint8_t K;
	uint8_t sumOfSensors = leftLongSensor + rightLongSensor;
	if(sumOfSensors <= 100)
	{
		return 0;
	}
	else
	{
		return (sumOfSensors - 100)>>3;
	}
}

// Hur ska denna vikta? Så fort vi kan reglera med denna 
// vill vi ju i princip göra det?
uint8_t calc36K(uint8_t shortFront, uint8_t shortRear)
{
	if(shortFront > shortRear)
	{
		if(shortFront-shortRear>40)
		{
			return 0;
		}
	}
	else
	{
		if(shortRear-shortFront>40)
		{
			return 0;
		}
	}
	return 25;//TODO Ordentligare
}

// När ska denna straffas?
uint8_t calc19K(uint8_t shortSensor, uint8_t longSensor)
{
	return 10; //TODO Ordentligare
}


uint8_t calcKOmega(uint16_t omega)
{
	if (omega < 0)
	{
		return (-omega)>>4; // TODO Ordentligare
	}
	else
	{
		return omega>>4; // TODO Ordentligare
	} 
}

uint8_t calcKirSensorK(uint8_t angle)
{
	// 90 / 8 = 11.25
	// 90 / 16 = 5.625 
	return (90-angle)>>3;//TODO Ordentligare
}

/*
 * return: [grader]
 */
int8_t calcOppositeSensors(uint8_t rightFrontSensor,uint8_t leftFrontSensor)
{
	uint8_t sumOfSensors = rightFrontSensor + leftFrontSensor;
	if(glob_theta > 0)
	{
		return (pgm_read_byte(&(lookupOpposite[sumOfSensors])));
	}
	else
	{
		return -((pgm_read_byte(&(lookupOpposite[sumOfSensors]))));
	}
}

/**
 * int8_t side, 0 om vänster, 1 om höger
 * return: [grader] *0.25 Pga dimensionen är 0.25grader
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
			return pgm_read_byte(&(lookup36[diff])); // >>2 = *0.25 Pga dimensionen är 0.25grader
		}
		else
		{
			diff = frontDistance - rearDistance;
			return -(pgm_read_byte(&(lookup36[diff]))); // >>2 = *0.25 Pga dimensionen är 0.25grader
			
		}
	}	
	//Reglerar mot höger sida			
	else
	{
		if(frontDistance < rearDistance)
		{
			diff = rearDistance - frontDistance;
			return -(pgm_read_byte(&(lookup36[diff]))); // >>2 = *0.25 Pga dimensionen är 0.25grader
		} 
		else
		{
			diff = frontDistance - rearDistance;
			
			return pgm_read_byte(&(lookup36[diff])); // >>2 = *0.25 Pga dimensionen är 0.25grader
		}
	}
}

/*
 * return [grader] 0.5 Pga dimensionen är 0.5 grade
 */
int8_t calcSideSensors19(uint8_t frontDistance,uint8_t longDistance, int8_t side)
{
	uint8_t realFrontDistance = frontDistance>>SHORTFACTOR-3; //Ändrar dimensionen till hela cm -3 pga att korta sitter längre in på chassit.
	uint8_t diff; //Dimension 1cm
	//Reglerar mot vänster sida
	if(side == 0)
	{
		if (realFrontDistance < longDistance)
		{
			diff = longDistance - realFrontDistance;
			return pgm_read_byte(&(lookup19[diff])); // >>1 = *0.5 Pga dimensionen är 0.5 grader
		}
		else
		{
			diff = realFrontDistance - longDistance;
			return -(pgm_read_byte(&(lookup19[diff]))); // >>1 = *0.5 Pga dimensionen är 0.5 grade
		}
	}
	//Reglerar mot höger sida
	else
	{
				if (realFrontDistance < longDistance)
				{
					diff = longDistance - realFrontDistance;
					return -(pgm_read_byte(&(lookup19[diff]))); // >>1 = *0.5 Pga dimensionen är 0.5 grader
				}
				else
				{
					diff = realFrontDistance - longDistance;
					return pgm_read_byte(&(lookup19[diff])); // >>1 = *0.5 Pga dimensionen är 0.5 grade
				}
	}
	return 0;
}
