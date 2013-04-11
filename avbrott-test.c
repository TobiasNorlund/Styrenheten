#include "global.h"
#include <avr/io.h>

void interrupt-beta()
{
	
	    /**
        Begär data från sensorenheten
        Får in data från 6 optiska sensorer
        och gyrot


        Får in:

        vänsterFram
        VänsterBak
        högerFram
        högerBak     

        omegaVägg = (thetaVägg[n]-thetaVägg [n-1])/T, T är tiden mellan sensorhämtningarna.
		v = (x[n] - x[n-1])/T

    */
		
	// Aståndet mellan sensorerna i Y-led. Dvs mellan fronLeftSensor&backLeftSensor, samt frontRightSensor, backRightSensor.
	int sensorDistance = 15;
	
	
	// Av anledningen att x beror på theta kan det vara en idé att alltid uppdatera
	// theta för
	void setXLeft(int frontLeftSensor, int backLeftSensor, int theta)
	{
		x = (frontLeftSensor+backLeftSensor)*(2 - theta^2)/4
		return;
	}		
	
	void setXRight(int frontRightSensor, int backRightSensor, int theta)
	{
		x = (fronRightSensor + backRightSensor)*(2 - theta^2)/4;
		return;
	}		
	
	void setThetaLeft(int frontLeftSensor, int backLeftSensor)
	{
		theta = (frontLeftSensor - backLeftSensor)/sensorDistance;
		return;
	}
	
	void setThetaRight(int frontRightSensor, int backRightSensor)
	{
		theta = (backRightSensor - frontRightSensor)/sensorDistance;
		return;
	}
	
	// Hur gör vi för att lagra gamla värden vilket behövs för att ta reda på V samt OMEGA?
	// Bör man skapa en array med två värden och hela tiden lagra över så vi får det nyaste
	// att jobba med?
}