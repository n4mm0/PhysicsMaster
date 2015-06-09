#pragma once
#include "Vector3.h"

class Collider abstract
{
public:
	int GetType() const;
	const Vector3 GetWorldPosition() const;
protected:
	void SetPosition(const Vector3& Pos);
	Collider(int type, const Vector3& displ);
private:
	int m_Type;
	Vector3 m_Displacement;
	const Vector3* m_Position;
};