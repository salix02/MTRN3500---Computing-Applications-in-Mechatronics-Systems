#ifndef RECTANGULAR_PRISM_H
#define RECTANGULAR_PRISM_H
#include "shape.hpp"
class RectangularPrism : public Shape
{
protected:
	double x_length;
	double y_length;
	double z_length;

public:
	RectangularPrism(double x_, double y_, double z_, double Xlength, double Ylength, double Zlength, double rotation_);
	RectangularPrism(double x_, double y_, double z_, double Xlength, double Ylength, double Zlength, double rotation_, float red_, float green_, float blue_);
	double getXlength();
	double getYlength();
	double getZlength();

	void draw();


	//void Move(double x, double y); //inherited move
};


#endif
