#include "Collider.h"
#include <iostream>
#include "GameObject.h"
#include "Transform.h"

Collider::Collider(const Vector3& displ) : m_Displacement(displ)
{
};

const Vector3 Collider::GetWorldPosition() const
{
	return GetOwner()->GetChild<Transform>()->GetPosition()+m_Displacement;
}

int Collider::GetType() const
{
	return m_Type;
};

void Collider::SetType(int type)
{
	m_Type=type;
};
