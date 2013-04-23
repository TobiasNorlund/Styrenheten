/*
 * observer.h
 *
 * Created: 4/19/2013 3:45:46 PM
 *  Author: carka684
 */ 


#ifndef OBSERVER_H_
#define OBSERVER_H_
#include <avr/io.h>
#include <avr/pgmspace.h>


void setXV(uint8_t shortForward, uint8_t shortRear);
void setThetaOmegaLeft(uint8_t shortLeftForward, uint8_t shortLeftRear);
void setThetaOmegaRight(uint8_t shortRightForward, uint8_t shortRightRear);
void setObserver(int frontLeftSensor, int backLeftSensor, int frontRightSensor, int backRightSensor);




#endif /* OBSERVER_H_ */