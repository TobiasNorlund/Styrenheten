	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: charting.h
	 * Skriven av: D. Molin
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Sköter kartläggningen av banan i styrenheten.
	 */	

#include <avr/io.h>
#include "global.h"
#include "../../TSEA27-include/message.h"

#ifndef CHARTING_H_
#define CHARTING_H

#define SENSOROFFSET_1 80
#define SENSOROFFSET_2 140
#define TIME 10 //i ms

void updateMapManual();
void updateMapAuto();
void pathfind_init();
void addToAdjecentNewSquares(uint8_t x, uint8_t y);
uint8_t foundInAdjOrMeta(uint8_t x, uint8_t y);

#endif /* CHARTING_H_ */