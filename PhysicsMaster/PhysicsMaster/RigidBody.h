#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Component.h"

class Collider;
class RigidBody : public Component
{

public:
	RigidBody(const Vector3& Inertia, float Mass, int ID, float Bounciness, float CompenetrationCorrection,
			float StaticFriction, float DynamicFriction, float SurfaceArea, float DragCoefficient, bool Dynamic = true);
	~RigidBody();
	void UpdatePhysic();
	void UpdatePosition();
	void ApplyForce(const Vector3& Force, const Vector3& PointOfApplication);
	void ApplyGravity(const Vector3& Gravity);

	// GETTERS
	int GetID() const;
	float GetMass() const;
	float GetInverseMass() const;
	float GetBounciness() const;
	float GetCompenetrationCorrection() const;
	float GetStaticFriction() const; 
	float GetDynamicFriction() const;
	Vector3 GetVelocity() const;
	Vector3 GetAngularVelocity() const;
	Vector3 GetInertia() const;
	Vector3 GetInverseInertia() const;
	
	void SetPosition(const Vector3& _NewPosition);
	void SetVelocity(const Vector3& _NewVelocity);
	void AddVelocity(const Vector3& Velocity);
	void AddAngularVelocity(const Vector3& AngularVelocity);
	
private:

	int		m_ID;
	float	m_Mass;
	float	m_InverseMass;
	Vector3	m_Velocity;
	Vector3	m_Inertia;
	Vector3 m_InverseInertia;
	Vector3	m_AngularVelocity;
	Vector3	m_QuantityOfMotion;
	Vector3	m_AngularMomentum;
	Vector3	m_ForceSum;
	Vector3	m_MomentumSum;
	float	m_Bounciness;
	float	m_CompenetrationCorrection;
	float	m_StaticFriction;
	float	m_DynamicFriction;
	float	m_Drag;
	float	m_Area;
	bool	m_IsDynamic;

	RigidBody(const RigidBody& _Other);	
	RigidBody& operator=(const RigidBody& _Other);
};
