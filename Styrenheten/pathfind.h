	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul: Styrenheten
	 * Filnamn: pathfind.h
	 * Skriven av: D. Molin
	 *			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Algoritmkod för roboten pathfinding.
	 */	

#ifndef PATHFIND_H_
#define PATHFIND_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../TSEA27-include/message.h"
#include "global.h"
#include "charting.h"

void pathfind();
void truncateMetaRoute();
void adjecentNewSquaresRemoveChartedSquares();
void initMetaRoute();
void createNewMetaRoute();
uint16_t norm2pow2(int16_t fromX, int16_t fromY, int16_t toX, int16_t toY);
void createRouteToNextMeta();
uint8_t getCostMove(uint8_t toX, uint8_t toY);
void addCurSQ(uint8_t* buffer, uint8_t* dirBuffer, uint8_t* bufferLength, uint8_t X, uint8_t Y, uint8_t dir);
uint8_t min(uint8_t a, uint8_t b);

#endif /* PATHFIND_H_ */