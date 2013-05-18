/*
 * global.h
 *
 * definierar alla globala variabler
 * Created: 4/5/2013 11:23:22 AM
 *  Author: tobno568
 */ 
	
#define START_PIN (PINA & 0b00010000)
#define MANUAL_AUTO_SWITCH_PIN (PINA & 0b00100000)>>5
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

#define TIMECONSTANT 12 // ms
#define INVERTTIMECONSTANT 83 //Dimension 1/s

#define HALFSQUARESUMY 6827//2*(HALFSQUAREWIDTH<<10)/TIMECONSTANT;

#include <util/delay.h>

#include <avr/io.h>
//dessa variabler kan diskuteras
//typedef struct  
//{
	volatile uint8_t glob_mapX; //Xcoord för nuvarande ruta i kartindex initieras i init
	volatile uint8_t glob_mapY; //Xcoord för nuvarande ruta i kartindex initieras i init
	volatile int8_t glob_x; //offset från origo för x i nuvarande ruta mäts i 1/2 cm origo är i mitten av rutan, initieras i styrinit
	volatile int8_t glob_y; //offset från origo för x i nuvarande ruta mäts i 1/2 cm, initieras i styrinit
	volatile int16_t glob_sum_y; // summera y från hastighet ((TIMECONSTANT*velocity)>>10); 12/1024
	volatile uint8_t glob_v; //hastighet i 1/2 cm/s, initieras i styrinit
	volatile int8_t glob_theta; // Vridning i grader
	volatile int16_t glob_SumTheta; // Vridning i grader
	int8_t glob_thetaOld; // Vridning i grader
	volatile int16_t glob_omega; //grader/sekund, initieras i styrinit
	uint8_t glob_virtual_direction; // initieras i reglering_init
	uint8_t glob_logical_direction; //ska initieras i pathfind_init
	uint8_t glob_syncSpike;

	//pathfind stuff
	volatile uint8_t glob_route[ROUTELENGTH];
	volatile uint8_t glob_routeLength; //initieras i clockedInterupt_init
	
	uint8_t glob_routeSquares[ROUTELENGTH*2]; //rutor som finns med i route
	uint8_t glob_routeSquaresLength; //initieras i pathfind init
	
	uint8_t glob_metaRoute[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t glob_metaRouteLenght; //initieras i pathfind init
	
	uint8_t glob_shouldPathfind; //initieras i pathfind init
	
	uint8_t glob_adjecentNewSquares[METAROUTEMAXLEN]; //jämna index X och udda Y
	uint8_t glob_adjecentNewSquaresLenght; //initieras i pathfind init

	uint8_t glob_map[16][16]; //initieras i pathfind_init

	volatile CircularBuffer glob_debugMesssageBuffer;
	volatile uint8_t glob_debugMesssageBufferLength;

	volatile uint8_t glob_curComm; //initieras i pathfind init

	volatile CircularBuffer glob_mapDataToSend; //initieras i clockedInterupt_init

	volatile uint8_t glob_longFront; // cm
	volatile uint8_t glob_longRight; // cm
	volatile uint8_t glob_longRear; // cm
	volatile uint8_t glob_longLeft; // cm
	volatile uint8_t glob_shortFrontRight; // cm/2
	volatile uint8_t glob_shortFrontLeft; // cm/2
	volatile uint8_t glob_shortRearRight; // cm/2
	volatile uint8_t glob_shortRearLeft; // cm/2
	
	volatile uint8_t glob_longFront_old; // cm
	volatile uint8_t glob_longRight_old; // cm
	volatile uint8_t glob_longRear_old; // cm
	volatile uint8_t glob_longLeft_old; // cm
	volatile uint8_t glob_shortFrontRight_old; // cm/2
	volatile uint8_t glob_shortFrontLeft_old; // cm/2
	volatile uint8_t glob_shortRearRight_old; // cm/2
	volatile uint8_t glob_shortRearLeft_old; // cm/2
	
	int16_t glob_gyro; // grader/s
	
	volatile uint8_t glob_vRight; // cm/s
	volatile uint8_t glob_vLeft; // cm/s

	volatile int8_t glob_vRightSign;
	volatile int8_t glob_vLeftSign;

	volatile int16_t glob_omegaWheelSum;// grader / ca 16 egentligen 17 mellan hjulen

	//parametrar
	volatile uint8_t glob_paramCustomLeft;
	volatile uint8_t glob_paramCustomRight;
	//rakt reglering
	volatile uint8_t glob_L1_straightX;
	volatile uint8_t glob_L2_straightTheta;
	volatile uint8_t glob_L3_straightOmega;
	//sväng
	volatile uint8_t glob_L1_turnTheta;
	volatile uint8_t glob_L2_turnOmega;
	
	volatile int16_t glob_max;
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

uint8_t getSensorLongForwardOld();
uint8_t getSensorLongRearOld();
uint8_t getSensorLongLeftOld();
uint8_t getSensorLongRightOld();
uint8_t getSensorShortLeftForwardOld();
uint8_t getSensorShortLeftRearOld();
uint8_t getSensorShortRightForwardOld();
uint8_t getSensorShortRightRearOld();

#endif /* GLOBAL_H_ */
