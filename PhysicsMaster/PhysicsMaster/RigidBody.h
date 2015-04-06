#pragma once
/*
** Oggetto base del mondo
*/
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"

class RigidBody {

public:
	RigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID);
	~RigidBody();
	void UpdatePhysic(float _Dt);
	void ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication);

	void ShowStatus();

	// GETTERS
	int GetID() const;
	float GetMass() const;
	Vector3 GetPosition() const;
	Vector3 GetVelocity() const;
	const Matrix<3, 3>& GetRotationMatrix() const;
	const Quaternion& GetRotationQuaternion() const;
	// SETTERS
	void SetPosition(const Vector3& _NewPosition);
	void SetVelocity(const Vector3& _NewVelocity);

private:
	int				m_ID;
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

	RigidBody(const RigidBody& _Other);		// We don't need that atm
};