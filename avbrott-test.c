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
		
		Vid k�rning rakt fram:
		xV�nster = (v�nsterBak+v�nsterFram)*(1-theta*theta/2)/2;
		xH�ger = (h�gerBak+h�gerFram)*(1-theta*theta/2)/2;
		
		Om(xV�nster < xH�ger)
		{
			Reglera mot v�nster v�gg
			
			thetaV�gg = (h�gerBak-h�gerFram)/avst�ndet mellan sensorerna	
		}			
		Annars
		{
			Reglera mot h�ger v�gg
			
			thetaV�gg = (v�nsterFram-v�nsterBak)/avst�ndet mellan sensorerna
		}		
		        

        omegaV�gg = (thetaV�gg[n]-thetaV�gg [n-1])/T, T �r tiden mellan sensorh�mtningarna.
		v = (x[n] - x[n-1])/T

    */
	
	
}