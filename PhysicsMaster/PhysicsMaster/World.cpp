#include "World.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Collision\CollisionAlgorithms.h"
#include "Madness.h"


const Vector3 World::m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
const float World::m_Dt = Constants::DeltaTime;

World::World()
{
	typedef AutomaticInsert<ColliderDispatcher, CollidersType, CollidersType, CollisionAlgorithm::CollisionDetectionAlgorithm> DispatcherInitializer;

	//Initalization 
	//Maybe not so Singleton need test
	CollisionList::Init();

	//Automatic initialization of Double dispatch 
	DispatcherInitializer::Init(m_Dispatcher);
	
	//NO MORE HUE HUE
	/*m_Dispatcher.Add<BoxCollider, BoxCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<BoxCollider, BoxCollider>::Fire);
	m_Dispatcher.Add<BoxCollider, SphereCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<BoxCollider, SphereCollider>::Fire<BoxCollider, SphereCollider>);
	m_Dispatcher.Add<BoxCollider, PlaneCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<BoxCollider, PlaneCollider>::Fire<BoxCollider, PlaneCollider>);

	m_Dispatcher.Add<SphereCollider, SphereCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<SphereCollider, SphereCollider>::Fire);
	m_Dispatcher.Add<SphereCollider, BoxCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<BoxCollider,SphereCollider>::Fire<SphereCollider, BoxCollider>);
	m_Dispatcher.Add<SphereCollider, PlaneCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<SphereCollider, PlaneCollider>::Fire<SphereCollider, PlaneCollider>);

	m_Dispatcher.Add<PlaneCollider, SphereCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<SphereCollider, PlaneCollider>::Fire<PlaneCollider, SphereCollider>);
	m_Dispatcher.Add<PlaneCollider, BoxCollider>(CollisionAlgorithm::CollisionDetectionAlgorithm<BoxCollider,PlaneCollider >::Fire<PlaneCollider, BoxCollider>);
	*/

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
		(*iter)->UpdatePhysic(m_Dt);
	}

	//Collision Detect N vs N
	bool MaxCollisionReached = false;
	int tmp = 0;
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
		(*iter)->UpdatePosition(m_Dt);
	}
}

RigidBody* World::CreateRigidBody(const Vector3& _Position, const Vector3& _Inertia, float _Mass, int _ID)
{
	RigidBody* newRB = new RigidBody(/*_Position,*/ _Inertia, _Mass, _ID,true);
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


void World::addRigidBody(RigidBody* body)
{
	m_RigidBodies.push_back(body);
};