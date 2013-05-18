/*
 * StyrReglering.c
 *
 * Created: 4/16/2013 11:32:16 AM
 *  Author: davmo049
 */ 

#define MAXSPEED 250
#define TURNSPEED 180
#define STOPTURN90 90
#define STOPTURN45 43
#define RIGHTWHEELDIFF 2
#define LENGTH_OFFSET -40
#define ROTMIN 95

#define FORWARD 1
#define BACKWARD 0

#include <avr/io.h>

#include "global.h"
#include "../../TSEA27-include/message.h"

#include "StyrReglering.h"

#include <util/delay.h>

uint8_t startSquareX;
uint8_t startSquareY;


void reglering_init()
{
	glob_virtual_direction = DIRECTION_FORWARD;
	glob_x = 0;
	glob_y = 0;
	glob_v = 0;
	glob_theta = 0;
	glob_omega = 0;
	glob_SumTheta=0;
	glob_omegaWheelSum=0;
	glob_thetaOld=0;
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
		CS21 = 1;
		CS22 = 1;
	*/
	TCCR2B = (0 << WGM22) | (1 << CS20) | (1 << CS21) | (1 << CS22);
	
	/*
	* Sätt upp port A och D
	*/
	DDRA = 0b00001100;
	DDRD = 0b11000000;
}

int8_t getRelativeX() //x som om roboten har riktning up
{
	int8_t ret = 0;
	switch(glob_logical_direction)
	{
		case(LOGICAL_DIR_UP):
			ret = glob_x;
			break;
		case(LOGICAL_DIR_RIGHT):
			ret = -glob_y;
			break;
		case(LOGICAL_DIR_DOWN):
			ret = -glob_x;
			break;
		case(LOGICAL_DIR_LEFT):
			ret = glob_y;
			break;
	}
	return ret;
}

int8_t getRelativeY() //Y som om roboten har riktning upp
{
	int8_t ret = 0;
	switch(glob_logical_direction)
	{
		case(LOGICAL_DIR_UP):
			ret = glob_y;
			break;
		case(LOGICAL_DIR_RIGHT):
			ret = glob_x;
			break;
		case(LOGICAL_DIR_DOWN):
			ret = -glob_y;
			break;
		case(LOGICAL_DIR_LEFT):
			ret = -glob_x;
			break;
	}
	return ret;
}

void setSpeedRight(uint8_t speed){
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		if(speed < LEFTWHEELDIFF)
		{
			OCR2A = 0;
		}
		else
		{
			OCR2A = speed-LEFTWHEELDIFF;//Vänster hjulpar	
		}
	}
	else
	{
		OCR2B = speed;	//Höger hjulpar
	}
}
void setSpeedLeft(uint8_t speed){
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		OCR2B = speed; //Höger hjulpar
	}
	else
	{
		if (speed < LEFTWHEELDIFF)
		{
			OCR2A = 0;
		}
		else
		{
			OCR2A = speed-LEFTWHEELDIFF; //vänster hjulpar
		}		
	}
	
}

void setDirLeft(uint8_t dir){
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		if(dir == FORWARD){
			PORTA = PORTA | (1 << PORTA2);
		}
		else{
			PORTA = PORTA & 0b11111011;
		}
	}
	else
	{
		if(dir == FORWARD){
			PORTA = PORTA & 0b11110111;
		}
		else{
			PORTA = PORTA | (1 << PORTA3);
		}
	}
	if(dir == FORWARD)
	{
	 	glob_vLeftSign=1;
	}
	else
	{
	 	glob_vLeftSign=-1;
	}
}
void setDirRight(uint8_t dir){
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		if(dir == FORWARD){
			PORTA = PORTA | (1 << PORTA3);
		}
		else{
			PORTA = PORTA & 0b11110111;
		}
	}	
	else
	{
		if(dir == FORWARD){
			PORTA = PORTA & 0b11111011;
		}
		else{
			PORTA = PORTA | (1 << PORTA2);
		}			
	}
	if(dir == FORWARD)
	{
	 	glob_vRightSign=1;
	}
	else
	{
	 	glob_vRightSign=-1;
	}
}

/************************************************************************/
/*	max måste skalas på något sätt så att det blir mellan -254 och 254
	Vet dock inte vad det kommer bli för värden på omega så vi måste 
	testa det först.                                                    */
