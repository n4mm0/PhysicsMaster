#include "GameObject.h"

int GameObject::m_iID = 0;


GameObject::GameObject()
{
	++m_iID;
	AddChild<Transform>(*(new Transform(Vector3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f))));
}

GameObject::~GameObject()
{
	for (int i = 0; i < Components::ComponentListLength::value; ++i)
	{
		delete m_vpChilds[i];
	}
}