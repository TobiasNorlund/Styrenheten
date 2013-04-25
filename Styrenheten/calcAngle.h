/*
 * calcAngle.h
 *
 * Created: 4/25/2013 7:14:49 PM
 *  Author: mikka789
 */ 


#ifndef CALCANGLE_H_
#define CALCANGLE_H_

#include <avr/pgmspace.h>
#include "globals.h"
#include "observer.h"

void calcAngle(uint8_t LongFront, uint8_t LongRear, uint8_t LongLeft, uint8_t LongRight, uint8_t ShortLeftFront, uint8_t ShortLeftRear, uint8_t ShortRightFront, uint8_t ShortRightRear);

uint8_t calcOppositeShortK(uint8_t leftShortSensor, uint8_t rightShortSensor);
uint8_t calcOppositeLongK(uint8_t leftLongSensor, uint8_t rightLongSensor);
uint8_t calc36K(uint8_t shortFront, uint8_t shortRear);
uint8_t calc19K(uint8_t short, uint8_t long);
uint8_t calcKOmega(uint16_t omega);
uint8_t calcKirSensorK(uint8_t angle);
int8_t calcOppositeSensors(uint8_t rightFrontSensor,uint8_t leftFrontSensor);
int8_t calcSideSensors36(uint8_t frontDistance,uint8_t rearDistance, int8_t side);
int8_t calcSideSensors19(uint8_t frontDistance,uint8_t longDistance, int8_t side);

#endif /* CALCANGLE_H_ */
