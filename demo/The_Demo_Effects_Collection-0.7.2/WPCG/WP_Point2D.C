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

#include "WP_Point2D.h"
#include "WP_Vector2D.h"

namespace WPCG
{
WP_Vector2D WP_Point2D::operator-(const WP_Point2D& p) const
{
	return WP_Vector2D(x - p.x, y - p.y);
}

WP_Point2D WP_Point2D::operator+(const WP_Vector2D& v) const
{
	return WP_Point2D(x + v.data[0], y + v.data[1]);
}

void WP_Point2D::draw() const
{
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}
}




