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
#include "WP_Vector2D.h"
#include "WP_Math.h"
#include "WP_Matrix2D.h"
#include "WP_Line2D.h"

namespace WPCG
{
WP_Vector2D::WP_Vector2D()
{ 
  data[0] = data[1] = data[2] = 0.0;
}

WP_Vector2D::WP_Vector2D(scalar x, scalar y)
{
  data[0] = x;
  data[1] = y;
  data[2] = 0.0;
}

WP_Vector2D& WP_Vector2D::operator=(const WP_Vector2D& v)
{
  if (this == &v)
    return *this;
  data[0] = v.data[0];
  data[1] = v.data[1];
  return *this;
}

//dot product
scalar WP_Vector2D::operator*(const WP_Vector2D& v) const
{
  return data[0] * v.data[0] + data[1] * v.data[1]; 
}

WP_Vector2D WP_Vector2D::operator*(scalar s) const
{
  WP_Vector2D temp;
  temp = *this;
  temp.data[0] *= s;
  temp.data[1] *= s;
  return temp;
}

WP_Vector2D WP_Vector2D::operator/(scalar s) const
{
  WP_Vector2D temp;
  temp = *this;
  temp.data[0] /= s;
  temp.data[1] /= s;
  return temp;
}

WP_Vector2D& WP_Vector2D::operator-()
{
  data[0] = -data[0];
  data[1] = -data[1];
  return *this;
}

WP_Vector2D WP_Vector2D::operator-(const WP_Vector2D& v) const
{
  WP_Vector2D temp;
  temp = *this;
  temp.data[0] -= v.data[0];
  temp.data[1] -= v.data[1];
  return temp;
}

WP_Vector2D WP_Vector2D::operator+(const WP_Vector2D& v) const
{
  WP_Vector2D temp;
  temp = *this;
  temp.data[0] += v.data[0];
  temp.data[1] += v.data[1];
  return temp;
}

WP_Vector2D& WP_Vector2D::operator*=(scalar f)
{
  data[0] *= f;
  data[1] *= f;
  return *this;
}

WP_Vector2D& WP_Vector2D::operator/=(scalar f)
{
  data[0] /= f;
  data[1] /= f;
  return *this;
}

WP_Vector2D& WP_Vector2D::operator*=(const WP_Matrix2D& m)
{
  WP_Vector2D v = *this;
  
  data[0] = m.data[0][0] * v.data[0] + m.data[0][1] * v.data[1];
  data[1] = m.data[1][0] * v.data[0] + m.data[1][1] * v.data[1];
  
  return *this;
}

WP_Vector2D WP_Vector2D::operator*(const WP_Matrix2D& m) const
{
  WP_Vector2D temp = *this;
  temp *= m;
  return temp;
}

WP_Vector2D& WP_Vector2D::operator -=(const WP_Vector2D& v)
{
  data[0] -= v.data[0];
  data[1] -= v.data[1];
  return *this;
}

WP_Vector2D& WP_Vector2D::operator +=(const WP_Vector2D& v)
{
  data[0] += v.data[0];
  data[1] += v.data[1];
  return *this;
}

scalar WP_Vector2D::length() const
{
  return sqrtf(data[0] * data[0] + data[1] * data[1]);
}

//returns old length
scalar WP_Vector2D::normalize()
{
  scalar l = length();
  data[0] /= l;
  data[1] /= l;
  return l;
}

void WP_Vector2D::perp()
{
  scalar temp = data[0];
  data[0] = -data[1];
  data[1] = temp;
}

scalar WP_Vector2D::getDegreesBetween(const WP_Vector2D& v) const
{
  WP_Math* m = WP_Math::getInstance();
  
  scalar dot = *this * v;
  dot /= length() * v.length();
  return m->fRadToDegree((scalar)acos(dot));
}

scalar WP_Vector2D::getRadsBetween(const WP_Vector2D& v) const
{
  scalar dot = *this * v;
  dot /= length() * v.length();
  return (scalar)acos(dot);
}

bool WP_Vector2D::orthogonal(const WP_Vector2D& v) const
{
  scalar dot = *this * v;
  return dot <= 0.00001 && dot >= -0.00001; 
}

void WP_Vector2D::richochet(WP_Vector2D normal)
{
  normal.normalize();
  WP_Vector2D v = *this;
  *this = v - ((normal * (v * normal)) * 2.0); 
}

void WP_Vector2D::draw(scalar startx, scalar starty) const
{
  WP_Line2D l(WP_Point2D(startx, starty), WP_Point2D(startx + data[0], starty + data[1]));
  l.draw();
}
}
