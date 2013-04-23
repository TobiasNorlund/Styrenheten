/*
 * Styrenheten.c
 *
 * Created: 4/5/2013 11:22:36 AM
 *  Author: carka684 
 */ 

#include "global.h"
#include "clockedInterrupt.h"
#include "charting.h"
#include "StyrReglering.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/spi_master.h"

#include <avr/io.h>
//#include <math.h> //need to install  AVR-LIbc http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=124695&start=0
#include <avr/interrupt.h>
#define F_CPU 20000000UL // 20mhz
#include <util/delay.h>


int8_t init(void)
{
	globals.mapX = 8;
	globals.mapY = 8;
	//external interupt
	EIMSK=(1<<INT0);// enable on int0
	EICRA=(1<<ISC01)|(1<<ISC00);//on rising edge
	
	clockedInterrupt_init();
	reglering_init();
	pathfind_init();
	_delay_ms(500); //ge tid innan styr börjar
	SPI_MASTER_init();
	sei();
}
//nödstopp
ISR(INT0_vect)
{
	setSpeedRight(0);
	setSpeedLeft(0);
	TIMSK0 = (0<<OCIE0A);// disable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	while(1)
	{
		setSpeedRight(0);
		setSpeedLeft(0);
	}
}


void autoSteering()
{
	return;
}

void executeCommand(uint8_t command)
{
	switch(command)
	{
		//implementera alla funktioner och se till att dessa är utan parametrar TODO
		case FORWARD_COMMAND:
			//regulateStraight();
			customSteering();
			break;
		case RIGHT_90_COMMAND:
			//turnRight90();
			break;
		case LEFT_90_COMMAND:
			//turnLeft90();
			break;
		case RIGHT_45_COMMAND:
			//turnRight45();
			break;
		case LEFT_45_COMMAND:
			//turnLeft45();
			break;
		case VIRTUAL_REVERSE_COMMAND:
			virtualTurn();
			break;
		case CUSTOM_STEERING_COMMAND:
			customSteering();
			break;
		default:
			break;
	}
}

void manualSteering()
{
	while(1)
	{
		updateMapManual();
		if(globals.routeLength != 0)
		{
			executeCommand(globals.route[globals.routeLength-1]);
			--globals.routeLength;
		}
	}
}

void turnRight45(int theta, int omega){
	int a  = 1;
	int l1 = 4 / a; //Ska räknas ut på egenvärden!
	int l2 = 4 / a; // -- || --
	int ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int max = - (l1*(3.14/4+theta) + l2*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max;
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
}

int main(void)
{
	init();
	//while(START_PIN == 0);
	if(1) //MANUAL_AUTO_SWITCH_PIN == MANUAL_SELECTED)
	{
		manualSteering();
	}
	else
	{
		autoSteering();
	}
	while(1)
	{
		
	}
}