#pragma once
#include "Matrix4x4.h"
class Vector3;
class Quaternion
{
private:
	//MEMBER
	scalar quaternion[4];

public:
	//CONSTRUCTORS
	Quaternion();
	Quaternion(scalar a, scalar i, scalar j, scalar k);
	Quaternion(const Quaternion &other);

	//DESTRUCTOR
	~Quaternion();

	//MEMBER FUNCTION
	scalar getA() const;
	scalar getI() const;
	scalar getJ() const;
	scalar getK() const;
	void setA(scalar a);
	void setI(scalar i);
	void setJ(scalar j);
	void setK(scalar k);
	scalar dot(const Quaternion &other) const;
	float magnitude() const;
	scalar sqrMagnitude() const;
	void normalize();
	bool isZero() const;
	Matrix4x4 toMatrix() const;
	
	//OPERATORS
	Quaternion operator +(const Quaternion &other) const;
	Quaternion operator -(const Quaternion &other) const;
	Quaternion operator *(const Quaternion &other) const;
	Quaternion operator /(const scalar scalar) const;
	Quaternion& operator =(const Quaternion &other);
	Quaternion& operator +=(const Quaternion& other);
	Quaternion& operator -=(const Quaternion& other);
	Quaternion& operator *=(const Quaternion& other);
	Quaternion& operator /=(const float scalar);
	float& operator[](int index);
	const float& operator[](int index) const;
};

void QuaternionRotate (const Quaternion& rot, const Vector3& vec, Vector3& result);
void QuaternionRotateT(const Quaternion& rot, const Vector3& vec, Vector3& result);