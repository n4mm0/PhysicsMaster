#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include <math.h>

/*
All functions should be inline, see: http://stackoverflow.com/questions/6424911/link-error-with-really-simple-functions-c-on-h-file
*/


inline Quaternion EulerToQuaternion(float _RotX, float _RotY, float _RotZ)
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

inline Vector3 CuboidInertia(float mass, float width, float height, float depth)
{
	/* More info at:
	** http://en.wikipedia.org/wiki/List_of_moments_of_inertia
	*/
	float x = mass*(height*height + depth*depth) / 12;
	float y = mass*(width*width + depth*depth) / 12;
	float z = mass*(height*height + width*width) / 12;
	return Vector3(x, y, z);
}

inline Vector3 SphereInertia(float mass, float radius)
{
	/* More info at:
	** http://en.wikipedia.org/wiki/List_of_moments_of_inertia
	*/
	float tmp = 2 * mass*radius*radius / 5;
	return Vector3(tmp, tmp, tmp);
}