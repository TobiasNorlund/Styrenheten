#include "global.h"
#include <avr/io.h>

void interrupt-beta()
{
	/**
	 Beg�r data fr�n sensorenheten
	 F�r in data fr�n 6 optiska sensorer
     och gyrot
     */
	
	// ---------- Variabler ----------
	// Ast�ndet mellan sensorerna i Y-led. Dvs mellan fronLeftSensor&backLeftSensor,
	// samt frontRightSensor, backRightSensor.	
	int sensorDistance = 15;	
	
	// Tiden mellan tv� timade avbrott. Beh�vs f�r att kunna a ut derivatorna v samt omega.
	int t;
	
	// Observat�rens variabler
	int x;
	int xOld;
	int theta;
	int thetaOld;
	int v;
	int omega;
	
	
	// ----------- Funktioner --------------
	// Av anledningen att x beror p� theta kan det vara en id� att alltid uppdatera
	// theta f�r
	void setXV(int frontSensor, int backSensor, int theta)
	{
		xOld = x;															
		x = (frontSensor + backSensor) * (2 - theta^2)/4
		v = (x - xOld)/t;
		return;
	}			
	
	void setThetaOmegaLeft(int frontLeftSensor, int backLeftSensor)
	{
		thetaOld = theta;
		theta = (frontLeftSensor - backLeftSensor)/sensorDistance;
		omega = (theta - thetaOld)/t;
		return;
	}
	
	void setThetaOmegaRight(int frontRightSensor, int backRightSensor)
	{
		thetaOld = theta;
		theta = (backRightSensor - frontRightSensor)/sensorDistance;
		omega = (theta - thetaOld)/t;
		return;
	}
	
	void setObserver(int frontLeftSensor, int backLeftSensor, int frontRightSensor, int backRightSensor)
	{
		if((frontLeftSensor + backLeftSensor)/2 < (frontRightSensor + backRightSensor)/2))
		{
			setThetaOmegaLeft(frontLeftSensor, backLeftSensor);
			setXV(frontLeftSensor, backLeftSensor);
		}
		else
		{
			setThetaOmegaRight(frontRightSensor, backRightSensor);
			setXV(frontRightSensor,backRightSensor);
		}
		return;
	}
	
	
	// Hur g�r vi f�r att lagra gamla v�rden vilket beh�vs f�r att ta reda p� V samt OMEGA?
	// B�r man skapa en array med tv� v�rden och hela tiden lagra �ver s� vi f�r det nyaste
	// att jobba med?
}