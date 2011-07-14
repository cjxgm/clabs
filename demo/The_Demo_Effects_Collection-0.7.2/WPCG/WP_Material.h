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

#ifndef WP_MATERIAL_H
#define WP_MATERIAL_H

#include "WP_Def.h"
#include "WP_Color.h"

namespace WPCG
{
/**
 * this class represents OpenGL material\n
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
class WP_Material
{
public:

	WP_Material();
	~WP_Material();

	/**
	 * assignment operator
	 * @param m the to be copied material
	 * @return a reference to this object containing the copied material settings
	 */
	WP_Material& operator=(const WP_Material& m);

	/**
	 * this function sets the ambient, diffuse, specular and emissive components of this material to default values
	 */
	void setDefault();

	/**
	 * this function sets the material properties in OpenGL
	 */
	void initMaterialGL() const;

	/**
	 * this function sets the transparant color component of all material properties
	 */
	void setTransparency(scalar t)
	{ 
	  transparency = t; 
	  ambient.components[3] = 1.0 - t;
	  diffuse.components[3] = ambient.components[3];
	  specular.components[3] = ambient.components[3];
	  emissive.components[3] = ambient.components[3];
	};

	/**
	 * a WP_Color object indicating the ambient color property of the material
	 */
	WP_Color ambient;

	/**
	 * a WP_Color object indicating the diffuse color property of the material
	 */
	WP_Color diffuse;
	
	/**
	 * a WP_Color object indicating the specular color property of the material
	 */
	WP_Color specular;

	/**
	 * a WP_Color object indicating the emissive color property of the material
	 */
	WP_Color emissive;

	/**
	 * the specular exponent of the material
	 */
	scalar specularExponent;

	/**
	 * the transparency of all material properties 
	 */
	scalar transparency;
}; 
}
#endif

