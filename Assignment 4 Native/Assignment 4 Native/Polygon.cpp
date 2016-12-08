#include "Polygon.h"
#include "gtx\matrix_transform_2d.hpp"
#include "Face.h"

// PRIVATE

Polygon::Polygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation, float mass, bool useGravity )
	: __vertices( NULL )
	, __position( position )
	, __rotation( rotation )
	, __mass( mass )
	, __useGravity( useGravity )
	, __globalVertices( std::vector<glm::vec2>() )
	, __faces( std::vector<Face>() )
	, __isAwake( true )
	, __isStatic( false )
{
	SetVertices( vertices );
}


Polygon::~Polygon()
{
	delete __vertices;
}


void Polygon::UpdateCenterOfMass()
{
	// Find centroid of vertices.
	glm::vec2 centroid;
	for ( glm::vec2 vertex : *__vertices )
	{
		centroid += vertex;
	}
	centroid /= __vertices->size();

	// Find area of each triangle around the main centroid and the total area of the shape and find the 
	// average position by weighting each face's centroid by the area the face contributes to the whole.
	float totalArea = 0.0f;
	glm::vec2 centerOfMass;
	for ( Face face : __faces )
	{
		float base = glm::length( face.GetVector() );
		float height = -face.GetDistance( centroid );
		float area = 0.5f * base * height;
		totalArea += area;

		glm::vec2 faceCentroid = 0.3333333f * ( centroid + face.GetVertex1() + face.GetVertex2() );
		centerOfMass += area * faceCentroid;
	}
	centerOfMass /= totalArea;

	// Move the vertices such that the center of mass is positioned at the origin so when we rotate
	// we're actually rotating about the center of mass.
	for ( int i = 0; i < __vertices->size(); ++i )
	{
		glm::vec2& vertex = __vertices->at( i );
		vertex -= centerOfMass;
	}
}

void Polygon::UpdateFaces()
{
	__faces.clear();
	for ( auto i = 0; i < __globalVertices.size(); i++ )
	{
		int vertexIndex1 = i;
		int vertexIndex2 = ( i + 1 ) % __globalVertices.size();
		__faces.emplace_back( this, vertexIndex1, vertexIndex2 );
	}
}

void Polygon::UpdateGlobalVertices()
{
	glm::mat3 transform = glm::mat3();
	transform = glm::translate( transform, __position );
	transform = glm::rotate( transform, __rotation );

	__globalVertices.clear();
	for ( auto i = 0; i < __vertices->size(); i++ )
	{
		glm::vec2 vertex = __vertices->at( i );
		glm::vec3 vertex3 = glm::vec3( vertex, 1.0f );
		glm::vec3 globalVertex = transform * vertex3;
		__globalVertices.push_back( glm::vec2( globalVertex ) );
	}
}

void Polygon::UpdateRotationalInertia()
{
	// Find the average radius of vertices from center of mass (origin).
	float averageRadius = 0.0f;
	for ( glm::vec2 vertex : *__vertices )
	{
		averageRadius += glm::length( vertex );
	}
	averageRadius /= __vertices->size();

	// Using mr^2 since arbitrary polygons can be oddly shaped. This isn't perfect but we could do 
	// better if we made subclasses for ideal shapes that we could define more accurately.
	__rotationalInertia = __mass * averageRadius * averageRadius;
}

// PUBLIC

bool Polygon::GetUseGravity()
{
	return __useGravity;
}

void Polygon::SetUseGravity( bool useGravity )
{
	__useGravity = useGravity;
}

float Polygon::GetMass()
{
	return __mass;
}

void Polygon::SetMass( float mass )
{
	__mass = mass;
	UpdateRotationalInertia();
}

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

glm::vec2 Polygon::GetVelocity()
{
	return __velocity;
}

void Polygon::SetVelocity( glm::vec2 velocity )
{
	__velocity = velocity;
}

void Polygon::Accelerate( glm::vec2 dVelocity )
{
	SetVelocity( __velocity + dVelocity );
}

float Polygon::GetRotationalInertia()
{
	return __rotationalInertia;
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

float Polygon::GetRotationalVelocity()
{
	return __rotationalVelocity;
}

void Polygon::SetRotationalVelocity( float rotationalVelocity )
{
	__rotationalVelocity = rotationalVelocity;
}

void Polygon::AccelerateRotation( float dRotationalVelocity )
{
	SetRotationalVelocity( __rotationalVelocity + dRotationalVelocity );
}

Face Polygon::GetFace( int index )
{
	return __faces.at( index );
}

std::vector<Face>& Polygon::GetFaces()
{
	return __faces;
}

glm::vec2 Polygon::GetVertex( int index )
{
	return __vertices->at( index );
}

glm::vec2 Polygon::GetGlobalVertex( int index )
{
	return __globalVertices.at( index );
}


std::vector<glm::vec2>& Polygon::GetVertices()
{
	return *__vertices;
}

std::vector<glm::vec2>& Polygon::GetGlobalVertices()
{
	return __globalVertices;
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
	UpdateCenterOfMass();      // Requires faces to be created.
	UpdateRotationalInertia(); // Requires center of mass.
}
