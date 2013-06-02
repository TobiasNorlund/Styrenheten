/*
 * charting.h
 *
 * Created: 4/17/2013 8:52:34 AM
 *  Author: davmo049
 */ 


#ifndef CHARTING_H_
#define CHARTING_H
#include <avr/io.h>

void updateMapManual();
void updateMapAuto();
void pathfind_init();
void addToAdjecentNewSquares(uint8_t x, uint8_t y);
uint8_t foundInAdjOrMeta(uint8_t x, uint8_t y);

#endif /* CHARTING_H_ */