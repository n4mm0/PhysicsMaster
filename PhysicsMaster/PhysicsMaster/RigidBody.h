#pragma once
/*
** Oggetto base del mondo
*/

class RigidBody {
private:
	float Massa;		// massa dell'oggetto
	float Inerzia[3];	// Momento d'inerzia dell'oggetto: difficoltà nel farlo ruotare nelle 3 dimensioni

	float Pos[3];		// Posizione iniziale
	float Rot[4];		// quaternione iniziale
	float Vel[3];		// Velocità
	float Vang[3];		// Velocità angolare: velocità con cui ruoto sui 3 assi

	float Qmoto[3];		// qmoto = massa * velocità; fris * dt = m * a * dt quindi fris * dt = qmoto
	float Mang[3];		// momento angolare: forza sviluppata dalla rotazione

	float MRot[9];		// matrice di rotazione

	float Fris[3];		// forza risultante (somma forze) 
	float Mris[3];		// momento risultante (somma momenti)


};