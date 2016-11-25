// ConsoleTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"
#include "glm.hpp"


int main()
{
    WorldStart( 0.02f );

	TransportVector2 position1;
	position1.x = -1.5f;
	position1.y = 0.0f;

	TransportVector2 position2;
	position2.x = 10.5f;
	position2.y = 0.0f;

	TransportVector2* square = new TransportVector2[4];
	square[0].x = 2.0f;
	square[0].y = 2.0f;
	square[1].x = 2.0f;
	square[1].y = -2.0f;
	square[2].x = -2.0f;
	square[2].y = -2.0f;
	square[3].x = -2.0f;
	square[3].y = 2.0f;

	POLYGON_HANDLE polygon1 = PolygonCreate(square, 4, position1, 3.14f / 4.0f);
	POLYGON_HANDLE polygon2 = PolygonCreate(square, 4, position2, 3.14f / 4.0f);

	WorldUpdate(0.02f);

	WorldUpdate( 0.02f );

	PolygonSetRotation(polygon1, 1.0f);

	WorldDestroy();
}

