	/**
	 * TSEA27 Elektronikprojekt
	 *
	 * IDENTIFIERING
	 *
	 * Modul: Styrenheten
	 * Filnamn: pathfind.c
	 * Skriven av: D. Molin
	 *			   
	 * Datum: 2013-05-15
	 * Version: 1.0
	 *
	 * BESKRIVNING
	 *
	 * Algoritmkod för roboten pathfinding.
	 */	

#include "pathfind.h"

/*
*	uppdaterar metarutten samt skapar en rutt (dvs en serie kommandon) som tar roboten till nästa oupptäckta ruta
*/
void pathfind()
{
	glob_shouldPathfind = 0;
	truncateMetaRoute(); //flyttar över element från metarutten till adj. new squares enl designspec.
	adjecentNewSquaresRemoveChartedSquares(); //tar bort element från adj. new squares som har blivit upptäkta
	if(glob_metaRouteLenght == 0)
	{
		if(glob_adjecentNewSquaresLenght == 0)
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

/*
*	hittar det element i metarutten som är nyligen upptäckt och är sist, samt flyttar alla element i metarutten som är före detta element och lägger till dessa i glob_adjecentNewSquares
*/
void truncateMetaRoute()
{
	for(uint8_t i = 0; i < glob_metaRouteLenght; i=i+2)
	{
		if(glob_map[glob_metaRoute[i+1]][glob_metaRoute[i]] != UNKNOWN)
		{
			uint8_t metaRouteSave = glob_metaRouteLenght;
			glob_metaRouteLenght = i;
			while(i < metaRouteSave)
			{
				if(foundInAdjOrMeta(glob_metaRoute[i], glob_metaRoute[i+1]) == 0)
				{
					glob_adjecentNewSquares[glob_adjecentNewSquaresLenght] = glob_metaRoute[i];
					glob_adjecentNewSquares[glob_adjecentNewSquaresLenght+1] = glob_metaRoute[i+1];
					glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght+2;
				}
				i = i+2;
			}
			return;
		}
	}
}

/*
*	tar bort alla element i glob_adjecentNewSquares som representerar en ruta som redan har blivit kartlagd
*/
void adjecentNewSquaresRemoveChartedSquares()
{
	uint8_t i = 0;
	while(i < glob_adjecentNewSquaresLenght)
	{
		if(glob_map[glob_adjecentNewSquares[i+1]][glob_adjecentNewSquares[i]] != UNKNOWN)
		{
			//ta bort elementet
			uint8_t k = i;
			while(k < glob_adjecentNewSquaresLenght)
			{
				glob_adjecentNewSquares[k] = glob_adjecentNewSquares[k+2];
				k = k + 1;
			}
			glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght-2;
		}			
		else
		{
			i = i+2;
		}
	}
}

/*
*	skapar en metarutt om det inte finns en sådan.
*/
void initMetaRoute()
{
	//get square closest to robot
	uint8_t closestSquareIndex = 1;
	uint16_t smallestLength = 500;
	uint8_t i = 0;
	while(i<glob_adjecentNewSquaresLenght)
	{
		uint16_t lenCand = norm2pow2(glob_adjecentNewSquares[i], glob_adjecentNewSquares[i+1], glob_mapX, glob_mapY);
		if(lenCand < smallestLength)
		{
			smallestLength = lenCand;
			closestSquareIndex = i;
		}
		i = i+2;
	}
	//add this square to the meta route
	glob_metaRoute[METAROUTEMAXLEN-2] = glob_adjecentNewSquares[closestSquareIndex];
	glob_metaRoute[METAROUTEMAXLEN-1] = glob_adjecentNewSquares[closestSquareIndex+1];
	glob_metaRouteLenght = 2; //ska vara tom innan
	uint8_t j = closestSquareIndex;
	while(j < glob_adjecentNewSquaresLenght)
	{
		glob_adjecentNewSquares[j] = glob_adjecentNewSquares[j+2];
		j = j + 1;
	}
	glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght-2;
	//for each adj square
	while(glob_adjecentNewSquaresLenght != 0)
	{
		//find square closest last order in meta route
		smallestLength = 500;
		i = 0;
		while(i<glob_adjecentNewSquaresLenght)
		{
			uint16_t lenCand = norm2pow2(glob_adjecentNewSquares[i], glob_adjecentNewSquares[i+1], glob_metaRoute[METAROUTEMAXLEN-glob_metaRouteLenght], glob_metaRoute[METAROUTEMAXLEN-glob_metaRouteLenght+1]);
			if(lenCand < smallestLength)
			{
				smallestLength = lenCand;
				closestSquareIndex = i;
			}
			i = i+2;
		}
		//add this square to the meta route
		glob_metaRoute[METAROUTEMAXLEN-glob_metaRouteLenght-2] = glob_adjecentNewSquares[closestSquareIndex];
		glob_metaRoute[METAROUTEMAXLEN-glob_metaRouteLenght-1] = glob_adjecentNewSquares[closestSquareIndex+1];
		glob_metaRouteLenght = glob_metaRouteLenght + 2;
		j = closestSquareIndex;
		while(j < glob_adjecentNewSquaresLenght)
		{
			glob_adjecentNewSquares[j] = glob_adjecentNewSquares[j+2];
			j = j + 1;
		}
		glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght-2;
	}
	i = 0;
	while(i < glob_metaRouteLenght)
	{
		glob_metaRoute[i] = glob_metaRoute[i+METAROUTEMAXLEN-glob_metaRouteLenght];
		i = i+1;
	}
}

/*
*	lägger till alla element i glob_adjecentNewSquares till metarutten
*/
void createNewMetaRoute()
{
	while(glob_adjecentNewSquaresLenght != 0)
	{
		uint8_t closestSquareIndex = 0;
		uint16_t smallestLength = 500;
		uint8_t i = 0;
		//find the square which is closest to the last in meta route
		while(i<glob_adjecentNewSquaresLenght)
		{
			uint16_t lenCand = norm2pow2(glob_adjecentNewSquares[i], glob_adjecentNewSquares[i+1], glob_metaRoute[glob_metaRouteLenght-2], glob_metaRoute[glob_metaRouteLenght-1]);
			if(lenCand < smallestLength)
			{
				smallestLength = lenCand;
				closestSquareIndex = i;
			}
			i = i+2;
		}
		//add this square to the meta route
		glob_metaRoute[glob_metaRouteLenght] = glob_adjecentNewSquares[closestSquareIndex];
		glob_metaRoute[glob_metaRouteLenght+1] = glob_adjecentNewSquares[closestSquareIndex+1];
		glob_metaRouteLenght = glob_metaRouteLenght + 2;
		//remove this square from the candidates
		uint8_t j = closestSquareIndex;
		while(j < glob_adjecentNewSquaresLenght)
		{
			glob_adjecentNewSquares[j] = glob_adjecentNewSquares[j+2];
			j = j + 1;
		}
		glob_adjecentNewSquaresLenght = glob_adjecentNewSquaresLenght-2;
	}
}

/*
*	returnerar avstånd från (x1, y1) till (x2, y2)
*/
uint16_t norm2pow2(int16_t fromX, int16_t fromY, int16_t toX, int16_t toY) //ska vara short för att undika overflow vid ^2 och underflow vid fromX-toX
{
	return (fromX-toX)*(fromX-toX)+(fromY-toY)*(fromY-toY); //går att optimera om det behövs
}

/*
*	skapar en rutt till första elementet i metarutten.
*/
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
	uint8_t currentSquaresLengthBuffer;
	TONEXTMETAROUTEPOINT:
	if(glob_metaRouteLenght == 0)
	{
		return;
	}
	currentSquaresLengthBuffer = 0; //anger hur många X,Y element som finns, inte hur många celler som är fyllda
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
	currentSquares[0] = glob_mapX;
	currentSquares[1] = glob_mapY;
	uint8_t currentSquaresLength = 1; //HÄR
	if(glob_logical_direction == LOGICAL_DIR_UP) //beror på hur riktning skall vara kodad senare
	{
		directionCurrentSquares[0] = LOGICAL_DIR_UP;
		mapCostUp[glob_mapY][glob_mapX] = 0;
	}
	else if(glob_logical_direction == LOGICAL_DIR_RIGHT)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_RIGHT;
		mapCostRight[glob_mapY][glob_mapX] = 0;
	}
	else if(glob_logical_direction == LOGICAL_DIR_DOWN)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_DOWN;
		mapCostDown[glob_mapY][glob_mapX] = 0;
	}
	else if(glob_logical_direction == LOGICAL_DIR_LEFT)
	{
		directionCurrentSquares[0] = LOGICAL_DIR_LEFT;
		mapCostLeft[glob_mapY][glob_mapX] = 0;
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
		uint8_t goalX = glob_metaRoute[glob_metaRouteLenght-2];
		uint8_t goalY = glob_metaRoute[glob_metaRouteLenght-1];
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
	uint8_t X = glob_metaRoute[glob_metaRouteLenght-2];
	uint8_t Y = glob_metaRoute[glob_metaRouteLenght-1];

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
	if(minAll == 128)
	{
		glob_metaRouteLenght = glob_metaRouteLenght-2;
		goto TONEXTMETAROUTEPOINT;
	}
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
	glob_routeSquares[0] = CurrentRoutingX;
	glob_routeSquares[1] = CurrentRoutingY;
	glob_routeSquaresLength  = 2;
	glob_routeLength = 0;
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

				glob_routeSquares[glob_routeSquaresLength] = CurrentRoutingX;
				glob_routeSquares[glob_routeSquaresLength+1] = CurrentRoutingY;
				glob_routeSquaresLength  = glob_routeSquaresLength+2;

				glob_route[glob_routeLength] = FORWARD_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				glob_route[glob_routeLength] = LEFT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				glob_route[glob_routeLength] = RIGHT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				glob_route[glob_routeLength] = VIRTUAL_REVERSE_COMMAND;
				glob_routeLength = glob_routeLength +1;
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

				glob_routeSquares[glob_routeSquaresLength] = CurrentRoutingX;
				glob_routeSquares[glob_routeSquaresLength+1] = CurrentRoutingY;
				glob_routeSquaresLength  = glob_routeSquaresLength+2;

				glob_route[glob_routeLength] = FORWARD_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				glob_route[glob_routeLength] = LEFT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				glob_route[glob_routeLength] = RIGHT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				glob_route[glob_routeLength] = VIRTUAL_REVERSE_COMMAND;
				glob_routeLength = glob_routeLength +1;
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

				glob_routeSquares[glob_routeSquaresLength] = CurrentRoutingX;
				glob_routeSquares[glob_routeSquaresLength+1] = CurrentRoutingY;
				glob_routeSquaresLength  = glob_routeSquaresLength+2;

				glob_route[glob_routeLength] = FORWARD_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_LEFT;

				glob_route[glob_routeLength] = LEFT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				glob_route[glob_routeLength] = RIGHT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				glob_route[glob_routeLength] = VIRTUAL_REVERSE_COMMAND;
				glob_routeLength = glob_routeLength +1;
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

				glob_routeSquares[glob_routeSquaresLength] = CurrentRoutingX;
				glob_routeSquares[glob_routeSquaresLength+1] = CurrentRoutingY;
				glob_routeSquaresLength  = glob_routeSquaresLength+2;

				glob_route[glob_routeLength] = FORWARD_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnLeftCost)
			{
				pathfinToDirection = LOGICAL_DIR_UP;

				glob_route[glob_routeLength] = LEFT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else if(minAllRoute == turnRightCost)
			{
				pathfinToDirection = LOGICAL_DIR_DOWN;

				glob_route[glob_routeLength] = RIGHT_90_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
			else
			{
				pathfinToDirection = LOGICAL_DIR_RIGHT;

				glob_route[glob_routeLength] = VIRTUAL_REVERSE_COMMAND;
				glob_routeLength = glob_routeLength +1;
			}
		}
	}
}

/*
*	returnerar kostnaden att gå in på rutan (toX, toY)
*/
uint8_t getCostMove(uint8_t toX, uint8_t toY)
{
	{
		if(glob_map[toY][toX] == OPEN)
		{
			return 1; //kostnad att åka genom en vanlig ruta kommer nog att ändras
		}

		else if(toY == 15 || toY == 0 || toX == 15 || toX == 0)
		{
			return 128;
		}
		else if(glob_map[toY][toX] == UNKNOWN)
		{
			return 1;
		}
		else //vägg eller okänt
		{
			return 128;
		}
	}
}

/*
*	lägger till en x, y koordinat i en buffer om denna koordinat ej redan finns
*/
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

/*
*	returnerar mininmum av a och b
*/
uint8_t min(uint8_t a, uint8_t b)
{
	if(a < b)
	{
		return a;
	}
	return b;
}