#pragma once
#include <vector>
#include "Collision.h"
#include "Singleton.h"
#include <assert.h>

template<int MaxCollision>
class CollisionHandler :public Singleton< CollisionHandler<MaxCollision> >
{
	IS_SINGLETON(CollisionHandler);
public:
	void HandleCollision() 
	{

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

	~CollisionHandler()
	{
		delete[] m_aoCollisionList;
	}

private:

	Collision m_aoCollisionList[ MaxCollision ];
	int m_iCollisionDetected = 0;
};