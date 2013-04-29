/*
 * pathfind.c
 *
 * Created: 4/29/2013 11:43:58 AM
 *  Author: davmo049
 */ 

#include "pathfind.h"
#include "../../TSEA27-include/message.h"
#include "global.h"
#include "charting.h"
#include <string.h>


void pathfind()
{
	globals.shouldPathfind = 0;
	truncateMetaRoute(); //flyttar över element från metarutten till adj. new squares enl designspec.
	adjecentNewSquaresRemoveChartedSquares(); //tar bort element från adj. new squares som har blivit upptäkta
	if(globals.metaRouteLenght == 0)
	{
		if(globals.adjecentNewSquaresLenght == 0)
		{
			return;
		}
		initMetaRoute();
	}
	else
	{
		createNewMetaRoute();
	}
	createRouteToNextMeta();
}

void truncateMetaRoute()
{
	for(uint8_t i = 0; i < globals.metaRouteLenght; i=i+2)
	{
		if(globals.map[globals.metaRoute[i+1]][globals.metaRoute[i]] != UNKNOWN)
		{
			uint8_t metaRouteSave = globals.metaRouteLenght;
			globals.metaRouteLenght = i;
			while(i < metaRouteSave)
			{
				if(foundInAdjOrMeta(globals.metaRoute[i], globals.metaRoute[i+1]) == 0)
				{
					globals.adjecentNewSquares[globals.adjecentNewSquaresLenght] = globals.metaRoute[i];
					globals.adjecentNewSquares[globals.adjecentNewSquaresLenght+1] = globals.metaRoute[i+1];
					globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght+2;
				}
				i = i+2;
			}
			return;
		}
	}
}

void adjecentNewSquaresRemoveChartedSquares()
{
	uint8_t i = 0;
	while(i < globals.adjecentNewSquaresLenght)
	{
		if(globals.map[i+1][i] != UNKNOWN)
		{
			//ta bort elementet
			uint8_t k = i;
			while(k < globals.adjecentNewSquaresLenght)
			{
				globals.adjecentNewSquares[k] = globals.adjecentNewSquares[k+2];
				k = k + 1;
			}
			globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght-2;
		}			
		else
		{
			i = i+2;
		}
	}
}

void initMetaRoute()
{
	//get square closest to robot
	uint8_t closestSquareIndex = 1;
	uint16_t smallestLength = 500;
	uint8_t i = 0;
	while(i<globals.adjecentNewSquaresLenght)
	{
		uint16_t lenCand = norm2pow2(globals.adjecentNewSquares[i], globals.adjecentNewSquares[i+1], globals.mapX, globals.mapY);
		if(lenCand < smallestLength)
		{
			smallestLength = lenCand;
			closestSquareIndex = i;
		}
		i = i+2;
	}
	//add this square to the meta route
	globals.metaRoute[METAROUTEMAXLEN-2] = globals.adjecentNewSquares[closestSquareIndex];
	globals.metaRoute[METAROUTEMAXLEN-1] = globals.adjecentNewSquares[closestSquareIndex+1];
	globals.metaRouteLenght = 2; //ska vara tom innan
	uint8_t j = closestSquareIndex;
	while(j < globals.adjecentNewSquaresLenght)
	{
		globals.adjecentNewSquares[j] = globals.adjecentNewSquares[j+2];
		j = j + 1;
	}
	globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght-2;
	//for each adj square
	while(globals.adjecentNewSquaresLenght != 0)
	{
		//find square closest last order in meta route
		smallestLength = 500;
		i = 0;
		while(i<globals.adjecentNewSquaresLenght)
		{
			uint16_t lenCand = norm2pow2(globals.adjecentNewSquares[i], globals.adjecentNewSquares[i+1], globals.metaRoute[METAROUTEMAXLEN-globals.metaRouteLenght], globals.metaRoute[METAROUTEMAXLEN-globals.metaRouteLenght+1]);
			if(lenCand < smallestLength)
			{
				smallestLength = lenCand;
				closestSquareIndex = i;
			}
			i = i+2;
		}
		//add this square to the meta route
		globals.metaRoute[METAROUTEMAXLEN-globals.metaRouteLenght-2] = globals.adjecentNewSquares[closestSquareIndex];
		globals.metaRoute[METAROUTEMAXLEN-globals.metaRouteLenght-1] = globals.adjecentNewSquares[closestSquareIndex+1];
		globals.metaRouteLenght = globals.metaRouteLenght + 2;
		j = closestSquareIndex;
		while(j < globals.adjecentNewSquaresLenght)
		{
			globals.adjecentNewSquares[j] = globals.adjecentNewSquares[j+2];
			j = j + 1;
		}
		globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght-2;
	}
	i = 0;
	while(i < globals.metaRouteLenght)
	{
		globals.metaRoute[i] = globals.metaRoute[i+METAROUTEMAXLEN-globals.metaRouteLenght];
		i = i+1;
	}
}

