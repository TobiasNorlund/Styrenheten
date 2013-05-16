	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul: Styrenheten
	 * Filnamn: StyrReglering.h
	 * Skriven av: C. Karlsson Schmidt, M. Karlsson, D. Molin			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Reglerar gaspådraget på robotens hjul givet kommandon.
	 */	
	
#include <avr/io.h>
#include "global.h"

#ifndef STYRREGLERING_H_
#define STYRREGLERING_H_

#define DIVISIONFACTOR  6 // DF = 4 -> 2^4 = 16
#define MAXSPEED 250
#define TURNSPEED 180
#define STOPTURN90 83
#define STOPTURN45 43
#define RIGHTWHEELDIFF 23
#define LENGTH_OFFSET -40
#define ROTMIN 100
#define TURN_FINE 8

/** 
 * Initierar regleringen:
 * Sätter globala variabler och initierar
 * register för PWM
 */
void regulate_init();

/** 
 * Sätter hastighet på roboten frammåt enligt
 * gaspådrag givna i glob_paramCustomRight/Left
 */
void customSteering();

/** 
 * Ändrar riktning på gaspådragen och ändrar den
 * globala riktningen.
 */
void virtualTurn();

/** 
 * Svänger 45grader åt vänster
 */
void turnRight45(); 

/** 
 * Svänger 45grader åt vänster
 */
void turnLeft45(); 

/** 
 * Svänger 90 grader åt höger
 */
void turnRight90();

/** 
 * Svänger 90 grader åt vänster
 */
void turnLeft90();

/** 
 * Kör en ruta rakt fram
 */
void regulateStraight();

/** 
 * Sätter hastigheten på höger hjulpar
 */
void setSpeedRight(uint8_t speed);

/** 
 * Sätter hastigheten på vänster hjulpar
 */
void setSpeedLeft(uint8_t speed);

/** 
 * Omvandlar inargument [grader] till utargument [radianer]
 */
int16_t degToRad(int8_t degree);

/** 
 * Sätter riktning på vänster hjulpar
 */
void setDirLeft(uint8_t dir);

/** 
 * Sätter riktning på höger hjulpar
 */
void setDirRight(uint8_t dir);

/** 
 * Returnerar x-värdet för roboten i dess eget
 * koordinatsystem
 */
int8_t getRelativeX(void);

/** 
 * Returnerar y-värdet för roboten i dess eget
 * koordinatsystem
 */
int8_t getRelativeY(void);

/** 
 * Rättar till vinkeln efter sväng, om möjligt.
 */
void cleanUpAngle();

#endif /* STYRREGLERING_H_ */