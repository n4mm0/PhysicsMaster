#include "RigidBody.h"
#include "Utilities.h"
#include "Transform.h"
#include "GameObject.h"
#include "World.h"
#include <iostream>

RigidBody::RigidBody(/*const Vector3& _Position, */const Vector3& _Inertia, float _Mass, int _ID, bool _Static) :/* m_Position(_Position), */m_Inertia(_Inertia), m_Mass(_Mass), m_ID(_ID), m_IsStatic(_Static)
{
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

void RigidBody::UpdatePhysic(float _Dt)
{
	if (m_IsStatic)
	{
		//m_ForceSum += World::m_Gravity;
		m_QuantityOfMotion = m_ForceSum * _Dt;
		
		m_Velocity += m_QuantityOfMotion / m_Mass;
		m_Velocity += World::m_Gravity * _Dt;
		//std::cout << "Velocity: (" << m_Velocity[0] << ", " << m_Velocity[1] << ", " << m_Velocity[2] << ")" << std::endl;

		m_AngularMomentum += m_MomentumSum * _Dt;
	
		QuaternionRotateT(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularMomentum, m_AngularVelocity);
		m_AngularVelocity[0] /= m_Inertia[0];
		m_AngularVelocity[1] /= m_Inertia[1];
		m_AngularVelocity[2] /= m_Inertia[2];
	
		Vector3 inverseVelocity(m_Velocity*-1.0f);
		float drag;
		float area = 4.0f;
		float modVelocity = m_Velocity.magnitude();
	
		//drag cubo 
		//TO DO based on shape
		drag = 1.05f;

		//drag Sphere
		//drag = 0.47f;

		// F = 1/2 * area * drag * airD * v^2 
		inverseVelocity = inverseVelocity * 0.5f * area * drag * 1.0f * modVelocity;
		inverseVelocity /= m_Mass; // Accelerazione
		inverseVelocity *= _Dt; // Velocità

		m_Velocity += inverseVelocity;
		// ------ Fine calcolo attrito dell'aria
	//	std::cout << "v: (" << m_Velocity[0] << ", " << m_Velocity[1] << ", " << m_Velocity[2] << ")" << std::endl;
	//	EditOwner()->EditChild<Transform>()->EditPosition() += m_Velocity*_Dt;
		
		m_ForceSum = Vector3::Zero;
		m_MomentumSum = Vector3::Zero;
	}
}
void RigidBody::UpdatePosition(float _Dt)
{
	if (m_IsStatic)
	{

	//	std::cout << "oldPosition: (" << EditOwner()->EditChild<Transform>()->EditPosition()[0] << ", " << EditOwner()->EditChild<Transform>()->EditPosition()[1] << ", " << EditOwner()->EditChild<Transform>()->EditPosition()[2] << ")" << std::endl;
	//	std::cout << "Velocity: (" << m_Velocity[0] << ", " << m_Velocity[1] << ", " << m_Velocity[2] << ")" << std::endl;
		EditOwner()->EditChild<Transform>()->EditPosition() += m_Velocity*_Dt;
	//	std::cout << "newPosition: (" << EditOwner()->EditChild<Transform>()->EditPosition()[0] << ", " << EditOwner()->EditChild<Transform>()->EditPosition()[1] << ", " << EditOwner()->EditChild<Transform>()->EditPosition()[2] << ")" << std::endl;
		
		Quaternion RotQuat(1, m_AngularVelocity.getX() * _Dt / 2, m_AngularVelocity.getY() * _Dt / 2, m_AngularVelocity.getZ() * _Dt / 2);
		QuaternionRotate(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularVelocity, m_AngularMomentum);
		/*
		if (GetOwner()->GetChild<Collider>()->GetType() == BoxCollider::getType())
		{
			std::cout << "AVelocity: (" << m_AngularVelocity[0] << ", " << m_AngularVelocity[1] << ", " << m_AngularVelocity[2] << ")" << std::endl;
			std::cout << "RotQuat: (" << RotQuat[0] << ", " << RotQuat[1] << ", " << RotQuat[2] << ", " << RotQuat[3] << ")" << std::endl;
		}
		*/
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
	/*	if (GetOwner()->GetChild<Collider>()->GetType() == BoxCollider::getType())
		{
			std::cout << "Velocity: (" << m_Velocity[0] << ", " << m_Velocity[1] << ", " << m_Velocity[2] << ")" << std::endl;
			std::cout << "Quat: (" << EditOwner()->EditChild<Transform>()->EditRotation()[0] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[1] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[2] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[3] << ")" << std::endl;
			system("pause");
		}
	//	std::cout << "Quat: " << EditOwner()->EditChild<Transform>()->GetRotation()[0] << ", " << EditOwner()->EditChild<Transform>()->GetRotation()[1] << ", " << EditOwner()->EditChild<Transform>()->GetRotation()[2] << ", " << EditOwner()->EditChild<Transform>()->GetRotation()[3] << ")" << std::endl;
	//	system("pause");*/
	}
}

void RigidBody::AddAngularVelocity(const Vector3& AngularVelocity)
{
	if (GetOwner()->GetChild<Collider>()->GetType() == SphereCollider::getType())
	{
		std::cout << "Quaternion: (" << EditOwner()->EditChild<Transform>()->EditRotation()[0] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[1] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[2] << ", " << EditOwner()->EditChild<Transform>()->EditRotation()[3] << ")" << std::endl;

		std::cout << "AngularVelocityCol: (" << AngularVelocity[0] << ", " << AngularVelocity[1] << ", " << AngularVelocity[2] << ")" << std::endl;
		std::cout << "AngularMomentum: (" << m_AngularMomentum[0] << ", " << m_AngularMomentum[1] << ", " << m_AngularMomentum[2] << ")" << std::endl;
		std::cout << "AngularVelocity: (" << m_AngularVelocity[0] << ", " << m_AngularVelocity[1] << ", " << m_AngularVelocity[2] << ")" << std::endl;
		//system("pause");
	}
	if (m_IsStatic)
		m_AngularVelocity += AngularVelocity;
}

Vector3 RigidBody::GetInertia() const
{
	return m_Inertia;
}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{

	if (m_IsStatic)
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
//	std::cout << "V: (" << Velocity[0] << ", " << Velocity[1] << ", " << Velocity[2] << ")" << std::endl;
	if (m_IsStatic)
		m_Velocity += Velocity;
	//std::cout << "V: (" << m_Velocity[0] << ", " << m_Velocity[1] << ", " << m_Velocity[2] << ")" << std::endl;
}