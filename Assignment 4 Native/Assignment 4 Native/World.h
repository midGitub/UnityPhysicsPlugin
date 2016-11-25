#pragma once
#include <glm.hpp>
#include <map>
#include "POLYGON_HANDLE.c"
#include "Polygon.h"

class World
{
	private:

	float __accumulatedTimeSeconds;
	float __currentTimeSeconds;
	float __fixedTimestepSeconds;
	POLYGON_HANDLE __nextHandle;
	std::map<POLYGON_HANDLE, Polygon*> __polygons;

	POLYGON_HANDLE GeneratePolygonHandle();

	void Step( float deltaTimeSeconds );


	public:

	World( float fixedTimestepSeconds );
	~World();

	void Update( float deltaTimeSeconds );

	POLYGON_HANDLE CreatePolygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation = 0.0f );
	void DestroyPolygon( POLYGON_HANDLE handle );
	Polygon& GetPolygon( POLYGON_HANDLE handle );

	float GetCurrentTimeSeconds();
};

