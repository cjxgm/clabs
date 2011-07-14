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

#ifndef WP_RAY2D_H
#define WP_RAY2D_H

#include "WP_Def.h"
#include "WP_Point2D.h"
#include "WP_Vector2D.h"

namespace WPCG
{
class WP_Line2D; //forward declaration

/**
 * this class represents a 2D ray\n
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
class WP_Ray2D
{
public:
	WP_Ray2D(){};
	
	/**
	 * @param p a WP_Point2D object representing the start point of the ray
	 * @param v a WP_Vector2D object representing the direction vector of the ray
	 */
	WP_Ray2D(const WP_Point2D& p, const WP_Vector2D& v);

	/**
	 * @param l a WP_Line2D object representing a 2D line from which a 2D ray is constructed
	 */
	WP_Ray2D(WP_Line2D& l);

	~WP_Ray2D(){};

	/**
	 * this function computes the intersection point of this ray and the given ray. See Chapter 4, page 186, Computer Graphics using OpenGL, F.S. Hill
	 * @param r a WP_Ray2D object representing the ray which may intersect with this ray object
	 * @param p a WP_Point2D object which will be filled with the intersection point if one is found
	 * @return -2 in case the rays overlap\n -1 in case this ray exits from the ray <i>r</i> (compared to its normal)\n 0 in case this ray is parallel to ray <i>r</i>\n 1 in case this ray enters the ray <i>r</i> (compared to its normal)
	 */
	int intersection(const WP_Ray2D& r, WP_Point2D& p) const;

	/**
	 * this function draws the 2D ray to the screen using OpenGL
	 */
	void draw() const;

	/**
	 * a WP_Point2D object representing the start point of the 2D ray
	 */
	WP_Point2D start;

	/**
	 * a WP_Vector2D object representing the directional vector of this 2D ray
	 */
	WP_Vector2D direction;
};
}
#endif

