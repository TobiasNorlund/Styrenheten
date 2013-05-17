/*
 * clockedInterrupt.c
 *
 * Created: 4/12/2013 10:37:35 AM
 *  Author: davmo049
 */ 
#include "clockedInterrupt.h"
#include <avr/interrupt.h>
#include "observer.h"
#include "../../TSEA27-include/message.h"
#include "global.h"
#include "../../TSEA27-include/SPI/spi_master.h"
#include <util/delay.h>

volatile uint8_t overFlowInteruptTimer1=0;
uint8_t sendcounter;

void clockedInterrupt_init()
{
	cbInit(&glob_mapDataToSend, 255	);
	cbInit(&glob_debugMesssageBuffer, 50);
	glob_routeLength = 0;
	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	
	//timed interupt init
	OCR1A = (uint16_t)8000;// Ger en interrupt var 50ms, vid 20MHz //6400000;//0xFF; //(1/10)*20000000/1024; // 10 gånger i sek ifall 20mhz
	TIMSK1 = (1<<OCIE1B);// Enable Interrupt TimerCounter1 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR1A = (1<<WGM11); // Mode = CTC, clear on compare, dvs reseta räknaren
	//enable overflow interupt
	//TIMSK1=(1<<TOIE1);//overflow interupt
	//TCNT1=0;//init value for counter 1
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10);// Clock/1024, 0.000128 seconds per tick
	sendcounter=0;
}

void updateState(void)
{
	observe();
}

uint8_t temp = 0;
ISR(TIMER1_COMPB_vect)
{
	if(PIND & 1<<PIND2)
	{
		setSpeedRight(0);
		setSpeedLeft(0);
		TIMSK0 = (0<<OCIE0A);// disable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
		while(1)
		{
			if(!(PIND & 1<<PIND2))
			{
				break;
			}
			setSpeedRight(0);
			setSpeedLeft(0);
		}
	}
	overFlowInteruptTimer1++;
	if(overFlowInteruptTimer1==1)
	{
		timedInterupt();
		overFlowInteruptTimer1=0;
	}
}

