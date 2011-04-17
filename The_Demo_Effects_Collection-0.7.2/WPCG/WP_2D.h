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

//WP_2D.h
//2D OpenGl helper classes

#ifndef WP_2D_H
#define WP_2D_H

#include <string>
#include "WPCG.h"
#include "WP_Def.h"

namespace WPCG
{
class WP_Point2D; //forward declaration

//////////////////// WP_Draw ////////////////////////////////
/**
 * this class is used for the drawing of 2D primitives to the screen by using OpenGL. OpenGL's coordinate system has defined 0,0 being in the lower left corner
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
class WP_Draw_2D
{
public:
  
  WP_Draw_2D();
  ~WP_Draw_2D();
  
  /**
   * this function draws a 2D point to the screen
   * @param x the x position of the point
   * @param y the y position of the point
   */
  void vDrawPoint(GLint x, GLint y) const;

  /**
   * this function draws a 2D point to the screen
   * @param point a pointer to a WP_Point2D object holding the x and y coordinates
   */
  void vDrawPoint(const WP_Point2D* point) const;

  /**
   * this function draws a 2D line to the screen
   * @param startx the x position of the start of the line
   * @param starty the y position of the start of the line
   * @param endx the x position of the end of the line
   * @param endy the y position of the end of the line
   */
  void vDrawLine(GLint startx, GLint starty, GLint endx, GLint endy) const;

  /**
   * this function draws a 2D line to the screen
   * @param startpoint a pointer to a WP_Point2D object holding the x and y coordinates of the start of the line
   * @param endpoint a pointer to a WP_Point2D object holding the x and y coordinates of the end of the line
   */
  void vDrawLine(const WP_Point2D* startpoint, const WP_Point2D* endpoint) const;

  /**
   * this function draws a 2D polyline (a line containing multiple line segments) to the screen
   * @param points an array of points representing the polyline
   * @param number the number of points in the array or the number of line segments in the polyline
   * @param closed a boolean indicating if the polyline is closed (start and endpoint will be connected and in doing so a polygon is created) or not
   */
  void vDrawPolyLine(WP_Point2D* points, GLint number, bool closed) const;
  
  /**
   * this function draws a 2D text to the screen
   * @param font the (glut) font used to draw the text
   * @param text the text which is written to the screen
   * @param xpos the x position of the start of the text (text is written from left to right)
   * @param ypos the y position of the start of the text (text is written from left to right)
   */
  void vDrawString(void* font, const string &text, GLint xpos, GLint ypos) const;
};
}
#endif

