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

	void Update();

	RigidBody* CreateRigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID);
	void DeleteRigidBody(RigidBody* _body);

private:
	RigidBodyCollection m_RigidBodies;
};