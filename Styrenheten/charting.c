/*
 * charting.c
 *
 * Created: 4/17/2013 8:52:18 AM
 *  Author: davmo049
 */ 

#define SENSOROFFSET_1 60
#define SENSOROFFSET_2 130

#include "charting.h"
#include "global.h"

void pathfind_init()
{
	globals.logical_direction = LOGICAL_DIR_UP;
}

void manual_logical_chart(uint8_t x, uint8_t y, uint8_t info)
{
	if(globals.map[y][x] != info)
	{
		globals.map[y][x] = info;
		//send data to PC
		globals.mapDataToSend[globals.mapDataToSendSize] = x;
		globals.mapDataToSend[globals.mapDataToSendSize+1] = y;
		globals.mapDataToSendSize = globals.mapDataToSendSize+2;	
	}
}

uint8_t foundInAdjOrMeta(uint8_t x, uint8_t y)
{
	uint8_t i = 0;
	while(i < globals.adjecentNewSquaresLenght>>1)
	{
		if(globals.adjecentNewSquares[i] == x && globals.adjecentNewSquares[i+1] == y)
		{
			return 1;
		}
		i = i+2;
	}
	i = 0;
	while(i < globals.metaRouteLenght/2)
	{
		if(globals.metaRoute[i] == x && globals.metaRoute[i+1] == y)
		{
			return 1;
		}
		i = i+2;
	}
	return 0;
}

void addToAdjecentNewSquares(uint8_t x, uint8_t y)
{
	if(foundInAdjOrMeta(x, y) == 0 && globals.map[y][x] == UNKNOWN)
	{
		globals.adjecentNewSquares[globals.adjecentNewSquaresLenght] = x;
		globals.adjecentNewSquares[globals.adjecentNewSquaresLenght+1] = y;
		globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght +2;
	}
}

void auto_logical_chart(uint8_t x, uint8_t y, uint8_t info)
{
	if(globals.map[y][x] != info)
	{
		globals.map[y][x] = info;
		//send data to PC
		globals.mapDataToSend[globals.mapDataToSendSize] = x;
		globals.mapDataToSend[globals.mapDataToSendSize+1] = y;
		globals.mapDataToSendSize = globals.mapDataToSendSize+2;
		if(info == 0)
		{
			addToAdjecentNewSquares(x+1, y);
			addToAdjecentNewSquares(x-1, y);
			addToAdjecentNewSquares(x, y+1);
			addToAdjecentNewSquares(x, y-1);
		}
		if(x == globals.metaRoute[globals.metaRouteLenght-2] && y == globals.metaRoute[globals.metaRouteLenght-1])
		{
			globals.shouldPathfind = 1;
		}
		for(uint8_t i = 0; i < globals.routeSquaresLength; i = i+2)
		{
			if(globals.routeSquares[i] == x && globals.routeSquares[i+1] == y)
			{
				globals.shouldPathfind = 1;
				break;
			}
		}
	}
}

void chart(uint8_t logical_direction, void (*charting_func)(uint8_t x, uint8_t y, uint8_t info))
{
	uint8_t sensorLength;
	uint8_t targetSensor = (logical_direction+(4-globals.logical_direction))%4;
	switch(targetSensor)
	{
		case LOGICAL_DIR_UP:
			sensorLength = getSensorLongForward();
			break;
		case LOGICAL_DIR_RIGHT:
			sensorLength = getSensorLongRight();
			break;
		case LOGICAL_DIR_DOWN:
			sensorLength = getSensorLongBack();
			break;
		case LOGICAL_DIR_LEFT:
			sensorLength = getSensorLongLeft();
			break;
		default:
			break;
	}
	switch(logical_direction)
	{
		case(LOGICAL_DIR_UP):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(globals.mapX, globals.mapY+1, OPEN);
				charting_func(globals.mapX, globals.mapY+2, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(globals.mapX, globals.mapY+2, WALL);
				charting_func(globals.mapX, globals.mapY+1, OPEN);
			}
			else
			{
				charting_func(globals.mapX, globals.mapY+1, WALL);
			}						
			break;
		case(LOGICAL_DIR_RIGHT):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(globals.mapX+1, globals.mapY, OPEN);
				charting_func(globals.mapX+2, globals.mapY, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(globals.mapX+2, globals.mapY, WALL);
				charting_func(globals.mapX+1, globals.mapY, OPEN);
			}
			else
			{
				charting_func(globals.mapX+1, globals.mapY, WALL);
			}
			break;
		case(LOGICAL_DIR_DOWN):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(globals.mapX, globals.mapY-1, OPEN);
				charting_func(globals.mapX, globals.mapY-2, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(globals.mapX, globals.mapY-2, WALL);
				charting_func(globals.mapX, globals.mapY-1, OPEN);
			}
			else
			{
				charting_func(globals.mapX, globals.mapY-1, WALL);
			}
			break;
		case(LOGICAL_DIR_LEFT):
			if(SENSOROFFSET_2 < sensorLength)
			{
				charting_func(globals.mapX-1, globals.mapY, OPEN);
				charting_func(globals.mapX-2, globals.mapY, OPEN);
			}
			else if(SENSOROFFSET_1 < sensorLength)
			{
				charting_func(globals.mapX-2, globals.mapY, WALL);
				charting_func(globals.mapX-1, globals.mapY, OPEN);
			}
			else
			{
				charting_func(globals.mapX-1, globals.mapY, WALL);
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