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
#include "WP_Matrix2D.h"
#include "WP_Math.h"

namespace WPCG
{
WP_Matrix2D::WP_Matrix2D()
{
  createIdentity();
}

WP_Matrix2D::WP_Matrix2D(const byte type, scalar x, scalar y)
{
  switch(type)
    {
    case TRANSLATION_MATRIX:
      data[0][0] = 1.0;
      data[0][1] = 0.0;
      data[0][2] = x;
      
      data[1][0] = 0.0;
      data[1][1] = 1.0;
      data[1][2] = y;
      
      data[2][0] = 0.0;
      data[2][1] = 0.0;
      data[2][2] = 1.0;
      break;
    case SCALING_MATRIX:
      data[0][0] = x;
      data[0][1] = 0.0;
      data[0][2] = 0.0;
      
      data[1][0] = 0.0;
      data[1][1] = y;
      data[1][2] = 0.0;
      
      data[2][0] = 0.0;
      data[2][1] = 0.0;
      data[2][2] = 1.0;
      break;
    default:
      createIdentity();
      break;
    }
}

WP_Matrix2D::WP_Matrix2D(const byte type, WP_Vector2D* v)
{
  WP_Matrix2D(type, v->data[0], v->data[1]);
}

WP_Matrix2D::WP_Matrix2D(scalar angle)
{
  if (angle != 0.0)
    {
      WP_Math* m = WP_Math::getInstance();
      
      data[0][0] = cos(m->fDegreeToRad(angle));
      data[0][1] = -sin(m->fDegreeToRad(angle));
      data[0][2] = 0.0;
      
      data[1][0] = sin(m->fDegreeToRad(angle));
      data[1][1] = cos(m->fDegreeToRad(angle));
      data[1][2] = 0.0;
      
      data[2][0] = 0.0;
      data[2][1] = 0.0;
      data[2][2] = 1.0;
    }
  else
    {
      createIdentity();
    }
}

WP_Matrix2D& WP_Matrix2D::operator=(const WP_Matrix2D& m)
{
  if (this == &m)
    return *this;

  for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
	{
	  data[i][j] = m.data[i][j];
	}
    }
  return *this;
}

WP_Matrix2D WP_Matrix2D::operator*(const WP_Matrix2D& m) const
{
  WP_Matrix2D temp = *this;
  temp *= m;
  return temp;
}

WP_Matrix2D WP_Matrix2D::operator*(scalar s) const
{
  WP_Matrix2D temp = *this;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.data[i][j] *= s;
  return temp;
}

WP_Matrix2D WP_Matrix2D::operator/(scalar s) const
{
  WP_Matrix2D temp = *this;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.data[i][j] /= s;
  return temp;
}

WP_Matrix2D WP_Matrix2D::operator-(const WP_Matrix2D& m) const
{
  WP_Matrix2D temp = *this;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.data[i][j] -= m.data[i][j];
  return temp;
}

WP_Matrix2D WP_Matrix2D::operator+(const WP_Matrix2D& m) const
{
  WP_Matrix2D temp = *this;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.data[i][j] += m.data[i][j];
  return temp;
}

WP_Matrix2D& WP_Matrix2D::operator-()
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i][j] = -data[i][j];
  
  return *this;
}

WP_Matrix2D& WP_Matrix2D::operator*=(const WP_Matrix2D& m)
{
  WP_Matrix2D matrix = *this;
  
  for (int i = 0; i < 3; i++)
    { 
      for (int j = 0; j < 3; j++)
	{
	  scalar sum = 0.0;
	  
	  for (int k = 0; k < 3; k++)
	    sum += matrix.data[i][k] * m.data[k][j];
	  
	  data[i][j] = sum;
	}
    }
  return *this;
}

WP_Matrix2D& WP_Matrix2D::operator*=(scalar s)
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i][j] *= s;
  
  return *this;
}


WP_Matrix2D& WP_Matrix2D::operator/=(scalar s)
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i][j] /= s;
  
  return *this;
}

WP_Matrix2D& WP_Matrix2D::operator+=(const WP_Matrix2D& m)
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i][j] += m.data[i][j];
  
  return *this;
}

WP_Matrix2D& WP_Matrix2D::operator-=(const WP_Matrix2D& m)
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i][j] -= m.data[i][j];
  
  return *this;
}

void WP_Matrix2D::createIdentity()
{
  data[0][0] = 1.0;
  data[0][1] = 0.0;
  data[0][2] = 0.0;
  
  data[1][0] = 0.0;
  data[1][1] = 1.0;
  data[1][2] = 0.0;
  
  data[2][0] = 0.0;
  data[2][1] = 0.0;
  data[2][2] = 1.0;
}
}
