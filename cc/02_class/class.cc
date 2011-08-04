using namespace std;
#include <iostream>
#include "Vector.h"

int main()
{
	Vector v1(3, 4);
	Vector v2;

	cout << "v1 = " << v1 << endl
		 << "v2 = " << v2 << endl
		 << endl;
	cout << "v1 * 3 = " << v1 * 3 << endl
		 << "v2 + v1 = " << v2 + v1 << endl
		 << "-v1 = " << -v1 << endl
		 << endl;
	
	v2 = v1 + Vector(0.2, 3);
	cout << "v2 = v1 + (0.2, 3), v1 = "
		 << v1 << ", v2 = " << v2 << endl;
	cout << "v1 - v2 = " << v1 - v2 << endl
		 << "|v1| = " << v1.mod() << endl
		 << "|v2| = " << v2.mod() << endl
		 << "normalize(v1) = " << v1 % 1
		 	<< ", |normalize(v1)| = " << (v1 % 1).mod() << endl
		 << "normalize(v2) = " << v2 % 1
		 	<< ", |normalize(v2)| = " << (v2 % 1).mod() << endl
		 << "strench(v1, 5) = " << v1 % 5
		 	<< ", |strench(v1, 5)| = " << (v1 % 5).mod() << endl
		 << "strench(v2, 6) = " << v2 % 6
		 	<< ", |strench(v2, 6)| = " << (v2 % 6).mod() << endl
		 << endl;
	
	cout << "cos(v1, v2) = " << v1.cos(v2)
		 	<< " = " << v2.cos(v1) << endl
		 << endl;
	return 0;
}
