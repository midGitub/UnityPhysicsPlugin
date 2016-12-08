#pragma once
#include <vector>
#include <glm.hpp>
#include "POLYGON_HANDLE.c"
#include "TransportVector2.c"


// EXTERNAL API (Available in Unity)

#define LAB3_API __declspec( dllexport )

// The extern "C" functions will be defined by you. I've provided you with delcarations below to 
// make clear what these functions need to take as arguments and what they return. I think their 
// names make what each of them does pretty clear, but feel free to ask if it isn't obvious.

extern "C"
{
	LAB3_API void WorldStart( float fixedTimestepSeconds, float gravityAcceleration = 0.0f );
	LAB3_API void WorldUpdate( float deltaTimeSeconds );
	LAB3_API void WorldDestroy();

	LAB3_API int PolygonCreate( TransportVector2 vertices[], int verticesLength, TransportVector2 position, float rotation = 0.0f, float mass = 1.0f, bool useGravity = false );
	LAB3_API void PolygonDestroy( POLYGON_HANDLE handle );

	LAB3_API void PolygonSetVertices( POLYGON_HANDLE handle, TransportVector2 vertices[], int verticesLength );

	LAB3_API float PolygonGetMass( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetMass( POLYGON_HANDLE handle, float mass );

	LAB3_API float PolygonGetRotationalInertia( POLYGON_HANDLE handle );

	LAB3_API TransportVector2 PolygonGetPosition( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetPosition( POLYGON_HANDLE handle, TransportVector2 position );
	LAB3_API void PolygonTranslate( POLYGON_HANDLE handle, TransportVector2 dPosition );

	LAB3_API TransportVector2 PolygonGetVelocity( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetVelocity( POLYGON_HANDLE handle, TransportVector2 velocity );
	LAB3_API void PolygonAccelerate( POLYGON_HANDLE handle, TransportVector2 dVelocity );

	LAB3_API float PolygonGetRotation( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetRotation( POLYGON_HANDLE handle, float rotation );
	LAB3_API void PolygonRotate( POLYGON_HANDLE handle, float dRotation );

	LAB3_API float PolygonGetRotationalVelocity( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetRotationalVelocity( POLYGON_HANDLE handle, float rotationalVelocity );
	LAB3_API void PolygonAccelerateRotation( POLYGON_HANDLE handle, float dRotationalVelocity );

	LAB3_API bool IsPolygonColliding( POLYGON_HANDLE handle );
}


// TYPE CONVERSIONS

// I've included this complete with coded definitions because this is the first time many of you 
// will have encountered GLM. Just read this functions over in main.cpp so you can follow the 
// (really simple!) things they're doing to transform between TransformVector2 and glm::vec2.

TransportVector2 Vector2GLMToTransport( glm::vec2 glmVector );
glm::vec2 Vector2TransportToGLM( TransportVector2 transportVector );

TransportVector2* VerticesGLMToTransport( std::vector<glm::vec2>* glmVertices );
std::vector<glm::vec2>* VerticesTransportToGLM( TransportVector2 transportVertices[], int transportVerticesLength );
