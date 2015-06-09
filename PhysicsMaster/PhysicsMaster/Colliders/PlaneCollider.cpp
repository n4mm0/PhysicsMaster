#include "PlaneCollider.h"
#include "ColliderUtil.h"
#include "RigidBody.h"

const int PlaneCollider::_PlaneColliderType = TL::IndexOf<ColliderType::ColliderTypeList, PlaneCollider>::value;


const int PlaneCollider::getType()
{
	return PlaneCollider::_PlaneColliderType;
}

PlaneCollider::PlaneCollider(const Vector3& pos, const Vector3& displ, const Vector3& norm)
:Collider(_PlaneColliderType, displ), m_Normal(norm)
{
	SetPosition(pos);
}


