#pragma once
/*
** Oggetto base del mondo
*/

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Component.h"

class Collider;
class RigidBody : public Component
{

public:
	RigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID);
	~RigidBody();
	void UpdatePhysic(float _Dt);
	void ApplyForce(const Vector3& _Force, const Vector3& _PointOfApplication);
	void ApplyGravity(const Vector3& _Gravity);

	// GETTERS
	int GetID() const;
	float GetMass() const;
	Vector3 GetVelocity() const;
	
	//Moved to Tranform
	//Vector3 GetPosition() const;
	//const Matrix4x4& GetRotationMatrix() const;
	//const Quaternion& GetRotationQuaternion() const;
	//Collider* GetCollider() const;
	// SETTERS
	void SetPosition(const Vector3& _NewPosition);
	void SetVelocity(const Vector3& _NewVelocity);

private:
	int				m_ID;
	float			m_Mass;
	Vector3			m_Velocity;
	Vector3			m_Inertia;
	Vector3			m_AngularVelocity;
	Vector3			m_QuantityOfMotion;
	Vector3			m_AngularMomentum;
	Vector3			m_ForceSum;
	Vector3			m_MomentumSum;
	//Moved To Tranform
	/*
	Vector3			m_Position;
	Quaternion		m_Rotation;
	Matrix4x4		m_RotationMatrix;
	
	//Sibiling
	Collider*		m_Collider;
	*/
	RigidBody(const RigidBody& _Other);		// We don't need that atm
	RigidBody& operator=(const RigidBody& _Other);
};
/*
inline Collider* RigidBody::GetCollider() const
{
	return m_Collider;
};
*/