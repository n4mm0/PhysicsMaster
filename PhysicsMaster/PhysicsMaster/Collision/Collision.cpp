#include "Collision.h"
#include "RigidBody.h"
#include <iostream>
#include "World.h"

Collision::Collision()
{
};
void Collision::Init(float deformation,const Vector3& pointOfApplication, const Vector3& normal)
{
	m_deformation = deformation;
	m_pointOfApplication = pointOfApplication; 
	m_normal = normal;
	m_force = m_normal*2*m_deformation;
	//m_force*=deformation;
};

void Collision::ApplyCollision()
{
	//TO DO CATTANI STYLE
	//std::cout << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[0] << ", " << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[1] << ", " << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[2] << std::endl;

	float f;
	float vn;
	float modVtang;

	vn = m_force.dot(m_normal);

	Vector3 Vnorm = m_normal*vn;
	Vector3 Vtang = m_force - Vnorm;

	// K = coefficente elastico L = coefficente anaelastico  TO DO
	f = (5.0f * m_deformation) +  (10.0f*vn); 
	m_normal *= f;

	// forza reagente (modulo) -> m = coefficente attrito TO DO
	f *= 10.0f;	
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
	std::cout << "Point Of Application: " << m_pointOfApplication[0] << " " << m_pointOfApplication[1] << " " << m_pointOfApplication[2] << std::endl;
	std::cout << "Collision Force: " << "( " << m_normal.getX() << ", " << m_normal.getY() << ", " << m_normal.getZ() << ") " << std::endl;
//	system("pause");
};

void Collision::SetBodies(RigidBody*first,RigidBody*second)
{
	m_firstObj = first;
	m_secondObj = second;
	//m_force = m_firstObj->GetVelocity() + m_secondObj->GetVelocity();
}
