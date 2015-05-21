#include "BoxCollider.h"
#include <iostream>

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

void BoxCollider::Collide(Collider& i_Collider)
{
	i_Collider.Collide(*this);
}

void BoxCollider::Collide(BoxCollider& i_Collider)
{
	std::cout << "Collision BoxBox! FIRE!" << std::endl;
}

void BoxCollider::Collide(SphereCollider& i_Collider)
{
	std::cout << "Collision BoxSphere! FIRE!" << std::endl;
}