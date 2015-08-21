#include "GameObjectFactory.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "Utilities\Utilities.h"
#include "Colliders\BoxCollider.h"
#include "Colliders\PlaneCollider.h"
#include "Colliders\SphereCollider.h"
#include "Constants.h"

int PhysicsObjectFactory::m_ID = 0;

void PhysicsObjectFactory::CreatePhysicsCube(GameObject& GameObj, const Vector3& Position,float RotationX,float RotationY,float RotationZ, float Mass, float Bounciness, float CompenetrationCorrection,
	float StaticFriction, float DynamicFriction, const Vector3& HalfSize) 
{
	BoxCollider* Box = new BoxCollider(Vector3(0, 0, 0), HalfSize);
	RigidBody* Body = new RigidBody(CuboidInertia(Mass, HalfSize[0], HalfSize[1], HalfSize[2]), Mass, m_ID, Bounciness, CompenetrationCorrection,
		StaticFriction, DynamicFriction, (HalfSize[0] * HalfSize[0] * 2.0f), Constants::CubeDragCoefficient);
	++m_ID;
	GameObj.AddChild<RigidBody>(*Body);
	GameObj.AddChild<Collider>(*Box);
	GameObj.EditChild<Transform>()->EditPosition() = Position;
	GameObj.EditChild<Transform>()->EditRotation() = EulerToQuaternion(RotationX, RotationY, RotationZ);
}

void PhysicsObjectFactory::CreatePhysicsSphere(GameObject& GameObj, const Vector3& Position, float RotationX, float RotationY, float RotationZ, float Mass, float Bounciness, float CompenetrationCorrection,
	float StaticFriction, float DynamicFriction, float Radius)
{
	SphereCollider* Sphere = new SphereCollider(Vector3(0, 0, 0), Radius);
	RigidBody* Body = new RigidBody(SphereInertia(Mass, Radius), Mass, m_ID, Bounciness, CompenetrationCorrection,
		StaticFriction, DynamicFriction, (Radius * Radius * 2.0f * 3.1415f ), Constants::SphereDragCoefficient);
	++m_ID;
	GameObj.AddChild<RigidBody>(*Body);
	GameObj.AddChild<Collider>(*Sphere);
	GameObj.EditChild<Transform>()->EditPosition() = Position;
	GameObj.EditChild<Transform>()->EditRotation() = EulerToQuaternion(RotationX, RotationY, RotationZ);;

}

void PhysicsObjectFactory::CreatePhysicsPlane(GameObject& GameObj, const Vector3& Position, const Vector3& Normal)
{
	PlaneCollider* Plane = new PlaneCollider(Vector3(0, 0, 0), Normal);
	RigidBody* Body = new RigidBody(Vector3::Zero, 0.0f, m_ID, 0.3f, 100.0f, 0.6f, 0.3f, 0.0f, 0.0f,false);
	++m_ID;
	GameObj.AddChild<RigidBody>(*Body);
	GameObj.AddChild<Collider>(*Plane);
	GameObj.EditChild<Transform>()->EditPosition() = Position;
}