#pragma once
#include "Singleton.h"
class GameObject;
class Vector3;

class PhysicsObjectFactory : public Singleton<PhysicsObjectFactory>
{
	IS_SINGLETON(PhysicsObjectFactory);

public:
	static void CreatePhysicsCube(GameObject& GameObj, const Vector3& Position, float RotationX, float RotationY, float RotationZ, float Mass, float Bounciness, float CompenetrationCorrection,
		float StaticFriction, float DynamicFriction, const Vector3& HalfSize);

	static void CreatePhysicsSphere(GameObject& GameObj, const Vector3& Position, float RotationX, float RotationY, float RotationZ, float Mass, float Bounciness, float CompenetrationCorrection,
		float StaticFriction, float DynamicFriction, float Radius);

	static void CreatePhysicsPlane(GameObject& GameObj, const Vector3& Position, const Vector3& Normal);
private:
	static int m_ID;
};