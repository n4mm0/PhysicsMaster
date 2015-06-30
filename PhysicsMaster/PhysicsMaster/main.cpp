//GRAFICA FATTA DI MERDA SENZA SBATTI
#define _USE_MATH_DEFINES
#include <gl\GLUT.H>
#include <math.h>
#include "Singleton.h"
#include "World.h"
#include "RigidBody.h"
#include "GameObject.h"
#include <iostream>

int width = 600;
int height = 600;

GameObject gameObj[3];

Transform *transform1 = gameObj[0].EditChild<Transform>();
Vector3 position1 = transform1->EditPosition();
Matrix4x4 rotationMatrix1 = transform1->GetRotationMatrix();
GLfloat rotationGL1[16];

Transform *transform2 = gameObj[1].EditChild<Transform>();
Vector3 position2 = transform2->EditPosition();
Matrix4x4 rotationMatrix2 = transform2->GetRotationMatrix();
GLfloat rotationGL2[16];

World w;

void MatrixToGLFloat(Matrix4x4 &matrix, GLfloat *m)
{
	for (int index = 0; index < 16; ++index)
		m[index] = matrix.getElementAt((index / 4), (index % 4));
	/*
	std::cout << std::endl;
	for (int index = 0; index < 4; ++index)
	{
		for (int index2 = 0; index2 < 4; ++index2)
			std::cout << m[index * 4 + index2] << ", ";
		std::cout<<std::endl;
	}
	*/
	
}
static bool stopped = false;
void processKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		exit(EXIT_SUCCESS);
		break;
				case 32:
	if (!stopped)
		{
			stopped = !stopped;
			system("pause");
		}
		else
		{
			stopped = !stopped;
			system("play");
		}
		break;
	}
}

void changeSize(int w, int h)
{
	int width = w;
	int height = h;

	if (height == 0.0f)
		height = 1.0f;

	float ratio = width * 1.0f / height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0, 0, -20);
}

