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
	LAB3_API void WorldStart( float fixedTimestepSeconds );
	LAB3_API void WorldUpdate( float deltaTimeSeconds );
	LAB3_API void WorldDestroy();

	LAB3_API int PolygonCreate( TransportVector2 vertices[], int verticesLength, TransportVector2 position, float rotation = 0.0f );
	LAB3_API void PolygonDestroy( POLYGON_HANDLE handle );

	LAB3_API TransportVector2 PolygonGetPosition( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetPosition( POLYGON_HANDLE handle, TransportVector2 position );

	LAB3_API float PolygonGetRotation( POLYGON_HANDLE handle );
	LAB3_API void PolygonSetRotation( POLYGON_HANDLE handle, float rotation );

	LAB3_API void PolygonSetVertices( POLYGON_HANDLE handle, TransportVector2 vertices[], int verticesLength );
}


// TYPE CONVERSIONS

// I've included this complete with coded definitions because this is the first time many of you 
// will have encountered GLM. Just read this functions over in main.cpp so you can follow the 
// (really simple!) things they're doing to transform between TransformVector2 and glm::vec2.

TransportVector2 Vector2GLMToTransport( glm::vec2 glmVector );
glm::vec2 Vector2TransportToGLM( TransportVector2 transportVector );

TransportVector2* VerticesGLMToTransport( std::vector<glm::vec2>* glmVertices );
std::vector<glm::vec2>* VerticesTransportToGLM( TransportVector2 transportVertices[], int transportVerticesLength );
