#define _USE_MATH_DEFINES

#include <assert.h>
#include <cmath>
#include "Vector4.h"

//STATIC MEMBER
const Vector4 Vector4::vectorZero = Vector4();
const Vector4 Vector4::pointZero = Vector4(0, 0, 0, 1);

//CONSTRUCTORS
Vector4::Vector4()
{
	vector[0] = 0;
	vector[1] = 0;
	vector[2] = 0;
	vector[3] = 0;
}

Vector4::Vector4(scalar x, scalar y, scalar z, scalar w)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
}

Vector4::Vector4(const Vector4 &other)
{
	vector[0] = other.getX();
	vector[1] = other.getY();
	vector[2] = other.getZ();
	vector[3] = other.getW();
}

//DESTRUCTOR
Vector4::~Vector4()
{
}

//MEMBER FUNCTION
scalar Vector4::getX() const
{
	return vector[0];
}

scalar Vector4::getY() const
{
	return vector[1];
}

scalar Vector4::getZ() const
{
	return vector[2];
}

scalar Vector4::getW() const
{
	return vector[3];
}

void Vector4::setX(scalar x)
{
	vector[0] = x;
}

void Vector4::setY(scalar y)
{
	vector[1] = y;
}

void Vector4::setZ(scalar z)
{
	vector[2] = z;
}

void Vector4::setW(scalar w)
{
	vector[3] = w;
}

void Vector4::set(scalar x, scalar y, scalar z, scalar w)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
}

scalar Vector4::dot(const Vector4 &other) const
{
	scalar x = vector[0] * other.getX();
	scalar y = vector[1] * other.getY();
	scalar z = vector[2] * other.getZ();
	scalar w = vector[3] * other.getW();

	return x + y + z + w;
}

scalar Vector4::magnitude() const
{
	return sqrt(sqrMagnitude());
}

scalar Vector4::sqrMagnitude() const
{
	return this->dot(*this);
}

void Vector4::normalize()
{
	if (!isZero())
		*this /= this->magnitude();
}

bool Vector4::isZero() const
{
	return vector[0] == 0 && vector[1] == 0 && vector[2] == 0 && vector[3] == 0;
}

scalar Vector4::angle(const Vector4 &other)
{
	assert(vector[3] == 0);

	float angle = acos(this->dot(other) / (this->magnitude() * other.magnitude()));
	return angle *= 180.0f / M_PI;
}

scalar Vector4::distance(const Vector4 &other)
{
	Vector4 difference = other - *this;
	return difference.magnitude();
}

Vector4 Vector4::lerp(const Vector4 &other, float t)
{
	if (t <= 0.0f)
		return *this;
	else if (t >= 1.0f)
		return other;
	return *this + (other - *this) * t;
}

void Vector4::rotationX(const int degree)
{
	assert(vector[3] == 0);

	float rad = degree * M_PI / 180.0f;

	float cosines = cos(rad);
	float sine = sin(rad);

	scalar y = this->dot(Vector4(0, cosines, sine, 0));
	scalar z = this->dot(Vector4(0, -sine, cosines, 0));

	this->set(0, y, z, 0);
}

void Vector4::rotationY(const int degree)
{
	assert(vector[3] == 0);
	
	float rad = degree * M_PI / 180.0f;

	float cosines = cos(rad);
	float sine = sin(rad);

	scalar x = this->dot(Vector4(cosines, 0, -sine, 0));
	scalar z = this->dot(Vector4(sine, 0, cosines, 0));

	this->set(x, 0, z, 0);
}

void Vector4::rotationZ(const int degree)
{
	assert(vector[3] == 0);

	float rad = degree * M_PI / 180.0f;

	float cosines = cos(rad);
	float sine = sin(rad);

	scalar x = this->dot(Vector4(cosines, sine, 0, 0));
	scalar y = this->dot(Vector4(-sine, cosines, 0, 0));

	this->set(x, y, 0, 0);
}

//OPERATORS
Vector4 Vector4::operator +(const Vector4 &other) const
{
	Vector4 result = *this;
	return result += other;
}

Vector4 Vector4::operator -(const Vector4 &other) const
{
	Vector4 result = *this;
	return result -= other;
}

Vector4 Vector4::operator *(const scalar k) const
{
	Vector4 result = *this;
	return result *= k;
}

Vector4 Vector4::operator /(const scalar k) const
{
	Vector4 result = *this;
	return result /= k;
}

Vector4 Vector4::operator -() const
{
	return Vector4(-this->getX(), -this->getY(), -this->getZ(), -this->getW());
}

Vector4& Vector4::operator =(const Vector4 &other)
{
	if (this != &other)
	{
		vector[0] = other.getX();
		vector[1] = other.getY();
		vector[2] = other.getZ();
		vector[3] = other.getW();
	}
	return *this;
}

Vector4& Vector4::operator +=(const Vector4& other)
{
	vector[0] += other.getX();
	vector[1] += other.getY();
	vector[2] += other.getZ();
	vector[3] += other.getW();
	
	return *this;
}

Vector4& Vector4::operator -=(const Vector4& other)
{
	vector[0] -= other.getX();
	vector[1] -= other.getY();
	vector[2] -= other.getZ();
	vector[3] -= other.getW();
	
	return *this;
}

Vector4& Vector4::operator *=(const scalar k)
{
	vector[0] *= k;
	vector[1] *= k;
	vector[2] *= k;
	vector[3] *= k;
	
	return *this;
}

Vector4& Vector4::operator /=(const scalar k)
{
	vector[0] /= k;
	vector[1] /= k;
	vector[2] /= k;
	vector[3] /= k;
	
	return *this;
}

bool Vector4::operator ==(Vector4 const &other) const
{
	return	vector[0] == other.getX() &&
			vector[1] == other.getY() &&
			vector[2] == other.getZ() &&
			vector[3] == other.getW();
}

bool Vector4::operator !=(Vector4 const &other) const
{
	return	vector[0] != other.getX() ||
			vector[1] != other.getY() ||
			vector[2] != other.getZ() ||
			vector[3] == other.getW();
}