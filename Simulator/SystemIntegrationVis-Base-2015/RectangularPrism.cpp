#include"RectangularPrism.h"
#include<GL/glut.h>


RectangularPrism:: RectangularPrism (double x_, double y_, double z_, double Xlength, double Ylength, double Zlength, double rotation_) :Shape(x, y, z, rotation)
{
	x = x_;
	y = y_;
	z = z_;
	x_length = Xlength;
	y_length = Ylength;
	z_length = Zlength;
	rotation = rotation_;
	red = 0.0;
	green = 1.0;
	blue = 0.0;
} 


RectangularPrism:: RectangularPrism (double x_, double y_, double z_, double Xlength, double Ylength, double Zlength, double rotation_, float red_, float green_, float blue_) : Shape(x, y, z, rotation) //probably don't need rotation
{
	x = x_;
	y = y_;
	z = z_; 
	x_length = Xlength;
	y_length = Ylength;
	z_length = Zlength;
	rotation = rotation_;
	/*red = red_;
	green = green_;
	blue = blue_;*/
	setColor(red_, green_, blue_);
} 

double RectangularPrism::getXlength()
{
	return x_length;
}
double RectangularPrism::getYlength()
{
	return y_length;
}
double RectangularPrism::getZlength()
{
	return z_length;
}


void RectangularPrism::draw()
{
	
	glPushMatrix();
	glTranslatef(0, y_length/2, 0);
	glTranslatef(x, y, z);

	glRotatef(rotation, 0.0, 0.0, 1.0);// make sure rotation is on the correct axis
	
	glBegin(GL_QUADS);
	//bottom face
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(red, green, blue);
	glVertex3f(-x_length / 2, -y_length / 2, -z_length / 2);
	glVertex3f(-x_length / 2, -y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, -y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, -y_length / 2, -z_length / 2);

	//top face
	glColor3f(red, green, blue);
	glVertex3f(-x_length / 2, +y_length / 2, -z_length / 2);
	glVertex3f(-x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, -z_length / 2);

	//back face
	glColor3f(red, green, blue);
	glVertex3f(-x_length / 2, -y_length / 2, -z_length / 2);
	glVertex3f(-x_length / 2, +y_length / 2, -z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, -z_length / 2);
	glVertex3f(+x_length / 2, -y_length / 2, -z_length / 2);

	//front face
	glColor3f(red, green, blue);
	glVertex3f(-x_length / 2, -y_length / 2, +z_length / 2);
	glVertex3f(-x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, -y_length / 2, +z_length / 2);

	//left face
	glColor3f(red, green, blue);
	glVertex3f(-x_length / 2, -y_length / 2, -z_length / 2);
	glVertex3f(-x_length / 2, +y_length / 2, -z_length / 2);
	glVertex3f(-x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(-x_length / 2, -y_length / 2, +z_length / 2);

	//right face
	glColor3f(red, green, blue);
	glVertex3f(+x_length / 2, -y_length / 2, -z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, -z_length / 2);
	glVertex3f(+x_length / 2, +y_length / 2, +z_length / 2);
	glVertex3f(+x_length / 2, -y_length / 2, +z_length / 2);

	glEnd();
	glPopMatrix();

}