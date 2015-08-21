#include "World.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Collision\CollisionAlgorithms.h"
#include "DispatcherInit.h"

World::World()
{
	//Automatic initialization of Double dispatch 
	typedef AutomaticInsert<ColliderDispatcher, CollidersType, CollidersType, CollisionAlgorithm::CollisionDetectionAlgorithm> DispatcherInitializer;
	DispatcherInitializer::Init(m_Dispatcher);

	CollisionList::Init();

}

//Free all rigidbodies when destroyed
World::~World()
{
	m_RigidBodies.clear();
}

//Update Physic World
void World::Update()
{
	RigidBodyCollection::iterator iter;
	RigidBodyCollection::iterator end = m_RigidBodies.end();
	for (iter = m_RigidBodies.begin(); iter != end; ++iter)
	{
		(*iter)->UpdatePhysic();
	}

	//Collision Detect N vs N
	bool MaxCollisionReached = CollisionList::GetSingleton().MaxCollisionNumberReached();

	for (iter = m_RigidBodies.begin(); iter != end - 1&&!MaxCollisionReached; ++iter)
	{
		for (RigidBodyCollection::iterator second = iter + 1; second != end&&!MaxCollisionReached; ++second)
		{
			//THIS LEAD TO CRASH IFF GAMEOBJ DOESN'T HAVE A COLLIDER 
			m_Dispatcher.Dispatch(*((*iter)->EditOwner()->EditChild<Collider>()), *((*second)->EditOwner()->EditChild<Collider>()));
			MaxCollisionReached = CollisionList::GetSingleton().MaxCollisionNumberReached();
			
		}
	}
	
	//Collision Responce	
	CollisionList::GetSingleton().HandleCollision();
	for (iter = m_RigidBodies.begin(); iter != end; ++iter)
	{
		(*iter)->UpdatePosition();
	}
}

void World::AddRigidBody(RigidBody* body)
{
	m_RigidBodies.push_back(body);
};