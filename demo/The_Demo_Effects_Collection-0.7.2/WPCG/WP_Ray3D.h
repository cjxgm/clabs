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

#ifndef WP_RAY3D_H
#define WP_RAY3D_H

#include "WP_Def.h"
#include "WP_Point3D.h"
#include "WP_Vector3D.h"

namespace WPCG
{
/**
 * this class represents a 3D ray with for example can be used for raytracing\n
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
class WP_Ray3D
{
public:
	WP_Ray3D(){};
	
	/**
	 * @param p a WP_Point3D object representing the start point of the ray
	 * @param v a WP_Vector3D object representing the direction vector of the ray
	 */
	WP_Ray3D(const WP_Point3D& p, const WP_Vector3D& v);

	~WP_Ray3D(){};

	/**
	 * this function computes the intersection time of this ray and a sphere.See F.S. Hill's Computer Graphics using OpenGL, second edition, page 739 and 747
	 */
	float sphere_intersection(const WP_Matrix3D& m) const;

	/**
	 * this function creates the inverse transformed ray of this ray according to an inverse object's transformation matrix. See F.S. Hill's Computer Graphics using OpenGL, second edition, page 740
	 * @param inverse_m the inverse transformation matrix of the obect according to which the inverse transformed ray must be computed
	 * @return a WP_Ray3D object representing the inverse transformed ray
	 */
	WP_Ray3D createInverseTransformedRay(const WP_Matrix3D& inverse_m) const;
	
	WP_Ray3D createReflectedRay(const WP_Vector3D& n) const;

	/**
	 * this function draws the 3D ray to the screen using OpenGL
	 */
	void draw() const;

	/**
	 * a WP_Point3D object representing the start point of the 3D ray
	 */
	WP_Point3D start;

	/**
	 * a WP_Vector3D object representing the directional vector of this 3D ray
	 */
	WP_Vector3D direction;
};
}
#endif
