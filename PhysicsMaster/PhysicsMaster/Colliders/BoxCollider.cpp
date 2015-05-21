#include "BoxCollider.h"

BoxCollider::BoxCollider() : semiDims(1, 1, 1)
{}

BoxCollider::BoxCollider(Vector3 _semiDims) : semiDims(_semiDims)
{}

BoxCollider::BoxCollider(Vector3 _semiDims, Quaternion _rotation) : semiDims(_semiDims), rotation(_rotation)
{}

void BoxCollider::SetRotation(Quaternion _rotation)
{
	rotation = _rotation;
}

Quaternion BoxCollider::GetRotation() const
{
	return rotation;
}