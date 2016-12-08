#pragma once
#include <glm.hpp>
#include <map>
#include "POLYGON_HANDLE.c"
#include "Polygon.h"

struct Collision;

class World
{
	private:

	float __gravityAcceleration;
	float __accumulatedTimeSeconds;
	float __currentTimeSeconds;
	float __fixedTimestepSeconds;
	POLYGON_HANDLE __nextHandle;
	std::map<POLYGON_HANDLE, Polygon*> __polygons;
	std::vector<Collision> __collisions;

	POLYGON_HANDLE GeneratePolygonHandle();

	void Step( float deltaTimeSeconds );

	bool TestCollision( Polygon* aPolygon, Polygon* bPolygon, Collision* collisionParams );
	bool TestSeparateAxisTheorem( Polygon* facePolygon, Polygon* vertexPolygon, Collision* collisionParams );


	public:

	World( float fixedTimestepSeconds, float gravityAcceleration = 0.0f );
	~World();

	void Update( float deltaTimeSeconds );

	POLYGON_HANDLE CreatePolygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation = 0.0f, float mass = 1.0f, bool useGravity = false );
	void DestroyPolygon( POLYGON_HANDLE handle );
	Polygon* GetPolygon( POLYGON_HANDLE handle );

	float GetCurrentTimeSeconds();

	bool IsPolygonColliding( Polygon* polygon );
};

