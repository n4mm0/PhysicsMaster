#include "PlaneCollider.h"
#include "ColliderUtil.h"
#include "RigidBody.h"
DoubleDispatchEnumaration(PlaneCollider);

PlaneCollider::PlaneCollider(const Vector3& displ, const Vector3& norm)
:Collider(displ), m_Normal(norm)
{
	ColliderInit
}

const Vector3& PlaneCollider::GetPlaneNormal() const 
{
	return m_Normal;
};

