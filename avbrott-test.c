#include "global.h"
#include <avr/io.h>

void interrupt-beta()
{
	
	    /**
        Beg�r data fr�n sensorenheten
        F�r in data fr�n 6 optiska sensorer
        och gyrot


        F�r in:

        v�nsterFram
        V�nsterBak
        h�gerFram
        h�gerBak     

        omegaV�gg = (thetaV�gg[n]-thetaV�gg [n-1])/T, T �r tiden mellan sensorh�mtningarna.
		v = (x[n] - x[n-1])/T

    */
		
	// Ast�ndet mellan sensorerna i Y-led. Dvs mellan fronLeftSensor&backLeftSensor, samt frontRightSensor, backRightSensor.
	int sensorDistance = 15;
	
	
	// Av anledningen att x beror p� theta kan det vara en id� att alltid uppdatera
	// theta f�r
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
	
	// Hur g�r vi f�r att lagra gamla v�rden vilket beh�vs f�r att ta reda p� V samt OMEGA?
	// B�r man skapa en array med tv� v�rden och hela tiden lagra �ver s� vi f�r det nyaste
	// att jobba med?
}