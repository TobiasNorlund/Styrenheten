/*
 * clockedInterrupt.c
 *
 * Created: 4/12/2013 10:37:35 AM
 *  Author: davmo049
 */ 

#include <avr/interrupt.h>
#include "spi_master.h"

void timedInterrupt()
{
	//skicka request till sensorenehten att skicka data
	uint8_t msgSend[4];
	SPI_MASTER_write(msgSend, 1);
	
	
	//ta emot data från sensorenheten
	uint8_t msgRecieve[32];
	int* len;
	SPI_MASTER_read(msgRecieve, len);
	
	//
}		


ISR(SPI_STC_vect)
{
	if( 1 ) //clockat interupt har kommit)
	{
		timedInterrupt();
	}
	else if(1)//annat..)
	{
		
	}
	else
	{
		
	}
}