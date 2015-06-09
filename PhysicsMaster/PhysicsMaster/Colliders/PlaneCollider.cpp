#include "PlaneCollider.h"
#include "ColliderUtil.h"
#include "RigidBody.h"

const int PlaneCollider::_PlaneColliderType = ColliderType::TypeIndex::PLANE;

const int PlaneCollider::getType()
{
	return PlaneCollider::_PlaneColliderType;
}

PlaneCollider::PlaneCollider(const Vector3& pos, const Vector3& displ, const Vector3& norm)
:Collider(_PlaneColliderType, displ), m_Normal(norm)
{
	SetPosition(pos);
}


