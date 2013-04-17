/*
 * clockedInterrupt.c
 *
 * Created: 4/12/2013 10:37:35 AM
 *  Author: davmo049
 */ 

#include <avr/interrupt.h>
#include "../../TSEA27-include/message.h"
#include "global.h"
#include "../../TSEA27-include/SPI/spi_master.h"

void clockedInterrupt_init()
{
	globals.mapDataToSendSize = 0;
	globals.debugMesssageBufferLength = 0;
	globals.routeLength = 0;
	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	TCCR1B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)

	//enable overflow interupt
	TIMSK1=(1<<TOIE1);//overflow interupt
	
	TCNT1=0;//init value for counter 1
}

void updateState(uint8_t gyro1, uint8_t gyro2, uint8_t vRight, uint8_t vLeft)
{
	//TODO
}

ISR(TIMER1_OVF)
{
	//skicka request till sensorenehten att skicka data
	uint8_t msgSend[32];
	SPI_set_sensor(START);
	SPI_MASTER_write(msgSend, TYPE_REQUEST_SENSOR_DATA, 0);
	
	//pausa??
	
	//ta emot data från sensorenheten
	uint8_t msgRecieve[32];
	uint8_t* type;
	uint8_t* len;
	SPI_MASTER_read(msgRecieve, type, len);
	SPI_set_sensor(END);
	//skicka vidare till PC
	SPI_set_kom(START);
	SPI_MASTER_write(msgRecieve, TYPE_DEBUG_DATA, *len);
	SPI_MASTER_write(globals.debugMesssageBuffer, TYPE_DEBUG_DATA, globals.debugMesssageBufferLength);
	globals.debugMesssageBufferLength = 0;
	//skriv in data
	uint8_t gyro1;
	uint8_t gyro2;
	uint8_t vRight;
	uint8_t vLeft;
	for(uint8_t i = 0; i < *len; ++i)
	{
		uint8_t id = msgRecieve[i];
		switch(id)
		{
			case IDSENSOR1:
				globals.avstandsensor_1 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR2:
				globals.avstandsensor_2 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR3:
				globals.avstandsensor_3 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR4:
				globals.avstandsensor_4 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR5:
				globals.avstandsensor_5 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR6:
				globals.avstandsensor_6 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR7:
				globals.avstandsensor_7 = msgRecieve[i+1];
				++i;
				break;
			case IDSENSOR8:
				globals.avstandsensor_8 = msgRecieve[i+1];
				++i;
				break;
			case IDGYROSENSOR:
				gyro1 = msgRecieve[i+1];
				gyro2 = msgRecieve[i+2];
				i = i+2;
				break;
			case IDSPEEDRIGHT:
				vRight = msgRecieve[i+1];
				++i;
				break;
			case IDSPEEDLEFT:
				vLeft = msgRecieve[i+1];
				++i;
				break;
		}	
	}
	
	//uppdatera tillstånd
	updateState(gyro1, gyro2, vRight, vLeft);
	
	//fråga om data från PC
	while(*type != TYPE_NO_PC_MESSAGES)
	{
		SPI_MASTER_write(msgSend, TYPE_REQUEST_PC_MESSAGE, 0); //flytta detta tills innan uppdatera tillstånd för att slippa paus
		
		//pausa??
		
		SPI_MASTER_read(msgRecieve, type, len);
		
		switch(*type)
		{
			case TYPE_MANUAL_COMMAND:
			{
				//lägg till msg[0] först i route
				for(uint8_t i = globals.routeLength; i > 0; --i)
				{
					globals.route[globals.routeLength] = globals.route[globals.routeLength-1];
				}
				globals.route[0] = msgRecieve[0];
				break;
			}
			case TYPE_CHANGE_PARM:
			{
				uint8_t ID = msgRecieve[0];
				uint8_t val = msgRecieve[1];
				
				switch(ID)
				{
					case PARAMLEFTCUSTOM:
						globals.paramCustomLeft = val;
						break;
					case PARAMRIGHTCUSTOM:
						globals.paramCustomRight = val;
						break;
					default: //add more TODO
						break;
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
					
	
	//skicka all kartdata till komm
	while(globals.mapDataToSendSize != 0)
	{
		uint8_t x = globals.mapDataToSend[globals.mapDataToSendSize-2];
		uint8_t y = globals.mapDataToSend[globals.mapDataToSendSize-1];
		globals.mapDataToSendSize = globals.mapDataToSendSize-2;
		msgSend[0] = x;
		msgSend[1] = y;
		msgSend[2] = globals.map[y][x];
		SPI_MASTER_write(msgSend, TYPE_MAP_DATA, 3); //flytta detta tills innan uppdatera tillstånd för att slippa paus
	}
	SPI_set_kom(END);
}