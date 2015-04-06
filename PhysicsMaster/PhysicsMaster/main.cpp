/* OpenGL testbed per engine dinamico
*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <math.h>
#include "Matrici.h"
#include <GL/glut.h>
#include "Matrix.h"
#include "Utilities.h"
#include "World.h"
#include "RigidBody.h"

// Debug
#include <iostream>
using namespace std;

/* Le seguenti funzioni permettono di disegnare sagome
* tridimensionali a video.
* Rot e' una matrice di rotazione 3x3 (opzionale)
*/

void DisegnaParall(float X, float Y, float Z, float Lx, float Ly, float Lz, const float *Rot = NULL);
void DisegnaSfera(float X, float Y, float Z, float R, const float *Mrot = NULL);
void DisegnaPianoXZ(float QuotaZ);

/* La funzione di integrazione.
* Viene invocata ogni DT secondi
*/

float DT = 0.010f;	//Tempo di integrazione
double TempoTotale = 0;

// Raggi parallelepipedo
#define SDIM_X	1
#define SDIM_Y	1
#define SDIM_Z	1
/* Momento d'inerzia di un parallelepipedo
** ((sa^2)+(sb^2) * m) / 12
** Il 4 viene fuori perchË sarebbe (2 * sa)^2 in quanto abbiamo solo mezzo lato e dobbiamo moltiplicare per 2 per avere il lato
** totale
*/
#define M_INERZIA(m, sa, sb)	(((4.0f * sa * sa) + (4.0f * sb * sb)) * m / 12.0f)

class corpo_rigido
{

public:

	float Massa;	// massa dell'oggetto
	float Inerzia[3];	// Momento d'inerzia dell'oggetto: difficolt‡ nel farlo ruotare nelle 3 dimensioni

	float Pos[3]; // Posizione iniziale
	float Rot[4]; // quaternione iniziale
	float Vel[3]; // Velocit‡
	float Vang[3]; // Velocit‡ angolare: velocit‡ con cui ruoto sui 3 assi

	float Qmoto[3]; // qmoto = massa * velocit‡; fris * dt = m * a * dt quindi fris * dt = qmoto
	float Mang[3];  // momento angolare: forza sviluppata dalla rotazione

	float MRot[9];  // matrice di rotazione

	float Fris[3]; // forza risultante (somma forze) 
	float Mris[3]; // momento risultante (somma momenti)

	void ApplicaForza(const float *Forza, const float *PuntoApplicazione)
	{
		float b[3];
		// Fris è la componente di forza che muove l'oggetto
		SommaVettori(Forza, Fris, Fris);
		// PuntoApplicazione = Vertice + Pos (-Pos) rimane il vertice
		SottraiVettori(PuntoApplicazione, Pos, b);
		ProdottoVettoriale(b, Forza, b);
		// Mris è la componente che ruota l'oggetto
		SommaVettori(b, Mris, Mris);
	}

	void IntegraStato(float dt)
	{
		float v[3];
		float q[4];

		MoltiplicaVettoreScalare(Fris, dt, v);
		SommaVettori(v, Qmoto, Qmoto);
		MoltiplicaVettoreScalare(Mris, dt, v);
		SommaVettori(v, Mang, Mang);

		DividiVettoreScalare(Qmoto, Massa, Vel);
		MoltiplicaVettoreScalare(Vel, dt, v);
		SommaVettori(v, Pos, Pos);

		RuotaRelative(MRot, Mang, Vang);	// Per risolvere problemi di inerzia, raddrizzo il mio
											// oggetto, altrimenti l'inerzia cambierebbe in base a come Ëè disposto l'oggetto

		Vang[0] /= Inerzia[0];
		Vang[1] /= Inerzia[1];
		Vang[2] /= Inerzia[2];

		q[0] = 1;
		q[1] = Vang[0] * dt / 2;
		q[2] = Vang[1] * dt / 2;
		q[3] = Vang[2] * dt / 2;

		NormalizzaQuaternione(q, q);
		MoltiplicaQuaternioni(Rot, q, Rot);
		NormalizzaQuaternione(Rot, Rot);

		RuotaAssolute(MRot, Vang, Vang); // Torno in assoluto per poi tornare in relativo
		MatriceDaQuaternione(Rot, MRot);
	}

};

