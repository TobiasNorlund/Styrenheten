	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: calcAngle.c
	 * Skriven av: M. Karlsson
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Används för att beräkna vinkeln global_theta vid rakreglering.
	 */	

#include "calcAngle.h"

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
	//Högra sidan, endast korta
	uint8_t right36AngleK=0;
	if(OK_SENSOR_VALUE(ShortRightFront)&&OK_SENSOR_VALUE(ShortRightRear))
	{
		right36AngleK = 1;
	}
	
	//Vänsta sidan, endast korta
	uint8_t left36AngleK = 0;
	if(OK_SENSOR_VALUE(ShortLeftFront)&&OK_SENSOR_VALUE(ShortLeftRear))
	{
		left36AngleK = 1;
	}

	glob_thetaOld = glob_theta;
	int16_t numerator = calcSideSensors36(ShortLeftFront,ShortLeftRear,0)*left36AngleK + calcSideSensors36(ShortRightFront,ShortRightRear,1)*right36AngleK;
	int16_t denominator = right36AngleK + left36AngleK;
	
	glob_theta = numerator/denominator;
}

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