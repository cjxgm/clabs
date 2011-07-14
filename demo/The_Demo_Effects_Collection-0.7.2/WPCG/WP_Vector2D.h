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

#ifndef WP_Vector2D_H
#define WP_Vector2D_H

#include "WP_Def.h"

namespace WPCG
{
//forward declarations
class WP_Matrix2D;

/**
 * this class represents a 2D vector\n
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
class WP_Vector2D
{
public:
  WP_Vector2D();
  
  /**
   * @param x the x component of the vector
   * @param y the y component of the vector
   */
  WP_Vector2D(scalar x, scalar y);

  ~WP_Vector2D(){};
  
  /**
   * assignment operator
   * @param v the WP_Vector2D object which is copied to this object
   * @return a reference to this object containing the copied vector values
   */
  WP_Vector2D& operator=(const WP_Vector2D& v);
  
  /**
   * this operator computes the dot product of the two 2D vectors
   * @param v a WP_Vector2D object representing a vector by which this vector is multiplied to get the dot product
   * @return the dot product
   */
  scalar operator*(const WP_Vector2D & v) const;

  /**
   * this operator scales the vector by multiplying its values by a scalar
   * @param s the scalar by which this vector is scaled
   * @return a WP_Vector2D object representing the scaled vector
   */
  WP_Vector2D operator*(scalar s) const; 

  /**
   * this operator scales the vector by dividing its values by a scalar
   * @param s the scalar by which this vector is scaled
   * @return a WP_Vector2D object representing the scaled vector
   */
  WP_Vector2D operator/(scalar s) const;

  /**
   * this operator subtracts a vector from this vector
   * @param v a WP_Vector2D object representing the vector which is subtracted from this vector
   * @return a WP_Vector2D object representing the vector after subtraction
   */
  WP_Vector2D operator-(const WP_Vector2D& v) const;

  /**
   * this operator adds a vector to this vector
   * @param v a WP_Vector2D object representing the vector which is added to this vector
   * @return a WP_Vector2D object representing the vector after addition
   */
  WP_Vector2D operator+(const WP_Vector2D& v) const;
  
  /**
   * this operator inverts the vector
   * @return a reference to this WP_Vector2D object containing the inverted values
   */
  WP_Vector2D& operator-();

  /**
   * this operator subtracts a vector from this vector and assigns the result to this vector
   * @param v a WP_Vector2D object representing the vector which is subtracted from this vector
   * @return a reference to this WP_Vector2D object containing the new values
   */
  WP_Vector2D& operator-=(const WP_Vector2D& v);
  
  /**
   * this operator adds a vector to this vector and assigns the result to this vector
   * @param v a WP_Vector2D object representing the vector which is added to this vector
   * @return a reference to this WP_Vector2D object containing the new values
   */
  WP_Vector2D& operator+=(const WP_Vector2D& v);

  /**
   * this operator scales this vector by using multiplication
   * @param s the scalar by which this vector is scaled
   * @return a reference to this WP_Vector2D object containing the new values
   */
  WP_Vector2D& operator*=(scalar s);

  /**
   * this operator scales this vector by using division
   * @param s the scalar by which this vector is scaled
   * @return a reference to this WP_Vector2D object containing the new values
   */
  WP_Vector2D& operator/=(scalar s);
  
  /**
   * this operator multiplies this vector by a 2D matrix
   * @param m a WP_Matrix2D object which represents the 2D matrix by which this vector is multiplied
   * @return a reference to this WP_Vector2D object containing the new values
   */ 
  WP_Vector2D& operator*=(const WP_Matrix2D& m);

   /**
   * this operator multiplies this vector by a 2D matrix
   * @param m a WP_Matrix2D object which represents the 2D matrix by which this vector is multiplied
   * @return a WP_Vector2D object containing the new values
   */ 
  WP_Vector2D operator*(const WP_Matrix2D& m) const;
  
  /**
   * this function computes the length of the vector
   * @return the lenght of the vector
   */
  scalar length() const;

  /**
   * this function normalizes the vector
   * @return the lenght of the vector <b>before</b> normalizing
   */
  scalar normalize();

  /**
   * this function makes the vector perpendicular to itself (or rotates the vector 90 degrees)
   */
  void perp();
  
  /**
   * this function computes the angle in degrees between two vectors
   * @param v a WP_Vector2D object representing the second vector for calculating the angle between two vectors
   * @return the angle in degrees
   */
  scalar getDegreesBetween(const WP_Vector2D& v) const;
  
  /**
   * this function computes the angle in radians between two vectors
   * @param v a WP_Vector2D object representing the second vector for calculating the angle between two vectors
   * @return the angle in radians
   */
  scalar getRadsBetween(const WP_Vector2D& v) const;

  /**
   * this function determines if two vectors are orthogonal (dot product is +- 0.0)
   * @param v a WP_Vector2D object representing the second vector for calculating if the two vectors are orthogonal
   * @return a boolean indicating if the two vectors are orthogonal
   */
  bool orthogonal(const WP_Vector2D& v) const;

  /**
   * this function computes the new vector after colliding to a line/plane/surface
   * @param v a WP_Vector2D object which represents the normal of the line/plane/surface against which this vector collides
   */
  void richochet(WP_Vector2D v);

  /**
   * this function draws the vector using OpenGL
   * @param startx the x position of the start of this vector
   * @param starty the y position of the start of this vector
   */
  void draw(scalar startx, scalar starty) const;
  
  /**
   * an array of 3 scalars holding the vector's values. data[0] = x, data[1] = y, data[2] = 0.0f
   */
  scalar data[3];
};
}
#endif

