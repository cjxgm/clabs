/* Copyright (C) 2001 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <cmath>
#include "WP_Point3D.h"
#include "WP_Matrix3D.h"
#include "WP_Ray3D.h"

namespace WPCG
{
WP_Ray3D::WP_Ray3D(const WP_Point3D& p, const WP_Vector3D& v):start(p), direction(v){}

float WP_Ray3D::sphere_intersection(const WP_Matrix3D& m) const
{
  WP_Matrix3D inverse = m;
  inverse.inverse();
  
  WP_Ray3D inverse_transformed = createInverseTransformedRay(inverse);

  scalar A, B, C;

  WP_Vector3D start = inverse_transformed.start.toVector();

  A = inverse_transformed.direction * inverse_transformed.direction;
  B = start * inverse_transformed.direction;
  C = (start * start) - 1.0f;

  scalar discrim = B * B - A * C;

  if (discrim < 0.0)
    {
      //the ray mishes the sphere
      return 0.0;
    }

  scalar discRoot = sqrtf(discrim);

  scalar t1 = (-B - discRoot) / A;

  if (t1 > 0.0001)
    {
      return t1;
    }

  scalar t2 = (-B + discRoot) / A;

  if (t2 > 0.0001)
    {
      return t2;
    }

  return 0.0;
}

WP_Ray3D WP_Ray3D::createInverseTransformedRay(const WP_Matrix3D& inverse_m) const
{
  WP_Ray3D r = *this;

  //translate
  r.start *= inverse_m;
  r.direction *= inverse_m;
  return r;
}

WP_Ray3D WP_Ray3D::createReflectedRay(const WP_Vector3D& n) const
{
	return WP_Ray3D(start + direction, direction - (n * ((direction * n) * 2.0f)));	
}

void WP_Ray3D::draw() const
{
  WP_Point3D end = start + direction;

  glBegin(GL_LINES);
  glVertex3fv(start.data);
  glVertex3fv(end.data);
  glEnd();
}
}
