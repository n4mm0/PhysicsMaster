#pragma once
#include "Collision\CollisionHandler.h"
namespace Constants
{
	static const int MAX_COLLISION = 20;
	typedef CollisionHandler<Constants::MAX_COLLISION> CollisionsCollection;
	const float AirDrag = 1.0f;
	const float PhysicsDeltaTime = 0.01f;
	const Vector3 GravityForce = Vector3(0.0f, -9.81f, 0.0f);
	const float CubeDragCoefficient = 1.05f;
	const float SphereDragCoefficient = 0.67f;
};