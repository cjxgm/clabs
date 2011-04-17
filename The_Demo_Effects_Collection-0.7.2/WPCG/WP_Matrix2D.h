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

#ifndef WP_Matrix2D_H
#define WP_Matrix2D_H

#include "WP_Def.h"

namespace WPCG
{
/**
 * this class represents a 2D matrix\n
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
class WP_Matrix2D
{
public:
	friend class WP_Vector2D;

	WP_Matrix2D();
	~WP_Matrix2D(){};	

	/**
	 * @param type the type of the matrix. TRANSLATION_MATRIX for a translation matrix and SCALING_MATRIX for a scaling matrix. Any other value creates the identity matrix
	 * @param x the x value of the translation or scaling operation
	 * @param y the y value of the translation or scaling operation
	 */
	WP_Matrix2D(const byte type, scalar x, scalar y);

	/**
	 * @param type the type of the matrix. TRANSLATION_MATRIX for a translation matrix and SCALING_MATRIX for a scaling matrix. Any other value creates the identity matrix
	 * @param v a WP_Vector2D object containing the x and y values to be used for translation or scaling
	 */
	WP_Matrix2D(const byte type, WP_Vector2D* v);

	/**
	 * @param angle the angle in degrees of the rotation matrix
	 */
	WP_Matrix2D(scalar angle);

	/**
	 * assigment operator
	 * @param m the matrix containing the to be copied values
	 * @return a reference to this matrix containing the copied values
	 */
	WP_Matrix2D& operator=(const WP_Matrix2D& m);

	/**
	 * this operator postmultiplies the current matrix by m
	 * @param m the matrix which will be multiplied by the current matrix
	 * @return a new matrix containing the multiplication of the current matrix and m
	 */
	WP_Matrix2D operator*(const WP_Matrix2D & m) const;

	/**
	 * this operator adds m to the current matrix
	 * @param m the matrix which will be added to the current matrix
	 * @return a new matrix containing the addition of the current matrix and m
	 */
	WP_Matrix2D operator+(const WP_Matrix2D & m) const;

	/**
	 * this operator subtracts m from the current matrix
	 * @param m the matrix which will be subtracted from the current matrix
	 * @return a new matrix containing the subtraction of the current matrix and m
	 */
	WP_Matrix2D operator-(const WP_Matrix2D & m) const;
	
	/**
	 * this operator multiplies every matrix cell by a scalar
	 * @param s the scalar by which every matrix cell is multiplied
	 * @return a new matrix containing the multiplied cells
	 */
	WP_Matrix2D operator*(scalar s) const;

	/**
	 * this operator divides every matrix cell by a scalar
	 * @param s the scalar by which every matrix cell is divided
	 * @return a new matrix containing the divided cells
	 */
	WP_Matrix2D operator/(scalar s) const;

	/**
	 * this operator inverts every matrix cell
	 */
	WP_Matrix2D& operator-();

	/**
	 * this operator postmultiplies the current matrix by m and stores the result in this object
	 * @param m the matrix which will be multiplied by this matrix
	 * @return a reference to this object
	 */
	WP_Matrix2D& operator*=(const WP_Matrix2D & m);

	/**
	 * this operator adds m to the current matrix and stores the result in this object
	 * @param m the matrix which will be added to this matrix
	 * @return a reference to this object
	 */
	WP_Matrix2D& operator+=(const WP_Matrix2D & m);

	/**
	 * this operator subtracts m from this matrix and stores the result in this object
	 * @param m the matrix which will be subtracted from this matrix
	 * @return a reference to this object
	 */
	WP_Matrix2D& operator-=(const WP_Matrix2D & m);

	/**
	 * this operator multiplies each matrix cell by a scalar and stores the result in this object
	 * @param s the scalar by which every matrix cell is multiplied
	 * @return a reference to this object
	 */
	WP_Matrix2D& operator*=(scalar s);

	/**
	 * this operator divides each matrix cell by a scalar and stores the result in this object
	 * @param s the scalar by which every matrix cell is divided
	 * @return a reference to this object
	 */
	WP_Matrix2D& operator/=(scalar s);

	/**
	 * this function creates the identity matrix
	 */
	void createIdentity();

	/**
	 * an 2D array representing a 2D matrix holding 9 cells, 3 columns of 3 rows
	 */ 
	scalar data[3][3];
};
}
#endif

