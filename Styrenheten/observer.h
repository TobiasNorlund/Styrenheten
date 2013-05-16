	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: observer.h
	 * Skriven av: C. Karlsson, M. Karlsson, D. Molin
	 *			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Har hand om styrenhetens observatör.
	 * Sätter observatörens värden m.h.a sensorvärden.
	 */	


#ifndef OBSERVER_H_
#define OBSERVER_H_

#define CHASSITOSHORTSIDE 10 //halva cm
#define CHASSITOLONGSIDE 16 //halva cm
#define CHASSITOLONGBACK 22 //halva cm
#define CHASSITOLONGFRONT 22 //halva cm

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "calcAngle.h"
#include "global.h"
#include "StyrReglering.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/utils.h"

#define TIMECONSTANT 50 // ms
#define INVERTTIMECONSTANT 20 //Dimension 1/s
#define HALFSQUAREWIDTH 80 //halva cm
#define OK_SENSOR_VALUE(x) (((x) != 0)||((x) != 255))
#define TIME 4 //i ms, ange i 2^n = t (TIME = 4 -> 16ms)

/** 
 * Sätter hela observatören vid rak reglering.
 */
void straightObserver();

/** 
 * Sätter hela observatören vid reglering vid sväng.
 */
void turnObserver();

/** 
 * Kör straightObserver()/rotateObserver() beroende
 * på vad för kommando som körs.
 */
void observe();

/** 
 * Sätter rätt global variabel beroende på robotens
 * riktning.
 */
void setRelativeX(int8_t val);

/** 
 * Sätter rätt global variabel beroende på robotens
 * riktning.
 */
void setRelativeY(int8_t val);

/** 
 * Returnerar 1/standardavvikelsen för den långa sensorn
 */
uint8_t getSensorLongOverNoise(uint8_t value);

/** 
 *Returnerar 1/standardavvikelsen för den korta sensorn
 */
uint16_t getSensorShortOverNoise(uint8_t value);

/** 
 * Returnerar det största av två tal
 */
int16_t max(int16_t a, int16_t b);

/** 
 * Returnerar sensorVal skiftat så att den ligger inom 
 * intervallet relativeX+-40
 */
int16_t getShiftedSensorX(int16_t sensorVal);

/** 
 * Returnerar sensorVal skiftat så att den ligger inom 
 * intervallet relativeY+-40
 */
int16_t getShiftedSensorY(int16_t sensorVal);

/** 
 * Ändrar den globala positionen beroende på
 * robetens riktning
 */
void moveForward();

/** 
 * Returnerar robotens hastighet
 */
int16_t getVelocity();

#endif /* OBSERVER_H_ */