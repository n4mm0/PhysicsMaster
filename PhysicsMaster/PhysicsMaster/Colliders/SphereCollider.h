#pragma once
#include "Collider.h"

class RigidBody;

class SphereCollider : public Collider
{
public:
	SphereCollider(const Vector3& pos, const Vector3& displ, float radius);
	float GetRadius() const;
	static const int getType();
private:
	float _Radius;
	static const int _SphereColliderType;
};


/*
#include "Vector3.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "RigidBody.h"

class SphereCollider : public Collider
{
private:
	Point3 center;
	float radius;
	bool empty;

	RigidBody* myBody;

	void Collide(AABB& i_Collider);
	void Collide(SphereCollider& i_Collider);

public:
	SphereCollider(Point3 i_Center, float i_Radius);
	~SphereCollider();

	void Clear();
	void SetCenter(Point3 i_NewCenter);
	Point3 GetCenter() const;
	void AttachToBody(RigidBody* i_RBody);
	void Collide(Collider& i_Collider);
};*/