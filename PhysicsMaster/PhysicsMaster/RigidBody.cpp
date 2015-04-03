#include "RigidBody.h"

RigidBody::RigidBody(const Vector3& _Position, float _Mass, int _ID) : m_Position(_Position), m_Mass(_Mass), m_ID(_ID)
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
	Vector3 Temp(m_ForceSum * _Dt);
	m_QuantityOfMotion += Temp;
	Temp = m_MomentumSum * _Dt;
	m_AngularMomentum += Temp;

	m_Velocity = m_QuantityOfMotion / m_Mass;
	Temp = m_Velocity * _Dt;
	m_Position += Temp;

	float RotVec[3];
	MatrixOp::RotateRelative(m_RotationMatrix, m_AngularMomentum.GetData(), RotVec);
	m_AngularVelocity.SetX(RotVec[0] / m_Inertia.getX());
	m_AngularVelocity.SetY(RotVec[1] / m_Inertia.getY());
	m_AngularVelocity.SetZ(RotVec[2] / m_Inertia.getZ());

	Quaternion RotQuat(1, m_AngularVelocity.getX() * _Dt / 2, m_AngularVelocity.getY() * _Dt / 2, m_AngularVelocity.getZ() * _Dt / 2);
	Normalize(RotQuat);
	m_Rotation *= RotQuat;
	Normalize(m_Rotation);

	MatrixOp::RotateAbsolute(m_RotationMatrix, m_AngularVelocity.GetData(), RotVec);
	m_AngularVelocity.SetX(RotVec[0]);
	m_AngularVelocity.SetY(RotVec[1]);
	m_AngularVelocity.SetZ(RotVec[2]);

	m_RotationMatrix = m_Rotation.ToMatrix();
}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{
	m_ForceSum += _Force;
	Vector3 Temp(_PointOfApplication - GetPosition());
	VectorialProduct(Temp, _Force, Temp);
	m_MomentumSum += Temp;
}

int RigidBody::GetID() const
{
	return m_ID;
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