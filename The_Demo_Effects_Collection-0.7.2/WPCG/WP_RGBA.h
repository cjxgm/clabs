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

#ifndef WP_RGBA_H
#define WP_RGBA_H

#include "WP_Def.h"

namespace WPCG
{
/**
 * this class is a RGBA container. It holds 4 bytes and can be used directly as a pixel in OpenGL
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
class WP_RGBA
{
 public:
  WP_RGBA():r(0), g(0), b(0), a(255){}
  
  /**
   * @param _r red component
   * @param _g green component
   * @param _b blue component
   * @param _a alpha component
   */
  WP_RGBA(byte _r, byte _g, byte _b, 
	  byte _a = 255):r(_r), g(_g), b(_b),a(_a){}
  
  /**
   * red component
   */
  byte r;

  /**
   * green component
   */
  byte g;

  /**
   * blue component
   */
  byte b;

  /**
   * alpha component
   */
  byte a;
};
}
#endif