//#define SENSOR_OFF
//#define KOM_OFF
void timedInterupt(void)
{
	//Toggla 
	if(temp==0){
		PORTB &= 0b11111110;
		temp = 1;
	}else{
		PORTB |= 0b00000001;
		temp = 0;
	}
	uint8_t msgRecieve[32];
	uint8_t type;
	uint8_t len;
	uint8_t msgSend[32];
	//skicka request till sensorenehten att skicka data
#ifndef SENSOR_OFF
	SPI_set_sensor(START);
	SPI_MASTER_write(msgSend, TYPE_REQUEST_SENSOR_DATA, 0);
	
	//ta emot data från sensorenheten
	uint8_t answerSensor=0;
	uint8_t answerCounterSensor=0;
	while((!answerSensor)&&(answerCounterSensor<254))//räknare så vi inte fastnar här om vi hamnar aout of sync med kom.
	{
		answerSensor = SPI_MASTER_read(msgRecieve, &type, &len);
		answerCounterSensor++;
	}
	SPI_set_sensor(END);
	
	if(answerCounterSensor == 255)
		return;
	
	glob_longFront_old = glob_longFront; // cm
	glob_longRight_old = glob_longRight; // cm
	glob_longRear_old = glob_longRear;// cm
	glob_longLeft_old = glob_longLeft;// cm
	glob_shortFrontRight_old = glob_shortFrontRight; // cm/2
	glob_shortFrontLeft_old = glob_shortFrontLeft; // cm/2
	glob_shortRearRight_old = glob_shortRearRight; // cm/2
	glob_shortRearLeft_old = glob_shortRearLeft;
		
	//tolka/spara sensordata
	if(type==TYPE_SENSOR_DATA && len!=0)
	{
		for(uint8_t i = 0; i < len; ++i)
		{
			uint8_t id = msgRecieve[i];
			switch(id)
			{
				case LONGFRONT:
					glob_longFront = msgRecieve[i+1];
					++i;
					break;
				case LONGRIGHT:
					glob_longRight = msgRecieve[i+1];
					++i;
					break;
				case LONGREAR:
					glob_longRear = msgRecieve[i+1];
					++i;
					break;
				case LONGLEFT:
					glob_longLeft = msgRecieve[i+1];
					++i;
					break;
				case SHORTFRONTRIGHT:
					glob_shortFrontRight = msgRecieve[i+1];
					++i;
					break;
				case SHORTFRONTLEFT:
					glob_shortFrontLeft = msgRecieve[i+1];
					++i;
					break;
				case SHORTREARRIGHT:
					glob_shortRearRight = msgRecieve[i+1];
					++i;
					break;
				case SHORTREARLEFT:
					glob_shortRearLeft = msgRecieve[i+1];
					++i;
					break;
				case IDGYROSENSOR:
					glob_gyro = (msgRecieve[i+1]<<8)|msgRecieve[i+2];
					i = i+2;
					break;
				case IDSPEEDRIGHT:
					glob_vRight = msgRecieve[i+1];
					++i;
					break;
				case IDSPEEDLEFT:
					glob_vLeft = msgRecieve[i+1];
					++i;
					break;
			}	
		}
	
		//uppdatera tillstånd
		updateState();
	}
#endif
#ifndef KOM_OFF
	sendcounter++;
	if(sendcounter==16)
	{
		sendcounter=0;
		//skicka vidare till PC
		SPI_set_kom(START);
		//väldigt ful kod gör om, gör rätt
		msgRecieve[len] = 12;
		msgRecieve[len+1] = glob_x;
		msgRecieve[len+2] = 13;
		msgRecieve[len+3] = glob_y;
		msgRecieve[len+4] = 14;
		msgRecieve[len+5] = glob_theta;

		glob_syncSpike = 0;
	
		SPI_MASTER_write(msgRecieve, TYPE_DEBUG_DATA, len+6);
		int8_t vinkelHastHjul = (glob_vRight-glob_vLeft)>>4; // 17 ca 16
		cbWrite(&glob_debugMesssageBuffer, 21);
		cbWrite(&glob_debugMesssageBuffer, vinkelHastHjul);
		//send debug data
		uint8_t bytesToSend = 0;
		while(cbBytesUsed(&glob_debugMesssageBuffer) != 0)
		{
			msgSend[bytesToSend] = cbRead(&glob_debugMesssageBuffer);
			++bytesToSend;
		}
		if(bytesToSend != 0)
		{
			SPI_MASTER_write(msgSend, TYPE_DEBUG_DATA, bytesToSend);
		}
			//end send debug data
		//uint8_t msg[4]={12, glob_x, 13, glob_y};
		//SPI_MASTER_write(msg, TYPE_DEBUG_DATA, 4);
	
		volatile uint8_t answer = 0;
		uint8_t answerCounter=0;
		do
		{
			SPI_MASTER_write(msgSend, TYPE_REQUEST_PC_MESSAGE, 0);

		
			answer = 0;
			answerCounter=0;
			while((!answer)&&(answerCounter<254))//räknare så vi inte fastnar här om vi hamnar aout of sync med kom.
			{
				answer = SPI_MASTER_read(msgRecieve, &type, &len);
				answerCounter++;
			}
				

			if(type == TYPE_MANUAL_COMMAND)
			{
				//lägg till msg[0] först i route
				for(uint8_t i = glob_routeLength; i > 0; --i)
				{
					glob_route[glob_routeLength] = glob_route[glob_routeLength-1];
				}
				glob_route[0] = msgRecieve[0];
				glob_routeLength = glob_routeLength+1;
				break;
			}
			else if(type == TYPE_CHANGE_PARM)
			{
				uint8_t ID = msgRecieve[0];
				uint8_t val = msgRecieve[1];
				
				if (ID==PARAMLEFTCUSTOM)
				{
					glob_paramCustomLeft = val;
				}
				else if (ID==PARAMRIGHTCUSTOM)
				{
					glob_paramCustomRight = val;
				}
				else if (ID==L1_STRAIGHTX)
				{
					glob_L1_straightX = val;
				}
				else if (ID==L2_STRAIGHTTHETA)
				{
					glob_L2_straightTheta = val;
				}
				else if (ID==L3_STRAIGHTOMEGA)
				{
					glob_L3_straightOmega = val;
				}
				else if (ID==L1_TURNTHETA)
				{
					glob_L1_turnTheta = val;
				}
				else if (ID==L2_TURNOMEGA)
				{
					glob_L2_turnOmega = val;
				}

			}		
		}while((type != TYPE_NO_PC_MESSAGES) && (type != TYPE_REQUEST_PC_MESSAGE) && (answerCounter<254));//type != TYPE_REQUEST_PC_MESSAGE betyder att vi ej laggt in ny data i kom. dvs ej handskakat.


		//skicka all kartdata till komm
		while(cbBytesUsed(&glob_mapDataToSend) > 1)
		{
			uint8_t x = cbRead(&glob_mapDataToSend);
			uint8_t y = cbRead(&glob_mapDataToSend);
			msgSend[0] = x;
			msgSend[1] = y;
			msgSend[2] = glob_map[y][x];
			SPI_MASTER_write(msgSend, TYPE_MAP_DATA, 3);
		}
		SPI_set_kom(END);
	}	
#endif
}