class collisione
{

public:

	float Deformazione;
	float PuntoImpatto[3];
	float Normale[3];
	float VelImpatto[3];

	// supponendo che c2 collida su c1 (c2 = piano, c1 = parall)
	void ApplicaForzeCollisione(corpo_rigido *c1, corpo_rigido *c2,
		float K, float L, float m)  // K = costante elastica L = costante viscosa
		// m = coeff. attrito.

	{
		float f;
		float vn; // modulo velocità normale
		float modVtang;
		float Vtang[3];
		float Vnorm[3];
		float Ftang[3];
		float Fnorm[3];

		// La velocità di impatto è la somma vettoriale fra la Velocità Normale
		// e la Velocità Tangenziale

		vn = ProdottoScalare(VelImpatto, Normale); // componente velocità normale (modulo)
		MoltiplicaVettoreScalare(Normale, vn, Vnorm); // normale (versore) * vn(modulo)
		// Utilizzo la relazione VelImpatto = Vnorm + Vtang
		SottraiVettori(VelImpatto, Vnorm, Vtang);

		// vn è quanto va a premere sull'oggetto, è ciò che mi interessa per il calcolo del
		// coeff. elastico e anaelastico
		f = (K * Deformazione) + (L * vn); // K = coefficente elastico L = coefficente anaelastico
		if (f < 0) // in questo caso la deformazione = 0
			f = 0;
		MoltiplicaVettoreScalare(Normale, f, Fnorm); // In questo modo trovo il vettore Fnorm

		// A Vnorm (negativo) si contrappone Fnorm che è >= 0

		// Considero l'effetto di elasticità anche lateralmente (Vtang velocità laterale) e gli moltiplico
		// il coefficente di attrito perchè è quello che interviene lateralmente quando i due corpi sono
		// a contatto.
		f *= m;	// forza reagente (modulo) -> m = coefficente attrito

		// in base alla velocità si può fare intervenire un if per selezionare se
		// utilizzare l'attrito dinamico (velocità più alta) o l'attrito statico (velocità nulla)

		modVtang = ModuloVettore(Vtang);
		// Moltiplicando Vtang * f = Ftang (ho una modifica del vettore Vtang fino a farlo diventare Ftang)
		MoltiplicaVettoreScalare(Vtang, f, Ftang);

		// evito che modVtang sia piccolo e che quindi venga una Ftang troppo grande
		// ho sempre una riduzione di Ftang (per evitare che Ftotale diventi negativo)
		if (modVtang > 9.8f * DT) // se è maggiore applico modVtang
			DividiVettoreScalare(Ftang, modVtang, Ftang);
		else
			DividiVettoreScalare(Ftang, 9.8f * DT, Ftang); // altrimenti applico 9,8 * dt

		SommaVettori(Fnorm, Ftang, Fnorm);	// ora Fnorm e' la F totale

		// Oggetto collissore con Fnorm positivo
		if (c1 != NULL)
			c1->ApplicaForza(Fnorm, PuntoImpatto);

		Fnorm[0] = -Fnorm[0];
		Fnorm[1] = -Fnorm[1];
		Fnorm[2] = -Fnorm[2];

		// Oggetto collidente (terreno) con Fnorm negativo
		if (c2 != NULL)
			c2->ApplicaForza(Fnorm, PuntoImpatto);
	}

};

class forma
{
	// riempie l'array passato con le collisioni trovate, è un interfaccia
public:
	virtual int CollidiConPiano(const corpo_rigido *r, float A, float B, float C, float D,
		collisione *c, int MaxCollisioni) = 0;
};

