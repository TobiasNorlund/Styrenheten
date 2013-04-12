/*
 * clockedInterrupt.c
 *
 * Created: 4/12/2013 10:37:35 AM
 *  Author: davmo049
 */ 

#include <avr/interrupt.h>
#include "spi_master.h"
#include "global.h"
#include "commands.h"

void timedInterrupt()
{
	//skicka request till sensorenehten att skicka data
	uint8_t msgSend[32];
	
	SPI_MASTER_write(msgSend, SENSORDATA_REQUEST, 0);
	
	//pausa??
	
	//ta emot data från sensorenheten
	uint8_t msgRecieve[32];
	uint8_t* type;
	uint8_t* len;
	SPI_MASTER_read(msgRecieve, type, len);
	//skicka vidare till PC
	SPI_MASTER_write(msgRecieve, SEND_DEBUG_DATAPC, *len);
	//skriv in data
	avstandsensor_1 = msgRecieve[0];
	avstandsensor_2 = msgRecieve[1];
	avstandsensor_3 = msgRecieve[2];
	avstandsensor_4 = msgRecieve[3];
	avstandsensor_5 = msgRecieve[4];
	avstandsensor_6 = msgRecieve[5];
	avstandsensor_7 = msgRecieve[6];
	avstandsensor_8 = msgRecieve[7];
	uint8_t gyro = msgRecieve[8]; // hur far man in denna data
	//ska rotationssensorer in här?
	
	//uppdatera tillstånd
	
	//TODO
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