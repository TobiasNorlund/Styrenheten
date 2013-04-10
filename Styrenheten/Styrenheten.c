/*
 * Styrenheten.c
 *
 * Created: 4/5/2013 11:22:36 AM
 *  Author: carka684 
 */ 

#include "global.h"
#include <avr/io.h>

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
int init(void)
{
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
	setSpeedLeft(255);
	setDirRight(1);
	setDirLeft(0);
	setDirLeft(1);
	setDirRight(0);
	setDirLeft(0);
	setDirRight(1);
	setDirLeft(1);
	
	
	while(1){
	}
}
void setSpeedRight(int speed){
	OCR2A = speed;
}
void setSpeedLeft(int speed){
	OCR2B = speed;
}
void setDirRight(int dir){
	if(dir == 1){
		PORTA = PORTA | (1 << PORTA2); 
	}		
	else{
		PORTA = PORTA & 0b11111011;
	}			
}
void setDirLeft(int dir){
	if(dir == 1){
		PORTA = PORTA | (1 << PORTA3); 
	}		
	else{
		PORTA = PORTA & 0b11110111;
	}			
}	