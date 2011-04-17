/* Copyright (C) 2001-2004 W.P. van Paassen - peter@paassen.tmfweb.nl

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
#include "WP_Def.h"
#include "WP_ObjectManager.h"
#include "WP_Camera.h"

namespace WPCG
{
WP_Camera* WP_Camera::_instance = 0;

WP_Camera::WP_Camera(): fixed_object(0), follow_distance(10.0f), 
			follow_angleX(0), follow_angleY(0), follow_angleZ(0),
			normal_viewing_volume(true), math(WP_Math::getInstance()),
			state(WP_GLState::getInstance()),origin(0.0,0.0,0.0){}

void WP_Camera::setFrustumAndCamera(scalar _viewAngle, unsigned int width, unsigned int height, scalar _nearPlane, scalar _farPlane, 
				    const WP_Point3D& _eye, const WP_Point3D& _look, const WP_Vector3D& _up)
{
  viewAngle		= _viewAngle;
  aspectRatio		= ((float)width) / ((float)height);
  nearPlane		= _nearPlane;
  farPlane		= _farPlane;
  maxRay		= 2.0f * farPlane;
  screen_width = width;
  screen_height = height;
  
  state->projection();
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(viewAngle, aspectRatio, nearPlane, farPlane);
  state->modelview();

  eye = _eye;
  look = _look;
  up = _up;
  n = eye - look;
  u = up;

  if (!u.crossProduct(n))
    {
      cerr << "Unable to compute crossproduct, it's likely that vectors n and u are pointing in the same direction" << endl;
    }
  
  n.normalize();
  u.normalize();
  
  v = n;
  v.crossProduct(u); //no need to check if crossproduct failed because we know for sure that n and u have an angle of 0 degrees between them
  matrix = WP_Matrix3D(eye.toVector(), u, v, n);
  glLoadMatrixf(matrix.data);

  //create camera's frustum
  frust.top = nearPlane * tanf((PI / 180.0f) * (scalar)(viewAngle / 2.0f));
  frust.bottom = -frust.top;
  frust.right = frust.top * aspectRatio;
  frust.left = -frust.right;
  computeFrustum();
}

void WP_Camera::setPickingVolume(int width, int height, int x, int y) 
{
  if (normal_viewing_volume)
    {
      GLint viewport[4];
      
      state->projection();
      glPushMatrix();
      glLoadIdentity();
      
      glGetIntegerv(GL_VIEWPORT,viewport);
      gluPickMatrix(x, viewport[3] - y,
		    width , height, viewport);
      gluPerspective(viewAngle, aspectRatio, nearPlane, farPlane);
      state->modelview();
      normal_viewing_volume = false;
    }
}

void WP_Camera::computeFrustum()
{
  WP_Point3D topLeft = eye - n * nearPlane + v * frust.top + u * frust.left;  
  frust.topRight = topLeft + u * (frust.right - frust.left);
  WP_Point3D bottomLeft = topLeft - v * ( 2.0f * frust.top);
  WP_Point3D bottomRight = bottomLeft + u * (frust.right - frust.left);
  
  //compute frustum plane normals
  frust.normalLeft = topLeft - eye;
  frust.normalLeft.crossProduct(bottomLeft - eye);
  frust.normalLeft.normalize();
  frust.normalRight = bottomRight - eye;
  frust.normalRight.crossProduct(frust.topRight - eye);
  frust.normalRight.normalize();
  frust.normalTop = frust.topRight - eye;
  frust.normalTop.crossProduct(topLeft - eye);
  frust.normalTop.normalize();
  frust.normalBottom = bottomLeft - eye;
  frust.normalBottom.crossProduct(bottomRight - eye);
  frust.normalBottom.normalize();
  frust.normalNear = frust.topRight- bottomRight;
  frust.normalNear.crossProduct(topLeft - frust.topRight);
  frust.normalNear.normalize();
  frust.normalFar = frust.normalNear; 
  -frust.normalFar;
  frust.far = eye - n * farPlane;

  //set frustum planes
  frustum[0].Set(frust.normalRight.data[0], frust.normalRight.data[1], frust.normalRight.data[2], frust.normalRight * (origin-eye));
  frustum[1].Set(frust.normalLeft.data[0], frust.normalLeft.data[1], frust.normalLeft.data[2], frust.normalLeft * (origin-eye));
  frustum[2].Set(frust.normalBottom.data[0], frust.normalBottom.data[1], frust.normalBottom.data[2], frust.normalBottom * (origin-eye));
  frustum[3].Set(frust.normalTop.data[0], frust.normalTop.data[1], frust.normalTop.data[2], frust.normalTop * (origin-eye));
  frustum[4].Set(frust.normalFar.data[0], frust.normalFar.data[1], frust.normalFar.data[2], frust.normalFar * (origin- frust.far));
  frustum[5].Set(frust.normalNear.data[0], frust.normalNear.data[1], frust.normalNear.data[2], frust.normalNear * (origin-frust.topRight)); 
}

//first rotated around X(U), then Y(V) and finally Z(N)
void WP_Camera::rotate(scalar angleU, scalar angleV, scalar angleN)
{
  static WP_Vector3D temp;
  scalar cos;
  scalar sin;
  
  if (angleU != 0.0f)
    {
      cos = cosf(math->fDegreeToRad(angleU));
      sin = sinf(math->fDegreeToRad(angleU));
      
      temp = v;
      v = (temp * cos) + (n * sin);
      n = (temp * -sin) + (n * cos);
    }
  if (angleV != 0.0f)
    {
      cos = cosf(math->fDegreeToRad(angleV));
      sin = sinf(math->fDegreeToRad(angleV));
      
      temp = n;
      n = (temp * cos) + (u * sin);
      u = (temp * -sin) + (u * cos);
    }
  if (angleN != 0.0f)
    {
      cos = cosf(math->fDegreeToRad(angleN));
      sin = sinf(math->fDegreeToRad(angleN));
      
      temp = u;
      u = (temp * cos) + (v * sin);
      v = (temp * -sin) + (v * cos);
    }

  matrix = WP_Matrix3D(eye.toVector(), u, v, n);
  glLoadMatrixf(matrix.data);

  computeFrustum();
}

void WP_Camera::followObject()
{
  //FIXME only update if object changed orientation

  if (fixed_object)
    {
      WP_Point3D center(fixed_object->getXPos(), fixed_object->getYPos(), fixed_object->getZPos());
      
      //determine new eye point
      eye = center + (fixed_object->dir * follow_distance);

      bool followX = (follow_angleX % 360) != 0;  
      bool followY = (follow_angleY % 360) != 0;  

      if (followX || followY)
	{
	  //because the eye must be rotated about the center, the center must be translated to the origin by translating the eye by that translation
	  WP_Matrix3D translation(TRANSLATION_MATRIX, -center.data[0], -center.data[1], -center.data[2]);
	  eye *= translation;
	  
	  //rotate eye to be in the northern position, in this way the camera will be transformed correctly regardless the object its heading
	  WP_Matrix3D heading(Y_ROTATION_MATRIX, fixed_object->heading);
	  WP_Matrix3D pitch(X_ROTATION_MATRIX, -fixed_object->pitch);
	  eye *= heading;
	  eye *= pitch;
	  
	  if (followX)
	    {
	      WP_Matrix3D x_rotation(X_ROTATION_MATRIX, follow_angleX);
	      eye *= x_rotation;
	    }
	  if (followY)
	    {
	      WP_Matrix3D y_rotation(Y_ROTATION_MATRIX, follow_angleY);
	      eye *= y_rotation;
	    }
	  
	  //rotate object back in original orientation
	  heading = WP_Matrix3D(Y_ROTATION_MATRIX, -fixed_object->heading);
	  pitch = WP_Matrix3D(X_ROTATION_MATRIX, fixed_object->pitch);
	  eye *= pitch;
	  eye *= heading;
	  
	  //translate back to original orientation
	  translation = WP_Matrix3D(TRANSLATION_MATRIX, center.data[0], center.data[1], center.data[2]);
	  eye *= translation;
	}
      
      //adjust camera to face object
      
      n = eye - center;
      u = fixed_object->up;
      n.normalize();
      
      if (!u.crossProduct(n))
	{
	  //crossproduct failed because n and u have an angle of 0 degrees between them
	  //in this case crossproduct of fixed_object->dir and fixed_object->up results in correct vector
	  WP_Vector3D temp = fixed_object->dir;
	  temp.crossProduct(fixed_object->up);
	  u.normalize();
	  
	  if (u.data[0] != n.data[0] || u.data[1] != n.data[1] || u.data[2] != n.data[2])
	    {
	      //vectors are pointing in opposite direction
	      u = -temp;
	    }
	  else
	    {
	      //vectors are pointing in same direction
	      u = temp;
	    }
	}
      
      u.normalize();
      
      v = n;
      v.crossProduct(u); //no need to check if crossproduct failed because we no for sure that n and u are not pointing in the same direction
      rotate(0, 0, follow_angleZ);

      computeFrustum();
    }
}
}
