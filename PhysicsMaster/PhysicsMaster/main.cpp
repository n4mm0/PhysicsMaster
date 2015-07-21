#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "RigidBody.h"
#include "Singleton.h"
#include "World.h"

GameObject gameObj[3];

Transform *transform1 = gameObj[0].EditChild<Transform>();
Vector3 position1 = transform1->EditPosition();
Matrix4x4 rotationMatrix1 = transform1->GetRotationMatrix();
GLfloat rotationGL1[16];

Transform *transform2 = gameObj[1].EditChild<Transform>();
Vector3 position2 = transform2->EditPosition();
Matrix4x4 rotationMatrix2 = transform2->GetRotationMatrix();
GLfloat rotationGL2[16];

World world;

//CALLBACK
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static bool stopped = false;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
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
	}
}

//FUNCTION
void MatrixToGLFloat(Matrix4x4 &matrix, GLfloat *m)
{
	for (int index = 0; index < 16; ++index)
		m[index] = matrix.getElementAt((index / 4), (index % 4));
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
	glMultMatrixf(rotationGL2);

	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 24);

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
	for (int index = -dimension; index <= dimension; ++index)
	{
		glVertex3f(-dimension, y, index);
		glVertex3f(dimension, y, index);
	}
	glEnd();
}

void render()
{
	GLfloat aLite[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat dLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat sLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat posLight[4] = { 1, 2, 1, 0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, posLight);

	drawFloor(-6.0f, 20.0f);
	
	drawParallelepiped(position1, rotationMatrix1, 1.0f, 1.0f, 1.0f);
	//drawSphere(position1, rotationMatrix1, 1.0f, 64, 64);
	drawParallelepiped(position2, rotationMatrix2, 1.0f, 1.0f, 1.0f);
	//drawSphere(position2, rotationMatrix2, 1.0f, 64, 64);
}

Vector3 CubeInertia(float mass,float semidim)
{
	return Vector3((2.0f*(semidim*semidim)) * mass / 12.0f, (2.0f*(semidim*semidim)) * mass / 12.0f, (2.0f*(semidim*semidim)) * mass / 12.0f);
}

int main()
{
	/*---TEST---*/
	RigidBody* r1 = new RigidBody(CubeInertia(10.0f,1.0f), 10.0f, 0, true);
	RigidBody* r2 = new RigidBody(CubeInertia(10.0f, 1.0f), 10.0f, 1, true);
	RigidBody* r3 = new RigidBody(Vector3(1, 1, 1), 1.0f, 2, false);

	gameObj[0].AddChild<RigidBody>(*r1);
	gameObj[1].AddChild<RigidBody>(*r2);
	gameObj[2].AddChild<RigidBody>(*r3);

	BoxCollider* c1 = new BoxCollider(r1->GetOwner()->GetChild<Transform>()->GetPosition(), Vector3(0, 0, 0), r1->GetOwner()->GetChild<Transform>()->GetRotation(), Vector3(1.0f, 1.0f, 1.0f));
	//SphereCollider* c1 = new SphereCollider(r1->GetOwner()->GetChild<Transform>()->GetPosition() , Vector3(0, 0, 0), 1);
	BoxCollider* c2 = new BoxCollider(r2->GetOwner()->GetChild<Transform>()->GetPosition(), Vector3(0, 0, 0), r2->GetOwner()->GetChild<Transform>()->GetRotation(), Vector3(1.0f, 1.0f, 1.0f));
	//SphereCollider* c2 = new SphereCollider(r2->GetOwner()->GetChild<Transform>()->GetPosition() , Vector3(0, 0, 0), 1);
	PlaneCollider* c3 = new PlaneCollider(r3->GetOwner()->GetChild<Transform>()->GetPosition(), Vector3(0.0f, -6.0f, 0.0f),Vector3(0,1,0));

	gameObj[0].EditChild<Transform>()->EditPosition()[0] -= 1.90f;
	gameObj[0].EditChild<Transform>()->EditPosition()[1] -= 0.0f;
	gameObj[0].EditChild<Transform>()->EditPosition()[2] -= 0.0f;

	gameObj[0].EditChild<Transform>()->EditRotation()[0] = 0.11f;
	gameObj[0].EditChild<Transform>()->EditRotation()[1] = 0.833f;
	gameObj[0].EditChild<Transform>()->EditRotation()[2] = 0.23f;
	gameObj[0].EditChild<Transform>()->EditRotation()[3] = 0.1f;
	gameObj[0].EditChild<Transform>()->EditRotation().normalize();
	
	gameObj[1].EditChild<Transform>()->EditRotation()[0] = 0.6781f;
	gameObj[1].EditChild<Transform>()->EditRotation()[1] = 0.8563f;
	gameObj[1].EditChild<Transform>()->EditRotation()[2] = 0.7f;
	gameObj[1].EditChild<Transform>()->EditRotation()[3] = 0.024f;
	gameObj[1].EditChild<Transform>()->EditRotation().normalize();
	
	gameObj[0].AddChild<Collider>(*c1);
	gameObj[1].AddChild<Collider>(*c2);
	gameObj[2].AddChild<Collider>(*c3);

//	gameObj[1].EditChild<RigidBody>()->ApplyForce(Vector3(0, 1, 0), Vector3(0, -1, 0));

//	world.addRigidBody(r1);
	world.addRigidBody(r2);
	world.addRigidBody(r3);

	/*---END TEST---*/
	
	/*---GLFW---*/

	glfwSetErrorCallback(error_callback);		//SET THE ERROR CALLBACK

	//INIT GLFW
	if (!glfwInit())
		exit(GLFW_NOT_INITIALIZED);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//HINT FOR WINDOW AND CONTEXT
	glfwWindowHint(GLFW_SAMPLES, 8);					//MSAA
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Physics", monitor, nullptr);	//OPEN FULLSCREEN WINDOW AND CREATE OpenGL CONTEXT

	GLFWwindow *window = glfwCreateWindow(800, 600, "Physic Engine", nullptr, nullptr);		//OPEN WINDOW AND CREATE OpenGL CONTEXT
	if (!window)
	{
		glfwTerminate();
		exit(GLFW_NO_CURRENT_CONTEXT);
	}

	glfwMakeContextCurrent(window);				//MAKE THE CONTEXT AS CURRENT
	
	//INIT GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);	//INPUT

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		float ratio = ratio = width / (float)height;
		glViewport(0, 0, width, height);
				
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		GLfloat ambientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, ratio, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -20.0f);
		
		render();								//RENDER SCENE

		world.Update();
		position1 = gameObj[0].EditChild<Transform>()->EditPosition();
		position2 = gameObj[1].EditChild<Transform>()->EditPosition();
		//gameObj[1].EditChild<RigidBody>()->ApplyForce(Vector3(1, 1, 1), Vector3(5, 10, 2));

		rotationMatrix1 = gameObj[0].EditChild<Transform>()->GetRotationMatrix();
		rotationMatrix2 = gameObj[1].EditChild<Transform>()->GetRotationMatrix();
		//system("pause");

		glfwSwapBuffers(window);				//DOUBLE BUFFER
		glfwPollEvents();						//PROCESS PENDING EVENTS
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}