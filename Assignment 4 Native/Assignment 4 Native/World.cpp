#include "World.h"
#include "Collision.h"
#include "Face.h"

//using the function find from this library
#include <algorithm>

// PRIVATE

// Returns a new unique HANDLE each time it is called that the World uses to make it possible 
// to look up Polygons uniquely using integer values.
POLYGON_HANDLE World::GeneratePolygonHandle()
{
	return __nextHandle++;
}

// Handles a single physics step.
void World::Step( float deltaTimeSeconds )
{
	// Clean out collisions from last frame.
	__collisions.clear();

	// Collision detection.
	for( auto aIterator = __polygons.begin(); aIterator != __polygons.end(); ++aIterator )
	{
		Polygon* aPolygon = aIterator->second;

		for( auto bIterator = aIterator; bIterator != __polygons.end(); ++bIterator )
		{
			Polygon* bPolygon = bIterator->second;

			// We don't want to test collisions between this polygon and itself!
			if( aPolygon == bPolygon )
			{
				continue;
			}

			std::vector<int> aCollisions = aPolygon->GetCollisionLayers();
			std::vector<int> bCollisions = bPolygon->GetCollisionLayers();

			//iterator to look for the collision layers
			std::vector<int>::iterator it;
			it = find(aCollisions.begin(), aCollisions.end(), bPolygon->GetLayer());
			
			//assuming that when there's no collision layer is the default and collide with everything
			//or check if the layer of the other polygon was found

			//only check for collision if the specified layer is supposed to collide
			if (aCollisions.size() == 0 || it != aCollisions.end())
			{
				// Actually test whether this pair collides and store the collision if so.
				Collision collision;
				if (TestCollision(aPolygon, bPolygon, &collision))
				{
					__collisions.push_back(collision);
				}
			}
		}
	}

	// Collision resolution.
	for( auto collision : __collisions )
	{
		// Do something with each collision...
		// TODO

		collision.contactPolygon->SetVelocity(-collision.contactPolygon->__velocity);
	}

	// Integrate force -> acceleration -> velocity -> position.
	for ( auto aIterator = __polygons.begin(); aIterator != __polygons.end(); ++aIterator )
	{
		Polygon* aPolygon = aIterator->second;

		//if polygon is static, no forces are acting on it
		if (aPolygon->isStatic()) {
			continue;
		}

		// Apply gravity if this polygon is expecting it.
		if ( aPolygon->GetUseGravity() )
		{
			aPolygon->Accelerate( glm::vec2( 0.0f, __gravityAcceleration * deltaTimeSeconds ) );
		}

		// Maybe here is a good place to account for any forces acting on your polygons...
		// TODO

		float inverseMass = 1.0f / aPolygon->GetMass();
		//aPolygon->Accelerate(aPolygon->GetNetForce() * inverseMass * deltaTimeSeconds);
		//aPolygon->AccelerateRotation(aPolygon->GetNetTorque() * inverseMass * deltaTimeSeconds);

		// Integrate velocity to position (Euler method).
		aPolygon->Translate(aPolygon->GetVelocity() * deltaTimeSeconds);
		aPolygon->Rotate(aPolygon->GetRotationalVelocity() * deltaTimeSeconds);
		//aPolygon->Translate( aPolygon->GetVelocity() * deltaTimeSeconds );

		//aPolygon->setnetforce(glm::vec2(0.0f, 0.0f));
		//aPolygon->setnettorque(0.0f);

		// Integrate rotational velocity to rotation (Euler method).
		//aPolygon->Rotate( aPolygon->GetRotationalVelocity() * deltaTimeSeconds );
	}
}

bool World::TestCollision( Polygon* aPolygon, Polygon* bPolygon, Collision* maybeCollision )
{
	// Test SAT with the faces of aPolygon and the vertices of bPolygon.
	if (!TestSeparateAxisTheorem(aPolygon, bPolygon, maybeCollision))
	{
		return false;
	}

	// Test SAT with the faces of bPolygon and the vertices of aPolygon.
	if (!TestSeparateAxisTheorem(bPolygon, aPolygon, maybeCollision))
	{
		return false;
	}

	// If we haven't exited out yet, return the collision object.
	return true;
}

