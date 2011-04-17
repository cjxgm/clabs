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

#ifndef WP_VERTEX_H
#define WP_VERTEX_H

#include "WP_Def.h"
#include "WP_Point3D.h"
#include "WP_Vector3D.h"

namespace WPCG
{
/**
 * this class represents a 3D vertex\n
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
class WP_Vertex
{
public:
  WP_Vertex(){};

  /**
   * @param p a WP_Point3D object which holds the vertex its coordinates
   * @param n a WP_Vector3D object representing the normal of this vertex. This is used for shading operations like for example gouraud shading
   */
  WP_Vertex(const WP_Point3D& p, const WP_Vector3D& n):point(p), normal(n)
    {
      texCoords[0] = texCoords[1] = 0.0;
    };

  ~WP_Vertex(){};

  /**
   * assignment operator
   * @param v the WP_Vertex object which is copied to this object
   * @return a reference to this object containing the copied vertex values
   */
  inline WP_Vertex& operator=(const WP_Vertex& v)
    {
      if (this == &v)
	return *this;

      point = v.point;
      normal = v.normal;
      texCoords[0] = v.texCoords[0];
      texCoords[1] = v.texCoords[1];
      return *this;
    }

  /**
   * this function linearly interpolates between two vertices and the resulting vertex is assigned to this vertex
   * @param v a pointer to a WP_Vertex object representing the second vertex used for interpolating between
   * @param t the amount of interpolation between the two vertices. 0.0 will result in this vertex. 1.0 will result in vertex <i>v</i>. 0.5 in between the two vertices etc.
   */
  inline void lerp3D(const WP_Vertex* v, float t)
    {
      WP_Vector3D c = v->point - point;
      point = point + (c * t); 
    };
  
  /**
   * a WP_point3D object holding the vertex its coordinates
   */
  WP_Point3D point;

  /**
   * a WP_Vector3D object representing the normal of this vector
   */
  WP_Vector3D normal;

  /**
   * an array of two scalars holding the texture coordinates for this vector. This is used for texture mapping. texCoords[0] = s and texCoords[1] = t
   */
  scalar texCoords[2];
};
}
#endif

