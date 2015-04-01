#pragma once
/*
** Mondo di gioco
*/
#include "RigidBody.h"
#include <vector>

class World {
private:
	std::vector<RigidBody> objects;
};