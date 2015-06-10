#include "Collision.h"
#include "RigidBody.h"
#include <iostream>
#include "World.h"

Collision::Collision()
{
};
void Collision::Init(float deformation, const Vector3& pointOfApplication, const Vector3& normal)
{
	m_deformation = deformation;
	m_pointOfApplication=pointOfApplication; 
	m_normal=normal;
};

void Collision::ApplyCollision()
{
	//TO DO CATTANI STYLE
	
	float f;
	float vn;
	float modVtang;
	vn = m_force.dot(m_normal);
	Vector3 Vnorm = m_normal*vn;
	Vector3 Vtang = m_force - Vnorm;
	// K = coefficente elastico L = coefficente anaelastico  TO DO
	f = (0.5f * m_deformation) + (5.0f * vn); 
	m_normal *= f;
	// forza reagente (modulo) -> m = coefficente attrito TO DO
	f *= 5;	
	modVtang = Vtang.magnitude();
	if (f<0)
		f = 0;
	Vtang*=f;
	if (modVtang > 9.8f *World::m_Dt)
		Vtang.normalize();
	else
		Vtang /= (9.8f)* World::m_Dt;
	
	m_normal += Vtang;

	m_firstObj->ApplyForce(m_normal, m_pointOfApplication);
	m_secondObj->ApplyForce(m_normal*-1.0f, m_pointOfApplication);

	std::cout << "After Collision Handling" << std::endl;
	std::cout << "Collision Force: " << "( " << m_normal.getX() << ", " << m_normal.getY() << ", " << m_normal.getZ() << ") " << std::endl;
	//_firstObj->PrintStatus();
	//_secondObj->PrintStatus();

};

void Collision::SetBodies(RigidBody*first,RigidBody*second)
{
	m_firstObj = first;
	m_secondObj = second;
	m_force += first->GetVelocity();
	m_force += second->GetVelocity();
}
