
#include "Vector/Vec3.h"
#include <iostream>
using namespace std;
using namespace CjLib;

int main()
{
	Vec3 v1(1, 2, 0), v2(3, 4, 0);
	cout << v1.dot(v2) << endl;
	cout << v1*v2 << endl;
	cout << v2*v1 << endl;
	return 0;
}

