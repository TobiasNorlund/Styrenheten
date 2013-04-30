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
//typedef struct  
//{
	uint8_t glob_mapX; //Xcoord för nuvarande ruta i kartindex initieras i init
	uint8_t glob_mapY; //Xcoord för nuvarande ruta i kartindex initieras i init
	int8_t glob_x; //offset från origo för x i nuvarande ruta mäts i 1/2 cm origo är i mitten av rutan, initieras i styrinit
	int8_t glob_y; //offset från origo för x i nuvarande ruta mäts i 1/2 cm, initieras i styrinit
	uint8_t glob_v; //hastighet i 1/2 cm/s, initieras i styrinit
	int8_t glob_theta; // Vridning i grader
	int8_t glob_thetaOld; // Vridning i grader
	int16_t glob_omega; //grader/sekund, initieras i styrinit
	uint8_t glob_virtual_direction; // initieras i reglering_init
	uint8_t glob_logical_direction; //ska initieras i pathfind_init

	//pathfind stuff
	uint8_t glob_route[ROUTELENGTH];
	uint8_t glob_routeLength; //initieras i clockedInterupt_init
	
	uint8_t glob_routeSquares[ROUTELENGTH*2]; //rutor som finns med i route
	uint8_t glob_routeSquaresLength; //initieras i pathfind init
	
	uint8_t glob_metaRoute[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t glob_metaRouteLenght; //initieras i pathfind init
	
	uint8_t glob_shouldPathfind; //initieras i pathfind init
	
	uint8_t glob_adjecentNewSquares[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t glob_adjecentNewSquaresLenght; //initieras i pathfind init

	uint8_t glob_map[16][16]; //initieras i pathfind_init

	CircularBuffer glob_debugMesssageBuffer;

	CircularBuffer glob_mapDataToSend; //initieras i clockedInterupt_init

	uint8_t glob_longFront; // cm
	uint8_t glob_longRight; // cm
	uint8_t glob_longRear; // cm
	uint8_t glob_longLeft; // cm
	uint8_t glob_shortFrontRight; // cm/2
	uint8_t glob_shortFrontLeft; // cm/2
	uint8_t glob_shortRearRight; // cm/2
	uint8_t glob_shortRearLeft; // cm/2
	
	int16_t glob_gyro; // grader/s
	
	uint8_t glob_vRight; // cm/s
	uint8_t glob_vLeft; // cm/s

	//parametrar
	uint8_t glob_paramCustomLeft;
	uint8_t glob_paramCustomRight;
	//rakt reglering
	uint8_t glob_L1_straightX;
	uint8_t glob_L2_straightTheta;
	uint8_t glob_L3_straightOmega;
	//sväng
	uint8_t glob_L1_turnTheta;
	uint8_t glob_L2_turnOmega;
//} Globals;

//volatile Globals globals;

uint8_t getSensorLongForward();
uint8_t getSensorLongRear();
uint8_t getSensorLongLeft();
uint8_t getSensorLongRight();
uint8_t getSensorShortLeftForward();
uint8_t getSensorShortLeftRear();
uint8_t getSensorShortRightForward();
uint8_t getSensorShortRightRear();

#endif /* GLOBAL_H_ */