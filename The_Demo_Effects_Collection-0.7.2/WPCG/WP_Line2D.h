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

#ifndef WP_LINE2D_H
#define WP_LINE2D_H

#include "WP_Def.h"
#include "WP_Point2D.h"
#include "WP_Vector2D.h"

namespace WPCG
{
/**
 * this class represents a 2D line\n
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
class WP_Line2D
{
public:
	WP_Line2D(){};

	/**
	 * @param p1 a WP_Point2D object indicating the start point of the line
	 * @param p2 a WP_Point2D object indicating the end point of the line
	 */
	WP_Line2D(const WP_Point2D& p1, const WP_Point2D& p2);
	~WP_Line2D();

	/**
	 * this function draws the line to the screen using OpenGL
	 */
	void draw();

	/**
	 * this function computes the length of the line
	 * @return a scalar indicating the length of the line
	 */
	scalar length() const;

	/**
	 * a WP_Point2D object indicating the start of the line
	 */
	WP_Point2D start;

	/**
	 * a WP_Point2D object indicating the end of the line
	 */
	WP_Point2D end;

	/**
	 * a WP_Vector2D object indicating the normal of this line
	 */
	WP_Vector2D normal;
};
}
#endif

