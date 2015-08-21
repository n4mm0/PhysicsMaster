#include "RigidBody.h"
#include "Utilities.h"
#include "Transform.h"
#include "GameObject.h"
#include "World.h"
#include <iostream>

RigidBody::RigidBody(const Vector3& _Inertia, float _Mass, int _ID, float _Bounciness, float _CompenetrationCorrection, 
			float _StaticFriction, float _DynamicFriction, float _SurfaceArea,float _DragCoefficient, bool _Dynamic)
			:m_Inertia(_Inertia), m_Mass(_Mass), m_ID(_ID), m_Bounciness(_Bounciness), m_CompenetrationCorrection(_CompenetrationCorrection),
			m_StaticFriction(_StaticFriction), m_DynamicFriction(_DynamicFriction), m_Area(_SurfaceArea),m_Drag(_DragCoefficient), m_IsDynamic(_Dynamic)
{

	m_InverseMass = m_Mass;
	if (m_Mass != 0)
	{
		m_InverseMass = 1.0f / m_Mass;
	}

	m_InverseInertia = Vector3::Zero;
	if (m_Inertia != Vector3::Zero)
	{
		m_InverseInertia[0] = 1.0f / m_Inertia[0];
		m_InverseInertia[1] = 1.0f / m_Inertia[1];
		m_InverseInertia[2] = 1.0f / m_Inertia[2];
	}
	m_Velocity = Vector3::Zero;
	m_AngularVelocity = Vector3::Zero;
	m_QuantityOfMotion = Vector3::Zero;
	m_AngularMomentum = Vector3::Zero;
	m_ForceSum = Vector3::Zero;
	m_MomentumSum = Vector3::Zero;

}

RigidBody::~RigidBody()
{
}

void RigidBody::UpdatePhysic()
{
	if (m_IsDynamic)
	{
		m_QuantityOfMotion = m_ForceSum * Constants::PhysicsDeltaTime;
		
		m_Velocity += m_QuantityOfMotion * m_InverseMass;
		m_Velocity += Constants::GravityForce *  Constants::PhysicsDeltaTime;

		m_AngularMomentum += m_MomentumSum *  Constants::PhysicsDeltaTime;
	
		QuaternionRotateT(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularMomentum, m_AngularVelocity);
		m_AngularVelocity *= m_InverseInertia;
		Vector3 AirDrag = m_Velocity * 0.5f * m_Area * m_Drag * Constants::AirDrag * m_Velocity.magnitude();
		AirDrag *= m_InverseMass; 
		AirDrag *= Constants::PhysicsDeltaTime;
		m_Velocity -= AirDrag;
		
		m_ForceSum = Vector3::Zero;
		m_MomentumSum = Vector3::Zero;
	}
}
void RigidBody::UpdatePosition()
{
	if (m_IsDynamic)
	{
		EditOwner()->EditChild<Transform>()->EditPosition() += m_Velocity* Constants::PhysicsDeltaTime;
		Quaternion RotQuat(1, m_AngularVelocity[0] * Constants::PhysicsDeltaTime * 0.5f, m_AngularVelocity[1] * Constants::PhysicsDeltaTime * 0.5f, m_AngularVelocity[2] * Constants::PhysicsDeltaTime * 0.5f);
		QuaternionRotate(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularVelocity, m_AngularMomentum);
		RotQuat.normalize();
		if (!RotQuat.isZero())
		{
			EditOwner()->EditChild<Transform>()->EditRotation() *= RotQuat;
			EditOwner()->EditChild<Transform>()->EditRotation().normalize();
		}
		else
		{
			m_AngularMomentum = Vector3::Zero;
		}
	}
}

void RigidBody::AddAngularVelocity(const Vector3& AngularVelocity)
{
	if (m_IsDynamic)
		m_AngularVelocity += AngularVelocity;
}

Vector3 RigidBody::GetInertia() const
{
	return m_Inertia;
}

Vector3 RigidBody::GetInverseInertia() const
{
	return m_InverseInertia;
}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{
	if (m_IsDynamic)
	{
		m_ForceSum += _Force;
		m_MomentumSum += _PointOfApplication.cross(_Force);
	}
}

void RigidBody::ApplyGravity(const Vector3& _Gravity)
{
	m_ForceSum += _Gravity*m_Mass;
}

int RigidBody::GetID() const
{
	return m_ID;
}

float RigidBody::GetMass() const
{
	return m_Mass;
}

float RigidBody::GetInverseMass() const
{
	return m_InverseMass;
}

Vector3 RigidBody::GetVelocity() const
{
	return m_Velocity;
}

void RigidBody::SetVelocity(const Vector3& _NewVelocity)
{
	m_Velocity = _NewVelocity;
}

Vector3 RigidBody::GetAngularVelocity() const
{
	return m_AngularVelocity;
}

void RigidBody::AddVelocity(const Vector3& Velocity)
{
	if (m_IsDynamic)
		m_Velocity += Velocity;
}

float RigidBody::GetBounciness() const
{
	return m_Bounciness;
}

float RigidBody::GetCompenetrationCorrection() const
{
	return m_CompenetrationCorrection;
}

float RigidBody::GetStaticFriction() const
{
	return m_StaticFriction;
}

float RigidBody::GetDynamicFriction() const
{
	return m_DynamicFriction;
}