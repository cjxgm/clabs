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

#ifndef WP_POINT2D_H
#define WP_POINT2D_H

#include "WP_Def.h"

namespace WPCG
{
class WP_Vector2D; //forward declaration

/**
 * this class represents a 2D point\n
 * @author Copyright (C) 2001 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
class WP_Point2D
{
public:
  WP_Point2D():x(0.0), y(0.0){};
  
  /**
   * @param _x the x coordinate of the point
   * @param _y the y coordinate of the point
   */
  WP_Point2D(scalar _x, scalar _y): x(_x), y(_y){};
  ~WP_Point2D(){};

  /**
   * this operator subtracts a point from this point resulting in a vector
   * @param p a WP_Point2D object representing a point which is subtracted from this object
   * @return a WP_Vector2D object representing the resulting 2D vector
   */
  WP_Vector2D operator-(const WP_Point2D& p) const;

  /**
   * this operator adds a vector to this point resulting in new point 
   * @param v a WP_Vector2D object representing the vector which is added to this object
   * @return a WP_Point2D object representing the resulting point 
   */
  WP_Point2D operator+(const WP_Vector2D& v) const;

  /**
   * this function draws the 2D point to the screen using OpenGL
   */
  void draw() const;

  /**
   * the x coordinate of this point
   */
  scalar x;

  /**
   * the y coordinate of this point
   */
  scalar y;
};
}
#endif

