#include "Collision.h"
#include "RigidBody.h"
#include <iostream>
#include "World.h"

Collision::Collision()
{
};

void Collision::Init(float deformation, const Vector3& pointOfApplication, const Vector3& normal, RigidBody*first, RigidBody*second)
{
	m_pointOfApplication = pointOfApplication; 
	m_normal = normal;
	m_firstObj = first;
	m_secondObj = second;
	m_deformation = deformation*(m_firstObj->GetCompenetrationCorrection() + m_secondObj->GetCompenetrationCorrection());
	m_collisionCoefficient = (m_firstObj->GetBounciness() + m_secondObj->GetBounciness()) * 0.5f;
};

void Collision::ApplyCollision()
{
	Vector3 Vab = m_firstObj->GetVelocity() - m_secondObj->GetVelocity();
	float impulse = Vab.dot(m_normal);
	//This is for avoid glued effect
	if (impulse >= 0.0f)
	{
		return;
	}

	float Denomitor = 0;
	float secondReciprocalMass = 0;
	Vector3 FirstRadius = m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition();
	Vector3 SecondRadius = m_pointOfApplication - m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition();
	Vector3 tmpv;
	Vector3 DeformationObj1 = m_normal * m_deformation, DeformationObj2 = DeformationObj1;

	Denomitor = m_firstObj->GetInverseMass() + m_secondObj->GetInverseMass();
	FirstRadius = FirstRadius.cross(m_normal);
	tmpv = FirstRadius;
	tmpv *= m_firstObj->GetInverseInertia();
	Denomitor += FirstRadius.dot(tmpv);
	if (m_firstObj->GetMass() != 0)
	{
		DeformationObj1 *= m_firstObj->GetInverseMass();
		DeformationObj2 *= m_firstObj->GetMass();
	}
		SecondRadius = SecondRadius.cross(m_normal);
		tmpv = SecondRadius;
		tmpv *= m_secondObj->GetInverseInertia();
		Denomitor += SecondRadius.dot(tmpv);
	if (m_secondObj->GetMass() != 0 )
	{
		DeformationObj1 *= m_secondObj->GetMass();
		DeformationObj2 *= m_secondObj->GetInverseMass();
	}
	impulse /= Denomitor;
	impulse *= -(1.0f + m_collisionCoefficient);
	tmpv = m_normal*(impulse * m_firstObj->GetInverseMass());
	tmpv += DeformationObj1;
	m_firstObj->AddVelocity(tmpv);
	tmpv = (m_pointOfApplication - m_firstObj->GetOwner()->GetChild<Transform>()->GetPosition()).cross(m_normal);
	tmpv *= impulse;
	tmpv *= m_firstObj->GetInverseInertia();
	m_firstObj->AddAngularVelocity(tmpv);
	tmpv = m_normal*(-impulse * m_secondObj->GetInverseMass());
	tmpv -= DeformationObj2;
	m_secondObj->AddVelocity(tmpv);
	tmpv = (m_secondObj->GetOwner()->GetChild<Transform>()->GetPosition()-m_pointOfApplication).cross(m_normal);
	tmpv *= -impulse;
	tmpv *= m_secondObj->GetInverseInertia();
	m_secondObj->AddAngularVelocity(tmpv);
	//COULOMB FRICTION
	Vab = m_firstObj->GetVelocity() - m_secondObj->GetVelocity();
	tmpv = Vab - m_normal *Vab.dot(m_normal);
	tmpv.normalize();

	float frictionImpulse = -Vab.dot(tmpv);
	frictionImpulse *= m_firstObj->GetInverseMass() + m_secondObj->GetInverseMass();

	float mu = m_firstObj->GetStaticFriction() + m_secondObj->GetStaticFriction();
	mu *= 0.5f;
	
	if (abs(frictionImpulse) < impulse * mu)
		tmpv *= frictionImpulse;
	else
	{
		mu = m_firstObj->GetDynamicFriction() + m_secondObj->GetDynamicFriction();
		mu *= 0.5f;
		tmpv *= -impulse * mu;
	}

	m_firstObj->AddVelocity(tmpv * m_firstObj->GetInverseMass());
	m_secondObj->AddVelocity(tmpv * -m_secondObj->GetInverseMass());

};

