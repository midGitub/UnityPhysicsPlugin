#include "Polygon.h"
#include "gtx\matrix_transform_2d.hpp"
#include "Face.h"

// PRIVATE

Polygon::Polygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation )
	: __vertices( NULL )
	, __position( position )
	, __rotation( rotation )
	, __globalVertices( new std::vector<glm::vec2>() )
	, __faces( new std::vector<Face*>() )
{
	SetVertices( vertices );
}


Polygon::~Polygon()
{
	delete __vertices;
	delete __globalVertices;
}


void Polygon::UpdateGlobalVertices()
{
	glm::mat3 transform = glm::mat3();
	transform = glm::rotate( transform, __rotation );
	transform = glm::translate( transform, __position );

	__globalVertices->clear();
	for ( auto i = 0; i < __vertices->size(); i++ )
	{
		glm::vec2 vertex = __vertices->at( i );
		glm::vec3 vertex3 = glm::vec3( vertex, 1.0f );
		glm::vec3 globalVertex = transform * vertex3;
		__globalVertices->push_back( glm::vec2( globalVertex ) );
	}
}


void Polygon::UpdateFaces()
{
	for (auto i = 0; i < __faces->size(); i++)
	{
		delete __faces->at( i );
	}
	__faces->clear();
	for ( auto i = 0; i < __globalVertices->size(); i++ )
	{
		int vertexIndex1 = i;
		int vertexIndex2 = ( i + 1 ) % __globalVertices->size();
		__faces->push_back( new Face( this, vertexIndex1, vertexIndex2 ) );
	}
}

// PUBLIC

glm::vec2 Polygon::GetPosition()
{
	return __position;
}


void Polygon::SetPosition( glm::vec2 position )
{
	__position = position;
	UpdateGlobalVertices();
}


void Polygon::Translate( glm::vec2 dPosition )
{
	SetPosition( __position + dPosition );
}


Face* Polygon::GetFace( int index )
{
	return __faces->at( index );
}


std::vector<Face*>& Polygon::GetFaces()
{
	return *__faces;
}


float Polygon::GetRotation()
{
	return __rotation;
}


void Polygon::SetRotation( float rotation )
{
	__rotation = rotation;
	UpdateGlobalVertices();
}


void Polygon::Rotate( float dRotation )
{
	SetRotation( __rotation + dRotation );
}


glm::vec2 Polygon::GetVertex( int index )
{
	return __vertices->at( index );
}


glm::vec2 Polygon::GetGlobalVertex( int index )
{
	return __globalVertices->at( index );
}


std::vector<glm::vec2>& Polygon::GetVertices()
{
	return *__vertices;
}


std::vector<glm::vec2>& Polygon::GetGlobalVertices()
{
	return *__globalVertices;
}


void Polygon::SetVertices( std::vector<glm::vec2>* vertices )
{
	if ( vertices == __vertices )
	{
		return;
	}
	if ( vertices == NULL )
	{
		throw std::exception( "vertices can't be null!" );
	}
	if ( __vertices != NULL )
	{
		delete __vertices;
	}
	__vertices = vertices;
	UpdateGlobalVertices();
	UpdateFaces();
}