void createNewMetaRoute()
{
	while(globals.adjecentNewSquaresLenght != 0)
	{
		uint8_t closestSquareIndex = 0;
		uint16_t smallestLength = 500;
		uint8_t i = 0;
		//find the square which is closest to the last in meta route
		while(i<globals.adjecentNewSquaresLenght)
		{
			uint16_t lenCand = norm2pow2(globals.adjecentNewSquares[i], globals.adjecentNewSquares[i+1], globals.metaRoute[globals.metaRouteLenght-2], globals.metaRoute[globals.metaRouteLenght-1]);
			if(lenCand < smallestLength)
			{
				smallestLength = lenCand;
				closestSquareIndex = i;
			}
			i = i+2;
		}
		//add this square to the meta route
		globals.metaRoute[globals.metaRouteLenght] = globals.adjecentNewSquares[closestSquareIndex];
		globals.metaRoute[globals.metaRouteLenght+1] = globals.adjecentNewSquares[closestSquareIndex+1];
		globals.metaRouteLenght = globals.metaRouteLenght + 2;
		//remove this square from the candidates
		uint8_t j = closestSquareIndex;
		while(j < globals.adjecentNewSquaresLenght)
		{
			globals.adjecentNewSquares[j] = globals.adjecentNewSquares[j+2];
			j = j + 1;
		}
		globals.adjecentNewSquaresLenght = globals.adjecentNewSquaresLenght-2;
	}
}

uint16_t norm2pow2(int16_t fromX, int16_t fromY, int16_t toX, int16_t toY) //ska vara short för att undika overflow vid ^2 och underflow vid fromX-toX
{
	return (fromX-toX)*(fromX-toX)+(fromY-toY)*(fromY-toY); //går att optimera om det behövs
}

