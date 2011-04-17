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

#ifndef WP_INIT_H
#define WP_INIT_H

#include "WP_Def.h"

namespace WPCG
{

/**
 * this class is used for world window and viewport setting in OpenGL\n
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
class WP_Init
{
 public:

  WP_Init(int argc, char* argv[]);
  ~WP_Init();
  
  /**
   * this function sets the world window in OpenGL. Pass normal screen coordinates to this function so 0,0 is upper left corner (left top) and 640,480 is lower right corner (right bottom)
   * @param left the left coordinate of the window
   * @param right the right coordinate of the window
   * @param bottom the bottom coordinate of the window
   * @param top the top coordinate of the window
   */
  void vSetWindow(scalar left, scalar right, scalar bottom, scalar top);
  
  /**
   * this function sets the viewport in OpenGL. Pass normal screen coordinates to this function so 0,0 is upper left corner (left top) and 640,480 is lower right corner (right bottom)
   * @param left the left coordinate of the viewport
   * @param right the right coordinate of the viewport
   * @param bottom the bottom coordinate of the viewport
   * @param top the top coordinate of the viewport
   */
  void vSetViewPort(GLint left, GLint right, GLint bottom, GLint top);
};
}
#endif

