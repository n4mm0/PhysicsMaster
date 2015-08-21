#include "SphereCollider.h"
#include "ColliderUtil.h"


DoubleDispatchEnumaration(SphereCollider)

SphereCollider::SphereCollider(const Vector3& displ, float radius)
:Collider(displ), _Radius(radius)
{
	ColliderInit
};

float SphereCollider::GetRadius() const
{
	return _Radius;
}
