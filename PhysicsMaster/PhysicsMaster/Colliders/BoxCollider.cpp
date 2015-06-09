#include "BoxCollider.h"
#include "ColliderUtil.h"
#include "RigidBody.h"

const int BoxCollider::_BoxColliderType = TL::IndexOf<ColliderType::ColliderTypeList, BoxCollider>::value;

const int  BoxCollider::getType()
{
	return BoxCollider::_BoxColliderType;
}

BoxCollider::BoxCollider(const Vector3& pos, const Vector3& displ, const Quaternion& Rot)
:Collider(_BoxColliderType, displ)
{
	//For SAT
	//_Axises[0] = Vector3(1, 0, 0);
	//_Axises[1] = Vector3(0, 1, 0);
	//_Axises[2] = Vector3(0, 0, 1);
	_HalfSize = Vector3(1, 1, 1);
	SetPosition(pos);
	_Rotation = &Rot;
};

BoxCollider::BoxCollider(const Vector3& pos, const Vector3& displ, const Quaternion& Rot, const Vector3& dimensions)
:Collider(_BoxColliderType, displ), _HalfSize(dimensions)
{
	//For SAT
	//_Axises[0] = Vector3(1, 0, 0);
	//_Axises[1] = Vector3(0, 1, 0);
	//_Axises[2] = Vector3(0, 0, 1);
	SetPosition(pos);
	_Rotation = &Rot;
};

//For SAT
/*
const Vector3& BoxCollider::GetFirstAxis()
{
	return _Axises[0];
};

const Vector3& BoxCollider::GetSecondAxis()
{
	return _Axises[1];
};

const Vector3& BoxCollider::GetThirdAxis()
{
	return _Axises[2];
};

const Vector3& BoxCollider::GetAxis(int i) const
{
return _Axises[i];
};

*/
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
	return *_Rotation;
};

/*
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
}*/
