/*
 * Styrenheten.c
 *
 * Created: 4/5/2013 11:22:36 AM
 *  Author: carka684 
 */ 

#include "global.h"
#include "clockedInterrupt.h"
#include <avr/io.h>
#include <math.h>

int8_t init(void)
{
	SPI_MASTER_init();
	/*
	*	Initierar registrerna för PWM
	*
	*/
	//TCCR2A
	/*
		COM2A0 = 0;
		COM2A1 = 1;
		COM2B0 = 0;
		COM2B1 = 1;
		WGM20  = 1;
		WGM21  = 1;
	*/
	TCCR2A = (0 << COM2A0) | (1 << COM2A1) | (0 << COM2B0) | (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);

	//TCCR2B
	
	/*
		WGM22 = 0;
		CS20 = 1;
		CS21 = 0;
		CS22 = 0;
	*/
	TCCR2B = (0 << WGM22) | (1 << CS20) | (0 << CS21) | (0 << CS22);
	
	/*
	* Sätt upp port A och D
	*/
	DDRA = 0b00001100;
	DDRD = 0b11000000;
	
	
	// Sätt gaspådrag på hjulen
	setSpeedRight(0);
	setSpeedRight(125);
	Delay(200);
	setSpeedRight(200);
	Delay(200);
	setSpeedRight(255);
	Delay(300);
	setDirRight(1);
	setSpeedRight(255);


	while(1){
	}
}
void setSpeedRight(uint8_t speed){
	OCR2A = speed;
}
void setSpeedLeft(uint8_t speed){
	OCR2B = speed;
}
void setDirRight(uint8_t dir){
	if(dir == 1){
		PORTA = PORTA | (1 << PORTA2); 
	}		
	else{
		PORTA = PORTA & 0b11111011;
	}			
}
void setDirLeft(uint8_t dir){
	if(dir == 1){
		PORTA = PORTA | (1 << PORTA3); 
	}		
	else{
		PORTA = PORTA & 0b11110111;
	}			
}
void regulateStraight(uint8_t x, uint8_t v, uint8_t theta, uint8_t omega)
{
	uint8_t a = 1;
	uint8_t l1 = 8 / v*a;
	uint8_t l2 = 12/a;
	uint8_t l3 = 6/a;
	uint8_t ur,ul;
	printf("v %d", l1);
	uint8_t max =-(l1*x + l2*theta + l3*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max;
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);
}
void turnLeft90(int theta, int omega){
	int a  = 1;
	int l1 = 4 / a; 
	int l2 = 4 / a; 
	int ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int max = - (l1*(3.14/2-theta) + l2*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max; 
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
}
void turnRight90(int theta, int omega){
	int a  = 1;
	int l1 = 4 / a; //Ska räknas ut på egenvärden!
	int l2 = 4 / a;
	int ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int max = - (l1*(3.14/2+theta) + l2*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max;
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
}
void turnLeft45(int theta, int omega){
	int a  = 1;
	int l1 = 4 / a; //Ska räknas ut på egenvärden!
	int l2 = 4 / a; // -- || --
	int ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int max = - (l1*(3.14/4-theta) + l2*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max;
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
}
void turnRight45(int theta, int omega){
	int a  = 1;
	int l1 = 4 / a; //Ska räknas ut på egenvärden!
	int l2 = 4 / a; // -- || --
	int ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int max = - (l1*(3.14/4+theta) + l2*omega);
	if(max > 0 )
	{
		ur = 255;
		ul = ur - max;
	}
	else
	{
		ul = 255;
		ur = max + ul;
	}
}

int main(void)
{
	
	init();
	while(1)
	{
		//TODO:: Please write your application code
	}
}