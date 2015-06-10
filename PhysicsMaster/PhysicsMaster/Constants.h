#pragma once
#include "Collision\CollisionHandler.h"
namespace Constants
{
	static const int MAX_COLLISION = 20;
	typedef CollisionHandler<Constants::MAX_COLLISION> CollisionsCollection;

	
};