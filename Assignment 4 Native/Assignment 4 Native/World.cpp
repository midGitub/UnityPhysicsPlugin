#include "World.h"


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
	// Check for collisions using SAT algorithm.
	for ( std::pair<POLYGON_HANDLE, Polygon*> pair : __polygons )
	{
		Polygon* aPolygon = pair.second;

		for ( std::pair<POLYGON_HANDLE, Polygon*> pair : __polygons )
		{
			Polygon* bPolygon = pair.second;

			// We don't want to test collisions between this polygon and itself!
			if ( aPolygon == bPolygon )
			{
				continue;
			}

			// Start testing for a collision...
			// TODO
		}
	}
}


// PUBLIC

// Constructor: Defaults a bunch of values on startup.
World::World( float fixedTimestepSeconds )
	: __accumulatedTimeSeconds( 0.0f )
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
	while ( __accumulatedTimeSeconds >= __fixedTimestepSeconds )
	{
		__accumulatedTimeSeconds -= __fixedTimestepSeconds;
		Step( __fixedTimestepSeconds );
		__currentTimeSeconds += __fixedTimestepSeconds;
	}
}

// Create a new Polygon instance and store it in the __polygons map so we can look it up by its
// handle later.
POLYGON_HANDLE World::CreatePolygon( std::vector<glm::vec2>* vertices, glm::vec2 position, float rotation )
{
	auto handle = GeneratePolygonHandle();
	__polygons.emplace( handle, new Polygon( vertices, position, rotation ) );
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
Polygon& World::GetPolygon( POLYGON_HANDLE handle )
{
	auto hasHandle = __polygons.count( handle );
	if ( !hasHandle )
	{
		throw std::exception( "No polygon exists at this handle!" );
	}
	return *__polygons.at( handle );
}

// Get the current physics clock time. This time exactly reflects the amount of time that the 
// World has simulated up to now and does not include accumulated time that has not factored 
// into a simulation step yet.
float World::GetCurrentTimeSeconds()
{
	return __currentTimeSeconds;
}