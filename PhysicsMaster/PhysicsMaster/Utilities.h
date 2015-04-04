#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <math.h>

Quaternion EulerToQuaternion(float _RotX, float _RotY, float _RotZ)
{
	/* More info at:
	** http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/
	*/
	float c1 = cos(_RotY / 2);
	float s1 = sin(_RotY / 2);
	float c2 = cos(_RotZ / 2);
	float s2 = sin(_RotZ / 2);
	float c3 = cos(_RotX / 2);
	float s3 = sin(_RotX / 2);
	float c1c2 = c1*c2;
	float s1s2 = s1*s2;

	Quaternion o_Result((c1c2*c3 - s1s2*s3), (c1c2*s3 + s1s2*c3), (s1*c2*c3 + c1*s2*s3), (c1*s2*c3 - s1*c2*s3));
	return o_Result;
}

Vector3 CuboidInertia(float mass, float width, float height, float depth)
{
	/* More info at:
	** http://en.wikipedia.org/wiki/List_of_moments_of_inertia
	*/
	float x = mass*(height*height + depth*depth) / 12;
	float y = mass*(width*width + depth*depth) / 12;
	float z = mass*(height*height + width*width) / 12;
	return Vector3(x, y, z);
}

Vector3 SphereInertia(float mass, float radius)
{
	/* More info at:
	** http://en.wikipedia.org/wiki/List_of_moments_of_inertia
	*/
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