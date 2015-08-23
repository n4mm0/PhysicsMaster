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
#include "GameObjectFactory.h"
#include "Utilities\ColliderUtil.h"

static const int GameObjectNumber = 4;


typedef void (*Draw)(const Transform &, const Collider &);

void DrawSphereF(const Transform& transform, const Collider& collider);
void DrawParallepipedF(const Transform& transform, const Collider& collider);
void DrawPlaneF(const Transform& transform, const Collider& collider){};

Draw DrawFunction[ColliderType::ColliderTypeListLength::value] = {DrawParallepipedF,DrawSphereF,DrawPlaneF};
GameObject gameObj[GameObjectNumber + 1];
World world;
GLfloat rotationGL1[16];

Transform *transform1 = gameObj[0].EditChild<Transform>();
Vector3 position1 = transform1->EditPosition();
Matrix4x4 rotationMatrix1 = transform1->GetRotationMatrix();

Transform *transform2 = gameObj[1].EditChild<Transform>();
Vector3 position2 = transform2->EditPosition();
Matrix4x4 rotationMatrix2 = transform2->GetRotationMatrix();

Transform *transform3 = gameObj[3].EditChild<Transform>();
Vector3 position3 = transform3->EditPosition();
Matrix4x4 rotationMatrix3 = transform3->GetRotationMatrix();

Transform *transform4 = gameObj[4].EditChild<Transform>();
Vector3 position4 = transform4->EditPosition();
Matrix4x4 rotationMatrix4 = transform4->GetRotationMatrix();


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
void MatrixToGLFloat(const Matrix4x4 &matrix, GLfloat *m)
{
	for (int index = 0; index < 16; ++index)
		m[index] = matrix.getElementAt((index / 4), (index % 4));
}

void drawSphere(const Vector3 &position, const Matrix4x4& rotation, float radius, int lats, int longs)
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

	for (float indexLat = 0; indexLat <= lats; ++indexLat)
	{
		float lat0 = M_PI * (-0.5f + (indexLat - 1.0f) / lats);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = M_PI * (-0.5f + indexLat / lats);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (float indexLng = 0; indexLng <= longs; ++indexLng)
		{
			float lng = 2.0f * M_PI * (indexLng - 1.0f) / longs;
			float x = cos(lng);
			float y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
	glPopMatrix();
}

void drawParallelepiped(const Vector3 &position,const Matrix4x4& rotation, float lX, float lY, float lZ)
{
	GLfloat green[] = { 0.2f, 0.8f, 0.6f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.getX(), position.getY(), position.getZ());
	MatrixToGLFloat(rotation, rotationGL1);
	glMultMatrixf(rotationGL1);

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
	for (float index = -dimension; index <= dimension; ++index)
	{
		glVertex3f(index, y, -dimension);
		glVertex3f(index, y, dimension);
	}
	for (float index = -dimension; index <= dimension; ++index)
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
	for (int i = 0; i < GameObjectNumber; ++i)
	{
		DrawFunction[ gameObj[i].GetChild<Collider>()->GetType() ] ( *(gameObj[i].GetChild<Transform>()),*(gameObj[i].GetChild<Collider>()) );
	}
}

void DrawSphereF(const Transform& transform, const Collider& collider)
{
	drawSphere(transform.GetPosition(), transform.GetRotationMatrix(), static_cast<const SphereCollider*>(&collider)->GetRadius(), 64, 64);
}

void DrawParallepipedF(const Transform& transform, const Collider& collider)
{
	const Vector3& Dim = static_cast<const BoxCollider*>(&collider)->GetHalfSize();
	drawParallelepiped(transform.GetPosition(), transform.GetRotationMatrix(), Dim[0], Dim[1], Dim[2]);
}





int main()
{
	/*---CREATING PHYSICS OBJ---*/
	
	PhysicsObjectFactory::CreatePhysicsCube(gameObj[0], Vector3(-1.0f, 10.0f, 0.0f), 0.0f, 0.0f, 0.0f, 10.0f, 0.3f, 10.0f, 0.6f, 0.3f, Vector3(1.0f, 1.0f, 1.0f));
	PhysicsObjectFactory::CreatePhysicsCube(gameObj[1], Vector3::Zero, -50.0f,10.0f, 0.0f, 10.0f, 0.3f, 10.0f, 0.6f, 0.3f, Vector3(1.0f, 1.0f, 1.0f));
	PhysicsObjectFactory::CreatePhysicsSphere(gameObj[2], Vector3(3.0f, 5.0f, 6.0f), 0.0f, 0.0f, 0.0f, 15.0f, 0.7f, 1.0f, 0.3f, 0.4f, 1.0f);
	PhysicsObjectFactory::CreatePhysicsSphere(gameObj[3], Vector3(1.0f, 10.0f, 8.0f), 0.0f, 0.0f, 0.0f, 10.0f, 0.7f, 1.0f, 0.3f, 0.4f, 1.0f);
	
	/*---END---*/
	
	//add terrain and bodies to world
	PhysicsObjectFactory::CreatePhysicsPlane(gameObj[GameObjectNumber-1], Vector3(0.0f, -6.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	for (int i = 0; i < GameObjectNumber; ++i)
	{
		world.AddRigidBody(gameObj[i].EditChild<RigidBody>());
	}
	
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
	//	position1 = gameObj[0].EditChild<Transform>()->EditPosition();
	//	position2 = gameObj[1].EditChild<Transform>()->EditPosition();
	//	position3 = gameObj[2].EditChild<Transform>()->EditPosition();
	//	position4 = gameObj[3].EditChild<Transform>()->EditPosition();
//
	//	rotationMatrix1 = gameObj[0].EditChild<Transform>()->GetRotationMatrix();
	//	rotationMatrix2 = gameObj[1].EditChild<Transform>()->GetRotationMatrix();
	//	rotationMatrix3 = gameObj[2].EditChild<Transform>()->GetRotationMatrix();
//		rotationMatrix4 = gameObj[3].EditChild<Transform>()->GetRotationMatrix();
	//	system("pause");
		glfwSwapBuffers(window);				//DOUBLE BUFFER
		glfwPollEvents();						//PROCESS PENDING EVENTS
		
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
