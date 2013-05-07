﻿/*
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



void init(void)
{
	glob_mapX = 8;
	glob_mapY = 8;
	glob_L1_straightX=8;
	glob_L2_straightTheta=12;
	glob_L3_straightOmega=4;
	glob_L1_turnTheta=12;
	glob_L2_turnOmega=4;
	
	clockedInterrupt_init();
	reglering_init();
	pathfind_init();
	_delay_ms(500); //ge tid innan styr börjar
	SPI_MASTER_init();
	sei();
	
	DDRB |= 0b00000001;
}

void signal_done()
{
	//TODO
	return;
}


void executeCommand(uint8_t command)
{
	glob_curComm = command;
	switch(command)
	{
		//implementera alla funktioner och se till att dessa är utan parametrar TODO
		case FORWARD_COMMAND:
			regulateStraight();
			break;
		case RIGHT_90_COMMAND:
			turnRight90();
			break;
		case LEFT_90_COMMAND:
			turnLeft90();
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
		
		//updateMapManual();
		if(glob_routeLength != 0)
		{
		
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
	while(!START_PIN);
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
