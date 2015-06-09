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
	T* GetChild() const
	{
		return static_cast<T*>(m_vpChilds[TL::IndexOf<Components::ComponentList,T>::value]);
	};

	template<typename T>
	void AddChild(T& Child)
	{
		m_vpChilds[TL::IndexOf<Components::ComponentList, T>::value] = &Child;
		Child.SetOwner(this);
	};

	GameObject()
	{
		m_vpChilds.reserve(Components::ComponentListLength::value);
		AddChild<Transform>( Transform(Vector3::Zero, Quaternion(0,0,0,0) ) );
	};
private:
	GameObject(const GameObject& other);
	GameObject& operator=(const GameObject& other);
	std::vector<Component*> m_vpChilds;
};