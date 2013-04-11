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
		
		Vid körning rakt fram:
		xVänster = (vänsterBak+vänsterFram)*(1-theta*theta/2)/2;
		xHöger = (högerBak+högerFram)*(1-theta*theta/2)/2;
		
		Om(xVänster < xHöger)
		{
			Reglera mot vänster vägg
			
			thetaVägg = (högerBak-högerFram)/avståndet mellan sensorerna	
		}			
		Annars
		{
			Reglera mot höger vägg
			
			thetaVägg = (vänsterFram-vänsterBak)/avståndet mellan sensorerna
		}		
		        

        omegaVägg = (thetaVägg[n]-thetaVägg [n-1])/T, T är tiden mellan sensorhämtningarna.
		v = (x[n] - x[n-1])/T

    */
	
	
}