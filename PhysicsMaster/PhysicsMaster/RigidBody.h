#pragma once
/*
** Oggetto base del mondo
*/
#include "vec3.h"
#include "vec4.h"

class RigidBody {
private:
	float	mass;			// massa dell'oggetto
	Vec3	inerzia;		// Momento d'inerzia dell'oggetto: difficoltà nel farlo ruotare nelle 3 dimensioni

	Vec3	position;		// Posizione iniziale
	Vec4	rotationQ;		// quaternione iniziale
	Vec3	velocity;		// Velocità
	Vec3	angularVel;		// Velocità angolare: velocità con cui ruoto sui 3 assi

	Vec3	quantMoto;		// qmoto = massa * velocità; fris * dt = m * a * dt quindi fris * dt = qmoto
	Vec3	momentAng;		// momento angolare: forza sviluppata dalla rotazione

	float	MRot[9];		// matrice di rotazione

	Vec3	forzaRis;		// forza risultante (somma forze) 
	Vec3	momentoRis;		// momento risultante (somma momenti)

public:
	RigidBody(float mass, Vec3 inerzia)
	{
		this->mass = mass;
		this->inerzia = inerzia;
		position = Vec3(0, 0, 0);
		rotationQ = Vec4(0, 0, 0, 1);
		velocity = Vec3(0, 0, 0);
		angularVel = Vec3(0, 0, 0);
		quantMoto = Vec3(0, 0, 0);
		momentAng = Vec3(0, 0, 0);
		// Todo Matrix
		forzaRis = Vec3(0, 0, 0);
		momentoRis = Vec3(0, 0, 0);
	}

	//Creare getter e setter
	//Creare funzione applyForce e update(dt)
};