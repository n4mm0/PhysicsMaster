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
};

void Collision::ApplyCollision()
{
	Vector3 Vab = m_firstObj->GetVelocity() - m_secondObj->GetVelocity();
	float impulse = Vab.dot(m_normal);
	if (impulse >= 0.0f)
		return;
	float Denomitor = 0;
	float secondReciprocalMass = 0;
	Vector3 FirstRadius = m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition();
	Vector3 SecondRadius = m_pointOfApplication - m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	Vector3 tmpv;
	Vector3 DeformationObj1, DeformationObj2;
	//std::cout << "Impulse: " << impulse << std::endl;
	if (m_firstObj->GetMass() != 0 && m_firstObj->GetInertia()!= Vector3::Zero)
	{
	//	impulse += ((FirstRadius.cross(m_normal)).dot(m_firstObj->GetAngularVelocity()));
		Denomitor = 1.0f / m_firstObj->GetMass();
		FirstRadius = FirstRadius.cross(m_normal);
		tmpv = FirstRadius;
	//	tmpv = FirstRadius.cross(m_normal);
	//	tmpv = tmpv.cross(FirstRadius);
		tmpv /= m_firstObj->GetInertia()[0];
		tmpv /= m_firstObj->GetInertia()[1];
		tmpv /= m_firstObj->GetInertia()[2];
		DeformationObj1 = m_normal *  m_deformation / m_firstObj->GetMass();
		DeformationObj2 = m_normal * m_deformation * m_firstObj->GetMass();
		Denomitor += FirstRadius.dot(tmpv);
	}
	if (m_secondObj->GetMass() != 0 && m_secondObj->GetInertia() != Vector3::Zero)
	{
	//	impulse -= ((SecondRadius.cross(m_normal)).dot(m_secondObj->GetAngularVelocity()));
		Denomitor += 1.0f / m_secondObj->GetMass();
		SecondRadius = SecondRadius.cross(m_normal);
		tmpv = SecondRadius;
		tmpv /= m_firstObj->GetInertia()[0];
		tmpv /= m_firstObj->GetInertia()[1];
		tmpv /= m_firstObj->GetInertia()[2];
		//Vector3 tmpv2 = SecondRadius.cross(m_normal);
		DeformationObj1 *= m_secondObj->GetMass();

		DeformationObj2 /= m_secondObj->GetMass();

		Denomitor += SecondRadius.dot(tmpv);
	}
	//std::cout << "s(" << secondTerm[0] << ", " << secondTerm[1] << ", " << secondTerm[2] <<")" <<std::endl;
	//std::cout << "r(" << secondTerm2[0] << ", " << secondTerm2[1] << ", " << secondTerm2[2] << ")" << std::endl;
	//FirstRadius += SecondRadius;
	//Denomitor += FirstRadius.dot(m_normal);
	//Denomitor += tmpv.dot(m_normal);
	//std::cout << "Den: " << Denomitor << std::endl;
	impulse /= Denomitor;
	//-(1+e) where e = coeff for now hardcoded
	impulse *= -(1.0f + 0.50f);
	//impulse +=2.0f* m_deformation/World::m_Dt;
	//std::cout << "Impulse: " << impulse << std::endl;


	if (m_firstObj->GetMass() != 0)
	{


		tmpv = m_normal*(impulse / m_firstObj->GetMass());
		tmpv += DeformationObj1;
	
		m_firstObj->AddVelocity(tmpv);

		tmpv = (m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()).cross(m_normal);
		tmpv *= impulse;
		
		if (tmpv.sqrMagnitude() > 0.01f)
		{
			tmpv[0] /= m_firstObj->GetInertia()[0];
			tmpv[1] /= m_firstObj->GetInertia()[1];
			tmpv[2] /= m_firstObj->GetInertia()[2];
			tmpv += DeformationObj1;
			m_firstObj->AddAngularVelocity(tmpv);
		}


	}

	if (m_secondObj->GetMass() != 0)
	{
		impulse *= -1.0f;
		tmpv = m_normal*(impulse / m_firstObj->GetMass());
		tmpv -= DeformationObj2;
		m_secondObj->AddVelocity(tmpv);
		tmpv = (m_pointOfApplication - m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition()).cross(m_normal);
		tmpv *= impulse;
	
		if (tmpv.sqrMagnitude() > 0.01f)
		{
			tmpv[0] /= m_secondObj->GetInertia()[0];
			tmpv[1] /= m_secondObj->GetInertia()[1];
			tmpv[2] /= m_secondObj->GetInertia()[2];
			tmpv += DeformationObj2;
			m_secondObj->AddAngularVelocity(tmpv);
		}
	}
	/*
	Vector3 impactSpeed = m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition();
	impactSpeed = m_firstObj->GetAngularVelocity().cross(impactSpeed);
	impactSpeed += m_secondObj->GetVelocity();

	Vector3 temp = m_pointOfApplication + m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	temp = m_secondObj->GetAngularVelocity().cross(temp);
	temp += m_firstObj->GetVelocity();

	impactSpeed = temp-impactSpeed;

	float modNormalVelocity = impactSpeed.dot(m_normal);
	Vector3 normalVelocity = m_normal * modNormalVelocity;
	Vector3 tangentVelocity = impactSpeed - normalVelocity;

	float force = (250.0f * m_deformation) + (5.0f * modNormalVelocity);

	// Se è < 0 ho deformazione nulla
	force = force < 0 ? 0 : force;

	Vector3 normalForce = m_normal * force;

	force *= 0.2f;

	Vector3 tangentForce = tangentVelocity * force;

	float modTangentVelocity = tangentVelocity.magnitude();

	// Facendo il modulo della gravità funziona anche per gravità che non agisce su y
	// Scalo la forza tangente
	if (modTangentVelocity >  World::m_Gravity.magnitude()*World::m_Dt)
		tangentForce /= modTangentVelocity;
	else
		tangentForce /= World::m_Gravity.magnitude();
	
	Vector3 totalForce = normalForce + tangentForce;
	Vector3 localPoint(m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition());

	m_firstObj->ApplyForce(totalForce, localPoint);
	localPoint = m_pointOfApplication - m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	m_secondObj->ApplyForce(totalForce*-1.0f, localPoint);

	//std::cout << "After Collision Handling" << std::endl;
	std::cout << "Point Of Application: " << m_pointOfApplication[0] << " " << m_pointOfApplication[1] << " " << m_pointOfApplication[2] << std::endl;
	std::cout << "Collision Force: " << "( " << totalForce.getX() << ", " << totalForce.getY() << ", " << totalForce.getZ() << ") " << std::endl;
	*/
	//system("pause");
};

void Collision::SetBodies(RigidBody*first,RigidBody*second)
{
	m_firstObj = first;
	m_secondObj = second;
}
