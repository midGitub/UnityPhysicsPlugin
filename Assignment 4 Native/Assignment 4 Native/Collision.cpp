#include "Collision.h"
#include "Polygon.h"


Collision::Collision()
	: facePolygon( NULL )
	, contactPolygon( NULL )
	, faceNormal( glm::vec2() )
	, contactVertex( glm::vec2() )
	, depth( -FLT_MAX )
{
}

float Collision::GetAngularMomentum()
{
	float restitution = 1.0f;

	glm::vec2 relativeVelocity = facePolygon->GetVelocity() - contactPolygon->GetVelocity();
	float relativeVelocityAlongNormal = glm::dot( faceNormal, relativeVelocity );

	float aInverseMass = 1.0f / facePolygon->GetMass();
	float bInverseMass = 1.0f / contactPolygon->GetMass();

	float aInverseRotationalInertia = 1.0f / facePolygon->GetRotationalInertia();
	float bInverseRotationalInertia = 1.0f / contactPolygon->GetRotationalInertia();

	glm::vec3 aCenterOfMassToContact = glm::vec3( contactVertex - facePolygon->GetPosition(), 0.0f );
	glm::vec3 bCenterOfMassToContact = glm::vec3( contactVertex - contactPolygon->GetPosition(), 0.0f );

	glm::vec3 normal = glm::vec3( faceNormal, 0.0f );
	glm::vec3 aRxN = glm::cross( aCenterOfMassToContact, normal );
	glm::vec3 bRxN = glm::cross( bCenterOfMassToContact, normal );

	glm::vec2 aRxNxR = glm::vec2( glm::cross( aRxN, aCenterOfMassToContact ) );
	glm::vec2 bRxNxR = glm::vec2( glm::cross( bRxN, bCenterOfMassToContact ) );

	float aRotationalInertiaTerm = glm::dot( faceNormal, aInverseRotationalInertia * aRxNxR );
	float bRotationalInertiaTerm = glm::dot( faceNormal, bInverseRotationalInertia * bRxNxR );

	float numerator = -( relativeVelocityAlongNormal * ( restitution + 1.0f ) );
	float denominator = ( aInverseMass + bInverseMass + aRotationalInertiaTerm + bRotationalInertiaTerm );

	float angularMomentum = numerator / denominator;

	return angularMomentum;
}
