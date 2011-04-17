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

#include "WP_Line2D.h"
#include "WP_Vector2D.h"

namespace WPCG
{
WP_Line2D::WP_Line2D(const WP_Point2D& p1, const WP_Point2D& p2): start(p1), end(p2)
{
	normal = end - start;
	normal.perp();
}

WP_Line2D::~WP_Line2D(){}

scalar WP_Line2D::length() const
{
	WP_Vector2D v = end - start;
	return v.length();
}

void WP_Line2D::draw()
{
	glBegin(GL_LINES);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}
}
