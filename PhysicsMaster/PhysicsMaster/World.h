#pragma once
/*
** Mondo di gioco
*/
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

	RigidBody* CreateRigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID);
	void DeleteRigidBody(RigidBody* _body);

	static const Vector3 m_Gravity;
	static const float m_Dt;

private:
	RigidBodyCollection m_RigidBodies;
	//CollisionCollection m_CollisionCollection;

	//test
public:
	ColliderDispatcher m_Dispatcher;
};