bool World::TestSeparateAxisTheorem( Polygon* facePolygon, Polygon* vertexPolygon, Collision* maybeCollision )
{
	// For each face in bPolygon
	for( Face aFace : facePolygon->GetFaces() )
	{
		// Find the vertex in bPolygon with the minimum distance from the face.
		float minDistance = FLT_MAX;
		glm::vec2 minVertex;
		for( glm::vec2 bVertex : vertexPolygon->GetGlobalVertices() )
		{
			float distance = aFace.GetGlobalDistance( bVertex );
			if( distance < minDistance )
			{
				minDistance = distance;
				minVertex = bVertex;
			}
		}

		// If the distance to the nearest vertex in b is greater than 0, we can't be in collision.
		if( minDistance > 0 )
		{
			return false;
		}

		// The least negative distance is the best candidate for depenetration.
		if( minDistance > maybeCollision->depth )
		{
			maybeCollision->facePolygon = facePolygon;
			maybeCollision->contactPolygon = vertexPolygon;
			maybeCollision->contactVertex = minVertex;
			maybeCollision->depth = minDistance;
			maybeCollision->faceNormal = aFace.GetGlobalNormal();
		}
	}

	return true;
}



// PUBLIC

// Constructor: Defaults a bunch of values on startup.
World::World( float fixedTimestepSeconds, float gravityAcceleration )
	: __accumulatedTimeSeconds( 0.0f )
	, __gravityAcceleration( gravityAcceleration )
	, __fixedTimestepSeconds( fixedTimestepSeconds )
	, __nextHandle( 1 )
	, __polygons( std::map<POLYGON_HANDLE, Polygon*>() )
{

}

// Destructor: Doesn't need to do anything.
World::~World()
{

}

// Update the World's clock by taking the in deltaTimeSeconds and calling Step() once for each 
// interval of __fixedTimestepSeconds so the simulations catches up to real time.
// Note: This is the time accumulator pattern that we'll discuss in class!
void World::Update( float deltaTimeSeconds )
{
	__accumulatedTimeSeconds += deltaTimeSeconds;
	while( __accumulatedTimeSeconds >= __fixedTimestepSeconds )
	{
		__accumulatedTimeSeconds -= __fixedTimestepSeconds;
		Step( __fixedTimestepSeconds );
		__currentTimeSeconds += __fixedTimestepSeconds;
	}
}

// Create a new Polygon instance and store it in the __polygons map so we can look it up by its
// handle later.
POLYGON_HANDLE World::CreatePolygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation, float mass, bool useGravity )
{
	auto handle = GeneratePolygonHandle();
	__polygons.emplace( handle, new Polygon( vertices, position, rotation, mass, useGravity ) );
	return handle;
}

POLYGON_HANDLE World::CreatePolygon(std::vector<glm::vec2>* vertices, glm::vec2 position, int layer, int* collisionLayers, int collisionLayersSize, bool isStatic, float rotation, float mass, bool useGravity)
{
	auto handle = GeneratePolygonHandle();
	__polygons.emplace(handle, new Polygon(vertices, position, layer, collisionLayers, collisionLayersSize, isStatic, rotation, mass, useGravity));
	return handle;
}

// Destroy the Polygon at the provided handle by erasing the mapping in __polygons and deleting 
// the Polygon instance from the heap.
void World::DestroyPolygon( POLYGON_HANDLE handle )
{
	auto pair = __polygons.find( handle );
	Polygon* polygon = pair->second;
	__polygons.erase( pair );
	delete polygon;
}

// If a Polygon exists at the provided handle, return a reference to it.
Polygon* World::GetPolygon( POLYGON_HANDLE handle )
{
	auto hasHandle = __polygons.count( handle );
	if( !hasHandle )
	{
		throw std::exception( "No polygon exists at this handle!" );
	}
	return __polygons.at( handle );
}

// Get the current physics clock time. This time exactly reflects the amount of time that the 
// World has simulated up to now and does not include accumulated time that has not factored 
// into a simulation step yet.
float World::GetCurrentTimeSeconds()
{
	return __currentTimeSeconds;
}

// Check if 2 polygons are intersecting.
bool World::IsPolygonColliding( Polygon* polygon )
{
	for( Collision collision : __collisions )
	{
		if( polygon == collision.facePolygon )
		{
			return true;
		}
		else if( polygon == collision.contactPolygon )
		{
			return true;
		}
	}
	return false;
}