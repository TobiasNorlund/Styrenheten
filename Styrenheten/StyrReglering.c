/*
 * StyrReglering.c
 *
 * Created: 4/16/2013 11:32:16 AM
 *  Author: davmo049
 */ 

#define STOPTURN90 85
#define STOPTURN45 43

#include <avr/io.h>
#include "global.h"

#include "StyrReglering.h"

uint8_t startSquareX;
uint8_t startSquareY;


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
	int8_t ret = 0;
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

int8_t getRelativeY(void) //Y som om roboten har riktning upp
{
	int8_t ret = 0;
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
	testa det först.                                                    */
/************************************************************************/
//turn off optimization 
//#pragma GCC push_options
//#pragma GCC optimize ("O0")
void regulateStraight()
{
	int16_t max;
	setDirRight(1);
	setDirLeft(1);
	while(!((startSquareX != globals.mapX || startSquareY != globals.mapY)&&(0 < getRelativeY())))
	{

		/*
		uint8_t a = 0;
		uint8_t l1 = 8 / (globals.v*2^a);
		uint8_t l2 = 12>>a;
		uint8_t l3 = 6>>a;
		*/
		//TODO: Ska inte använda globals.vLeft!!!!
		int16_t ur,ul;
		max = -(((10*globals.L1_straightX*getRelativeX()/globals.vLeft)>>SHORTFACTOR) + ((globals.L2_straightTheta*degToRad(globals.theta))>>DIVISIONFACTOR) + (globals.L3_straightOmega*globals.omega));
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
}
//#pragma GCC pop_options
//end turn off optimization 

void turnLeft90(){
	setDirLeft(0);
	setDirRight(1);
	while(1)
	{
		if(globals.theta > STOPTURN90)
		{
			return;
		}
		
	
		/*
		uint8_t a  = 0;
		uint8_t l1 = 4 >> a;
		uint8_t l2 = 4 >> a;
		*/
		uint8_t ur,ul;
	
		int16_t max ;//= - ((globals.L1_turnTheta*degToRad(90-globals.theta))>>DIVISIONFACTOR + globals.L2_turnOmega*globals.omega);
		if(max > 0 )
		{
			ur = 254 - max;
			ul = ur;
		}
		else
		{
			ul = 254 + max;
			ur = ul;
		}
		setSpeedLeft(ul);
		setSpeedRight(ur);
	}	
}
void turnRight90(){
	setDirLeft(1);
	setDirRight(0);
	while(1)
	{
		if(globals.theta < -STOPTURN90)
		{
			return;
		}
		/*
		uint8_t a  = 0;
		uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
		uint8_t l2 = 4 >> a;
		*/
		uint8_t ur,ul;
	
		int16_t max;// = - ((globals.L1_turnTheta*degToRad(90+globals.theta))>>DIVISIONFACTOR + globals.L2_turnOmega*globals.omega);
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
}
void turnLeft45(){
	setDirLeft(0);
	setDirRight(1);
	while(1)
	{
		if(globals.theta > STOPTURN45)
		{
			return;
		}
		/*
		uint8_t a  = 0;
		uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
		uint8_t l2 = 4 >> a; // -- || --
		*/
		uint8_t ur,ul;
	
		int16_t max;// = - ((globals.L1_turnTheta*degToRad(45-globals.theta))>>DIVISIONFACTOR + globals.L2_turnOmega*globals.omega);
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
}
	
void turnRight45(){
	setDirLeft(1);
	setDirRight(0);
	while(1)
	{
		/*if(globals.theta < -STOPTURN45)
		{
			return;
		}*/
		/*
		uint8_t a  = 0;
		uint8_t l1 = 4 >> a; //Ska räknas ut på egenvärden!
		uint8_t l2 = 4 >> a; // -- || --
		*/
		uint8_t ur,ul;
	
		int16_t max;// = - ((globals.L1_turnTheta*degToRad(45+globals.theta))>>DIVISIONFACTOR + globals.L2_turnOmega*globals.omega);
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

//Returnerar 2^DIVISIONFACTOR*radianer 
int8_t degToRad(int8_t degree)
{
	return (2^DIVISIONFACTOR*9*degree>>9); // pi/180 = 0.0174532, 9/(2^9) = 0.017578
}