#include "RigidBody.h"
#include "Utilities.h"
#include <iostream>


RigidBody::RigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID) : m_Position(_Position), m_Inertia(_Inertia), m_Mass(_Mass), m_ID(_ID)
{
	m_Velocity = Vector3::Zero;
	m_AngularVelocity = Vector3::Zero;
	m_QuantityOfMotion = Vector3::Zero;
	m_AngularMomentum = Vector3::Zero;
	m_ForceSum = Vector3::Zero;
	m_MomentumSum = Vector3::Zero;
	m_RotationMatrix = m_Rotation.ToMatrix();
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

	MatrixOp::RotateToObjectSpace(m_RotationMatrix, m_AngularMomentum, m_AngularVelocity);
	m_AngularVelocity.SetX(m_AngularVelocity.GetX() / m_Inertia.GetX());
	m_AngularVelocity.SetY(m_AngularVelocity.GetY() / m_Inertia.GetY());
	m_AngularVelocity.SetZ(m_AngularVelocity.GetZ() / m_Inertia.GetZ());

	Quaternion RotQuat(1, m_AngularVelocity.GetX() * _Dt / 2, m_AngularVelocity.GetY() * _Dt / 2, m_AngularVelocity.GetZ() * _Dt / 2);
	
	RotQuat.Normalize();
	m_Rotation *= RotQuat;
	m_Rotation.Normalize();

	MatrixOp::RotateToWorldSpace(m_RotationMatrix, m_AngularVelocity, m_AngularVelocity);

	m_RotationMatrix = m_Rotation.ToMatrix();
}

void RigidBody::ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication)
{
	m_ForceSum += _Force;
	Vector3 Temp(_PointOfApplication - GetPosition());
	VectorOp::VectorialProduct(Temp, _Force, Temp);
	m_MomentumSum += Temp;
}

void RigidBody::ShowStatus()
{
	std::cout << "----------------------------- \n" << std::endl;
	std::cout << "RigidBody: " << m_ID << std::endl;
	std::cout << "Mass: " << m_Mass << std::endl;
	std::cout << "Position: ";
	Print(m_Position);
	std::cout << "Velocity: ";
	Print(m_Velocity);
	std::cout << "Inertia: ";
	Print(m_Inertia);
	std::cout << "AngularVelocity: ";
	Print(m_AngularVelocity);
	std::cout << "QuantityOfMotion: ";
	Print(m_QuantityOfMotion);
	std::cout << "AngularMomentum: ";
	Print(m_AngularMomentum);
	std::cout << "ForceSum: ";
	Print(m_ForceSum);
	std::cout << "MomentumSum: ";
	Print(m_MomentumSum);
	std::cout << "Rotation: ";
	Print(m_Rotation);
	std::cout << "RotationMatrix: \n";
	Print(m_RotationMatrix);
	std::cout << "----------------------------- \n" << std::endl;
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

const Matrix<3, 3>& RigidBody::GetRotationMatrix() const
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

void RigidBody::SetVelocity(const Vector3& _NewVelocity)
{
	m_Velocity = _NewVelocity;
}