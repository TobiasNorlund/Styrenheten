#include "global.h"
#include <avr/io.h>

void interrupt-beta()
{
	/**
	 Begär data från sensorenheten
	 Får in data från 6 optiska sensorer
     och gyrot
     */
	
	// ---------- Variabler ----------
	// Aståndet mellan sensorerna i Y-led. Dvs mellan fronLeftSensor&backLeftSensor,
	// samt frontRightSensor, backRightSensor.	
	int sensorDistance = 15;	
	
	// Tiden mellan två timade avbrott. Behövs för att kunna a ut derivatorna v samt omega.
	int t;
	
	// Observatörens variabler
	int x;
	int xOld;
	int theta;
	int thetaOld;
	int v;
	int omega;
	
	
	// ----------- Funktioner --------------
	// Av anledningen att x beror på theta kan det vara en idé att alltid uppdatera
	// theta för
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
	
	
	// Hur gör vi för att lagra gamla värden vilket behövs för att ta reda på V samt OMEGA?
	// Bör man skapa en array med två värden och hela tiden lagra över så vi får det nyaste
	// att jobba med?
}