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

#ifndef WP_COLOR_H
#define WP_COLOR_H

#include "WP_Def.h"
namespace WPCG
{
/**
 * this class represents an OpenGL color object with an alpha component\n
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
class WP_Color
{
public:
	WP_Color(){ components[0] = components[1] = components[2] = 0.0; components[3] = 1.0;};

	/**
	 * @param r the red color component in float format (0.0-1.0)
	 * @param g the green color component in float format (0.0-1.0)
	 * @param b the blue color component in float format (0.0-1.0)
	 * @param a the optional alpha component in float format (0.0-1.0)
	 */
	WP_Color(scalar r, scalar g, scalar b, scalar a=1.0){ components[0] = r; components[1] = g; components[2] = b; components[3] = a;};

	/**
	 * @param r the red color component in byte format (0-255)
	 * @param g the green color component in byte format (0-255)
	 * @param b the blue color component in byte format (0-255)
	 * @param a the optional alpha (transparancy) color component in byte format (0-255)
	 */
	WP_Color(byte r, byte g, byte b, byte a = 255)
	  { 
	    float m = 1.0/255.0;
	    components[0] = r * m; 
	    components[1] = g * m; 
	    components[2] = b * m;
	    components[3] = a * m;
	  };

	~WP_Color(){};

	//copy constructor
	WP_Color(const WP_Color& c)
	  {
	    components[0] = c.components[0];
	    components[1] = c.components[1];
	    components[2] = c.components[2];
	    components[3] = c.components[3];   
	  }

	/**
	 * assigment operator
	 * @param c the to be copied color
	 * @return a reference to this new object containing the new color components
	 */
	WP_Color& operator=(const WP_Color& c)
	{
	  if (this == &c)
	    return *this;
	  components[0] = c.components[0];
	  components[1] = c.components[1]; 
	  components[2] = c.components[2];
	  components[3] = c.components[3];
	  return *this;
	};

	/**
	 * this function converts a color component from float to byte
	 * @param index in index in the component array. 0-r, 1-g, 2-b, 3-a
	 * @return a byte containing the value of the wanted color component
	 */
	byte fromFloatToByte(int index)
	  {
	    if (index >= 0 && index < 4)
	      {
		if (components[index] >= 1.0f)
			return 255;
		else if (components[index] <= 0.0)
			return 0;
		else
			return (byte)(components[index] * 255.0f);
	      }
	    return (byte)0;
	  }

	/**
	 * the components red, green, blue and alpha of the color object. These components are stored in float format (0.0-1.0), so in case byte format is needed use the fromFloatToByte function
	 */
	scalar components[4];
};
}
#endif

