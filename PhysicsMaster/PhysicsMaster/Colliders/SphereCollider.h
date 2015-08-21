#pragma once
#include "Collider.h"

class RigidBody;

class SphereCollider : public Collider
{
	DoubleDispatchMethod;
public:
	SphereCollider(const Vector3& displ, float radius);
	float GetRadius() const;
	~SphereCollider(){};
private:
	float _Radius;
};
