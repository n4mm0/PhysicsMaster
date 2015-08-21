#pragma once
#include "Collider.h"

class RigidBody;

class PlaneCollider : public Collider
{
	DoubleDispatchMethod;
public:
	PlaneCollider(const Vector3& displ, const Vector3& norm);
	const Vector3& GetPlaneNormal() const;
	~PlaneCollider(){};
private:
	const Vector3 m_Normal;
};

