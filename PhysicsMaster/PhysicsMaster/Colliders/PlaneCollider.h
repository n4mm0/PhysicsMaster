#pragma once
#include "Collider.h"

class RigidBody;

class PlaneCollider : public Collider
{
public:
	PlaneCollider(const Vector3& pos, const Vector3& displ, const Vector3& norm);
	
	const Vector3& GetPlaneNormal() const
	{
		return m_Normal;
	};
	
	static const int getType();

private:
	const Vector3 m_Normal;
	static const int _PlaneColliderType;
};

