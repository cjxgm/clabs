
#ifndef VEC3_H
#define VEC3_H

namespace CjLib
{
	class Vec3
	{
	public:
		float x, y, z;

		inline Vec3() : x(0), y(0), z(0) {}
		inline Vec3(float vx, float vy, float vz)
				: x(vx), y(vy), z(vz) {}
		inline ~Vec3() {}
		Vec3(const Vec3 & v);

		Vec3 & operator=(const Vec3 & v);
		Vec3 operator+(const Vec3 & v);
		Vec3 operator-(const Vec3 & v);
		Vec3 operator-();
		Vec3 operator*(const Vec3 & v);
		Vec3 operator*(float s);
		Vec3 operator/(float s);

		/* TODO
		Vec3 & operator+=(Vec3 & v0, const Vec3 & v1);
		Vec3 & operator-=(Vec3 & v0, const Vec3 & v1);
		Vec3 & operator*=(Vec3 & v, float s);
		Vec3 & operator/=(Vec3 & v, float s);
		*/

		float dot(const Vec3 & v);

		inline float length();
		inline float lengthSq();
		inline void normalize();
	};
};

#ifdef DEBUG
#include <iostream>
using namespace std;
ostream & operator<<(ostream & o, CjLib::Vec3 v);
#endif

#endif

