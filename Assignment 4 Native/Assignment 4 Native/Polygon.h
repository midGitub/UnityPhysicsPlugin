#pragma once
#include <vector>
#include <glm.hpp>

class Face;

class Polygon
{
	private:

	std::vector<glm::vec2>* __vertices;
	std::vector<glm::vec2>* __globalVertices;
	std::vector<Face*>* __faces;
	glm::vec2 __position;
	float     __rotation;

	void UpdateGlobalVertices();
	void UpdateFaces();


	public:

	Polygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation = 0.0f );
	~Polygon();

	glm::vec2 GetPosition();
	void SetPosition( glm::vec2 position );
	void Translate( glm::vec2 dPosition );

	float GetRotation();
	void SetRotation( float rotation );
	void Rotate( float dRotation );

	Face* GetFace( int index );
	std::vector<Face*>& GetFaces();

	glm::vec2 GetVertex( int index );
	glm::vec2 GetGlobalVertex( int index );
	std::vector<glm::vec2>& GetVertices();
	std::vector<glm::vec2>& GetGlobalVertices();
	void SetVertices( std::vector<glm::vec2>* vertices );
};
