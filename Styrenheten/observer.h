/*
 * observer.h
 *
 * Created: 4/19/2013 3:45:46 PM
 *  Author: carka684
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
#include "../../TSEA27-include/geometrics.h"
#include "StyrReglering.h"

#define OK_SENSOR_VALUE(x) (((x) != 0)||((x) != 255))
#define TIME 4 //i ms, ange i 2^n = t (TIME = 4 -> 16ms)

void setXV(uint8_t shortForward, uint8_t shortRear, uint8_t side);
//void setThetaOmegaLeft(uint8_t shortLeftForward, uint8_t shortLeftRear);
//void setThetaOmegaRight(uint8_t shortRightForward, uint8_t shortRightRear);
void setOmega();
void straightObserver();
void rotateObserver();
void observe();
void setRelativeX(int8_t val);
void setRelativeY(int8_t val);
void turnObserver();

#endif /* OBSERVER_H_ */