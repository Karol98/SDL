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

void map::calculateMap(int offSet)
{	
	int rand1, rand2, rand3, x, y;
	rand1 = rand() % 90+10;
	rand2 = rand() % 40+10;
	rand3 = rand() % 2;
	totalLength += rand1;
	x = mapValues[mapValues.size()-1][0];
	y = mapValues[mapValues.size()-1][1];


	if (actualHeight + rand2 > 600 || rand3 == 0 && rand3 < 500) {
		rand2 = -rand2;
	}

	actualHeight += rand2;
	mapValues.push_back({ x+rand1, y+rand2 });
	if (mapValues[0][0] + offSet < -100) {
		mapValues.erase(mapValues.begin());
	}
}

bool map::checkColisions(int rocketX, int rocketY, int index, int scrollingOfSetMap, int rocketWidth) {
	bool coolision = false;
	double x1, x2, y1, y2, lineLength, lineHeight, myCollisionX, collisionPoint;
	x1 = mapValues[index - 1][0] + scrollingOfSetMap;
	y1 = mapValues[index - 1][1];
	x2 = mapValues[index][0] + scrollingOfSetMap;
	y2 = mapValues[index][1];
//	cout << "x1: " << x1 << endl;
//	cout << "x2: " << x2 << endl;
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
	if (collisionPoint + rocketY < 20 ) {
		cout << "collisionPoint: " << collisionPoint << endl;
		cout << "lineLength: " << lineLength << endl;
		cout << "lineHeigh: " << lineHeight << endl;
		cout << "Collision X: " << myCollisionX << endl;
		cout << "Rocket X: " << rocketX << endl;
		cout << "x1: " << x1 << endl;
		cout << "y1: " << y1 << endl;
		cout << "x2: " << x2 << endl;
		cout << "y2: " << y2 << endl;
		cout << "rocketY: " << rocketY << endl;
		cout << "KOLIZJA!!!!" << endl;
		coolision = true;
	}
	return coolision;
}



int map::getTotalLength() {
	return totalLength;
}

map::map()
{
	actualHeight = 500;
	mapValues.push_back({ 0,500 });
}
