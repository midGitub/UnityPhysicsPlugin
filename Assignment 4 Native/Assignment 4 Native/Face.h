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

	Polygon* GetPolygon();
	int GetVertexIndex1();
	int GetVertexIndex2();
	glm::vec2 GetVertex1();
	glm::vec2 GetVertex2();
	glm::vec2 GetGlobalVertex1();
	glm::vec2 GetGlobalVertex2();

	glm::vec2 GetVector();
	glm::vec2 GetGlobalVector();

	glm::vec2 GetNormal();
	glm::vec2 GetGlobalNormal();

	float GetDistance( glm::vec2 localPoint );
	float GetGlobalDistance( glm::vec2 globalPoint );
};

