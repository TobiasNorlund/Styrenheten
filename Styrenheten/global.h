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
#define UNKNOWN 2 //UNKNOWN if open or wall

//Divisionfaktorer för att få högre precision: 2^n
#define SHORTFACTOR 1 //Korta sensorn är i cm*2


#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "../../TSEA27-include/circularbuffer.h"
#define TURNCOST 1
#define VIRTUALREVERSECOST 1

#define METAROUTEMAXLEN 128
#define CURRENT_SQUARES_SIZE 128

#define ROUTELENGTH 64

#include <util/delay.h>

#include <avr/io.h>
//dessa variabler kan diskuteras
typedef struct  
{
	volatile uint8_t mapX; //Xcoord för nuvarande ruta i kartindex initieras i init
	volatile uint8_t mapY; //Xcoord för nuvarande ruta i kartindex initieras i init
	volatile int8_t x; //offset från origo för x i nuvarande ruta mäts i 1/2 cm origo är i mitten av rutan, initieras i styrinit
	volatile int8_t volatile y; //offset från origo för x i nuvarande ruta mäts i 1/2 cm, initieras i styrinit
	volatile uint8_t v; //hastighet i 1/2 cm/s, initieras i styrinit
	volatile int8_t  theta; // Vridning i grader
	volatile int8_t  thetaOld; // Vridning i grader
	volatile int16_t  omega; //grader/sekund, initieras i styrinit
	uint8_t virtual_direction; // initieras i reglering_init
	uint8_t logical_direction; //ska initieras i pathfind_init

	//pathfind stuff
	uint8_t volatile route[ROUTELENGTH];
	uint8_t volatile routeLength; //initieras i clockedInterupt_init
	
	uint8_t routeSquares[ROUTELENGTH*2]; //rutor som finns med i route
	uint8_t routeSquaresLength;
	
	uint8_t metaRoute[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t metaRouteLenght;
	
	uint8_t shouldPathfind;
	
	uint8_t adjecentNewSquares[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t adjecentNewSquaresLenght;

	uint8_t map[16][16]; //initieras i pathfind_init

	volatile CircularBuffer debugMesssageBuffer;

	volatile CircularBuffer mapDataToSend; //initieras i clockedInterupt_init

	volatile uint8_t longFront; // cm
	volatile uint8_t longRight; // cm
	volatile uint8_t longRear; // cm
	volatile uint8_t longLeft; // cm
	volatile uint8_t shortFrontRight; // cm/2
	volatile uint8_t shortFrontLeft; // cm/2
	volatile uint8_t shortRearRight; // cm/2
	volatile uint8_t shortRearLeft; // cm/2
	
	volatile int16_t gyro; // grader/s
	
	volatile uint8_t vRight; // cm/s
	volatile uint8_t vLeft; // cm/s

	//parametrar
	volatile uint8_t paramCustomLeft;
	volatile uint8_t paramCustomRight;
	//rakt reglering
	volatile uint8_t L1_straightX;
	volatile uint8_t L2_straightTheta;
	volatile uint8_t L3_straightOmega;
	//sväng
	volatile uint8_t L1_turnTheta;
	volatile uint8_t L2_turnOmega;
} Globals;

extern volatile Globals globals;

uint8_t getSensorLongForward();
uint8_t getSensorLongRear();
uint8_t getSensorLongLeft();
uint8_t getSensorLongRight();
uint8_t getSensorShortLeftForward();
uint8_t getSensorShortLeftRear();
uint8_t getSensorShortRightForward();
uint8_t getSensorShortRightRear();

#endif /* GLOBAL_H_ */