	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: global.h
	 * Skriven av: I. Junaeus, C. Karlsson Schmidt, M. Karlsson, J. Källström, 
	 *			   D. Molin, T. Norlund		
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Definierar alla de globala variabler som styrenheten använder sig av.
	 */	
	
#include <util/delay.h>
#include <avr/io.h>
#include "../../TSEA27-include/circularbuffer.h"

//Divisionfaktorer för att få högre precision: 2^n

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define START_PIN (PINA & 0b00010000)
#define MANUAL_AUTO_SWITCH_PIN (PINA & 0b00100000)>>5
#define MANUAL_SELECTED 1

#define DIRECTION_FORWARD 1
#define DIRECTION_REVERSE 0

#define LOGICAL_DIR_UP 0
#define LOGICAL_DIR_RIGHT 1
#define LOGICAL_DIR_DOWN 2
#define LOGICAL_DIR_LEFT 3

#define OPEN 0
#define WALL 1
#define UNKNOWN 2 //UNKNOWN if open or wall

#define TURNCOST 1
#define VIRTUALREVERSECOST 1

#define METAROUTEMAXLEN 128
#define CURRENT_SQUARES_SIZE 128

#define ROUTELENGTH 64

#define SHORTFACTOR 1 //Korta sensorn är i cm*2

/** 
 * Robotens nuvarande ruta.
 */
volatile uint8_t glob_mapX; // Xcoord för nuvarande ruta i kartindex initieras i init
volatile uint8_t glob_mapY; // Xcoord för nuvarande ruta i kartindex initieras i init

/** 
 * Observatör värden
 */
volatile int8_t glob_x; // Dimension [cm/2], offset från origo för x i nuvarande ruta,
						// origo är i mitten av rutan, initieras i styrinit
volatile int8_t glob_y; // Dimension [cm/2], offset från origo för x i nuvarande ruta,
						// initieras i styrinit
						
volatile uint8_t glob_v; //Dimension [cm/2s], initieras i styrinit
volatile int8_t glob_theta; // Dimension [grader], vridningen
int8_t glob_thetaOld; // Dimension [cm/2], vridningen
volatile int16_t glob_omega; //Dimension [grader/s], initieras i styrinit

uint8_t glob_virtual_direction; 
uint8_t glob_logical_direction; 

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

/** 
 * Nuvarande värde på avståndsensorerna
 */
volatile uint8_t glob_longFront; // cm
volatile uint8_t glob_longRight; // cm
volatile uint8_t glob_longRear; // cm
volatile uint8_t glob_longLeft; // cm
volatile uint8_t glob_shortFrontRight; // cm/2
volatile uint8_t glob_shortFrontLeft; // cm/2
volatile uint8_t glob_shortRearRight; // cm/2
volatile uint8_t glob_shortRearLeft; // cm/2

/** 
 * Gyrovärdet
 * Dimension: [grader/s]
 */
int16_t glob_gyro; // grader/s

/** 
 * Hjulhastigheter
 */
volatile uint8_t glob_vRight; // cm/s
volatile uint8_t glob_vLeft; // cm/s

/** 
 * Manuellt gaspådrag
 */
volatile uint8_t glob_paramCustomLeft;
volatile uint8_t glob_paramCustomRight;

/** 
 * Reglerparametrar vid rak reglering
 */
volatile uint8_t glob_L1_straightX;
volatile uint8_t glob_L2_straightTheta;
volatile uint8_t glob_L3_straightOmega;

/**
 * Reglerparametrar vid sväng
 */
volatile uint8_t glob_L1_turnTheta;
volatile uint8_t glob_L2_turnOmega;

/** 
 * Bestämmer subtrahenden som används vid
 * bestämning av gaspådrag på motererna.
 */
volatile int16_t glob_max;

/**
 * DEBUG, SKA TAS BORT SENARE
 */
uint8_t glob_syncSpike;

/** 
 * Returnerar sensorvärden från given sensor.
 * Dimension: getSensorLong [cm]
 *	          getSensorShort [cm/2]
 */
uint8_t getSensorLongForward();
uint8_t getSensorLongRear();
uint8_t getSensorLongLeft();
uint8_t getSensorLongRight();
uint8_t getSensorShortLeftForward();
uint8_t getSensorShortLeftRear();
uint8_t getSensorShortRightForward();
uint8_t getSensorShortRightRear();

#endif /* GLOBAL_H_ */