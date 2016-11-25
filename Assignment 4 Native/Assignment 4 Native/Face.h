#pragma once
#include <glm.hpp>

class Polygon;

class Face
{
	private:

	Polygon* __polygon;
	int __vertexIndex1;
	int __vertexIndex2;

	public:

	Face( Polygon* polygon, int vertexIndex1, int vertexIndex2 );
	~Face();

	glm::vec2 GetNormal();
	float GetDistance( glm::vec2 globalPoint );
};