class forma_parallelepipedo : public forma
{

private:
	float Vertici[8][3]; // Vertici parallelepipedo

public:
	// Costruttore che in base alle semi coordinate crea il parallelepipedo
	forma_parallelepipedo(float SemiX, float SemiY, float SemiZ)
	{
		// Faccia front e back in senso orario coordinata X
		Vertici[0][0] = Vertici[4][0] = -SemiX;
		Vertici[1][0] = Vertici[5][0] = -SemiX;
		Vertici[2][0] = Vertici[6][0] = SemiX;
		Vertici[3][0] = Vertici[7][0] = SemiX;

		// Faccia front e back in senso orario coordinata Y
		Vertici[0][1] = Vertici[4][1] = -SemiY;
		Vertici[1][1] = Vertici[5][1] = SemiY;
		Vertici[2][1] = Vertici[6][1] = SemiY;
		Vertici[3][1] = Vertici[7][1] = -SemiY;

		// Faccia front senso orario 0, 1, 2, 3
		Vertici[0][2] = SemiZ;
		Vertici[1][2] = SemiZ;
		Vertici[2][2] = SemiZ;
		Vertici[3][2] = SemiZ;

		// Faccia back senso orario 4, 5, 6, 7
		Vertici[4][2] = -SemiZ;
		Vertici[5][2] = -SemiZ;
		Vertici[6][2] = -SemiZ;
		Vertici[7][2] = -SemiZ;
	}

	// riempie l'array passato con le collisioni trovate
	int CollidiConPiano(const corpo_rigido *r, float A, float B, float C, float D,
		collisione *c, int MaxCollisioni)
	{
		int n = 0; // Numeri di collisioni

		for (int i = 0; i < 8; i++)
		{
			RuotaAssolute(r->MRot, Vertici[i], c[n].PuntoImpatto); // Ruota il vertice in coord. assolute
			ProdottoVettoriale(r->Vang, c[n].PuntoImpatto, c[n].VelImpatto);
			SommaVettori(r->Pos, c[n].PuntoImpatto, c[n].PuntoImpatto); // gli aggiunge la posizione
			SommaVettori(r->Vel, c[n].VelImpatto, c[n].VelImpatto); // Somma la velocità con la velocità angolare

			// Se Deformazione = 0 allora sono sul piano
			// Se Deformazione > 0 allora sono sopra al piano
			// Se Deformazione è < 0 allora lo sto compenetrando
			c[n].Deformazione = (A * c[n].PuntoImpatto[0] + B * c[n].PuntoImpatto[1] + C *
				c[n].PuntoImpatto[2] + D);

			// In caso di non collisione passa al ciclo successivo
			if (c[n].Deformazione > 0)
				continue;

			// Deformazione deve essere positiva, altrimenti continua a scendere
			c[n].Deformazione = -c[n].Deformazione;

			// normale del piano (0, 1, 0)
			c[n].Normale[0] = A; // x
			c[n].Normale[1] = B; // y
			c[n].Normale[2] = C; // z

			c[n].VelImpatto[0] = -c[n].VelImpatto[0]; // La velocità di impatto diventa opposta
			c[n].VelImpatto[1] = -c[n].VelImpatto[1];
			c[n].VelImpatto[2] = -c[n].VelImpatto[2];

			n++;
			if (n == MaxCollisioni) break;
		}

		return(n);
	}
};

forma_parallelepipedo FormaParal(SDIM_X, SDIM_Y, SDIM_Z);
corpo_rigido Paral;

collisione Collisioni[8];