/************************************************************************/
//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
void regulateStraight()
{
	setDirRight(FORWARD);
	setDirLeft(FORWARD);
	startSquareX = glob_mapX;
	startSquareY = glob_mapY;
	glob_omegaWheelSum=glob_theta<<4;//init wheel sum
	glob_sum_y=(getRelativeY()<<10)/TIMECONSTANT;
	while(!((startSquareX != glob_mapX || startSquareY != glob_mapY)&&(LENGTH_OFFSET < getRelativeY())))
	{
		int16_t ur,ul;
		int16_t xRelative = int8to16(getRelativeX());
		int16_t xFactor = (glob_L1_straightX*xRelative)>>SHORTFACTOR;
		int16_t thetaDeg = degToRad(glob_theta);
		int16_t thetaFactor = (glob_L2_straightTheta*thetaDeg)>>(DIVISIONFACTOR-4);
		int16_t omegaFactor = glob_L3_straightOmega*glob_omega;
		glob_max = xFactor-thetaFactor;
		/*
		cbWrite(&glob_debugMesssageBuffer, 17);
		cbWrite(&glob_debugMesssageBuffer, glob_theta>>8);
		cbWrite(&glob_debugMesssageBuffer, glob_theta);
		cbWrite(&glob_debugMesssageBuffer, 19);
		cbWrite(&glob_debugMesssageBuffer, xRelative>>8);
		cbWrite(&glob_debugMesssageBuffer, xRelative);
		*/
		if(glob_max > MAXSPEED)
		{
			glob_max = MAXSPEED;
		}
		else if(glob_max < -MAXSPEED)
		{
			glob_max = -MAXSPEED;
		}
		if(glob_max > 0 )
		{
			ur = MAXSPEED;
			ul = ur - glob_max;
		}
		else
		{
			ul = MAXSPEED;
			ur = glob_max + ul;
		}
		setSpeedLeft(ul);
		setSpeedRight(ur);
	}
	setSpeedLeft(0);
	setSpeedRight(0);
}

void turnLeft90(){
	glob_SumTheta = glob_theta<<8;
	setDirLeft(BACKWARD);
	setDirRight(FORWARD);
	setSpeedRight(TURNSPEED);
	setSpeedLeft(TURNSPEED);
	while(glob_theta < (int16_t)STOPTURN90)
	{
		
	}
	glob_SumTheta -= 90<<8;
	glob_theta = glob_theta - (int16_t) 90;
	
	switch(glob_logical_direction)
	{
		case LOGICAL_DIR_UP:
			glob_logical_direction = LOGICAL_DIR_LEFT;
			break;
		case LOGICAL_DIR_RIGHT:
			glob_logical_direction = LOGICAL_DIR_UP;
			break;
		case LOGICAL_DIR_DOWN:
			glob_logical_direction = LOGICAL_DIR_RIGHT;
			break;
		default:
			glob_logical_direction = LOGICAL_DIR_DOWN;
			break;
	}
	cleanUpAngle();
}

void turnRight90(){
	glob_SumTheta = glob_theta<<8;
	setDirLeft(FORWARD);
	setDirRight(BACKWARD);
	setSpeedRight(TURNSPEED);
	setSpeedLeft(TURNSPEED); 
	while(glob_theta > (int16_t)(-STOPTURN90))
	{
	}
	glob_SumTheta += 90<<8;
	glob_theta = glob_theta + (int16_t) 90;
	switch(glob_logical_direction)
	{
		case LOGICAL_DIR_UP:
			glob_logical_direction = LOGICAL_DIR_RIGHT;
			break;
		case LOGICAL_DIR_RIGHT:
			glob_logical_direction = LOGICAL_DIR_DOWN;
			break;
		case LOGICAL_DIR_DOWN:
			glob_logical_direction = LOGICAL_DIR_LEFT;
			break;
		default:
			glob_logical_direction = LOGICAL_DIR_UP;
			break;
		}
	cleanUpAngle();
}
void turnLeft45(){
	glob_SumTheta = glob_theta<<8;
	setDirLeft(BACKWARD);
	setDirRight(FORWARD);
	setSpeedRight(TURNSPEED);
	setSpeedLeft(TURNSPEED); 
	while(glob_theta < STOPTURN45)
	{
	}
	setSpeedRight(0); 
	setSpeedLeft(0);
	glob_SumTheta -= 45<<8;
	glob_theta = glob_theta - (int16_t) 45;
	
}
	
