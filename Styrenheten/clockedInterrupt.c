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
#include <util/delay.h>

void clockedInterrupt_init()
{
	cbInit(&globals.mapDataToSend, 32);
	cbInit(&globals.debugMesssageBuffer, 32);
	globals.routeLength = 0;
	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	//TCCR1B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	
	//timed interupt init
	TIMSK1 = (1<<OCIE1A);// Enable Interrupt TimerCounter1 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR1A = (1<<WGM11); // Mode = CTC, clear on compare, dvs reseta räknaren
	TCCR1B = (1<<CS12)|(0<<CS11)|(1<<CS10);// Clock/1024, 0.000128 seconds per tick
	OCR1A = (1/10)*20000000/1024; // 10 gånger i sek ifall 20mhz

	//enable overflow interupt
	//TIMSK1=(1<<TOIE1);//overflow interupt
	
	//TCNT1=0;//init value for counter 1
}

void updateState(void)
{
	//TODO
}

ISR(TIMER1_COMPA_vect)
{
	//skicka request till sensorenehten att skicka data
	uint8_t msgSend[32];
	SPI_set_sensor(START);
	SPI_MASTER_write(msgSend, TYPE_REQUEST_SENSOR_DATA, 0);
	
	_delay_us(900);
	//ta emot data från sensorenheten
	uint8_t msgRecieve[32];
	uint8_t type;
	uint8_t len;
	while(!SPI_MASTER_read(msgRecieve, &type, &len));//vänta tills buffetrent fylls
	SPI_set_sensor(END);
	_delay_us(1000);
	//skicka vidare till PC
	SPI_set_kom(START);
	SPI_MASTER_write(msgRecieve, TYPE_DEBUG_DATA, len);
	
	//send debug data
	uint8_t bytesToSend = 0;
	while(cbBytesUsed(&globals.debugMesssageBuffer) != 0)
	{
		msgSend[bytesToSend] = cbRead(&globals.debugMesssageBuffer);
		++bytesToSend;
	}
	if(bytesToSend != 0)
	{
		SPI_MASTER_write(msgSend, TYPE_DEBUG_DATA, bytesToSend);	
	}
	//end send debug data
	
	//tolka/spara sensordata
	if(type==TYPE_SENSOR_DATA&&len!=0)
	{
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
					globals.gyro = (msgRecieve[i+1]<<8)|msgRecieve[i+2];
					i = i+2;
					break;
				case IDSPEEDRIGHT:
					globals.vRight = msgRecieve[i+1];
					++i;
					break;
				case IDSPEEDLEFT:
					globals.vLeft = msgRecieve[i+1];
					++i;
					break;
			}	
		}
	
		//uppdatera tillstånd
		updateState();
	}
	
	//TODO STÅR här i oändlihet. Kan bero på att pc:n ej var inkopplad.
	/*
	//fråga om data från PC
	do
	{
		SPI_MASTER_write(msgSend, TYPE_REQUEST_PC_MESSAGE, 0);
		
		_delay_us(10);
		
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
	}*/
	SPI_set_kom(END);
}