void createRouteToNextMeta()
{
	uint8_t mapCostRight[17][17];
	uint8_t mapCostLeft[17][17];
	uint8_t mapCostUp[17][17];
	uint8_t mapCostDown[17][17];

	uint8_t currentSquares[CURRENT_SQUARES_SIZE*2]; //X, Y
	uint8_t directionCurrentSquares[CURRENT_SQUARES_SIZE];

	uint8_t currentSquaresBuffer[CURRENT_SQUARES_SIZE*2]; //X, Y
	uint8_t directionCurrentSquaresBuffer[CURRENT_SQUARES_SIZE];
	uint8_t currentSquaresLengthBuffer = 0; //anger hur många X,Y element som finns, inte hur många celler som är fyllda
	uint8_t i = 0;
	while(i < 17)
	{
		uint8_t j = 0;
		while(j < 17)
		{
			mapCostUp[j][i] = 128;
			mapCostDown[j][i] = 128;
			mapCostLeft[j][i] = 128;
			mapCostRight[j][i] = 128;
			j = j+1;
		}
		i = i+1;
	}
	//init values
	currentSquares[0] = globals.mapX;
	currentSquares[1] = globals.mapY;
	uint8_t currentSquaresLength = 1; //HÄR
	if(globals.logical_direction == LOGICAL_DIR_UP) //beror på hur riktning skall vara kodad senare
	{
		directionCurrentSquares[0] = LOGICAL_DIR_UP;
		mapCostUp[globals.mapY][globals.mapX] = 0;
	}
	else if(globals.logical_direction == LOGICAL_DIR_RIGHT)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_RIGHT;
		mapCostRight[globals.mapY][globals.mapX] = 0;
	}
	else if(globals.logical_direction == LOGICAL_DIR_DOWN)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_DOWN;
		mapCostDown[globals.mapY][globals.mapX] = 0;
	}
	else if(globals.logical_direction == LOGICAL_DIR_LEFT)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_LEFT;
		mapCostLeft[globals.mapY][globals.mapX] = 0;
	}
	//for each
	uint8_t notDone = 1;
	while(notDone)
	{
		//så länge det finns kvar rutor att iterera över
		while(currentSquaresLength != 0)
		{
			uint8_t curXIter = currentSquares[currentSquaresLength*2-2];
			uint8_t curYIter = currentSquares[currentSquaresLength*2-1];
			//uppdatera denna ruta
			if(directionCurrentSquares[currentSquaresLength-1] == LOGICAL_DIR_UP)
			{
				//rutan är i upppos.
				uint8_t toXIter = curXIter;
				uint8_t toYIter = curYIter+1;
				if(mapCostUp[curYIter][curXIter] + getCostMove(toXIter, toYIter) < mapCostUp[toYIter][toXIter])
				{
					mapCostUp[toYIter][toXIter] = mapCostUp[curYIter][curXIter] + getCostMove(toXIter, toYIter);
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, toXIter, toYIter, LOGICAL_DIR_UP);
				}
				if(mapCostUp[curYIter][curXIter] + TURNCOST < mapCostRight[curYIter][curXIter])
				{
					mapCostRight[curYIter][curXIter] = mapCostUp[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_RIGHT);
				}
				if(mapCostUp[curYIter][curXIter] + TURNCOST < mapCostLeft[curYIter][curXIter])
				{
					mapCostLeft[curYIter][curXIter] = mapCostUp[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_LEFT);
				}
				if(mapCostUp[curYIter][curXIter] + VIRTUALREVERSECOST < mapCostDown[curYIter][curXIter])
				{
					mapCostDown[curYIter][curXIter] = mapCostUp[curYIter][curXIter] + VIRTUALREVERSECOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_DOWN);
				}
			}
			else if(directionCurrentSquares[currentSquaresLength-1] == LOGICAL_DIR_RIGHT)
			{
				//rutan är högerriktning
				uint8_t toXIter = curXIter+1;
				uint8_t toYIter = curYIter;
				if(mapCostRight[curYIter][curXIter] + getCostMove(toXIter, toYIter) < mapCostRight[toYIter][toXIter])
				{
					mapCostRight[toYIter][toXIter] = mapCostRight[curYIter][curXIter] + getCostMove(toXIter, toYIter);
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, toXIter, toYIter, LOGICAL_DIR_RIGHT);
				}
				if(mapCostRight[curYIter][curXIter] + TURNCOST < mapCostUp[curYIter][curXIter])
				{
					mapCostUp[curYIter][curXIter] = mapCostRight[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_UP);
				}
				if(mapCostRight[curYIter][curXIter] + TURNCOST < mapCostDown[curYIter][curXIter])
				{
					mapCostDown[curYIter][curXIter] = mapCostRight[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_DOWN);
				}
				if(mapCostRight[curYIter][curXIter] + VIRTUALREVERSECOST < mapCostLeft[curYIter][curXIter])
				{
					mapCostLeft[curYIter][curXIter] = mapCostRight[curYIter][curXIter] + VIRTUALREVERSECOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_LEFT);
				}
			}
			else if(directionCurrentSquares[currentSquaresLength-1] == LOGICAL_DIR_DOWN)
			{
				//rutan är i upppos.
				uint8_t toXIter = curXIter;
				uint8_t toYIter = curYIter-1;
				if(mapCostDown[curYIter][curXIter] + getCostMove(toXIter, toYIter) < mapCostDown[toYIter][toXIter])
				{
					mapCostDown[toYIter][toXIter] = mapCostDown[curYIter][curXIter] + getCostMove(toXIter, toYIter);
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, toXIter, toYIter, LOGICAL_DIR_DOWN);
				}
				if(mapCostDown[curYIter][curXIter] + TURNCOST < mapCostRight[curYIter][curXIter])
				{
					mapCostRight[curYIter][curXIter] = mapCostDown[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_RIGHT);
				}
				if(mapCostDown[curYIter][curXIter] + TURNCOST < mapCostLeft[curYIter][curXIter])
				{
					mapCostLeft[curYIter][curXIter] = mapCostDown[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_LEFT);
				}
				if(mapCostDown[curYIter][curXIter] + VIRTUALREVERSECOST < mapCostUp[curYIter][curXIter])
				{
					mapCostUp[curYIter][curXIter] = mapCostDown[curYIter][curXIter] + VIRTUALREVERSECOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_UP);
				}
			}
			else
			{
				//rutan är vänster
				uint8_t toXIter = curXIter-1;
				uint8_t toYIter = curYIter;
				if(mapCostLeft[curYIter][curXIter] + getCostMove(toXIter, toYIter) < mapCostLeft[toYIter][toXIter])
				{
					mapCostLeft[toYIter][toXIter] = mapCostLeft[curYIter][curXIter] + getCostMove(toXIter, toYIter);
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, toXIter, toYIter, LOGICAL_DIR_LEFT);
				}
				if(mapCostLeft[curYIter][curXIter] + TURNCOST < mapCostUp[curYIter][curXIter])
				{
					mapCostUp[curYIter][curXIter] = mapCostLeft[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_UP);
				}
				if(mapCostLeft[curYIter][curXIter] + TURNCOST < mapCostDown[curYIter][curXIter])
				{
					mapCostDown[curYIter][curXIter] = mapCostLeft[curYIter][curXIter] + TURNCOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_DOWN);
				}
				if(mapCostLeft[curYIter][curXIter] + VIRTUALREVERSECOST < mapCostRight[curYIter][curXIter])
				{
					mapCostRight[curYIter][curXIter] = mapCostLeft[curYIter][curXIter] + VIRTUALREVERSECOST;
					addCurSQ(currentSquaresBuffer, directionCurrentSquaresBuffer, &currentSquaresLengthBuffer, curXIter, curYIter, LOGICAL_DIR_RIGHT);
				}
			}
			currentSquaresLength = currentSquaresLength - 1;
		}
		memcpy(currentSquares, currentSquaresBuffer, currentSquaresLengthBuffer*2);
		memcpy(directionCurrentSquares, directionCurrentSquaresBuffer, currentSquaresLengthBuffer); //kan krasha allt möjligt om currentSquaresLengthBuffer >  CURRENT_SQUARES_SIZE
		currentSquaresLength = currentSquaresLengthBuffer;
		currentSquaresLengthBuffer = 0;
		if(currentSquaresLength == 0) //kan optimeras till current rutor har en kostnad som är större än mårutans kostnad
		{
			notDone = 0;
		}
		uint8_t minCurSqCost = 128;
		for(uint8_t i = 0; i < currentSquaresLength; ++i)
		{
			uint8_t cand;
			switch(directionCurrentSquares[i])
			{
				case LOGICAL_DIR_UP:
					cand = mapCostUp[currentSquares[2*i+1]][currentSquares[2*i]];
					break;
				case LOGICAL_DIR_RIGHT:
					cand = mapCostRight[currentSquares[2*i+1]][currentSquares[2*i]];
					break;
				case LOGICAL_DIR_DOWN:
					cand = mapCostDown[currentSquares[2*i+1]][currentSquares[2*i]];
					break;
				default:
					cand = mapCostLeft[currentSquares[2*i+1]][currentSquares[2*i]];
					break;
			}
			if(cand < minCurSqCost)
			{
				minCurSqCost = cand;
			}
		}
		uint8_t goalX = globals.metaRoute[globals.metaRouteLenght-2];
		uint8_t goalY = globals.metaRoute[globals.metaRouteLenght-1];
		if(mapCostUp[goalY][goalX] < minCurSqCost)
		{
			notDone = 0;
		}
	}
	//skapa route utifrån mapup, mapdown, ...
	//hitta vilken ruta som har lägst kostnad av alla rutor som ligger brevid målrutan
	uint8_t CurrentRoutingX;
	uint8_t CurrentRoutingY;
	uint8_t pathfinToDirection;
	uint8_t X = globals.metaRoute[globals.metaRouteLenght-2];
	uint8_t Y = globals.metaRoute[globals.metaRouteLenght-1];

	uint8_t overCostHoriz = min(mapCostLeft[Y+1][X], mapCostLeft[Y+1][X]);
	uint8_t underCostHoriz = min(mapCostLeft[Y-1][X], mapCostRight[Y-1][X]);
	uint8_t leftCostHoriz = min(mapCostLeft[Y][X-1], mapCostRight[Y][X-1]);
	uint8_t rightCostHoriz = min(mapCostLeft[Y][X+1], mapCostRight[Y][X+1]);

	uint8_t overCostVert = min(mapCostUp[Y+1][X], mapCostDown[Y+1][X]);
	uint8_t underCostVert = min(mapCostUp[Y-1][X], mapCostDown[Y-1][X]);
	uint8_t leftCostVert = min(mapCostUp[Y][X-1], mapCostDown[Y][X-1]);
	uint8_t rightCostVert = min(mapCostUp[Y][X+1], mapCostDown[Y][X+1]);

	uint8_t overCost = min(overCostHoriz, overCostVert);
	uint8_t underCost = min(underCostHoriz, underCostVert);
	uint8_t leftCost = min(leftCostHoriz, leftCostVert);
	uint8_t rightCost = min(rightCostHoriz, rightCostVert);

	uint8_t minAll =  min(min(overCost, underCost), min(leftCost, rightCost));
	if(min(overCost, underCost) == minAll)
	{
		if(overCost == minAll)
		{
			CurrentRoutingX = X;
			CurrentRoutingY = Y+1;
		}
		else
		{
			CurrentRoutingX = X;
			CurrentRoutingY = Y-1;
		}
	}
	else
	{
		if(leftCost == minAll)
		{
			CurrentRoutingX = X-1;
			CurrentRoutingY = Y;
		}
		else
		{
			CurrentRoutingX = X+1;
			CurrentRoutingY = Y;
		}
	}
	if(mapCostUp[CurrentRoutingY][CurrentRoutingX] == minAll)
	{
		pathfinToDirection = LOGICAL_DIR_UP;
	}
	else if(mapCostRight[CurrentRoutingY][CurrentRoutingX] == minAll)
	{
		pathfinToDirection = LOGICAL_DIR_RIGHT;
	}
	else if(mapCostDown[CurrentRoutingY][CurrentRoutingX] == minAll)
	{
		pathfinToDirection = LOGICAL_DIR_DOWN;
	}
	else
	{
		pathfinToDirection = LOGICAL_DIR_LEFT;
	}
	// pathfindToX, pathfindToY och pathfinToDirection har målrutan
	globals.routeSquares[0] = CurrentRoutingX;
	globals.routeSquares[1] = CurrentRoutingY;
	globals.routeSquaresLength  = 2;
	globals.routeLength = 0;
	uint8_t currentCost = minAll;
	while(currentCost != 0)
	{
		if(pathfinToDirection == LOGICAL_DIR_UP)
		{
			uint8_t forwardCost = mapCostUp[CurrentRoutingY-1][CurrentRoutingX]; //allt är bakvänt för att man går från mål till start
			uint8_t turnLeftCost = mapCostRight[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnRightCost = mapCostLeft[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnVirtualCost = mapCostDown[CurrentRoutingY][CurrentRoutingX];
			uint8_t minAllRoute = min(min(forwardCost, turnLeftCost), min(turnRightCost, turnVirtualCost));
			currentCost = minAllRoute;
			if(minAllRoute == forwardCost)
			{
				CurrentRoutingY = CurrentRoutingY-1;

				globals.routeSquares[globals.routeSquaresLength] = CurrentRoutingX;
				globals.routeSquares[globals.routeSquaresLength+1] = CurrentRoutingY;
				globals.routeSquaresLength  = globals.routeSquaresLength+2;

				globals.route[globals.routeLength] = FORWARD_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				globals.route[globals.routeLength] = LEFT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				globals.route[globals.routeLength] = RIGHT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				globals.route[globals.routeLength] = VIRTUAL_REVERSE_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
		}
		else if(pathfinToDirection == LOGICAL_DIR_RIGHT)
		{
			uint8_t forwardCost = mapCostRight[CurrentRoutingY][CurrentRoutingX-1]; //allt är bakvänt för att man går från mål till start
			uint8_t turnLeftCost = mapCostDown[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnRightCost = mapCostUp[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnVirtualCost = mapCostLeft[CurrentRoutingY][CurrentRoutingX];
			uint8_t minAllRoute = min(min(forwardCost, turnLeftCost), min(turnRightCost, turnVirtualCost));
			currentCost = minAllRoute;
			if(minAllRoute == forwardCost)
			{
				CurrentRoutingX = CurrentRoutingX-1;

				globals.routeSquares[globals.routeSquaresLength] = CurrentRoutingX;
				globals.routeSquares[globals.routeSquaresLength+1] = CurrentRoutingY;
				globals.routeSquaresLength  = globals.routeSquaresLength+2;

				globals.route[globals.routeLength] = FORWARD_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				globals.route[globals.routeLength] = LEFT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				globals.route[globals.routeLength] = RIGHT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				globals.route[globals.routeLength] = VIRTUAL_REVERSE_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
		}
		else if(pathfinToDirection == LOGICAL_DIR_DOWN)
		{
			uint8_t forwardCost = mapCostDown[CurrentRoutingY+1][CurrentRoutingX]; //allt är bakvänt för att man går från mål till start
			uint8_t turnLeftCost = mapCostLeft[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnRightCost = mapCostRight[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnVirtualCost = mapCostUp[CurrentRoutingY][CurrentRoutingX];
			uint8_t minAllRoute = min(min(forwardCost, turnLeftCost), min(turnRightCost, turnVirtualCost));
			currentCost = minAllRoute;
			if(minAllRoute == forwardCost)
			{
				CurrentRoutingY = CurrentRoutingY+1;

				globals.routeSquares[globals.routeSquaresLength] = CurrentRoutingX;
				globals.routeSquares[globals.routeSquaresLength+1] = CurrentRoutingY;
				globals.routeSquaresLength  = globals.routeSquaresLength+2;

				globals.route[globals.routeLength] = FORWARD_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				globals.route[globals.routeLength] = LEFT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				globals.route[globals.routeLength] = RIGHT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				globals.route[globals.routeLength] = VIRTUAL_REVERSE_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
		}
		else
		{
			uint8_t forwardCost = mapCostLeft[CurrentRoutingY][CurrentRoutingX+1]; //allt är bakvänt för att man går från mål till start
			uint8_t turnLeftCost = mapCostUp[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnRightCost = mapCostDown[CurrentRoutingY][CurrentRoutingX];
			uint8_t turnVirtualCost = mapCostRight[CurrentRoutingY][CurrentRoutingX];
			uint8_t minAllRoute = min(min(forwardCost, turnLeftCost), min(turnRightCost, turnVirtualCost));
			currentCost = minAllRoute;
			if(minAllRoute == forwardCost)
			{
				CurrentRoutingX = CurrentRoutingX+1;

				globals.routeSquares[globals.routeSquaresLength] = CurrentRoutingX;
				globals.routeSquares[globals.routeSquaresLength+1] = CurrentRoutingY;
				globals.routeSquaresLength  = globals.routeSquaresLength+2;

				globals.route[globals.routeLength] = FORWARD_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				globals.route[globals.routeLength] = LEFT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				globals.route[globals.routeLength] = RIGHT_90_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				globals.route[globals.routeLength] = VIRTUAL_REVERSE_COMMAND;
				globals.routeLength = globals.routeLength +1;
			}
		}
	}
}

uint8_t getCostMove(uint8_t toX, uint8_t toY)
{
	{
		if(globals.map[toY][toX] == OPEN)
		{
			return 1; //kostnad att åka genom en vanlig ruta kommer nog att ändras
		}

		else if(toY == 15 || toY == 0 || toX == 15 || toX == 0)
		{
			return 128;
		}
		else if(globals.map[toY][toX] == UNKNOWN)
		{
			return 1;
		}
		else //vägg eller okänt
		{
			return 128;
		}
	}
}

void addCurSQ(uint8_t* buffer, uint8_t* dirBuffer, uint8_t* bufferLength, uint8_t X, uint8_t Y, uint8_t dir)
{
	for(uint8_t i = 0; i < *bufferLength; ++i)
	{
		if(buffer[i*2] == X && buffer[i*2+1] == Y && dirBuffer[i] == dir)
		{
			return;
		}
	}
	buffer[*bufferLength*2] = X;
	buffer[*bufferLength*2+1] = Y;
	dirBuffer[*bufferLength] = dir;
	*bufferLength = *bufferLength+1;
}

uint8_t min(uint8_t a, uint8_t b)
{
	if(a < b)
	{
		return a;
	}
	return b;
}