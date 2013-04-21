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
	cbInit(&globals.mapDataToSend, 32);
	cbInit(&globals.debugMesssageBuffer, 32);
	globals.routeLength = 0;
	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	//TCCR1B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	
	//timed interupt init
	TIMSK0 = (1<<OCIE0A);// Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR0A = (1<<WGM01); // Mode = CTC, clear on compare, dvs reseta räknaren
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);// Clock/1024, 0.000128 seconds per tick
	OCR0A = 0.1f/0.000128f; // 0.2f/0.000128f ger 50 gånger i sekunden 1/50= 0.02

	//enable overflow interupt
	//TIMSK1=(1<<TOIE1);//overflow interupt
	
	//TCNT1=0;//init value for counter 1
}

void updateState(uint8_t gyro1, uint8_t gyro2, uint8_t vRight, uint8_t vLeft)
{
	//TODO
}

ISR(TIMER0_COMPA_vect)
{
	//skicka request till sensorenehten att skicka data
	uint8_t msgSend[32];
	/*SPI_set_sensor(START);
	SPI_MASTER_write(msgSend, TYPE_REQUEST_SENSOR_DATA, 0);
	
	_delay_us(2);
	*/
	//ta emot data från sensorenheten
	uint8_t msgRecieve[32];
	uint8_t type;
	uint8_t len;
	/*SPI_MASTER_read(msgRecieve, type, len);
	SPI_set_sensor(END);*/
	//skicka vidare till PC
	SPI_set_kom(START);
	/*SPI_MASTER_write(msgRecieve, TYPE_DEBUG_DATA, *len);
	
	uint8_t bytesToSend = 0;
	while(cbBytesUsed(&globals.debugMesssageBuffer) != 0)
	{
		msgSend[bytesToSend] = cbRead(&globals.debugMesssageBuffer);
		++bytesToSend;
	}
	
	//SPI_MASTER_write(msgSend, TYPE_DEBUG_DATA, bytesToSend);
	//skriv in data */
	uint8_t gyro1;
	uint8_t gyro2;
	uint8_t vRight;
	uint8_t vLeft;
	for(uint8_t i = 0; i < len; ++i)
	{
		uint8_t id = msgRecieve[i];
		switch(id)
		{
			case LONGFRONT:
				globals.longFront = msgRecieve[i+1];
				++i;
				break;
			case LONGRIGHT:
				globals.longRight = msgRecieve[i+1];
				++i;
				break;
			case LONGREAR:
				globals.longRear = msgRecieve[i+1];
				++i;
				break;
			case LONGLEFT:
				globals.longLeft = msgRecieve[i+1];
				++i;
				break;
			case SHORTFRONTRIGHT:
				globals.shortFrontRight = msgRecieve[i+1];
				++i;
				break;
			case SHORTFRONTLEFT:
				globals.shortFrontLeft = msgRecieve[i+1];
				++i;
				break;
			case SHORTREARRIGHT:
				globals.shortRearRight = msgRecieve[i+1];
				++i;
				break;
			case SHORTREARLEFT:
				globals.shortRearLeft = msgRecieve[i+1];
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
	do
	{
		SPI_MASTER_write(msgSend, TYPE_REQUEST_PC_MESSAGE, 0);
		
		_delay_us(100);
		
		SPI_MASTER_read(msgRecieve, &type, &len);
		
		switch(type)
		{
			case TYPE_MANUAL_COMMAND:
			{
				//lägg till msg[0] först i route
				for(uint8_t i = globals.routeLength; i > 0; --i)
				{
					globals.route[globals.routeLength] = globals.route[globals.routeLength-1];
				}
				globals.route[0] = msgRecieve[0];
				globals.routeLength = globals.routeLength+1;
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
	}while(type != TYPE_NO_PC_MESSAGES);

	//skicka all kartdata till komm
	while(cbBytesUsed(&globals.mapDataToSend) > 1)
	{
		uint8_t x = cbRead(&globals.mapDataToSend);
		uint8_t y = cbRead(&globals.mapDataToSend);
		msgSend[0] = x;
		msgSend[1] = y;
		msgSend[2] = globals.map[y][x];
		SPI_MASTER_write(msgSend, TYPE_MAP_DATA, 3);
	}
	SPI_set_kom(END);
}