
#include "Obstacle.hpp"
#include"RectangularPrism.h"
#include <cmath>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

Obstacle::Obstacle(double x_, double z_, double radius_)
	: Shape(x_, 0, z_) {
		blue = .5;

		radius = radius_;
};

Obstacle::Obstacle(double x_, double z_)
	: Shape(x_, 0, z_) {
		blue = .5;

};


void Obstacle::draw() {
	// student section
	static GLUquadric * myQuadric = gluNewQuadric();
	glPushMatrix();
	positionInGL();
	//void DrawAxis();
	//DrawAxis();
	setColorInGL();
	//glRotatef(-90, 0, 0, 1);
	glScalef(radius, radius, radius);	

	//make a wall here

	glRotatef(-90, 0, 1, 0);
	glTranslatef(-0.05, 0, 0);

	/*
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.1, 0.0, 0.0);
	glVertex3f(0.1, 6.0, 0.0);
	glVertex3f(0.0, 6.0, 0.0);
	glEnd(); */

	glLineWidth(0.1);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0, 5, 0);
	glEnd();

	/*RectangularPrism  wall(0, 0, 0, 0.1, 3, 0.1, 0);
	wall.draw();
	*/

	glPopMatrix();
};

bool Obstacle::equals(Obstacle & other) {

	double threshold = 0.001;

	return 
		(fabs(other.getX() - x) < threshold) &&
		(fabs(other.getZ() - z) < threshold);

};

double Obstacle::getRadius() {
	return radius;
}