void turnRight45(){
	glob_SumTheta = glob_theta<<8;
	setDirLeft(FORWARD);
	setDirRight(BACKWARD);
	setSpeedRight(TURNSPEED); 
	setSpeedLeft(TURNSPEED); 
	while(glob_theta > -STOPTURN45)
	{
	}
	setSpeedRight(0); 
	setSpeedLeft(0);
	glob_SumTheta += 45<<8;
	glob_theta = glob_theta + (int16_t) 45;
}
#pragma GCC pop_options
//end turn off optimization

void cleanUpAngle()
{
	if(glob_logical_direction == LOGICAL_DIR_UP || glob_logical_direction == LOGICAL_DIR_DOWN)
	{
		if(glob_map[glob_mapY][glob_mapX+1] == OPEN && glob_map[glob_mapY][glob_mapX-1] == OPEN)
		{
			setSpeedRight(0);
			setSpeedLeft(0);
			return;
		}
	}
	else
	{
		if(glob_map[glob_mapY+1][glob_mapX] == OPEN && glob_map[glob_mapY-1][glob_mapX] == OPEN)
		{
			setSpeedRight(0);
			setSpeedLeft(0);
			return;
		}
	}
	glob_curComm = TURN_FINE;
	uint8_t rotSpeed = TURNSPEED-50;
	uint8_t prev_dir = 2;
	while(!(glob_theta == 0 && rotSpeed == ROTMIN))
	{
		if(glob_theta > 0)
		{
			setDirLeft(FORWARD);
			setDirRight(BACKWARD);
			setSpeedRight(rotSpeed);
			setSpeedLeft(rotSpeed);
			if(prev_dir == 2)
			{
				prev_dir = 1;
			}
			else if(prev_dir == 0)
			{
				prev_dir = 1;
				rotSpeed = rotSpeed-50;
				if(rotSpeed < ROTMIN)
				{
					rotSpeed = ROTMIN;
				}
				else
				{
					uint8_t theta_sav = glob_theta;
					for(uint8_t i = 0; i < 5; ++i)
					{
						while(glob_theta == theta_sav)
						{
							
						}
					}
				}
			}
		}
		else
		{
			setDirLeft(BACKWARD);
			setDirRight(FORWARD);
			setSpeedRight(rotSpeed);
			setSpeedLeft(rotSpeed);
			if(prev_dir == 2)
			{
				prev_dir = 0;
			}
			else if(prev_dir == 1)
			{
				prev_dir = 0;
				rotSpeed = rotSpeed-50;
				if(rotSpeed < ROTMIN)
				{
					rotSpeed = ROTMIN;
				}
				else
				{
					uint8_t theta_sav = glob_theta;
					for(uint8_t i = 0; i < 5; ++i)
					{
						while(glob_theta == theta_sav)
						{
						}	
					}
				} 
			}
		}
	}
	setSpeedRight(0);
	setSpeedLeft(0);
}

void virtualTurn()
{
	if(glob_virtual_direction == DIRECTION_FORWARD)
	{
		glob_virtual_direction = DIRECTION_REVERSE;
	}
	else
	{
		glob_virtual_direction = DIRECTION_FORWARD;
	}
	switch(glob_logical_direction)
	{
		case LOGICAL_DIR_UP:
			glob_logical_direction = LOGICAL_DIR_DOWN;
			break;
		case LOGICAL_DIR_RIGHT:
			glob_logical_direction = LOGICAL_DIR_LEFT;
			break;
		case LOGICAL_DIR_DOWN:
			glob_logical_direction = LOGICAL_DIR_UP;
			break;
		default:
			glob_logical_direction = LOGICAL_DIR_RIGHT;
			break;
	}
	return;
}

void customSteering()
{
	setDirLeft(FORWARD);
	setDirRight(FORWARD);
	setSpeedRight(glob_paramCustomRight);
	setSpeedLeft(glob_paramCustomLeft);
}

//Returnerar 2^DIVISIONFACTOR*radianer 
int16_t degToRad(int8_t degree)
{
	return (9*int8to16(degree))>>(9-DIVISIONFACTOR); // pi/180 = 0.0174532, 9/(2^9) = 0.017578
}
