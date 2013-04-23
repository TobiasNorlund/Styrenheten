﻿/*
 * StyrReglering.h
 *
 * Created: 4/16/2013 11:32:40 AM
 *  Author: davmo049
 */ 


#ifndef STYRREGLERING_H_
#define STYRREGLERING_H_

void reglering_init();
void customSteering();
void virtualTurn();
void turnLeft45(); //ska vara utan parametrar
void turnRight90();
void turnLeft90();
void regulateStraight();
void setSpeedRight(uint8_t speed);
void setSpeedLeft(uint8_t speed);
int8_t degToRad(int8_t degree);


#endif /* STYRREGLERING_H_ */