void drawSphere(Vector3 &position, Matrix4x4 rotation, float radius, int lats, int longs)
{
	GLfloat blue[] = { 0.4f, 0.4f, 1.0f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.getX(), position.getY(), position.getZ());
	MatrixToGLFloat(rotation, rotationGL1);
	glMultMatrixf(rotationGL1);

	glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	for (int indexLat = 0; indexLat <= lats; ++indexLat)
	{
		double lat0 = M_PI * (-0.5 + (double)(indexLat - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)indexLat / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (int indexLng = 0; indexLng <= longs; ++indexLng)
		{
			double lng = 2 * M_PI * (double)(indexLng - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
	glPopMatrix();
}

void drawParallelepiped(Vector3 &position, Matrix4x4 rotation, float lX, float lY, float lZ)
{
	GLfloat green[] = { 0.2f, 0.8f, 0.6f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.getX(), position.getY(), position.getZ());
	MatrixToGLFloat(rotation, rotationGL2);
//	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glMultMatrixf(rotationGL2);

	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(lX, lY, lZ);
	glVertex3f(lX, -lY, lZ);
	glVertex3f(lX, -lY, -lZ);
	glVertex3f(lX, lY, -lZ);

	glNormal3f(0, 1, 0);
	glVertex3f(lX, lY, lZ);
	glVertex3f(lX, lY, -lZ);
	glVertex3f(-lX, lY, -lZ);
	glVertex3f(-lX, lY, lZ);

	glNormal3f(0, 0, 1);
	glVertex3f(lX, lY, lZ);
	glVertex3f(-lX, lY, lZ);
	glVertex3f(-lX, -lY, lZ);
	glVertex3f(lX, -lY, lZ);

	glNormal3f(-1, 0, 0);
	glVertex3f(-lX, lY, lZ);
	glVertex3f(-lX, lY, -lZ);
	glVertex3f(-lX, -lY, -lZ);
	glVertex3f(-lX, -lY, lZ);

	glNormal3f(0, -1, 0);
	glVertex3f(lX, -lY, lZ);
	glVertex3f(-lX, -lY, lZ);
	glVertex3f(-lX, -lY, -lZ);
	glVertex3f(lX, -lY, -lZ);

	glNormal3f(0, 0, -1);
	glVertex3f(lX, lY, -lZ);
	glVertex3f(lX, -lY, -lZ);
	glVertex3f(-lX, -lY, -lZ);
	glVertex3f(-lX, lY, -lZ);
	glEnd();

	glPopMatrix();
}

void drawFloor(float y, float dimension)
{
	GLfloat green[] = { 0.4f, 1.0f, 0.4f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_LINES);
	glNormal3f(0, 1, 0);
	for (int index = -dimension; index <= dimension; ++index)
	{
		glVertex3f(index, y, -dimension);
		glVertex3f(index, y, dimension);
	}
	for (int index = -dimension; index < dimension; ++index)
	{
		glVertex3f(-dimension, y, index);
		glVertex3f(dimension, y, index);
	}
	glEnd();
}

void drawWallXZ(float axis, float halfHeight, float dimension)
{
	GLfloat red[] = { 0.8f, 0.4f, 0.2f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 16);

	glBegin(GL_LINES);
	glNormal3f(0, 1, 0);
	for (int index = -halfHeight; index < halfHeight; ++index)
	{
		glVertex3f(axis, index, -dimension);
		glVertex3f(axis, index, dimension);
	}
	for (int index = -dimension; index <= dimension; ++index)
	{
		glVertex3f(axis, -halfHeight, index);
		glVertex3f(axis, halfHeight, index);
	}
	glEnd();

	glBegin(GL_LINES);
	glNormal3f(0, 1, 0);
	for (int index = -halfHeight; index < halfHeight; ++index)
	{
		glVertex3f(-dimension, index, axis);
		glVertex3f(dimension, index, axis);
	}
	for (int index = -dimension; index <= dimension; ++index)
	{
		glVertex3f(index, -halfHeight, axis);
		glVertex3f(index, halfHeight, axis);
	}
	glEnd();
}

void renderScene()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat aLite[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat dLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat sLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat posLight[4] = { 1, 2, 1, 0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, posLight);

	//drawFloor(6.0f, 20.0f);
	drawFloor(-6.0f, 20.0f);
	/*drawWallXZ(-20.0f, 6.0f, 20.0f);
	drawWallXZ(20.0f, 6.0f, 20.0f);
	*/
	drawSphere(position1, rotationMatrix1, 1.0f, 64, 64);
	drawParallelepiped(position2, rotationMatrix2, 1.0f, 1.0f, 1.0f);
	//drawSphere(position2, rotationMatrix2, 1.0f, 64, 64);

	glutSwapBuffers();
}

void renderIdleScene()
{
	w.Update();
	position1 = gameObj[0].EditChild<Transform>()->EditPosition();
	position2 = gameObj[1].EditChild<Transform>()->EditPosition();
	//gameObj[1].EditChild<RigidBody>()->ApplyForce(Vector3(1, 1, 1), Vector3(5, 10, 2));

	rotationMatrix1 = gameObj[0].EditChild<Transform>()->GetRotationMatrix();
	rotationMatrix2 = gameObj[1].EditChild<Transform>()->GetRotationMatrix();
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	/*---TEST---*/
	
	RigidBody* r1 = new RigidBody(Vector3(1, 1, 1), 1.0f, 0, true);
	RigidBody* r2 = new RigidBody(Vector3(1, 1, 1), 1.0f, 1, true);
	RigidBody* r3 = new RigidBody(Vector3(1, 1, 1), 1.0f, 2, false);

	gameObj[0].AddChild<RigidBody>(*r1);
	gameObj[1].AddChild<RigidBody>(*r2);
	gameObj[2].AddChild<RigidBody>(*r3);

	SphereCollider* c1 = new SphereCollider(r1->GetOwner()->GetChild<Transform>()->GetPosition() , Vector3(0, 0, 0), 1);
	BoxCollider* c2 = new BoxCollider(r2->GetOwner()->GetChild<Transform>()->GetPosition(), Vector3(0, 0, 0), r2->GetOwner()->GetChild<Transform>()->GetRotation(), Vector3(1.0f, 1.0f, 1.0f));
	//SphereCollider* c2 = new SphereCollider(r2->GetOwner()->GetChild<Transform>()->GetPosition() , Vector3(0, 0, 0), 1);
	PlaneCollider* c3 = new PlaneCollider(r3->GetOwner()->GetChild<Transform>()->GetPosition(), Vector3(0.0f, -6.0f, 0.0f),Vector3(0,1,0));

	gameObj[0].EditChild<Transform>()->EditPosition()[0] -= 1.90f;
	gameObj[0].EditChild<Transform>()->EditPosition()[1] -= 0.0f;
	gameObj[0].EditChild<Transform>()->EditPosition()[2] -= 0.0f;

	
	gameObj[1].EditChild<Transform>()->EditRotation()[0] = 0.6781f;
	gameObj[1].EditChild<Transform>()->EditRotation()[1] = 0.8563f;
	gameObj[1].EditChild<Transform>()->EditRotation()[2] = 0.7f;
	gameObj[1].EditChild<Transform>()->EditRotation()[3] = 0.024f;
	gameObj[1].EditChild<Transform>()->EditRotation().normalize();
	

	gameObj[0].AddChild<Collider>(*c1);
	gameObj[1].AddChild<Collider>(*c2);
	gameObj[2].AddChild<Collider>(*c3);

	gameObj[1].EditChild<RigidBody>()->ApplyForce(Vector3(0, 1, 0), Vector3(0, -1, 0));

	//w.addRigidBody(r1);
	w.addRigidBody(r2);
	w.addRigidBody(r3);

	/*---END TEST---*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Physic Engine");
//	glutFullScreen();

	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(processKeys);
	glutIdleFunc(renderIdleScene);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLfloat ambientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glutMainLoop();
	
	return 0;
}
