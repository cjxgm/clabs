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

#include "WP_Ray2D.h"
#include "WP_Line2D.h"

namespace WPCG
{
WP_Ray2D::WP_Ray2D(const WP_Point2D& p, const WP_Vector2D& v):start(p), direction(v){}

WP_Ray2D::WP_Ray2D(WP_Line2D& l)
{
	start = l.start;
	direction = l.end - start;
}

//Chapter 4, page 186, Computer Graphics using opengl, F.S. Hill
//return -2 in case of failure
//return -1 in case of ray exiting from line/plane
//return 0 in case of parallel to line/plane
//return 1 in case of ray entering line/plane
int WP_Ray2D::intersection(const WP_Ray2D& r, WP_Point2D& p) const
{
  WP_Vector2D normal = r.direction;
  normal.perp();
  WP_Vector2D bMina = r.start - start;
  WP_Vector2D temp = direction;
  temp.normalize();
  scalar numerator = normal * bMina;
  scalar denominator = normal * temp;
  
  if (numerator < 0.0001 && numerator > -0.0001)
    //ray lies entirely in the line/plane because it has the same normal form as r
    return -2;
  
  if (denominator < 0.0001 && denominator > -0.0001)
    //ray is parallel to r
    return 0;
  
  scalar s =	numerator / denominator;
  
  WP_Point2D end = start + (temp * s); //hitpoint
  p = end;
  
  if (denominator > 0.0)
    return -1;
  else
    return 1;
}

void WP_Ray2D::draw() const
{
  WP_Line2D l(start, start + direction);
  l.draw();
}
}
