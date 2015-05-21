#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "RigidBody.h"

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
};