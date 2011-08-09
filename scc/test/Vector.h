
#ifndef VECTOR_H
#define VECTOR_H
using namespace std;
#include <iostream>

class Vector
{
public:
	double x, y;

	Vector(double x = 0.0, double y = 0.0);

	Vector operator + (Vector v);
	Vector operator - (Vector v);
	double operator * (Vector v);	// dot-product
	Vector operator * (double lambda);
	Vector operator / (double lambda);
	Vector operator % (double lambda);
	Vector operator - ();
	double& operator [] (int id);
	double mod();
	double cos(Vector v);
};

ostream& operator << (ostream& o, Vector v);

#endif

