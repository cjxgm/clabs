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

#ifndef WP_DEF_H
#define WP_DEF_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
using namespace std;

namespace WPCG
{
  typedef unsigned char byte;
  typedef GLfloat scalar;
  const scalar DOUBLE_PI = 6.2831853071795;
  const scalar Pi = 3.1415926535897932384;
  const scalar HALF_PI = 1.57079632679;
  const scalar RADDEG  = 57.2957795130823208768;
  const scalar DEGRAD = 0.01745329251994329577;
  const byte TRANSLATION_MATRIX = 1;
  const byte SCALING_MATRIX = 2;
  const byte X_ROTATION_MATRIX = 3;
  const byte Y_ROTATION_MATRIX = 4;
  const byte Z_ROTATION_MATRIX = 5;
}
#endif



