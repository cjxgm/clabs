
#include "Vector.h"
#include <cmath>

Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector Vector::operator + (Vector v)
{
	return Vector(x + v.x, y + v.y);
}

Vector Vector::operator - (Vector v)
{
	return Vector(x - v.x, y - v.y);
}

double Vector::operator * (Vector v)
{
	return x * v.x + y * v.y;
}

Vector Vector::operator * (double lambda)
{
	return Vector(x * lambda, y * lambda);
}

Vector Vector::operator / (double lambda)
{
	return Vector(x / lambda, y / lambda);
}

Vector Vector::operator % (double lambda)
{
	double m = this->mod();
	return Vector(x / m * lambda,
				  y / m * lambda);
}

Vector Vector::operator - ()
{
	return Vector(-x, -y);
}

double Vector::mod()
{
	return sqrt(x * x + y * y);
}

double Vector::cos(Vector v)
{
	return (*this) * v / this->mod() / v.mod();
}

double& Vector::operator [] (int idx)
{
	if (idx == 0)
		return x;
	else if (idx == 1)
		return y;
	else
		throw ErrVectorIndexOutOfBound(idx);
}

ostream& operator<<(ostream& o, Vector v)
{
	o << '(' << v.x << ", " << v.y << ')';
	return o;
}


ErrVectorIndexOutOfBound::ErrVectorIndexOutOfBound(int idx)
{
	cerr << endl << "\e[1;31m" << "Error: " << "\e[m"
		 << "Vector[" << idx << "], x = 0 or x = 1 only!"
		 << endl << endl;
}

