#pragma once
/*
** Mondo di gioco
*/
#include <vector>
#include "RigidBody.h"
#include "Vector3.h"
#include "Collision\CollisionHandler.h"
#include "Utilities\ColliderUtil.h"
#include "Utilities\Dispatcher.h"

typedef std::vector<RigidBody*> RigidBodyCollection;

typedef ColliderType::ColliderTypeList CollidersType;
typedef CollisionHandler<20> CollisionCollection;

//Need Change
typedef Dispatcher<Collider, CollidersType, Collision*> ColliderDispatcher;

class World {
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
	CollisionCollection m_CollisionCollection;
	ColliderDispatcher m_Dispatcher;
};