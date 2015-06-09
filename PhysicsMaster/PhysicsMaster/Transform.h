#pragma once
#include "Utilities\Quaternion.h"
#include "Utilities\Vector3.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform(const Vector3& Pos, const Quaternion& Rot);
	const Quaternion& GetRotation() const;
	const Vector3& GetPosition() const;
	const Matrix4x4 GetRotationMatrix() const;
	Quaternion& EditRotation();
	Vector3& EditPosition();
private:
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);

	Quaternion m_qRotation;
	Vector3 m_v3Position;
};

inline const Quaternion& Transform::GetRotation() const
{
	return m_qRotation;
};

inline const Vector3& Transform::GetPosition() const
{
	return m_v3Position;
};

inline Quaternion& Transform::EditRotation()
{
	return m_qRotation;
};

inline Vector3& Transform::EditPosition()
{
	return m_v3Position;
};

inline const Matrix4x4 Transform::GetRotationMatrix() const
{
	return m_qRotation.toMatrix();
};

