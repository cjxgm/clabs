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

#ifndef WP_TERRAIN_H
#define WP_TERRAIN_H

#include "WP_Def.h"
#include "WP_Point3D.h"

namespace WPCG
{
//forward declaration
class WP_Vertex;

/**
 * this class represents a 3D terrain which can be rendered into a 3D scene\n
 * Code based on an article by Bob Nystrom (www.robot-frog.com), thanks mate, I'll buy you a beer one day\n
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
class WP_Terrain
{
public:
	WP_Terrain(): displayID(0){};

	WP_Terrain(int width, int height, int number_iterations, scalar resolution, int scale);
	
	~WP_Terrain();

	void drawTerrain() const;

	void setMiddlePoint(const WP_Point3D& p)
	  {
	    middle = p;
	  }

 private:
	GLuint displayID;
	scalar displacement_x, displacement_y;
	WP_Point3D middle;
};
}
#endif

