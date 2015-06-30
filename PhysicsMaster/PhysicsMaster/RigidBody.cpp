#include "RigidBody.h"
#include "Utilities.h"
#include "Transform.h"
#include "GameObject.h"
#include <iostream>

RigidBody::RigidBody(/*const Vector3& _Position, */const Vector3& _Inertia, float _Mass, int _ID, bool _Static) :/* m_Position(_Position), */m_Inertia(_Inertia), m_Mass(_Mass), m_ID(_ID), m_IsStatic(_Static)
{
	m_Velocity = Vector3::Zero;
	m_AngularVelocity = Vector3::Zero;
	m_QuantityOfMotion = Vector3::Zero;
	m_AngularMomentum = Vector3::Zero;
	m_ForceSum = Vector3::Zero;
	m_MomentumSum = Vector3::Zero;

	//m_RotationMatrix = m_Rotation.toMatrix();
}

RigidBody::~RigidBody()
{
}

void RigidBody::UpdatePhysic(float _Dt)
{
	if (m_IsStatic)
	{
	Vector3 Temp(m_ForceSum * _Dt);

	m_QuantityOfMotion += Temp;
	Temp = m_MomentumSum * _Dt;
	m_AngularMomentum += Temp;
	m_Velocity = m_QuantityOfMotion / m_Mass;
	Temp = m_Velocity * _Dt;
	
	//m_Position += Temp;
	EditOwner()->EditChild<Transform>()->EditPosition() += Temp;

	//Matrix4x4::RotateToObjectSpace(m_RotationMatrix, m_AngularMomentum, m_AngularVelocity);
	//QuaternionRotateT(m_Rotation, m_AngularMomentum, m_AngularVelocity);
	QuaternionRotateT(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularMomentum, m_AngularVelocity);

	m_AngularVelocity.setX(m_AngularVelocity.getX() / m_Inertia.getX());
	m_AngularVelocity.setY(m_AngularVelocity.getY() / m_Inertia.getY());
	m_AngularVelocity.setZ(m_AngularVelocity.getZ() / m_Inertia.getZ());
	Quaternion RotQuat(1, m_AngularVelocity.getX() * _Dt / 2, m_AngularVelocity.getY() * _Dt / 2, m_AngularVelocity.getZ() * _Dt / 2);
	
	RotQuat.normalize();

	//Matrix4x4::RotateToWorldSpace(m_RotationMatrix, m_AngularVelocity, m_AngularVelocity);
	//QuaternionRotate(m_Rotation, m_AngularVelocity, m_AngularVelocity);
	QuaternionRotate(GetOwner()->GetChild<Transform>()->GetRotation(), m_AngularVelocity, m_AngularVelocity);

	// m_Rotation *= RotQuat;
	EditOwner()->EditChild<Transform>()->EditRotation() *= RotQuat;
	
	//m_Rotation.normalize();
	EditOwner()->EditChild<Transform>()->EditRotation().normalize();

	//m_RotationMatrix = m_Rotation.toMatrix();
	m_ForceSum = Vector3::Zero;
	m_MomentumSum = Vector3::Zero;
	}
}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{
	if (m_IsStatic)
	{
		m_ForceSum += _Force;
		Vector3 Temp(_PointOfApplication - GetOwner()->GetChild<Transform>()->GetPosition());
		Temp = Temp.cross(_Force);
		m_MomentumSum += Temp;
	}
}

void RigidBody::ApplyGravity(const Vector3& _Gravity)
{
	m_ForceSum += _Gravity * m_Mass;
}

int RigidBody::GetID() const
{
	return m_ID;
}

float RigidBody::GetMass() const
{
	return m_Mass;
}
/*
Vector3 RigidBody::GetPosition() const
{
	return m_Position;
}*/

Vector3 RigidBody::GetVelocity() const
{
	return m_Velocity;
}
/*
const Matrix4x4& RigidBody::GetRotationMatrix() const
{
	return m_RotationMatrix;
};

const Quaternion& RigidBody::GetRotationQuaternion() const
{
	return m_Rotation;
};

void RigidBody::SetPosition(const Vector3& _NewPosition)
{
	m_Position = _NewPosition;
}
*/
void RigidBody::SetVelocity(const Vector3& _NewVelocity)
{
	m_Velocity = _NewVelocity;
}