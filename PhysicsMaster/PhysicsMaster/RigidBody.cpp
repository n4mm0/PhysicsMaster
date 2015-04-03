#include "RigidBody.h"

RigidBody::RigidBody(const Vector3& _Position, float _Mass) : m_Position(_Position), m_Mass(_Mass)
{
	m_Velocity = Vector3::zeroes();
	m_Inertia = Vector3::zeroes();
	m_AngularVelocity = Vector3::zeroes();
	m_QuantityOfMotion = Vector3::zeroes();
	m_AngularMomentum = Vector3::zeroes();
	m_ForceSum = Vector3::zeroes();
	m_MomentumSum = Vector3::zeroes();
}

RigidBody::~RigidBody(){}

void RigidBody::UpdatePhysic(float _Dt)
{

}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{

}

float RigidBody::GetMass() const
{
	return m_Mass;
}

Vector3 RigidBody::GetPosition() const
{
	return m_Position;
}

Vector3 RigidBody::GetVelocity() const
{
	return m_Velocity;
}

void RigidBody::SetPosition(const Vector3& _NewPosition)
{
	m_Position = _NewPosition;
}

void RigidBody::SetVelocity(const Vector3& _NewVelocity)
{
	m_Velocity = _NewVelocity;
}