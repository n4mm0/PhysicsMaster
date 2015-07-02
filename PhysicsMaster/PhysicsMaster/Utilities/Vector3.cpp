#define _USE_MATH_DEFINES

#include <cmath>
#include "Vector3.h"
#include <iostream>
//STATIC MEMBER
const Vector3 Vector3::Zero = Vector3();

//CONSTRUCTORS
Vector3::Vector3()
{
	vector[0] = 0;
	vector[1] = 0;
	vector[2] = 0;
}

Vector3::Vector3(scalar x, scalar y, scalar z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

Vector3::Vector3(const Vector3& other)
{
	vector[0] = other.getX();
	vector[1] = other.getY();
	vector[2] = other.getZ();
}

//DESTRUCTOR
Vector3::~Vector3()
{
}

//MEMBER FUNCTION
scalar Vector3::getX() const
{
	return vector[0];
}

scalar Vector3::getY() const
{
	return vector[1];
}

scalar Vector3::getZ() const
{
	return vector[2];
}

void Vector3::setX(scalar x)
{
	vector[0] = x;
}

void Vector3::setY(scalar y)
{
	vector[1] = y;
}

void Vector3::setZ(scalar z)
{
	vector[2] = z;
}

void Vector3::set(scalar x, scalar y, scalar z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

scalar Vector3::dot(const Vector3 &other) const
{
	scalar x = vector[0] * other.getX();
	scalar y = vector[1] * other.getY();
	scalar z = vector[2] * other.getZ();
	
	return x + y + z;
}

Vector3 Vector3::cross(const Vector3 &other) const
{
	scalar x = vector[1] * other.getZ() - vector[2] * other.getY();
	scalar y = vector[2] * other.getX() - vector[0] * other.getZ();
	scalar z = vector[0] * other.getY() - vector[1] * other.getX();
	
	return Vector3(x, y, z);
}

scalar Vector3::magnitude() const
{
	return sqrt(sqrMagnitude());
}

scalar Vector3::sqrMagnitude() const
{
	return this->dot(*this);
}

void Vector3::normalize()
{
	if (!isZero())
		*this /= this->magnitude();
}

bool Vector3::isZero() const
{
	return vector[0] == 0 && vector[1] == 0 && vector[2] == 0;
}

scalar Vector3::angle(const Vector3 &other)
{
	float angle = acos(this->dot(other) / (this->magnitude() * other.magnitude()));
	return angle *= 180.0f / M_PI;
}

scalar Vector3::distance(const Vector3 &other)
{
	Vector3 difference = other - *this;
	return difference.magnitude();
}

Vector3 Vector3::lerp(const Vector3 &other, float t)
{
	if (t <= 0.0f)
		return *this;
	else if (t >= 1.0f)
		return other;
	return *this + (other - *this) * t;
}

void Vector3::rotationX(const int degree)
{
	float rad = degree * M_PI / 180.0f;

	float cosines = cos(rad);
	float sine = sin(rad);

	scalar y = this->dot(Vector3(0, cosines, sine));
	scalar z = this->dot(Vector3(0, -sine, cosines));

	this->set(0, y, z);
}

void Vector3::rotationY(const int degree)
{
	float rad = degree * M_PI / 180.0f;
	
	float cosines = cos(rad);
	float sine = sin(rad);

	scalar x = this->dot(Vector3(cosines, 0, -sine));
	scalar z = this->dot(Vector3(sine, 0, cosines));
	
	this->set(x, 0, z);
}

void Vector3::rotationZ(const int degree)
{
	float rad = degree * M_PI / 180.0f;

	float cosines = cos(rad);
	float sine = sin(rad);

	scalar x = this->dot(Vector3(cosines, sine, 0));
	scalar y = this->dot(Vector3(-sine, cosines, 0));

	this->set(x, y, 0);
}

void Vector3::clamp(clamping clamping)
{
	if (clamping == Vector3::clamping::MAX)
	{
		float max = vector[0];
		max = abs(max)>vector[1] ? max : vector[1];
		max = abs(max)>vector[2] ? max : vector[2];

		vector[0] = vector[0] == max ? max : 0;
		vector[1] = vector[1] == max ? max : 0;
		vector[2] = vector[2] == max ? max : 0;
	}
	else
	{
		float min = vector[0];
		min = abs(min)<vector[1] ? min : vector[1];
		min = abs(min)<vector[2] ? min : vector[2];

		vector[0] = vector[0] == min ? min : 0;
		vector[1] = vector[1] == min ? min : 0;
		vector[2] = vector[2] == min ? min : 0;
	}
}

//OPERATORS
Vector3 operator+(const Vector3 &first, const Vector3& second)
{
	Vector3 result(first);
	return result += second;
};

Vector3 operator-(const Vector3 &first, const Vector3& second)
{
	Vector3 result(first);
	return result -= second;
};

Vector3 operator*(const Vector3 &vector, const scalar k)
{
	return Vector3(vector.getX() * k, vector.getY()* k, vector.getZ() * k);
};

Vector3 operator/(const Vector3 &vector, const scalar k)
{
	return Vector3(vector.getX() / k, vector.getY()/ k, vector.getZ()/ k);
};


Vector3 Vector3::operator -() const
{
	return Vector3(-this->getX(), -this->getY(), -this->getZ());
}

Vector3& Vector3::operator =(const Vector3 &other)
{
	if (this != &other)
	{
		vector[0] = other.getX();
		vector[1] = other.getY();
		vector[2] = other.getZ();
	}
	return *this;
}

Vector3& Vector3::operator += (const Vector3& other)
{
	vector[0] += other.getX();
	vector[1] += other.getY();
	vector[2] += other.getZ();
	
	return *this;
}

Vector3& Vector3::operator -= (const Vector3& other)
{
	vector[0] -= other.getX();
	vector[1] -= other.getY();
	vector[2] -= other.getZ();
	
	return *this;
}

Vector3& Vector3::operator *= (const scalar k)
{
	vector[0] *= k;
	vector[1] *= k;
	vector[2] *= k;
	
	return *this;
}

Vector3& Vector3::operator /= (const scalar k)
{
	vector[0] /= k;
	vector[1] /= k;
	vector[2] /= k;
	
	return *this;
}

bool Vector3::operator == (Vector3 const &other) const
{
	return	vector[0] == other.getX() &&
			vector[1] == other.getY() &&
			vector[2] == other.getZ();
}

bool Vector3::operator != (Vector3 const &other) const
{
	return	vector[0] != other.getX() ||
			vector[1] != other.getY() ||
			vector[2] != other.getZ();
}

scalar& Vector3::operator[](int index)
{
	return vector[index];
};
const scalar& Vector3::operator[](int index) const
{
	return vector[index];
};