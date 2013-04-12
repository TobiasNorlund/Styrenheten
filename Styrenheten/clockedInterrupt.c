/*
 * clockedInterrupt.c
 *
 * Created: 4/12/2013 10:37:35 AM
 *  Author: davmo049
 */ 

#include <avr/interrupt.h>

void timedInterrupt()
{
	//skicka request till sensorenehten att skicka data
	
	//ta emot data från sensorenheten
	
	//
}		


ISR(SPI_STC_vect)
{
	if(//clockat interupt har kommit)
	{
		timedInterrupt();
	}
	else if(//annat..)
	else
	{
		
	}
}