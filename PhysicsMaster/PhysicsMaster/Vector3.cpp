#include "Vector3.h"
// FOR DEBUG
#include <iostream>

const Vector3 Vector3::Zero = Vector3(0, 0, 0);

Vector3::Vector3()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector3::Vector3(const Vector3& other)
{
	v[0] = other.GetX();
	v[1] = other.GetY();
	v[2] = other.GetZ();
}

inline
float Vector3::GetX() const
{
	return v[0];
}

inline
float Vector3::GetY() const
{
	return v[1];
}

inline
float Vector3::GetZ() const
{
	return v[2];
}

void Vector3::SetX(float x)
{
	v[0] = x;
}

void Vector3::SetY(float y)
{
	v[1] = y;
}

void Vector3::SetZ(float z)
{
	v[2] = z;
}

void Vector3::Set(float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector3& Vector3::operator=(const Vector3& other)
{
	if (this != &other)
	{
		v[0] = other.GetX();
		v[1] = other.GetY();
		v[2] = other.GetZ();
	}
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	v[0] += other.GetX();
	v[1] += other.GetY();
	v[2] += other.GetZ();
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	v[0] -= other.GetX();
	v[1] -= other.GetY();
	v[2] -= other.GetZ();
	return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
	v[0] /= scalar;
	v[1] /= scalar;
	v[2] /= scalar;
	return *this;
}

float Vector3::Modulus() const
{
	return(sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2])));
};

float Vector3::SqrMagnitude() const
{
	return (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
};

void Vector3::Normalize()
{
	float mod = Modulus();
	if (mod > 0.000001f) {
		*this /= mod;
	}
}

Vector3 operator+(const Vector3& first, const Vector3& second)
{
	Vector3 result(first);
	return result += second;
}

Vector3 operator-(const Vector3& first, const Vector3& second)
{
	Vector3 result(first);
	return result -= second;
}

Vector3 operator*(const Vector3& vector, float scalar)
{
	return Vector3(vector.GetX() * scalar, vector.GetY()* scalar, vector.GetZ() * scalar);
}

Vector3 operator/(const Vector3& vector, float scalar)
{
	return Vector3(vector.GetX() / scalar, vector.GetY() / scalar, vector.GetZ() / scalar);
}


namespace VectorOp
{
	void VectorialProduct(const Vector3& first, const Vector3& second, Vector3& result)
	{
		//float X = (p3[1] * s3[2]) - (p3[2] * s3[1]);
		//float Y = (p3[2] * s3[0]) - (p3[0] * s3[2]);
		//float Z = (p3[0] * s3[1]) - (p3[1] * s3[0]);
		float X = (first.GetY() * second.GetZ()) - (first.GetZ() * second.GetY());
		float Y = (first.GetZ() * second.GetX()) - (first.GetX() * second.GetZ());
		float Z = (first.GetX() * second.GetY()) - (first.GetY() * second.GetX());
		result.SetX(X);
		result.SetY(Y);
		result.SetZ(Z);
	}

	float DotProduct(const Vector3& first, const Vector3& second)
	{
		return((first.GetX() * second.GetX()) + (first.GetY() * second.GetY()) + (first.GetZ() * second.GetZ()));
	}

	float DistanceBetween(const Vector3& first, const Vector3& second)
	{
		return sqrt(pow(first.GetX() + second.GetX(), 2) + pow(first.GetY() + second.GetY(), 2) + pow(first.GetZ() + second.GetZ(), 2));
	};
}