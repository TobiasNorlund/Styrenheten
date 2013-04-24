/*
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
void turnLeft45(int theta, int omega); //ska vara utan parametrar
void turnRight90(int theta, int omega);
void turnLeft90(int theta, int omega);
void regulateStraight(uint8_t x, uint8_t v, uint8_t theta, uint8_t omega);
void setSpeedRight(uint8_t speed);
void setSpeedLeft(uint8_t speed);
void setDirLeft(uint8_t dir);
void setDirRight(uint8_t dir)


#endif /* STYRREGLERING_H_ */