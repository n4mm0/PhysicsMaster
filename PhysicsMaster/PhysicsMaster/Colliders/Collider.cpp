#include "Collider.h"
#include <iostream>
Collider::Collider(int type, const Vector3& displ) : m_Type(type), m_Displacement(displ)
{
};

const Vector3 Collider::GetWorldPosition() const
{
	return *(m_Position)+m_Displacement;
}

int Collider::GetType() const
{
	return m_Type;
};
void Collider::SetPosition(const Vector3& Pos)
{
	m_Position = &Pos;
}