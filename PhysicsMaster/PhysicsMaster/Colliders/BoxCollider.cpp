#include "BoxCollider.h"
#include "ColliderUtil.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"

DoubleDispatchEnumaration(BoxCollider);


BoxCollider::BoxCollider(const Vector3& displ, const Vector3& dimensions)
:Collider(displ), _HalfSize(dimensions)
{
	ColliderInit
};

const Vector3& BoxCollider::GetHalfSize() const
{
	return _HalfSize;
};

float BoxCollider::GetSecondSize() const
{
	return _HalfSize.getY();
};

float BoxCollider::GetThirdSize() const
{
	return _HalfSize.getZ();
};

const Quaternion& BoxCollider::GetRotation() const
{
	return GetOwner()->GetChild<Transform>()->GetRotation();
};

void  BoxCollider::GetVertex(Vector3* Vertex) const
{

	Vertex[0][0] = _HalfSize[0];
	Vertex[0][1] = _HalfSize[1];
	Vertex[0][2] = _HalfSize[2];

	Vertex[1][0] = _HalfSize[0];
	Vertex[1][1] = _HalfSize[1];
	Vertex[1][2] = -_HalfSize[2];

	Vertex[2][0] = _HalfSize[0];
	Vertex[2][1] = -_HalfSize[1];
	Vertex[2][2] = _HalfSize[2];

	Vertex[3][0] = _HalfSize[0];
	Vertex[3][1] = -_HalfSize[1];
	Vertex[3][2] = -_HalfSize[2];

	Vertex[4][0] = -_HalfSize[0];
	Vertex[4][1] = _HalfSize[1];
	Vertex[4][2] = _HalfSize[2];

	Vertex[5][0] = -_HalfSize[0];
	Vertex[5][1] = _HalfSize[1];
	Vertex[5][2] = -_HalfSize[2];

	Vertex[6][0] = -_HalfSize[0];
	Vertex[6][1] = -_HalfSize[1];
	Vertex[6][2] = _HalfSize[2];

	Vertex[7][0] = -_HalfSize[0];
	Vertex[7][1] = -_HalfSize[1];
	Vertex[7][2] = -_HalfSize[2];

}