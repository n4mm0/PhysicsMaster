#pragma once

#include <vector>
#include "Collision\CollisionHandler.h"
#include "Utilities\ColliderUtil.h"
#include "Utilities\Dispatcher.h"
#include "Collision\CollisionAlgorithms.h"
#include "Constants.h"
class RigidBody;

class World
{
	typedef std::vector<RigidBody*> RigidBodyCollection;
	typedef ColliderType::ColliderTypeList CollidersType;
	typedef Dispatcher<Collider, CollidersType, int> ColliderDispatcher;
	typedef Constants::CollisionsCollection CollisionList;
public:
	World();
	~World();

	void Update();
	void AddRigidBody(RigidBody* body);
private:
	RigidBodyCollection m_RigidBodies;
	ColliderDispatcher m_Dispatcher;
};