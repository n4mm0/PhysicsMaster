#pragma once
#include "Collider.h"

class Quaternion;
class RigidBody;

class BoxCollider : public Collider
{
	DoubleDispatchMethod;
public:

	BoxCollider(const Vector3& displ,const Vector3& dimensions);
	void  GetVertex(Vector3* Vertex) const;

	const Quaternion& GetRotation() const;
	
	const Vector3& GetHalfSize() const;
	float GetSecondSize() const;
	float GetThirdSize() const;
	~BoxCollider(){};
private:
	Vector3 _Center;
	Vector3 _HalfSize;
};
