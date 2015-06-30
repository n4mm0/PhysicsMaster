#pragma once
#include <vector>
#include "Collision.h"
#include "Singleton.h"
#include <assert.h>
//debug
#include <iostream>
template<int MaxCollision>
class CollisionHandler :public Singleton< CollisionHandler<MaxCollision> >
{
	IS_SINGLETON(CollisionHandler);
public:
	void HandleCollision() 
	{
		/*
		STD VECTOR
		int i = 0;
		for (Iterator collision = _CollisionList.begin(); collision != _CollisionList.end(); ++collision,++i)
		{
			(*collision)->ApplyCollision();
		}
		std::cout << "Total Collision Handled: " << i << std::endl;
		Clear();
		*/
		for (int i = 0; i < m_iCollisionDetected; ++i)
		{
			m_aoCollisionList[i].ApplyCollision();
		}
		m_iCollisionDetected = 0;
	}

	Collision& EditCollision()
	{
		assert(m_iCollisionDetected < MaxCollision && "CRASH! FIX WORLD TOO MANY COLLISION ");
		return m_aoCollisionList[m_iCollisionDetected++];
	}
	
	int& EditCollisionDetected()
	{
		return m_iCollisionDetected;
	}
	
	bool MaxCollisionNumberReached()
	{
		return m_iCollisionDetected == MaxCollision;
	}

	bool AddCollision(Collision& Collision, int CollisionResult, RigidBody* first, RigidBody* second)
	{
		if (m_iCollisionDetected < MaxCollision)
		{
			m_iCollisionDetected += CollisionResult;
			Collision.SetBodies(first, second);
		}

		return m_iCollisionDetected == MaxCollision;
	}
	/*		STD VECTOR
	bool AddCollision(Collision* col,RigidBody* first,RigidBody* second)
	{

		if (_CollisionList.size() < MaxCollision && col != nullptr)
		{
			col->SetBodies(first,second);
			_CollisionList.push_back(col);
		}
		return _CollisionList.size() == MaxCollision;
	}
	*/

	/*
	STD VECTOR
	void Clear()
	{
		for (Iterator collision = _CollisionList.begin(); collision != _CollisionList.end(); ++collision)
		{
			delete *collision;
		}
		_CollisionList.clear();
	}*/
	
private:
	//CollisionHandler();

	//CollisionHandler(const CollisionHandler& other);
	//CollisionHandler& operator=(const CollisionHandler& other);
	Collision m_aoCollisionList[ MaxCollision ];
	int m_iCollisionDetected = 0;
};