static void IntegraSistema()
{
	int nCol; // Numero collisioni

	// Inizializzazione solo una volta
	if (TempoTotale < DT)
	{
		Paral.Pos[2] = -5.0f;

		Paral.Massa = 10;

		Paral.Inerzia[0] = M_INERZIA(Paral.Massa, SDIM_Y, SDIM_Z);
		Paral.Inerzia[1] = M_INERZIA(Paral.Massa, SDIM_X, SDIM_Z);
		Paral.Inerzia[2] = M_INERZIA(Paral.Massa, SDIM_X, SDIM_Y);

		Paral.Rot[0] = 1; // parte reale del quaternione a 1

		Paral.MRot[0] = Paral.MRot[4] = Paral.MRot[8] = 1; // Matrice identica

		Paral.Mang[2] = 10;
		Paral.Mang[1] = 1;
		Paral.Mang[0] = 3;

		Paral.Qmoto[1] = 40; // quantit‡ moto che agisce su Y
	}

	// *** Cicla ogni t = DT

	Paral.Fris[0] = Paral.Fris[1] = Paral.Fris[2] = 0;
	Paral.Mris[0] = Paral.Mris[1] = Paral.Mris[2] = 0;

	// gravita'
	Paral.Fris[1] = -3.8f * Paral.Massa;

	// collisioni col piano Y = -5
	// 0x + 1y + 0z + 5 -> y = -5
	nCol = FormaParal.CollidiConPiano(&Paral, 0, 1, 0, 5, Collisioni, 8);

	// calcolo delle forze in base al numero di collisioni
	for (int i = 0; i < nCol; i++)
	{
		// corpo rigigo1, corpo rigido2, parametri attrito
		Collisioni[i].ApplicaForzeCollisione(&Paral, NULL, 600, 10, 0.3);
	}
	Paral.IntegraStato(DT);
}

/* La funzione di visualizzazione.
* Viene periodicamente invocata per visualizzare la simulazione
*/

static void VisualizzaSistema()
{
	DisegnaPianoXZ(-5);

	DisegnaParall(Paral.Pos[0], Paral.Pos[1], Paral.Pos[2], SDIM_X, SDIM_Y, SDIM_Z, Paral.MRot);
	//DisegnaSfera(Paral.Pos[0], Paral.Pos[1], Paral.Pos[2], SDIM_X, Paral.MRot);
}

/* La funzione di interazione.
* Viene invocata se l'utente preme un tasto
*/

static void TastoPremuto(unsigned char Tasto)
{

}


/* Tutto quanto segue e' legato alla sola creazione del testbed
* utilizzando OpenGL e Glut - non ha importanza ai fini della
* trattazione
*/

static void CambiaDimensioni(int w, int h)
{
	glViewport(0, 0, w, h);

	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -12);
}


static void AzioneTasto(unsigned char Tasto, int, int)
{
	TastoPremuto(Tasto);
}

static void EseguiCiclicamente()
{
	double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	while (TempoTotale < t) {
		IntegraSistema();
		TempoTotale += DT;
	}

	glutPostRedisplay();
}

static GLfloat Rot[16];

static GLfloat rosso[] = { 1.0f, 0.2f, 0.2f, 1.0f };
//static GLfloat verde[] = { 0.2f, 0.8f, 0.2f, 1.0f };
static GLfloat verde2[] = { 0.4f, 1.0f, 0.4f, 1.0f };
static GLfloat blu[] = { 0.4f, 0.4f, 1.0f, 1.0f };
static GLfloat bianco[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void DisegnaSfera(float X, float Y, float Z, float R, const float *M)
{
	int i;
	float j, X1, Y1, X2, Y2, s, c;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(X, Y, Z);

	if (M != NULL) {
		Rot[0] = M[0]; Rot[1] = M[1]; Rot[2] = M[2]; Rot[3] = 0;
		Rot[4] = M[3]; Rot[5] = M[4]; Rot[6] = M[5]; Rot[7] = 0;
		Rot[8] = M[6]; Rot[9] = M[7]; Rot[10] = M[8]; Rot[11] = 0;
		Rot[12] = 0; Rot[13] = 0; Rot[14] = 0; Rot[15] = 1;
		glMultMatrixf(Rot);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, rosso);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rosso);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	//glutSolidSphere(R, 64, 64);
	X2 = 0;
	Y2 = -1;
	for (i = 1; i <= 64; i++) {
		Y1 = Y2;
		X1 = X2;
		Y2 = (i / 32.0f) - 1.0f;
		X2 = 1 - (Y2 * Y2);
		if (X2 > 0) X2 = R * sqrt(X2);

		if (i == 17) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, bianco);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, bianco);
		}
		if (i == 48) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, rosso);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, rosso);
		}

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j < 6.2831f; j += (6.283f / 64)) {
			s = sin(j);
			c = cos(j);
			glNormal3f(c * X1, Y1, s * X1);
			glVertex3f(c * X1 * R, Y1 * R, s * X1 * R);
			glNormal3f(c * X2, Y2, s * X2);
			glVertex3f(c * X2 * R, Y2 * R, s * X2 * R);
		}
		glEnd();
	}

	glPopMatrix();
}

