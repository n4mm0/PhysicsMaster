#pragma once
#pragma once
#include "Collider.h"
class Quaternion;
class RigidBody;
class BoxCollider : public Collider
{
public:
	BoxCollider(const Vector3& pos, const Vector3& displ, const Quaternion& Rot);
	BoxCollider(const Vector3& pos, const Vector3& displ, const Quaternion& Rot, const Vector3& dimensions);

	static const int getType();
	const Quaternion& GetRotation() const;
	
	//For SAT
	//const Vector3& GetAxis(int i) const;
	//const Vector3& GetFirstAxis();
	//const Vector3& GetSecondAxis();
	//const Vector3& GetThirdAxis();
	
	const Vector3& GetHalfSize() const;
	float GetSecondSize() const;
	float GetThirdSize() const;

private:
	static const int _BoxColliderType;
	const Quaternion* _Rotation;
	Vector3 _Center;
	Vector3 _HalfSize;
	//For SAT
	//Vector3 _Axises[3];
};


/*#include "Vector3.h"
#include "Quaternion.h"
#include "Collider.h"
#include "RigidBody.h"

class SphereCollider;

class BoxCollider : public Collider
{
private:
	Vector3 semiDims;
	Quaternion rotation;

	void Collide(BoxCollider& i_Collider);
	void Collide(SphereCollider& i_Collider);

public:
	BoxCollider();
	BoxCollider(Vector3 semiDims);
	BoxCollider(Vector3 semiDims, Quaternion rotation);
	~BoxCollider();

	void SetRotation(Quaternion rotation);
	Quaternion GetRotation() const;
	void Collide(Collider& i_Collider);
};*/