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

#ifndef WP_TRIANGLE_H
#define WP_TRIANGLE_H

#include "WP_Def.h"
#include "WP_Vector3D.h"

namespace WPCG
{
//forward declarations
class WP_Vertex;

/**
 * this class represents a triangle\n
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
class WP_Triangle
{
public:
	WP_Triangle(){};
	~WP_Triangle(){};

	/**
	 * this function draws the solid triangle using OpenGL
	 */
	void drawOpenGL() const; 

	/**
	 * an array of 3 pointers to WP_Vertex objects which make up the vertices of the triangle
	 */
	WP_Vertex* vertices[3];

	/**
	 * a WP_Vector3D object containing the normal of the triangle
	 */
	WP_Vector3D normal;
};

/**
 * this abstract class represents a trianglegroup\n
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
class WP_TriangleGroup
{
public:
	WP_TriangleGroup(){};
	WP_TriangleGroup(const WP_TriangleGroup &group);

	virtual ~WP_TriangleGroup(){
	  delete[] indices;
	  delete[] texCoords;
	};

	WP_TriangleGroup& operator=(const WP_TriangleGroup &group);

	virtual void drawOpenGL(const WP_Vertex *verticesFrameA, const WP_Vertex *verticesFrameB, scalar interpolation) const = 0; 
	virtual void drawOpenGL(const WP_Vertex *verticesFrameA) const = 0; 

	unsigned int *indices;
	unsigned int numberIndices;
	scalar *texCoords;
};

/**
 * this class represents a OpenGL trianglestrip\n
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
class WP_TriangleStrip: public WP_TriangleGroup
{
public:
	WP_TriangleStrip(){};
	WP_TriangleStrip(const WP_TriangleStrip &strip):WP_TriangleGroup(strip){};
	~WP_TriangleStrip(){};

	WP_TriangleStrip& operator=(const WP_TriangleStrip &strip);

	void drawOpenGL(const WP_Vertex *verticesFrameA, const WP_Vertex *verticesFrameB, scalar interpolation) const; 
	void drawOpenGL(const WP_Vertex *verticesFrameA) const; 
};


/**
 * this class represents a OpenGL trianglefan\n
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
class WP_TriangleFan: public WP_TriangleGroup
{
public:
	WP_TriangleFan(){};
	WP_TriangleFan(const WP_TriangleFan &fan):WP_TriangleGroup(fan){};
	~WP_TriangleFan(){};

	WP_TriangleFan& operator=(const WP_TriangleFan &fan);

	void drawOpenGL(const WP_Vertex *verticesFrameA, const WP_Vertex *verticesFrameB, scalar interpolation) const; 
	void drawOpenGL(const WP_Vertex *verticesFrameA) const; 
};
}
#endif



