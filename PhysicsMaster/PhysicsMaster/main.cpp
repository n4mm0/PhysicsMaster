//GRAFICA FATTA DI MERDA SENZA SBATTI
#define _USE_MATH_DEFINES

#include <gl\GLUT.H>
#include <math.h>

int width = 600;
int height = 600;

void processKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		exit(EXIT_SUCCESS);
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

void drawSphere(float x, float y, float z, float radius, int lats, int longs)
{
	GLfloat blue[] = { 0.4f, 0.4f, 1.0f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);

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

void drawCube(float x, float y, float z, float lX, float lY, float lZ)
{
	GLfloat green[] = { 0.2f, 0.8f, 0.6f, 1.0f };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);

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

	drawFloor(6.0f, 20.0f);
	drawFloor(-6.0f, 20.0f);
	drawWallXZ(-20.0f, 6.0f, 20.0f);
	drawWallXZ(20.0f, 6.0f, 20.0f);
	drawCube(10.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f);
	drawSphere(-10.0f, 0.0f, 0.0f, 1.0f, 64, 64);

	glutSwapBuffers();
}

void renderIdleScene()
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Physic Engine");
	glutFullScreen();

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