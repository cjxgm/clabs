
#include "Vec3.h"
#include <cmath>
using namespace CjLib;

Vec3::Vec3(const Vec3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vec3 & Vec3::operator=(const Vec3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vec3 Vec3::operator+(const Vec3 & v)
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 & v)
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator-()
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator*(const Vec3 & v)
{
	return Vec3(y*v.z - v.y*z,
				z*v.x - v.z*x,
				x*v.y - v.x*y);
}

Vec3 Vec3::operator*(float s)
{
	return Vec3(x * s, y * s, z * s);
}

Vec3 Vec3::operator/(float s)
{
	return Vec3(x / s, y / s, z / s);
}

float Vec3::dot(const Vec3 & v)
{
	return x*v.x + y*v.y + z*v.z;
}

float Vec3::length()
{
	return sqrtf(lengthSq());
}

float Vec3::lengthSq()
{
	return x*x + y*y + z*z;
}

void Vec3::normalize()
{
	float len = length();
	x /= len;
	y /= len;
	z /= len;
}

#ifdef DEBUG
ostream & operator<<(ostream & o, CjLib::Vec3 v)
{
	o << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	return o;
}
#endif

