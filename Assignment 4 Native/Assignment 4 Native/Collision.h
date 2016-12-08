#pragma once
#include <glm.hpp>

class Face;
class Polygon;

struct Collision
{
	Polygon* facePolygon;
	Polygon* contactPolygon;
	glm::vec2 faceNormal;
	glm::vec2 contactVertex;
	float depth;

	Collision();

	float GetAngularMomentum();
};
