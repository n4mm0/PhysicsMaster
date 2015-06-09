#include "World.h"
#include "RigidBody.h"
const Vector3 World::m_Gravity = Vector3(0.0f, -9.8f, 0.0f);
const float World::m_Dt = 0.01f;

World::World()
{
	//Initalization
}

//Free all rigidbodies when destroyed
World::~World()
{
	m_RigidBodies.clear();
}

//Update Physic World
void World::Update()
{
	//Riassunto in base alla funzione IntegraStato() di Cattani...

	//Reset Forza e Momento risultanti (di tutti i rigidbody?)	- CHECK
	//Applica gravità											- CHECK
	//Calcola e applica collisioni
	//IntegraStato di tutti i rigidbody							- CHECK

	RigidBodyCollection::iterator iter;
	RigidBodyCollection::iterator end = m_RigidBodies.end();
	for (iter = m_RigidBodies.begin(); iter != end; ++iter)
	{
		(*iter)->ApplyGravity(m_Gravity);
		(*iter)->UpdatePhysic(m_Dt);
	}
	bool MaxCollisionReached = false;
	for (iter = m_RigidBodies.begin(); iter != end - 1&&!MaxCollisionReached; ++iter)
	{
		for (RigidBodyCollection::iterator second = iter + 1; second != end&&!MaxCollisionReached; ++second)
		{
			MaxCollisionReached = m_CollisionCollection.AddCollision(m_Dispatcher.Dispatch(*((*iter)->GetCollider()), *((*second)->GetCollider())), (*iter), (*second));

		}
	}
}

RigidBody* World::CreateRigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID)
{
	RigidBody* newRB = new RigidBody(_Position, _Inertia, _Mass, _ID);
	m_RigidBodies.push_back(newRB);
	return newRB;
}

void World::DeleteRigidBody(RigidBody* _body)
{
	RigidBodyCollection::iterator iter;
	for (iter = m_RigidBodies.begin(); iter != m_RigidBodies.end(); )
	{
		if ((*iter) == _body)
		{
			iter = m_RigidBodies.erase(iter); //Remove the RigidBody from the collection
		}
		else
		{
			++iter;
			//Spiegazione: invece che aumentare l'iteratore nel for loop lo faccio qua
			//perché se ad esempio avessi cancellato l'ultimo elemento del vector,
			//iter diventerebbe uguale al .end() per via del funzionamento di .erase() e il loop in 
			//automatico mi farà ++iter. Ma visto che non è permesso incrementare l'iteratore .end(), CRASH!
		}
	}

	delete _body; //Destroy the body, regardless if it was inside the vector
}