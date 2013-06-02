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
#ifndef CHARTING_H_
#define CHARTING_H
#include <avr/io.h>
#include "global.h"
#include "../../TSEA27-include/message.h"




#define SENSOROFFSET_1 80
#define SENSOROFFSET_2 140
#define TIME 10 //i ms

/*
 * Funktion som kartlägger åt alla håll för manuell styrning
 */
void updateMapManual();

/*
 * Funktion som kartlägger åt alla håll för auto styrning
 */
void updateMapAuto();

/*
 * Funktion som kartlägger i en riktning, givet en kartläggningsfunktion för att kartlägga en ruta
 */
void chart(uint8_t logical_direction, void (*charting_func)(uint8_t x, uint8_t y, uint8_t info));

/*
 * Initierar variabler relaterade till pathfinding
 */
void pathfind_init();

/*
 * Lägger till x och y i glob_adjecentNewSquares om denna inte redan finns
 */
void addToAdjecentNewSquares(uint8_t x, uint8_t y);

/*
 * Returnerar om en ruta redan finns i glob_adjecentNewSquares eller glob_metaRoute
 */
uint8_t foundInAdjOrMeta(uint8_t x, uint8_t y);

/*
 * Kartlägger en ruta för manuell styrning
 */
void manual_logical_chart(uint8_t x, uint8_t y, uint8_t info);

/*
 * Kartlägger en ruta för autostyrning
 */
void auto_logical_chart(uint8_t x, uint8_t y, uint8_t info);

#endif /* CHARTING_H_ */