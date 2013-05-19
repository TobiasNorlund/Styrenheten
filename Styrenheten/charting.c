	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul:Styrenheten
	 * Filnamn: charting.c
	 * Skriven av: D. Molin
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Sköter kartläggningen av banan i styrenheten.
	 */	

#include "observer.h"

#include "charting.h"

/*
* initierar variabler relaterade till pathfinding
*/
void pathfind_init()
{
	for(uint8_t i = 0; i < 16; ++i)
	{
		for(uint8_t j = 0; j < 16; ++j)
		{
			glob_map[j][i] = UNKNOWN;
		}
	}
	glob_map[8][8] = OPEN;
	glob_logical_direction = LOGICAL_DIR_UP;
	glob_routeSquaresLength = 0;
	glob_metaRouteLenght = 0;
	glob_shouldPathfind = 1;
	glob_adjecentNewSquaresLenght  = 0;
	glob_curComm = NULL_COMMAND;
}

/*
* kartlägger en ruta för manuell styrning
*/
void manual_logical_chart(uint8_t x, uint8_t y, uint8_t info)
{
	if(glob_map[y][x] != info)
	{
		glob_map[y][x] = info;
		//send data to PC
		cbWrite(&glob_mapDataToSend, x);
		cbWrite(&glob_mapDataToSend, y);
	}
}

/*
* kartlägger en ruta för autostyrning
*/
void auto_logical_chart(uint8_t x, uint8_t y, uint8_t info)
{
	if(glob_map[y][x] != info)
	{
		glob_map[y][x] = info;
		//send data to PC
		cbWrite(&glob_mapDataToSend, x);
		cbWrite(&glob_mapDataToSend, y);
		if(info == 0)
		{
			addToAdjecentNewSquares(x+1, y);
			addToAdjecentNewSquares(x-1, y);
			addToAdjecentNewSquares(x, y+1);
			addToAdjecentNewSquares(x, y-1);
		}
		if(x == glob_metaRoute[glob_metaRouteLenght-2] && y == glob_metaRoute[glob_metaRouteLenght-1])
		{
			glob_shouldPathfind = 1;
		}
		for(uint8_t i = 0; i < glob_routeSquaresLength; i = i+2)
		{
			if(glob_routeSquares[i] == x && glob_routeSquares[i+1] == y)
			{
				glob_shouldPathfind = 1;
				break;
			}
		}
	}
}

uint8_t foundInAdjOrMeta(uint8_t x, uint8_t y)
{
	uint8_t i = 0;
	while(i < glob_adjecentNewSquaresLenght>>1)
	{
		if(glob_adjecentNewSquares[i] == x && glob_adjecentNewSquares[i+1] == y)
		{
			return 1;
		}
		i = i+2;
	}
	i = 0;
	while(i < glob_metaRouteLenght/2)
	{
		if(glob_metaRoute[i] == x && glob_metaRoute[i+1] == y)
		{
			return 1;
		}
		i = i+2;
	}
	return 0;
}

void addToAdjecentNewSquares(uint8_t x, uint8_t y)
{
	if(foundInAdjOrMeta(x, y) == 0 && glob_map[y][x] == UNKNOWN)
	{
		glob_adjecentNewSquares[glob_adjecentNewSquaresLenght] = x;
		glob_adjecentNewSquares[glob_adjecentNewSquaresLenght+1] = y;
		glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght +2;
	}
}

void chart(uint8_t logical_direction, void (*charting_func)(uint8_t x, uint8_t y, uint8_t info))
{
	int16_t sensorLength;
	int16_t posOffset;
	uint8_t targetSensor = (logical_direction+(4-glob_logical_direction))%4;
	
	switch(logical_direction)
	{
		case LOGICAL_DIR_UP:
			posOffset = int8to16(glob_y)>>1;
			break;
		case LOGICAL_DIR_RIGHT:
			posOffset = int8to16(glob_x)>>1;
			break;
		case LOGICAL_DIR_DOWN:
			posOffset = int8to16(-glob_y)>>1;
			break;
		default:
			posOffset = int8to16(-glob_x)>>1;
			break;
	}
	
	switch(targetSensor)
	{
		case LOGICAL_DIR_UP:
			sensorLength = getSensorLongForward()+(CHASSITOLONGFRONT>>1);
			break;
		case LOGICAL_DIR_RIGHT:
			sensorLength = getSensorLongRight()+(CHASSITOLONGSIDE>>1);
			break;
		case LOGICAL_DIR_DOWN:
			sensorLength = getSensorLongRear()+(CHASSITOLONGBACK>>1);
			break;
		default:
			sensorLength = getSensorLongLeft()+(CHASSITOLONGSIDE>>1);
			break;
	}
	sensorLength = sensorLength+posOffset;
	switch(logical_direction)
	{
		case(LOGICAL_DIR_UP):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(glob_mapX, glob_mapY+1, OPEN);
				charting_func(glob_mapX, glob_mapY+2, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(glob_mapX, glob_mapY+2, WALL);
				charting_func(glob_mapX, glob_mapY+1, OPEN);
			}
			else
			{
				charting_func(glob_mapX, glob_mapY+1, WALL);
			}						
			break;
		case(LOGICAL_DIR_RIGHT):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(glob_mapX+1, glob_mapY, OPEN);
				charting_func(glob_mapX+2, glob_mapY, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(glob_mapX+2, glob_mapY, WALL);
				charting_func(glob_mapX+1, glob_mapY, OPEN);
			}
			else
			{
				charting_func(glob_mapX+1, glob_mapY, WALL);
			}
			break;
		case(LOGICAL_DIR_DOWN):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(glob_mapX, glob_mapY-1, OPEN);
				charting_func(glob_mapX, glob_mapY-2, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(glob_mapX, glob_mapY-2, WALL);
				charting_func(glob_mapX, glob_mapY-1, OPEN);
			}
			else
			{
				charting_func(glob_mapX, glob_mapY-1, WALL);
			}
			break;
		case(LOGICAL_DIR_LEFT):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(glob_mapX-1, glob_mapY, OPEN);
				charting_func(glob_mapX-2, glob_mapY, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(glob_mapX-2, glob_mapY, WALL);
				charting_func(glob_mapX-1, glob_mapY, OPEN);
			}
			else
			{
				charting_func(glob_mapX-1, glob_mapY, WALL);
			}
			break;
		default:
			break;
	}
	return;
}

void updateMapManual()
{
	chart(LOGICAL_DIR_UP, &manual_logical_chart);
	chart(LOGICAL_DIR_RIGHT, &manual_logical_chart);
	chart(LOGICAL_DIR_DOWN, &manual_logical_chart);
	chart(LOGICAL_DIR_LEFT, &manual_logical_chart);
}

void updateMapAuto()
{
	chart(LOGICAL_DIR_UP, &auto_logical_chart);
	chart(LOGICAL_DIR_RIGHT, &auto_logical_chart);
	chart(LOGICAL_DIR_DOWN, &auto_logical_chart);
	chart(LOGICAL_DIR_LEFT, &auto_logical_chart);
}