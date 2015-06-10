#pragma once
#include "Vector3.h"

class RigidBody;

class Collision
{
public:
	Collision();
	void Init(float deformation, const Vector3& pointOfApplication, const Vector3& normal);
	void SetBodies(RigidBody* first,RigidBody* second);
	void ApplyCollision();
private:
	Collision(const Collision& other);
	Collision& operator=(const Collision& other);
	RigidBody* m_firstObj;
	RigidBody* m_secondObj;
	float m_deformation;
	Vector3 m_pointOfApplication;
	Vector3 m_force;
	Vector3 m_normal;
};