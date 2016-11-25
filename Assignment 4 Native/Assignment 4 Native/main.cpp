#include <exception>
#include "main.h"
#include "Polygon.h"
#include "World.h"


// When WorldStart() is called, we'll create a new World instance and store a pointer to it here
// and when WorldDestroy() is called we'll delete this and reset it to NULL.
World* __world = NULL;


// EXTERNAL API (Available in Unity)

extern "C"
{
	// Create a new World and store it in __world.
	void WorldStart( float fixedTimestepSeconds )
	{
		__world = new World( fixedTimestepSeconds );
	}

	// Tell the World to update, given the amount of time that has passed since last update.
	void WorldUpdate( float deltaTimeSeconds )
	{
		__world->Update( deltaTimeSeconds );
	}

	// If there is a World, destroy it and reset the pointer to NULL.
	void WorldDestroy()
	{
		if ( __world != NULL )
		{
			delete __world;
			__world = NULL;
		}
	}

	// Tell the World to create a new Polygon and return its HANDLE to the caller.
	// Note: Check out the VerticesTransformToGLM() and Vector2TransformToGLM() functions below.
	POLYGON_HANDLE PolygonCreate( TransportVector2 vertices[], int verticesLength, TransportVector2 position, float rotation )
	{
		return __world->CreatePolygon( VerticesTransportToGLM( vertices, verticesLength ), Vector2TransportToGLM( position ), rotation );
	}

	// Tell the World to destroy the Polygon at the provided handle.
	void PolygonDestroy( POLYGON_HANDLE handle )
	{
		__world->DestroyPolygon( handle );
	}

	// Get the Polygon at the provided handle from the World and return its position as a TransportVector2.
	// Note: Check out the Vector2GLMToTransform() function below.
	TransportVector2 PolygonGetPosition( POLYGON_HANDLE handle )
	{
		return Vector2GLMToTransport( __world->GetPolygon( handle ).GetPosition() );
	}

	// Get the Polygon at the provided handle and set its position as a glm::vec2.
	// Note: Check out the Vector2TransformToGLM() function below.
	void PolygonSetPosition( POLYGON_HANDLE handle, TransportVector2 position )
	{
		__world->GetPolygon( handle ).SetPosition( Vector2TransportToGLM( position ) );
	}

	// Get the Polygon at the provided handle from the World and return its rotation.
	float PolygonGetRotation( POLYGON_HANDLE handle )
	{
		return __world->GetPolygon( handle ).GetRotation();
	}

	// Get the Polygon at the provided handle and set its rotation.
	void PolygonSetRotation( POLYGON_HANDLE handle, float rotation )
	{
		__world->GetPolygon( handle ).SetRotation( rotation );
	}

	// Get the Polygon at the provided handle and set its vertices as glm::vec2s.
	// Note: Check out the VerticesTransformToGLM() function below.
	void PolygonSetVertices( POLYGON_HANDLE handle, TransportVector2 vertices[], int verticesLength )
	{
		__world->GetPolygon( handle ).SetVertices( VerticesTransportToGLM( vertices, verticesLength ) );
	}
}


// TYPE CONVERSIONS

// Converts a single glm::vec2 into a TransportVector2 to be sent across the extern "C" interface.
TransportVector2 Vector2GLMToTransport( glm::vec2 glmVector )
{
	auto nativeVector = TransportVector2();
	nativeVector.x = glmVector.x;
	nativeVector.y = glmVector.y;
	return nativeVector;
}

// Converts a single TransformVector2 into a glm::vec2 for internal use (doing math with/on).
glm::vec2 Vector2TransportToGLM( TransportVector2 transportVector )
{
	return glm::vec2( transportVector.x, transportVector.y );
}

// Converts a std::vector of glm::vec2s into a C-style array of TransformVector2s.
// This isn't used anywhere right now, but if you want to use it for something be my guest.
TransportVector2* VerticesGLMToTransport( std::vector<glm::vec2>* glmVertices )
{
	auto transportVertices = new TransportVector2[ glmVertices->size() ];
	for ( unsigned int i = 0; i < glmVertices->size(); i++ )
	{
		transportVertices[ i ] = Vector2GLMToTransport( glmVertices->at( i ) );
	}
	return transportVertices;
}

// Converts a C-style array of TransformVector2s into a std::vector of glm::vec2s.
std::vector<glm::vec2>* VerticesTransportToGLM( TransportVector2 transportVertices[], int transportVerticesLength )
{
	auto glmVertices = new std::vector<glm::vec2>();
	for ( auto i = 0; i < transportVerticesLength; i++ )
	{
		glmVertices->push_back( Vector2TransportToGLM( transportVertices[ i ] ) );
	}
	return glmVertices;
}
