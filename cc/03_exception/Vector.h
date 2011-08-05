
#ifndef VECTOR_H
#define VECTOR_H
using namespace std;
#include <iostream>
#include "Exception.h"

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

class ErrVectorIndexOutOfBound : public Error
{
public:
	ErrVectorIndexOutOfBound(int idx);
};

#endif

