#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include "Quaternion.h"

Vector3 CuboidInertia(float mass, float width, float height, float depth)
{
	float x = mass*(height*height + depth*depth) / 12;
	float y = mass*(width*width + depth*depth) / 12;
	float z = mass*(height*height + width*width) / 12;
	return Vector3(x, y, z);
}
Vector3 SphereInertia(float mass, float radius)
{
	float tmp = 2 * mass*radius*radius / 5;
	return Vector3(tmp, tmp, tmp);
}

void Print(const Vector3& v)
{
	std::cout << "(" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ")" << std::endl;
}

void Print(const Quaternion& v)
{
	std::cout << "(" << v.GetValue(0) << ", " << v.GetValue(1) << ", " << v.GetValue(2) << ", " << v.GetValue(3) << ")" << std::endl;
}

void Print(const Matrix<3, 3>& m)
{
	std::cout << m.GetElementAt(0) << ", " << m.GetElementAt(1) << ", " << m.GetElementAt(2) << std::endl;
	std::cout << m.GetElementAt(3) << ", " << m.GetElementAt(4) << ", " << m.GetElementAt(5) << std::endl;
	std::cout << m.GetElementAt(6) << ", " << m.GetElementAt(7) << ", " << m.GetElementAt(8) << std::endl;
}