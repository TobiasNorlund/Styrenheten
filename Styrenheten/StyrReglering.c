/*
 * StyrReglering.c
 *
 * Created: 4/16/2013 11:32:16 AM
 *  Author: davmo049
 */ 

#include <avr/io.h>
#include <math.h>
#include "global.h"

#include "StyrReglering.h"
#define DIVISIONFACTOR  4 // DF = 4 -> 2^4 = 16

void reglering_init()
{
	globals.virtual_direction = DIRECTION_FORWARD;
	globals.x = 0;
	globals.y = 0;
	globals.v = 0;
	globals.theta = 0;
	globals.omega = 0;
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
}

int8_t getRelativeX() //x som om roboten har riktning up
{
	int8_t ret;
	switch(globals.logical_direction)
	{
		case(LOGICAL_DIR_UP):
			ret = globals.x;
			break;
		case(LOGICAL_DIR_RIGHT):
			ret = -globals.y;
			break;
		case(LOGICAL_DIR_DOWN):
			ret = -globals.x;
			break;
		case(LOGICAL_DIR_LEFT):
			ret = globals.y;
			break;
	}
	return ret;
}

int8_t getRelativeY() //Y som om roboten har riktning upp
{
	int8_t ret;
	switch(globals.logical_direction)
	{
		case(LOGICAL_DIR_UP):
		ret = globals.y;
		break;
		case(LOGICAL_DIR_RIGHT):
		ret = globals.x;
		break;
		case(LOGICAL_DIR_DOWN):
		ret = -globals.y;
		break;
		case(LOGICAL_DIR_LEFT):
		ret = -globals.x;
		break;
	}
	return ret;
}

void setSpeedRight(uint8_t speed){
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		OCR2B = speed;	
	}
	else
	{
		OCR2A = speed;	
	}
}
void setSpeedLeft(uint8_t speed){
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		OCR2A = speed;
	}
	else
	{
		OCR2B = speed;
	}
}

void setDirLeft(uint8_t dir){
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		if(dir == 1){
			PORTA = PORTA | (1 << PORTA2);
		}
		else{
			PORTA = PORTA & 0b11111011;
		}
	}
	else
	{
		if(dir == 1){
			PORTA = PORTA & 0b11110111;
		}
		else{
			PORTA = PORTA | (1 << PORTA3);
		}
	}
}
void setDirRight(uint8_t dir){
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		if(dir == 1){
			PORTA = PORTA | (1 << PORTA3);
		}
		else{
			PORTA = PORTA & 0b11110111;
		}
	}	
	else
	{
		if(dir == 1){
			PORTA = PORTA & 0b11111011;
		}
		else{
			PORTA = PORTA | (1 << PORTA2);
		}			
	}
			
}

/************************************************************************/
/*	max måste skalas på något sätt så att det blir mellan -254 och 254
	Vet dock inte vad det kommer bli för värden på omega så vi måste 
	testa det först.                                                     */
/************************************************************************/
void regulateStraight()
{
	uint8_t a = 0;
	uint8_t l1 = 8 / (globals.v*2^a);
	uint8_t l2 = 12>>a;
	uint8_t l3 = 6>>a;
	uint8_t ur,ul;
	printf("v %d", l1);
	int8_t max =-(l1*globals.x + (l2*degToRad(globals.theta))>>DIVISIONFACTOR + l3*globals.omega);
	if(max > 0 )
	{
		ur = 254;
		ul = ur - max;
	}
	else
	{
		ul = 254;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);
}
void turnLeft90(){
	uint8_t a  = 0;
	uint8_t l1 = 4 >> a;
	uint8_t l2 = 4 >> a;
	uint8_t ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int8_t max = - ((l1*degToRad(90-globals.theta))>>DIVISIONFACTOR + l2*globals.omega);
	if(max > 0 )
	{
		ur = 254;
		ul = ur - max;
	}
	else
	{
		ul = 254;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);
}
void turnRight90(){
	uint8_t a  = 0;
	uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
	uint8_t l2 = 4 >> a;
	uint8_t ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int8_t max = - ((l1*degToRad(90+globals.theta))>>DIVISIONFACTOR + l2*globals.omega);
	if(max > 0 )
	{
		ur = 254;
		ul = ur - max;
	}
	else
	{
		ul = 254;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);
}
void turnLeft45(){
	uint8_t a  = 0;
	uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
	uint8_t l2 = 4 >> a; // -- || --
	uint8_t ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int8_t max = - ((l1*degToRad(45-globals.theta))>>DIVISIONFACTOR + l2*globals.omega);
	if(max > 0 )
	{
		ur = 254;
		ul = ur - max;
	}
	else
	{
		ul = 254;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);
}
void turnRight45(){
	uint8_t a  = 0;
	uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
	uint8_t l2 = 4 >> a; // -- || --
	uint8_t ur,ul;
	
	setDirLeft(1);
	setDirRight(0);
	
	int8_t max = - ((l1*degToRad(45+globals.theta))>>DIVISIONFACTOR + l2*globals.omega);
	if(max > 0 )
	{
		ur = 254;
		ul = ur - max;
	}
	else
	{
		ul = 254;
		ur = max + ul;
	}
	setSpeedLeft(ul);
	setSpeedRight(ur);

}

void virtualTurn()
{
	if(globals.virtual_direction == DIRECTION_FORWARD)
	{
		globals.virtual_direction = DIRECTION_REVERSE;
	}
	else
	{
		globals.virtual_direction = DIRECTION_FORWARD;
	}
	//globals.logical_direction = TODO;
	return;
}

void customSteering()
{
	setDirLeft(1);
	setDirRight(1);
	setSpeedRight(globals.paramCustomRight);
	setSpeedLeft(globals.paramCustomLeft);
}
int8_t degToRad(int8_t degree)
{
	return (2^DIVISIONFACTOR*9*degree>>9); // pi/180 = 0.0174532, 9/(2^9) = 0.17578
}
