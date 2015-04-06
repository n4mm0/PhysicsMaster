#include "World.h"

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

	//Reset Forza e Momento risultanti (di tutti i rigidbody?)
	//Applica gravità
	//Calcola e applica collisioni
	//IntegraStato di tutti i rigidbody
	/*
	RigidBodyCollection::iterator iter;
	for (iter == m_RigidBodies.begin(); iter != m_RigidBodies.end(); ++iter)
	{
		(*iter)->UpdatePhysic(DT);
	}
	*/
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