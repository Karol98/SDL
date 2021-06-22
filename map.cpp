#include <stdio.h>
#include <vector>
#include "map.h"
#include <SDL.h>
#include <iostream>

using namespace std;

std::vector<vector<int>> map::getMap()
{
	return mapValues;
}

void map::calculateMap(int offSet, int difficult)
{	
	int rand1, rand2, rand3, x, y;
	rand1 = rand() % 90+10;
	rand2 = rand() % 90+10;
	rand3 = rand() % 2;
	totalLength += rand1;
	x = mapValues[mapValues.size()-1][0];
	y = mapValues[mapValues.size()-1][1];


	if (actualHeight + rand2 > 600 || rand3 == 0 && actualHeight - difficult > 0 ) {
		rand2 = -rand2;
	}

	actualHeight += rand2;
	mapValues.push_back({ x+rand1, y+rand2 });
	if (mapValues[0][0] + offSet < -100) {
		mapValues.erase(mapValues.begin());
	}
}

bool map::checkColisions(int rocketX, int rocketY, int index, int scrollingOfSetMap, int rocketWidth, int difficult) {
	bool coolision = false;
	double x1, x2, y1, y2, lineLength, lineHeight, myCollisionX, collisionPoint;
	x1 = mapValues[index - 1][0] + scrollingOfSetMap;
	y1 = mapValues[index - 1][1];
	x2 = mapValues[index][0] + scrollingOfSetMap;
	y2 = mapValues[index][1];
	lineLength = x2 - x1;
	lineHeight = y2 - y1;
	myCollisionX = rocketX + rocketWidth/2 - x1;
	
	if (myCollisionX == 0) {
		collisionPoint = y1;
	}
	else
	{
		collisionPoint = y1 + (myCollisionX / lineLength) * lineHeight;
	}
	if (collisionPoint + rocketY < 35 || collisionPoint-difficult + rocketY > 0)
		coolision = true;
	return coolision;
}



int map::getTotalLength() {
	return totalLength;
}

map::map()
{
	actualHeight = 400;
	mapValues.push_back({ 0,400 });
}
