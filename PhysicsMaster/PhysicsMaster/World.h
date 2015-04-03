#pragma once
/*
** Mondo di gioco
*/
#include <vector>
#include "RigidBody.h"

typedef std::vector<RigidBody*> RigidBodyCollection;

class World {
public:
	World();
	~World();

private:
	RigidBodyCollection m_RigidBodies;
};