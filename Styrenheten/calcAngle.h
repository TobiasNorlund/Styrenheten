/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul:Styrenheten
 * Filnamn: calcAngle.h
 * Skriven av: M. Karlsson
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Används för att beräkna vinkeln global_theta vid rakreglering.
 */	

#ifndef CALCANGLE_H_
#define CALCANGLE_H_

#include <avr/pgmspace.h>
#include "global.h"
#include "observer.h"

/**
 * Sätter theta med hjälp av sensorvärden.
 */
void setTheta(uint8_t ShortLeftFront, uint8_t ShortLeftRear, uint8_t ShortRightFront, uint8_t ShortRightRear);

/**
 * Returnerar ett theta som givs av en sidas sensorer.
 * int8_t side, 0 om vänster, 1 om höger
 * return: [grader]
 */
int8_t calcSideSensors36(uint8_t frontDistance,uint8_t rearDistance, int8_t side);

#endif /* CALCANGLE_H_ */
