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
	 * Algoritmkod för robotens pathfinding.
	 */	

#ifndef PATHFIND_H_
#define PATHFIND_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Uppdaterar metarutten samt skapar en rutt (dvs en serie kommandon) som tar roboten till nästa oupptäckta ruta
 */
void pathfind();
/*
 * Hittar det element i metarutten som är nyligen upptäckt och är sist, samt flyttar alla element i metarutten som är före detta element och lägger till dessa i glob_adjecentNewSquares
 */
void truncateMetaRoute();
/*
 * Tar bort alla element i glob_adjecentNewSquares som representerar en ruta som redan har blivit kartlagd
 */
void adjecentNewSquaresRemoveChartedSquares();
/*
 * Skapar en metarutt om det inte finns en sådan.
 */
void initMetaRoute();
/*
 * Lägger till alla element i glob_adjecentNewSquares till metarutten
 */
void createNewMetaRoute();
/*
 * Returnerar avstånd från (x1, y1) till (x2, y2)
 */
uint16_t norm2pow2(int16_t fromX, int16_t fromY, int16_t toX, int16_t toY);
/*
 * Skapar en rutt till första elementet i metarutten.
 */
void createRouteToNextMeta();
/*
 * Returnerar kostnaden att gå in på rutan (toX, toY)
 */
uint8_t getCostMove(uint8_t toX, uint8_t toY);
/*
 * Lägger till en x, y koordinat i en buffer om denna koordinat ej redan finns
 */
void addCurSQ(uint8_t* buffer, uint8_t* dirBuffer, uint8_t* bufferLength, uint8_t X, uint8_t Y, uint8_t dir);
/*
 * Returnerar mininmum av a och b
 */
uint8_t min(uint8_t a, uint8_t b);

#endif /* PATHFIND_H_ */