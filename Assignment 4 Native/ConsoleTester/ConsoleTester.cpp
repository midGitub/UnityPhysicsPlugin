// ConsoleTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"
#include "glm.hpp"


int main()
{
	WorldStart( 0.02f, -9.81f );

	TransportVector2 position1;
	position1.x = 0.0f;
	position1.y = 0.0f;

	TransportVector2 position2;
	position2.x = 0.0f;
	position2.y = -5.0f;

	TransportVector2* rect1 = new TransportVector2[ 4 ];
	rect1[ 0 ].x = 0.5f;
	rect1[ 0 ].y = 0.5f;
	rect1[ 1 ].x = 0.5f;
	rect1[ 1 ].y = -0.5f;
	rect1[ 2 ].x = -0.5f;
	rect1[ 2 ].y = -0.5f;
	rect1[ 3 ].x = -0.5f;
	rect1[ 3 ].y = 0.5f;

	TransportVector2* rect2 = new TransportVector2[ 4 ];
	rect2[ 0 ].x = 5.0f;
	rect2[ 0 ].y = 1.0f;
	rect2[ 1 ].x = 5.0f;
	rect2[ 1 ].y = -1.0f;
	rect2[ 2 ].x = -5.0f;
	rect2[ 2 ].y = -1.0f;
	rect2[ 3 ].x = -5.0f;
	rect2[ 3 ].y = 1.0f;

	POLYGON_HANDLE polygon1 = PolygonCreate( rect1, 4, position1, glm::radians<float>( 30.0f ) );
	POLYGON_HANDLE polygon2 = PolygonCreate( rect2, 4, position2, 0.0f );

	long step = 20L;
	float deltaTime = step / 1000.0f;
	while( true )
	{
		printf( "%d", IsPolygonColliding( polygon1 ) );
		PolygonRotate( polygon1, glm::radians<float>( 90.0f ) * deltaTime );
		PolygonRotate( polygon2, glm::radians<float>( 90.0f ) * deltaTime );
		_sleep( step );
		WorldUpdate( deltaTime );
	}

	WorldDestroy();
}

