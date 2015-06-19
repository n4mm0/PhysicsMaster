#pragma once
#include <vector>
#include "Utilities\TypeList.h"
#include "ComponentList.h"
#include "Transform.h"

class Component;


class GameObject
{
public:
	template<typename T> 
	const T* GetChild() const
	{
		return static_cast<T*>(m_vpChilds[TL::IndexOf<Components::ComponentList,T>::value]);
	};

	template<typename T>
	T* EditChild() 
	{
		return static_cast<T*>(m_vpChilds[TL::IndexOf<Components::ComponentList, T>::value]);
	};

	template<typename T>
	void AddChild(T& Child)
	{
		m_vpChilds[TL::IndexOf<Components::ComponentList, T>::value] = &Child;
		Child.SetOwner(this);
	};

	GameObject()
	{
		++m_iID;
		AddChild<Transform>( Transform(Vector3::Zero, Quaternion(1,0,0,0) ) );
	};

private:
	static int m_iID;
	GameObject(const GameObject& other);
	GameObject& operator=(const GameObject& other);
	Component* m_vpChilds[Components::ComponentListLength::value];
};