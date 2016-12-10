#pragma once
#include <vector>
#include <glm.hpp>

class Face;

class Polygon
{
	friend class World;

	private:

	std::vector<glm::vec2>* __vertices;
	std::vector<glm::vec2> __globalVertices;
	std::vector<Face> __faces;
	bool      __useGravity;
	float     __mass;
	float     __rotationalInertia;
	glm::vec2 __position;
	glm::vec2 __velocity;
	float     __rotation;
	float     __rotationalVelocity;
	float	  __netTorque;

	bool	  __isAwake;
	bool	  __isStatic;

	int __layer;
	std::vector<int> __collisionLayers;

	Polygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation = 0.0f, float mass = 1.0f, bool useGravity = false );
	Polygon(std::vector<glm::vec2>* vertices, glm::vec2 position, int layer, int* collisionLayers, int collisionLayersSize, bool isStatic, float rotation, float mass, bool useGravity);
	~Polygon();

	void UpdateCenterOfMass();
	void UpdateFaces();
	void UpdateGlobalVertices();
	void UpdateRotationalInertia();


	public:

	bool GetUseGravity();
	void SetUseGravity( bool useGravity );

	float GetMass();
	void SetMass( float mass );

	glm::vec2 GetPosition();
	void SetPosition( glm::vec2 position );
	void Translate( glm::vec2 dPosition );

	glm::vec2 GetVelocity();
	void SetVelocity( glm::vec2 velocity );
	void Accelerate( glm::vec2 dVelocity );

	float GetRotationalInertia();

	float GetRotation();
	void SetRotation( float rotation );
	void Rotate( float dRotation );

	float GetRotationalVelocity();
	void SetRotationalVelocity( float rotationalVelocity );
	void AccelerateRotation( float dRotationalVelocity );

	Face GetFace( int index );
	std::vector<Face>& GetFaces();

	glm::vec2 GetVertex( int index );
	glm::vec2 GetGlobalVertex( int index );
	std::vector<glm::vec2>& GetVertices();
	std::vector<glm::vec2>& GetGlobalVertices();
	void SetVertices( std::vector<glm::vec2>* vertices );

	void SetCollisionsLayers(int* collisionLayers, int sizeCollisionLayers);

	std::vector<int> GetCollisionLayers() const {
		return __collisionLayers;
	}

	inline int GetLayer() const {
		return __layer;
	}

	inline bool isAwake() const {
		return __isAwake;
	}

	inline void setSleeping() {
		__isAwake = false;
	}

	inline bool isStatic() const {
		return __isStatic;
	}

	inline void setStatic(bool b) {
		__isStatic = b;
	}
};
