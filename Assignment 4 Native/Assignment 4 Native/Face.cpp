#include "Face.h"
#include "Polygon.h"



Face::Face( Polygon* polygon, int vertexIndex1, int vertexIndex2 )
	: __polygon( polygon )
	, __vertexIndex1( vertexIndex1 )
	, __vertexIndex2( vertexIndex2 )
{
}


Face::~Face()
{
}


Polygon* Face::GetPolygon()
{
	return __polygon;
}


int Face::GetVertexIndex1()
{
	return __vertexIndex1;
}


int Face::GetVertexIndex2()
{
	return __vertexIndex2;
}


glm::vec2 Face::GetVertex1()
{
	return __polygon->GetVertex( __vertexIndex1 );
}


glm::vec2 Face::GetVertex2()
{
	return __polygon->GetVertex( __vertexIndex2 );
}


glm::vec2 Face::GetGlobalVertex1()
{
	return __polygon->GetGlobalVertex( __vertexIndex1 );
}


glm::vec2 Face::GetGlobalVertex2()
{
	return __polygon->GetGlobalVertex( __vertexIndex2 );
}


glm::vec2 Face::GetVector()
{
	return GetVertex2() - GetVertex1();
}


glm::vec2 Face::GetGlobalVector()
{
	return GetGlobalVertex2() - GetGlobalVertex1();
}


glm::vec2 Face::GetNormal()
{
	// CW ordering so we compute a left-normal.
	glm::vec2 normalized = glm::normalize( GetVector() );
	return glm::vec2( -normalized.y, normalized.x );
}


glm::vec2 Face::GetGlobalNormal()
{
	// CW ordering so we compute a left-normal.
	glm::vec2 normalized = glm::normalize( GetGlobalVector() );
	return glm::vec2( -normalized.y, normalized.x );
}


float Face::GetDistance( glm::vec2 localPoint )
{
	return glm::dot( localPoint - GetVertex1(), GetNormal() );
}


float Face::GetGlobalDistance( glm::vec2 globalPoint )
{
	return glm::dot( globalPoint - GetGlobalVertex1(), GetGlobalNormal() );
}