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
	//m_force = m_normal*m_deformation;
	//m_force*=deformation;
};

void Collision::ApplyCollision()
{
	//TO DO CATTANI STYLE
	//std::cout << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[0] << ", " << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[1] << ", " << m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()[2] << std::endl;

	Vector3 impactSpeed = m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition();
	impactSpeed = m_firstObj->GetAngularVelocity().cross(impactSpeed);
	impactSpeed += m_firstObj->GetVelocity();

	Vector3 temp = m_pointOfApplication + m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	temp = m_secondObj->GetAngularVelocity().cross(temp);
	temp += m_secondObj->GetVelocity();

	impactSpeed -= temp;

	float modNormalVelocity = impactSpeed.dot(m_normal);
	Vector3 normalVelocity = m_normal * modNormalVelocity;
	Vector3 tangentVelocity = impactSpeed - normalVelocity;

	float force = (250.0f * m_deformation) + (5.0f * modNormalVelocity);

	// Se è < 0 ho deformazione nulla
	force = force < 0 ? 0 : force;

	Vector3 normalForce = m_normal * force;

	force *= 0.2f;

	Vector3 tangentForce = tangentVelocity * force;

	float modTangentVelocity = tangentVelocity.sqrMagnitude();

	// Facendo il modulo della gravità funziona anche per gravità che non agisce su y
	// Scalo la forza tangente
	if (modTangentVelocity > 9.8f)
		tangentForce /= modTangentVelocity;
	else
		tangentForce /= 9.8f;
	
	Vector3 totalForce = normalForce + tangentForce;

	Vector3 localPosition;

	localPosition = m_pointOfApplication - m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	m_secondObj->ApplyForce(totalForce,localPosition);

	// Inverto la forza che sarà quella che riceverà l'oggetto B

	localPosition = m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition();
	m_firstObj->ApplyForce(totalForce*-1.0f,localPosition);

	//std::cout << "After Collision Handling" << std::endl;
	std::cout << "Point Of Application: " << m_pointOfApplication[0] << " " << m_pointOfApplication[1] << " " << m_pointOfApplication[2] << std::endl;
	std::cout << "Collision Force: " << "( " << m_normal.getX() << ", " << m_normal.getY() << ", " << m_normal.getZ() << ") " << std::endl;
	//system("pause");
};

void Collision::SetBodies(RigidBody*first,RigidBody*second)
{
	m_firstObj = first;
	m_secondObj = second;
	m_force = m_firstObj->GetVelocity() - m_secondObj->GetVelocity();
	//m_force *= 2.0f*m_deformation;
}
