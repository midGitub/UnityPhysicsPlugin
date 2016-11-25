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


glm::vec2 Face::GetNormal()
{
	glm::vec2 v1 = __polygon->GetGlobalVertex( __vertexIndex1 );
	glm::vec2 v2 = __polygon->GetGlobalVertex( __vertexIndex2 );
	glm::vec2 v1tov2 = v2 - v1;
	glm::vec2 normalized = glm::normalize( v1tov2 );
	// CW ordering so we compute a left-normal.
	return glm::vec2( -normalized.y, normalized.x );
}


float Face::GetDistance( glm::vec2 globalPoint )
{
	glm::vec2 v1 = __polygon->GetGlobalVertex(__vertexIndex1);
	return glm::dot( globalPoint - v1, GetNormal() );
}