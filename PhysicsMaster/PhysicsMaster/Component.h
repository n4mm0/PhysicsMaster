#pragma once
class GameObject;

class Component abstract
{
public:
	void SetOwner(GameObject* Owner)
	{
		m_oOwner = Owner;
	};
	const GameObject* GetOwner() const
	{
		return m_oOwner;
	};
	GameObject* EditOwner() 
	{
		return m_oOwner;
	};
private:
	GameObject* m_oOwner;
};
