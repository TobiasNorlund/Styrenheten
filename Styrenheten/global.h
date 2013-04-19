/*
 * global.h
 *
 * definierar alla globala variabler
 * Created: 4/5/2013 11:23:22 AM
 *  Author: tobno568
 */ 

#define START_PIN PB4
#define MANUAL_AUTO_SWITCH_PIN PB5
#define MANUAL_SELECTED 1

#define DIRECTION_FORWARD 1
#define DIRECTION_REVERSE 0

//ändra ej denna ordning utan att fråga david behövs för funktionen chart
#define LOGICAL_DIR_UP 0
#define LOGICAL_DIR_RIGHT 1
#define LOGICAL_DIR_DOWN 2
#define LOGICAL_DIR_LEFT 3

#define OPEN 0
#define WALL 1

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <util/delay.h>
#define F_CPU 8000000UL // 8mhz

#include <avr/io.h>
//dessa variabler kan diskuteras
typedef struct  
{
	uint8_t volatile mapX; //Xcoord för nuvarande ruta i kartindex initieras i init
	uint8_t volatile mapY; //Xcoord för nuvarande ruta i kartindex initieras i init
	int8_t volatile x; //offset från origo för x i nuvarande ruta mäts i 1/3 cm origo är i mitten av rutan, initieras i styrinit
	int8_t volatile y; //offset från origo för x i nuvarande ruta mäts i 1/3 cm, initieras i styrinit
	uint8_t volatile v; //hastighet i 1/2 cm/s, initieras i styrinit
	int8_t volatile theta; //vridning i 360/255 grader, initieras i styrinit
	int8_t volatile omega; //vinkelhastighet i ?? grader/sekund, initieras i styrinit
	uint8_t virtual_direction; // initieras i reglering_init
	uint8_t logical_direction; //ska initieras i pathfind_init

	uint8_t volatile route[16];
	uint8_t volatile routeLength; //initieras i clockedInterupt_init

	uint8_t volatile debugMesssageBuffer[16];
	uint8_t volatile debugMesssageBufferLength; //initieras i clockedInterupt_init

	uint8_t map[16][16];

	uint8_t volatile mapDataToSend[16];
	uint8_t volatile mapDataToSendSize; //initieras i clockedInterupt_init

	uint8_t volatile avstandsensor_1;
	uint8_t volatile avstandsensor_2;
	uint8_t volatile avstandsensor_3;
	uint8_t volatile avstandsensor_4;
	uint8_t volatile avstandsensor_5;
	uint8_t volatile avstandsensor_6;
	uint8_t volatile avstandsensor_7;
	uint8_t volatile avstandsensor_8;

	//parametrar
	uint8_t volatile paramCustomLeft;
	uint8_t volatile paramCustomRight;
} Globals;

extern Globals volatile globals;

uint8_t getSensorLongForward();
uint8_t getSensorLongBack();
uint8_t getSensorLongLeft();
uint8_t getSensorLongRight();
uint8_t getSensorShortLeftForward();
uint8_t getSensorShortLeftBack();
uint8_t getSensorShortRightForward();
uint8_t getSensorShortRightBack();

#endif /* GLOBAL_H_ */