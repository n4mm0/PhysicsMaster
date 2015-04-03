#pragma once
/*
** Oggetto base del mondo
*/
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"

class RigidBody {

public:
	RigidBody(const Vector3& _Position, float _Mass);

private:
	float			m_Mass;
	Vector3			m_Position;
	Vector3			m_Velocity;
	Vector3			m_Inertia;
	Vector3			m_AngularVelocity;
	Vector3			m_QuantityOfMotion;
	Vector3			m_AngularMomentum;
	Vector3			m_ForceSum;
	Vector3			m_MomentumSum;
	Quaternion		m_Rotation;
	Matrix<3, 3>	m_RotationMatrix;

};