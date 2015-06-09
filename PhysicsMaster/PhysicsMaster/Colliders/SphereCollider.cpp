#include "SphereCollider.h"
#include "ColliderUtil.h"

const int SphereCollider::_SphereColliderType = TL::IndexOf<ColliderType::ColliderTypeList, SphereCollider>::value;

SphereCollider::SphereCollider(const Vector3& pos, const Vector3& displ, float radius)
:Collider(_SphereColliderType, displ), _Radius(radius)
{
	SetPosition(pos);
};

float SphereCollider::GetRadius() const
{
	return _Radius;
}

const int SphereCollider::getType()
{
	return SphereCollider::_SphereColliderType;
};