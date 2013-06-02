/*
 * Styrenheten.c
 *
 * Created: 4/5/2013 11:22:36 AM
 *  Author: carka684 
 */

// in "Makefile" 
//#define F_CPU 20000000UL // 20mhz
#include <util/delay.h>

#include "global.h"
#include "clockedInterrupt.h"
#include "charting.h"
#include "StyrReglering.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/spi_master.h"
#include "pathfind.h"

#include <avr/io.h>
#include <avr/interrupt.h>


void sendAllMapData()
{
	for(uint8_t i = 0; i < 16; ++i)
	{
		for(uint8_t j = 0; j < 16; ++j)
		{
			if(glob_map[j][i] != UNKNOWN)
			{
				cbWrite(&glob_mapDataToSend, i);
				cbWrite(&glob_mapDataToSend, j);
			}
		}
	}
}


void init(void)
{
	glob_mapX = 8;
	glob_mapY = 8;
	glob_L1_straightX=10;
	glob_L2_straightTheta=20;
	glob_L3_straightOmega=4;
	glob_L1_turnTheta=12;
	glob_L2_turnOmega=4;
	
	clockedInterrupt_init();
	reglering_init();
	pathfind_init();
	_delay_ms(500); //ge tid innan styr börjar
	SPI_MASTER_init();
	sei();
	
	DDRB |= 0b00000101;
}

void signal_done()
{
	PORTB |= 0b00000100;
	while(1)
	{
		sendAllMapData();
		_delay_ms(1000);
		
	}
}


void executeCommand(uint8_t command)
{
	glob_curComm = command;
	switch(command)
	{
		case FORWARD_COMMAND:
			regulateStraight();
			//if(!(glob_routeLength != 1 && glob_route[glob_routeLength-2] == FORWARD_COMMAND))
			//{
				_delay_ms(500);	
			//}
			break;
		case RIGHT_90_COMMAND:
			turnRight90();
			_delay_ms(300);
			break;
		case LEFT_90_COMMAND:
			turnLeft90();
			_delay_ms(300);
			break;
		case RIGHT_45_COMMAND:
			turnRight45();
			break;
		case LEFT_45_COMMAND:
			turnLeft45();
			break;
		case VIRTUAL_REVERSE_COMMAND:
			virtualTurn();
			break;
		case CUSTOM_STEERING_COMMAND:
			sendAllMapData();
			customSteering();
			break;
		default:
			break;
	}
	glob_curComm = NULL_COMMAND;
}

void autoSteering()
{
	while(1)
	{
		updateMapAuto();
		if(glob_shouldPathfind)
		{
			pathfind();
		}
		if(glob_routeLength != 0)
		{
			executeCommand(glob_route[glob_routeLength-1]);
			--glob_routeLength;
		}
		else
		{
			signal_done();
			return;
		}
	}
	return;
}
#pragma GCC push_options
#pragma GCC optimize ("O0")
void manualSteering()
{
	while(1)
	{
		if(glob_routeLength != 0)
		{
			updateMapManual();
			executeCommand(glob_route[glob_routeLength-1]);
			glob_routeLength--;
		}
	}
}
#pragma GCC pop_options
//end turn off optimization


int main(void)
{
	init();
	volatile uint8_t start_pin = START_PIN;
	while(!start_pin)
	{
		start_pin = START_PIN;
	}
	if(MANUAL_AUTO_SWITCH_PIN) 
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
