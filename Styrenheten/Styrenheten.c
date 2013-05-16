	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul: Styrenheten
	 * Filnamn: Styrenheten.c
	 * Skriven av: C. Karlsson Schmidt, D. Molin			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Main-fil för styrenheten.
	 */	
	
// in "Makefile" 
//#define F_CPU 20000000UL // 20mhz

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"
#include "clockedInterrupt.h"
#include "charting.h"
#include "StyrReglering.h"
#include "pathfind.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/spi_master.h"


#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY 500
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

/** 
 * Initierar styrenheten.
 */
void init(void)
{
	glob_mapX = 8;
	glob_mapY = 8;
	glob_L1_straightX=4;
	glob_L2_straightTheta=16;
	glob_L3_straightOmega=4;
	glob_L1_turnTheta=12;
	glob_L2_turnOmega=4;
	
	clockedInterrupt_init();
	regulate_init();
	pathfind_init();
	_delay_ms(500); //ge tid innan styr börjar
	SPI_MASTER_init();
	sei();
	
	DDRB |= 0b00000101;
}

/** 
 * Tänder en lampa på roboten och skickar kartdata
 * kontinuerligt tills reset. 
 */
void signal_done()
{
	PORTB |= 0b00000100;
	while(1)
	{
		sendAllMapData();
		_delay_ms(1000);
		
	}
}

/** 
 * Anropar nuvarande kommando i kön
 */
void executeCommand(uint8_t command)
{
	glob_curComm = command;
	glob_syncSpike = 255;
	switch(command)
	{
		case FORWARD_COMMAND:
			regulateStraight();
			break;
		case RIGHT_90_COMMAND:
			turnRight90();
			_delay_ms(DELAY);
			break;
		case LEFT_90_COMMAND:
			turnLeft90();
			_delay_ms(DELAY);
			break;
		case RIGHT_45_COMMAND:
			turnRight45();
			_delay_ms(DELAY);
			break;
		case LEFT_45_COMMAND:
			turnLeft45();
			_delay_ms(DELAY);
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

/** 
 * Kör autonomt
 */
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

/** 
 * Kör manuellt
 */
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