void DisegnaParall(float X, float Y, float Z, float Lx, float Ly, float Lz, const float *R)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(X, Y, Z);

	if (R != NULL) {
		Rot[0] = R[0]; Rot[1] = R[1]; Rot[2] = R[2]; Rot[3] = 0;
		Rot[4] = R[3]; Rot[5] = R[4]; Rot[6] = R[5]; Rot[7] = 0;
		Rot[8] = R[6]; Rot[9] = R[7]; Rot[10] = R[8]; Rot[11] = 0;
		Rot[12] = 0; Rot[13] = 0; Rot[14] = 0; Rot[15] = 1;
		glMultMatrixf(Rot);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, blu);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blu);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(Lx, -Ly, Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glVertex3f(Lx, Ly, -Lz);
	glNormal3f(0, 1, 0);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(Lx, Ly, -Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glVertex3f(-Lx, Ly, Lz);
	glNormal3f(0, 0, 1);
	glVertex3f(Lx, Ly, Lz);
	glVertex3f(-Lx, Ly, Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glVertex3f(Lx, -Ly, Lz);
	glNormal3f(-1, 0, 0);
	glVertex3f(-Lx, Ly, Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glNormal3f(0, -1, 0);
	glVertex3f(Lx, -Ly, Lz);
	glVertex3f(-Lx, -Ly, Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glNormal3f(0, 0, -1);
	glVertex3f(Lx, Ly, -Lz);
	glVertex3f(Lx, -Ly, -Lz);
	glVertex3f(-Lx, -Ly, -Lz);
	glVertex3f(-Lx, Ly, -Lz);
	glEnd();

	glPopMatrix();
}

void DisegnaPianoXZ(float Y)
{
	int i;
	float Dim = 20;

	glMaterialfv(GL_FRONT, GL_AMBIENT, verde2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verde2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bianco);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_LINES);
	glNormal3f(0, 1, 0);
	for (i = -Dim; i < Dim; i++) {
		glVertex3f(i, Y, -Dim);
		glVertex3f(i, Y, Dim);
	}
	for (i = -Dim; i < Dim; i++) {
		glVertex3f(-Dim, Y, i);
		glVertex3f(Dim, Y, i);
	}
	glEnd();

}

static GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat aLite[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLfloat dLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat sLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat LucePos[4] = { 1, 2, 1, 0 };

static void DisegnaTutto()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, LucePos);

	VisualizzaSistema();

	glutSwapBuffers();
}



int main(int argc, char **argv)
{
	//Rigidbody creation example
	/*World world;
	RigidBody* rb = world.CreateRigidBody(Vector3::Zero, Vector3::Zero, 0, 0);
	rb->ShowStatus();
	world.DeleteRigidBody(rb);
	system("pause");*/

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(600, 100);
	//glutInitWindowPosition(800, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Physics Testbed");

	glutReshapeFunc(CambiaDimensioni);
	glutDisplayFunc(DisegnaTutto);
	glutKeyboardFunc(AzioneTasto);
	glutIdleFunc(EseguiCiclicamente);

	// Z buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// luci
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glutMainLoop();

	return(0);
}