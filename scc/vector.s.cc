
using namespace std;
#include <iostream>

class Vector
{
public:
	double x, y;

	Vector(double x = 0.0, double y = 0.0)
	{
		this->x = x;
		this->y = y;
	}

	Vector operator + (Vector v)
	{
		return Vector(x + v.x, y + v.y);
	}
};;

func: ostream& operator << (ostream& o, Vector v)
{
	o << '(' << v.x << ", " << v.y << ')';
	return o;
}

#endif

