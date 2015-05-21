#pragma once
#include "Vector3.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "RigidBody.h"

class AABB : public Collider
{
private:
	//MEMBER
	Point3 min;
	Point3 max;
	bool empty;

	RigidBody* myBody;

	void Collide(AABB& i_Collider);
	void Collide(SphereCollider& i_Collider);

public:
	//CONSTRUCTOR
	AABB();
	~AABB();

	//MEMBER FUNCTION
	void clear();
	void add(Point3 point);
	bool isEmpty() const;
	Point3 center() const;
	Point3 minPoint() const;
	Point3 maxPoint() const;
	void AttachToBody(RigidBody* i_RBody);
	void Collide(Collider& i_Collider);
};