/*
 * Styrenheten.c
 *
 * Created: 4/5/2013 11:22:36 AM
 *  Author: carka684 
 */ 

#include "global.h"
#include "clockedInterrupt.h"
#include "../../TSEA27-include/message.h"
#include <avr/io.h>
#include <math.h>

int8_t init(void)
{
	SPI_MASTER_init();
	clockedInterrupt_init();
	reglering_init();
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
		//updateMapManual();
		if(routeLength != 0)
		{
			executeCommand(route[routeLength-1]);
			--routeLength;
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
	while(START_PIN == 0);
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