#include <cmath>
#include "Quaternion.h"

//CONSTRUCTORS
Quaternion::Quaternion()
{
	quaternion[0] = 1;
	quaternion[1] = 0;
	quaternion[2] = 0;
	quaternion[3] = 0;
}

Quaternion::Quaternion(scalar a, scalar i, scalar j, scalar k)
{
	quaternion[0] = a;
	quaternion[1] = i;
	quaternion[2] = j;
	quaternion[3] = k;
}

Quaternion::Quaternion(const Quaternion &other)
{
	quaternion[0] = other.getA();
	quaternion[1] = other.getI();
	quaternion[2] = other.getJ();
	quaternion[3] = other.getK();
}

//DESTRUCTOR
Quaternion::~Quaternion()
{

}

//MEMBER FUNCTION
scalar Quaternion::getA() const
{
	return quaternion[0];
}

scalar Quaternion::getI() const
{
	return quaternion[1];
}

scalar Quaternion::getJ() const
{
	return quaternion[2];
}

scalar Quaternion::getK() const
{
	return quaternion[3];
}

void Quaternion::setA(scalar a)
{
	quaternion[0] = a;
}

void Quaternion::setI(scalar i)
{
	quaternion[1] = i;
}

void Quaternion::setJ(scalar j)
{
	quaternion[2] = j;
}

void Quaternion::setK(scalar k)
{
	quaternion[3] = k;
}

scalar Quaternion::dot(const Quaternion &other) const
{
	scalar a = quaternion[0] * other.getA();
	scalar i = quaternion[1] * other.getI();
	scalar j = quaternion[2] * other.getJ();
	scalar k = quaternion[3] * other.getK();

	return a + i + j + k;
}

float Quaternion::magnitude() const
{
	return sqrt(sqrMagnitude());
}

scalar Quaternion::sqrMagnitude() const
{
	return this->dot(*this);
}

void Quaternion::normalize()
{
	if (!isZero())
		*this /= this->magnitude();
}

bool Quaternion::isZero() const
{
	return quaternion[0] == 0 && quaternion[1] == 0 && quaternion[2] == 0 && quaternion[3] == 0;
}

Matrix4x4 Quaternion::toMatrix() const
{
	float X2 = quaternion[1] * quaternion[1];
	float Y2 = quaternion[2] * quaternion[2];
	float Z2 = quaternion[3] * quaternion[3];
	float XY = quaternion[1] * quaternion[2];
	float XZ = quaternion[1] * quaternion[3];
	float YZ = quaternion[2] * quaternion[3];
	float XW = quaternion[0] * quaternion[1];
	float YW = quaternion[0] * quaternion[2];
	float ZW = quaternion[0] * quaternion[3];
	
	Matrix4x4 result;

	result.setElementAt(0, 0, 1.0f - (2.0f * Y2) - (2.0f * Z2));
	result.setElementAt(0, 1, (2.0f * XY) - (2.0f * ZW));
	result.setElementAt(0, 2, (2.0f * XZ) + (2.0f * YW));
	result.setElementAt(1, 0, (2.0f * XY) + (2.0f * ZW));
	result.setElementAt(1, 1, 1.0f - (2.0f * X2) - (2.0f * Z2));
	result.setElementAt(1, 2, (2.0f * YZ) - (2.0f * XW));
	result.setElementAt(2, 0, (2.0f * XZ) - (2.0f * YW));
	result.setElementAt(2, 1, (2.0f * YZ) - (2.0f * XW));
	result.setElementAt(2, 2, 1.0f - (2.0f * X2) - (2.0f * Y2));

	return result;
}

//OPERATORS
Quaternion Quaternion::operator +(const Quaternion &other) const
{
	Quaternion result = *this;
	return result += other;
}

Quaternion Quaternion::operator -(const Quaternion &other) const
{
	Quaternion result = *this;
	return result -= other;
}

Quaternion Quaternion::operator *(const Quaternion &other) const
{
	Quaternion result = *this;
	return result *= other;
}

Quaternion Quaternion::operator /(const scalar scalar) const
{
	Quaternion result = *this;
	return result /= scalar;
}

Quaternion& Quaternion::operator =(const Quaternion &other)
{
	if (this != &other)
	{
		quaternion[0] = other.getA();
		quaternion[1] = other.getI();
		quaternion[2] = other.getJ();
		quaternion[3] = other.getK();
	}
	return *this;
}

Quaternion& Quaternion::operator +=(const Quaternion& other)
{
	quaternion[0] += other.getA();
	quaternion[1] += other.getI();
	quaternion[2] += other.getJ();
	quaternion[3] += other.getK();

	return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& other)
{
	quaternion[0] -= other.getA();
	quaternion[1] -= other.getI();
	quaternion[2] -= other.getJ();
	quaternion[3] -= other.getK();

	return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other)
{
	quaternion[0] = quaternion[0] * other.getA() - quaternion[1] * other.getI() - quaternion[2] * other.getJ() - quaternion[3] * other.getK();
	quaternion[1] = quaternion[0] * other.getI() + quaternion[1] * other.getA() + quaternion[2] * other.getK() - quaternion[3] * other.getJ();
	quaternion[2] = quaternion[0] * other.getJ() - quaternion[1] * other.getJ() + quaternion[2] * other.getA() + quaternion[3] * other.getI();
	quaternion[3] = quaternion[0] * other.getK() + quaternion[1] * other.getK() - quaternion[2] * other.getI() + quaternion[3] * other.getA();
	return *this;

	return *this;
}

Quaternion& Quaternion::operator /= (const float scalar)
{
	quaternion[0] /= scalar;
	quaternion[1] /= scalar;
	quaternion[2] /= scalar;
	quaternion[3] /= scalar;

	return *this;
}