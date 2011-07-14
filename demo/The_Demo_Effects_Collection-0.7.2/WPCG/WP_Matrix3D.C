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

#include <iostream>
#include <cmath>
#include "WP_Vector3D.h"
#include "WP_Matrix3D.h"
#include "WP_Math.h"

namespace WPCG
{
WP_Matrix3D::WP_Matrix3D()
{
  createIdentity();
}

WP_Matrix3D::WP_Matrix3D(const byte type, scalar x, scalar y, scalar z)
{
  switch(type)
    {
    case TRANSLATION_MATRIX:
      data[0] = 1.0;
      data[1] = 0.0;
      data[2] = 0.0;
      data[3] = 0.0;
      
      data[4] = 0.0;
      data[5] = 1.0;
      data[6] = 0.0;
      data[7] = 0.0;
      
      data[8] = 0.0;
      data[9] = 0.0;
      data[10] = 1.0;
      data[11] = 0.0;
      
      data[12] = x;
      data[13] = y;
      data[14] = z;
      data[15] = 1.0;
      break;
    case SCALING_MATRIX:
      data[0] = x;
      data[1] = 0.0;
      data[2] = 0.0;
      data[3] = 0.0;
      
      data[4] = 0.0;
      data[5] = y;
      data[6] = 0.0;
      data[7] = 0.0;
      
      data[8] = 0.0;
      data[9] = 0.0;
      data[10] = z;
      data[11] = 0.0;
      
      data[12] = 0.0;
      data[13] = 0.0;
      data[14] = 0.0;
      data[15] = 1.0;
      break;
    default:
      createIdentity();
      break;
    }
}

WP_Matrix3D::WP_Matrix3D(const byte type, WP_Vector3D* v)
{
  WP_Matrix3D(type, v->data[0], v->data[1], v->data[2]);
}

WP_Matrix3D::WP_Matrix3D(const byte type, scalar angle)
{
  switch(type)
    {
    case X_ROTATION_MATRIX:
      createXRotation(angle);
      break;
    case Y_ROTATION_MATRIX:
      createYRotation(angle);
      break;
    case Z_ROTATION_MATRIX:
      createZRotation(angle);
      break;
    default:
      createIdentity();
      break;
    }
}

WP_Matrix3D::WP_Matrix3D(WP_Vector3D eye, const WP_Vector3D& u,
	const WP_Vector3D& v , const WP_Vector3D& n)
{
  eye = -eye;
  
  data[0] = u.data[0];
  data[1] = v.data[0];
  data[2] = n.data[0];
  data[3] = 0.0;
  data[4] = u.data[1];
  data[5] = v.data[1];
  data[6] = n.data[1];
  data[7] = 0.0;
  data[8] = u.data[2];
  data[9] = v.data[2];
  data[10] = n.data[2];
  data[11] = 0.0;
  data[12] = eye * u;
  data[13] = eye * v;
  data[14] = eye * n;
  data[15] = 1.0;
}

WP_Matrix3D::WP_Matrix3D(scalar f[4][4])
{
  data[0] = f[0][0];
  data[1] = f[0][1];
  data[2] = f[0][2];
  data[3] = f[0][3];
  data[4] = f[1][0];
  data[5] = f[1][1];
  data[6] = f[1][2];
  data[7] = f[1][3];
  data[8] = f[2][0];
  data[9] = f[2][1];
  data[10] = f[2][2];
  data[11] = f[2][3];
  data[12] = f[3][0];
  data[13] = f[3][1];
  data[14] = f[3][2];
  data[15] = f[3][3];
}

WP_Matrix3D::WP_Matrix3D(scalar* f)
{
  data[0] = f[0];
  data[1] = f[1];
  data[2] = f[2];
  data[3] = f[3];
  data[4] = f[4];
  data[5] = f[5];
  data[6] = f[6];
  data[7] = f[7];
  data[8] = f[8];
  data[9] = f[9];
  data[10] = f[10];
  data[11] = f[11];
  data[12] = f[12];
  data[13] = f[13];
  data[14] = f[14];
  data[15] = f[15];
}

WP_Matrix3D& WP_Matrix3D::operator*=(const WP_Matrix3D& m)
{
  //postmultiply
  WP_Matrix3D matrix = *this;
  
  data[0] = matrix.data[0] * m.data[0] + matrix.data[4] * m.data[1] + 
    matrix.data[8] * m.data[2] + matrix.data[12] * m.data[3];
  data[4] = matrix.data[0] * m.data[4] + matrix.data[4] * m.data[5] + 
    matrix.data[8] * m.data[6] + matrix.data[12] * m.data[7];
  data[8] = matrix.data[0] * m.data[8] + matrix.data[4] * m.data[9] + 
    matrix.data[8] * m.data[10] + matrix.data[12] * m.data[11];
  data[12] = matrix.data[0] * m.data[12] + matrix.data[4] * m.data[13] + 
    matrix.data[8] * m.data[14] + matrix.data[12] * m.data[15];
  data[1] = matrix.data[1] * m.data[0] + matrix.data[5] * m.data[1] + 
    matrix.data[9] * m.data[2] + matrix.data[13] * m.data[3];
  data[5] = matrix.data[1] * m.data[4] + matrix.data[5] * m.data[5] + 
    matrix.data[9] * m.data[6] + matrix.data[13] * m.data[7];
  data[9] = matrix.data[1] * m.data[8] + matrix.data[5] * m.data[9] + 
    matrix.data[9] * m.data[10] + matrix.data[13] * m.data[11];
  data[13] = matrix.data[1] * m.data[12] + matrix.data[5] * m.data[13] + 
    matrix.data[9] * m.data[14] + matrix.data[13] * m.data[15];
  data[2] = matrix.data[2] * m.data[0] + matrix.data[6] * m.data[1] + 
    matrix.data[10] * m.data[2] + matrix.data[14] * m.data[3];
  data[6] = matrix.data[2] * m.data[4] + matrix.data[6] * m.data[5] + 
    matrix.data[10] * m.data[6] + matrix.data[14] * m.data[7];
  data[10] = matrix.data[2] * m.data[8] + matrix.data[6] * m.data[9] + 
    matrix.data[10] * m.data[10] + matrix.data[14] * m.data[11];
  data[14] = matrix.data[2] * m.data[12] + matrix.data[6] * m.data[13] + 
    matrix.data[10] * m.data[14] + matrix.data[14] * m.data[15];
  data[3] = matrix.data[3] * m.data[0] + matrix.data[7] * m.data[1] + 
    matrix.data[11] * m.data[2] + matrix.data[15] * m.data[3];
  data[7] = matrix.data[3] * m.data[4] + matrix.data[7] * m.data[5] + 
    matrix.data[11] * m.data[6] + matrix.data[15] * m.data[7];
  data[11] = matrix.data[3] * m.data[8] + matrix.data[7] * m.data[9] + 
    matrix.data[11] * m.data[10] + matrix.data[15] * m.data[11];
  data[15] = matrix.data[3] * m.data[12] + matrix.data[7] * m.data[13] + 
    matrix.data[11] * m.data[14] + matrix.data[15] * m.data[15];
  return *this;
}

void WP_Matrix3D::preMultiply(const WP_Matrix3D& m)
{
  //premultiply
  WP_Matrix3D matrix = *this;
  
  data[0] = m.data[0] * matrix.data[0] + m.data[4] * matrix.data[1] + 
    m.data[8] * matrix.data[2] + m.data[12] * matrix.data[3];
  data[4] = m.data[0] * matrix.data[4] + m.data[4] * matrix.data[5] + 
    m.data[8] * matrix.data[6] + m.data[12] * matrix.data[7];
  data[8] = m.data[0] * matrix.data[8] + m.data[4] * matrix.data[9] + 
    m.data[8] * matrix.data[10] + m.data[12] * matrix.data[11];
  data[12] = m.data[0] * matrix.data[12] + m.data[4] * matrix.data[13] + 
    m.data[8] * matrix.data[14] + m.data[12] * matrix.data[15];
  data[1] = m.data[1] * matrix.data[0] + m.data[5] * matrix.data[1] + 
    m.data[9] * matrix.data[2] + m.data[13] * matrix.data[3];
  data[5] = m.data[1] * matrix.data[4] + m.data[5] * matrix.data[5] + 
    m.data[9] * matrix.data[6] + m.data[13] * matrix.data[7];
  data[9] = m.data[1] * matrix.data[8] + m.data[5] * matrix.data[9] + 
    m.data[9] * matrix.data[10] + m.data[13] * matrix.data[11];
  data[13] = m.data[1] * matrix.data[12] + m.data[5] * matrix.data[13] + 
    m.data[9] * matrix.data[14] + m.data[13] * matrix.data[15];
  data[2] = m.data[2] * matrix.data[0] + m.data[6] * matrix.data[1] + 
    m.data[10] * matrix.data[2] + m.data[14] * matrix.data[3];
  data[6] = m.data[2] * matrix.data[4] + m.data[6] * matrix.data[5] + 
    m.data[10] * matrix.data[6] + m.data[14] * matrix.data[7];
  data[10] = m.data[2] * matrix.data[8] + m.data[6] * matrix.data[9] + 
    m.data[10] * matrix.data[10] + m.data[14] * matrix.data[11];
  data[14] = m.data[2] * matrix.data[12] + m.data[6] * matrix.data[13] + 
    m.data[10] * matrix.data[14] + m.data[14] * matrix.data[15];
  data[3] = m.data[3] * matrix.data[0] + m.data[7] * matrix.data[1] + 
    m.data[11] * matrix.data[2] + m.data[15] * matrix.data[3];
  data[7] = m.data[3] * matrix.data[4] + m.data[7] * matrix.data[5] + 
    m.data[11] * matrix.data[6] + m.data[15] * matrix.data[7];
  data[11] = m.data[3] * matrix.data[8] + m.data[7] * matrix.data[9] + 
    m.data[11] * matrix.data[10] + m.data[15] * matrix.data[11];
  data[15] = m.data[3] * matrix.data[12] + m.data[7] * matrix.data[13] + 
    m.data[11] * matrix.data[14] + m.data[15] * matrix.data[15];
}

void WP_Matrix3D::transpose()
{
  WP_Matrix3D m = *this;
  data[1] = m.data[4];
  data[2] = m.data[8];
  data[3] = m.data[12];
  data[4] = m.data[1];
  data[6] = m.data[9];
  data[7] = m.data[13];
  data[8] = m.data[2];
  data[9] = m.data[6];
  data[11] = m.data[14];
  data[12] = m.data[3];
  data[13] = m.data[7];
  data[14] = m.data[11];
}

bool WP_Matrix3D::inverse()
{
  //using Cramer's law, Hill, Computer Graphics using OpenGL page 825
  scalar determinant = this->determinant();

  if (determinant < 0.0001 && determinant > -0.0001)
    {
      //no inverse possible
      return false;
    }
  WP_Matrix3D cofactors;
  
  // if (isHomogenous())
  //  {
      cofactors.data[0] = data[5] * data[10] + (-data[6] * data[9]);
      cofactors.data[1] = -(data[4] * data[10] + (-data[6] * data[8]));
      cofactors.data[2] = data[4] * data[9] + (-data[5] * data[8]);
      cofactors.data[3] = -((data[4] * (data[9] * data[14] - data[13] * data[10])) + (-data[5] * (data[8] * data[14] - data[10] * data[12]))
			    + (data[6] * (data[8] * data[13] - data[9] * data[12])));
      
      cofactors.data[4] = -(data[1] * data[10] + (-data[2] * data[9]));
      cofactors.data[5] = data[0] * data[10] + (-data[2] * data[8]);
      cofactors.data[6] = -(data[0] * data[9] + (-data[1] * data[8]));
      cofactors.data[7] = (data[0] * (data[9] * data[14] - data[10] * data[13])) + (-data[1] * (data[8] * data[14] - data[10] * data[12])) + 
	(data[2] * (data[8] * data[13] - data[9] * data[12]));
      
      cofactors.data[8] = data[1] * data[6] + (-data[2] * data[5]);
      cofactors.data[9] = -(data[0] * data[6] + (-data[2] * data[4]));
      cofactors.data[10] = data[0] * data[5] + (-data[1] * data[4]);
      cofactors.data[11] = -((data[0] * (data[5] * data[14] - data[6] * data[13])) + (-data[1] * (data[4] * data[14] - data[6] * data[12])) +
			     (data[2] * (data[4] * data[13] - data[5] * data[12])));
      
      cofactors.data[12] = 0.0f;
      cofactors.data[13] = 0.0f;
      cofactors.data[14] = 0.0f;
      
      cofactors.data[15] = (data[0] * (data[5] * data[10] - data[6] * data[9])) + (-data[1] * (data[4] * data[10] - data[6] * data[8])) + 
	(data[2] * (data[4] * data[9] - data[5] * data[8]));
      
      determinant = data[15] * cofactors.data[15];
      
      cofactors.transpose();
      
      cofactors *= (scalar)(1.0f / determinant);
      
      *this = cofactors;
      //  }
      //else
      // {
      //return false; //FIXME implement inverse for non homogenous matrices
      //}
  return true;
}

bool WP_Matrix3D::isIdentity()
{
  if (data[0] != 1.0)
    return false;
  if (data[5] != 1.0)
    return false;
  if (data[10] != 1.0)
    return false;

  if (data[1] != 0.0)
    return false;
  if (data[2] != 0.0)
    return false;
 
 if (data[4] != 0.0)
    return false;
  if (data[6] != 0.0)
    return false;
 
 if (data[8] != 0.0)
    return false;
  if (data[9] != 0.0)
    return false;

 if (data[12] != 0.0)
    return false;
  if (data[13] != 0.0)
    return false;
  if (data[14] != 0.0)
    return false;

 if (data[3] != 0.0)
   return false; 
 if (data[7] != 0.0)
   return false;
 if (data[11] != 0.0)
   return false;

 return data[15] == 1.0;
}

void WP_Matrix3D::createIdentity()
{
  data[0] = 1.0;
  data[1] = 0.0;
  data[2] = 0.0;
  data[3] = 0.0;
  
  data[4] = 0.0;
  data[5] = 1.0;
  data[6] = 0.0;
  data[7] = 0.0;
  
  data[8] = 0.0;
  data[9] = 0.0;
  data[10] = 1.0;
  data[11] = 0.0;
  
  data[12] = 0.0;
  data[13] = 0.0;
  data[14] = 0.0;
  data[15] = 1.0;
}

void WP_Matrix3D::createXRotation(scalar angle)
{
  angle = WP_Math::getInstance()->fDegreeToRad(angle);

  data[0] = 1.0;
  data[1] = 0.0;
  data[2] = 0.0;
  data[3] = 0.0;
  
  data[4] = 0.0;
  data[5] = cos(angle);
  data[6] = sin(angle);
  data[7] = 0.0;
  
  data[8] = 0.0;
  data[9] = -sin(angle);
  data[10] = cos(angle);
  data[11] = 0.0;
  
  data[12] = 0.0;
  data[13] = 0.0;
  data[14] = 0.0;
  data[15] = 1.0;
}

void WP_Matrix3D::createYRotation(scalar angle)
{
  angle = WP_Math::getInstance()->fDegreeToRad(angle);

  data[0] = cos(angle);
  data[1] = 0.0;
  data[2] = -sin(angle);
  data[3] = 0.0;
  
  data[4] = 0.0;
  data[5] = 1.0;
  data[6] = 0.0;
  data[7] = 0.0;
  
  data[8] = sin(angle);
  data[9] = 0.0;
  data[10] = cos(angle);
  data[11] = 0.0;
  
  data[12] = 0.0;
  data[13] = 0.0;
  data[14] = 0.0;
  data[15] = 1.0;
}

void WP_Matrix3D::createZRotation(scalar angle)
{
  angle = WP_Math::getInstance()->fDegreeToRad(angle);

  data[0] = cos(angle);
  data[1] = sin(angle);
  data[2] = 0.0;
  data[3] = 0.0;
  
  data[4] = -sin(angle);
  data[5] = cos(angle);
  data[6] = 0.0;
  data[7] = 0.0;
  
  data[8] = 0.0;
  data[9] = 0.0;
  data[10] = 1.0;
  data[11] = 0.0;
  
  data[12] = 0.0;
  data[13] = 0.0;
  data[14] = 0.0;
  data[15] = 1.0;
}

void WP_Matrix3D::print() const
{
  cout << data[0] << "\t" << data[4] << "\t " << data[8] << "\t " << data[12] << endl;
  cout << data[1] << "\t " << data[5] << "\t " << data[9] << "\t " << data[13] << endl;
  cout << data[2] << "\t " << data[6] << "\t " << data[10] << "\t" << data[14] << endl;
  cout << data[3] << "\t " << data[7] << "\t " << data[11] << "\t " << data[15] << endl << endl;
}
}


