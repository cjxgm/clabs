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

#include <fstream>
#include <cmath>
#include <GL/glut.h>
#include "WP_2D.h"
#include "WP_Point2D.h"

namespace WPCG
{
//////////////////// WP_Draw_2D ////////////////////////////////

WP_Draw_2D::WP_Draw_2D(){}
WP_Draw_2D::~WP_Draw_2D(){}

void WP_Draw_2D::vDrawPoint(GLint x, GLint y) const
{
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void WP_Draw_2D::vDrawPoint(const WP_Point2D* point) const
{
	glBegin(GL_POINTS);
		glVertex2f(point->x, point->y);
	glEnd();
}

void WP_Draw_2D::vDrawLine(GLint startx, GLint starty, GLint endx, GLint endy) const
{
	glBegin(GL_LINES);
		glVertex2i(startx, starty);
		glVertex2i(endx, endy);
	glEnd();
}

void WP_Draw_2D::vDrawLine(const WP_Point2D* startpoint, const WP_Point2D* endpoint) const
{
	glBegin(GL_LINES);
		glVertex2f(startpoint->x, startpoint->y);
		glVertex2f(endpoint->x, endpoint->y);
	glEnd();
}

void WP_Draw_2D::vDrawPolyLine(WP_Point2D* points, GLint number, bool closed) const
{
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
	for (GLint i = 0; i < number; i++)
		glVertex2f(points[i].x, points[i].y);	
	glEnd();
	glFlush();
}

void WP_Draw_2D::vDrawString(void* font, const string &text,
			  GLint xpos, GLint ypos) const
{
  string::const_iterator iter = text.begin();
  for(;iter != text.end();++iter)
    {
      glRasterPos2i(xpos, ypos);
      glutBitmapCharacter(font, *iter);
      xpos += glutBitmapWidth(font,*iter);
    }
}
}
