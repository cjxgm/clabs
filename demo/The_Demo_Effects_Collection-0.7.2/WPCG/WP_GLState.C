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

#include "WP_GLState.h"

namespace WPCG
{
WP_GLState* WP_GLState::_instance = 0;

WP_GLState::WP_GLState(): lighting(false), culling(false), texture2D(false), blending(false), 
	depthTest(false), normalize(false), light0(false), light1(false), light2(false), light3(false),
  light4(false), light5(false), light6(false), light7(false), polygonSmooth(false), fog(false), _modelview(false), _projection(false), _selection(false)
{}

void WP_GLState::enableLighti(GLuint number)
{
	switch(number)
	{
	case 0:
		if (!light0)
		{
			glEnable(GL_LIGHT0);
			light0 = true;
		}
	break;
	case 1:
		if (!light1)
		{
			glEnable(GL_LIGHT1);
			light1 = true;
		}
	break;
	case 2:
		if (!light2)
		{
			glEnable(GL_LIGHT2);
			light2 = true;
		}
	break;
	case 3:
		if (!light3)
		{
			glEnable(GL_LIGHT3);
			light3 = true;
		}
	break;
	case 4:
		if (!light4)
		{
			glEnable(GL_LIGHT4);
			light4 = true;
		}
	break;
	case 5:
		if (!light5)
		{
			glEnable(GL_LIGHT5);
			light5 = true;
		}
	break;
	case 6:
		if (!light6)
		{
			glEnable(GL_LIGHT6);
			light6 = true;
		}
	break;
	case 7:
		if (!light7)
		{
			glEnable(GL_LIGHT7);
			light7 = true;
		}
	break;
	}
}

void WP_GLState::disableLighti(GLuint number)
{
	switch(number)
	{
	case 0:
		if (light0)
		{
			glDisable(GL_LIGHT0);
			light0 = false;
		}
	break;
	case 1:
		if (light1)
		{
			glDisable(GL_LIGHT1);
			light1 = false;
		}
	break;
	case 2:
		if (light2)
		{
			glDisable(GL_LIGHT2);
			light2 = false;
		}
	break;
	case 3:
		if (light3)
		{
			glDisable(GL_LIGHT3);
			light3 = false;
		}
	break;
	case 4:
		if (light4)
		{
			glDisable(GL_LIGHT4);
			light4 = false;
		}
	break;
	case 5:
		if (light5)
		{
			glDisable(GL_LIGHT5);
			light5 = false;
		}
	break;
	case 6:
		if (light6)
		{
			glDisable(GL_LIGHT6);
			light6 = false;
		}
	break;
	case 7:
		if (light7)
		{
			glDisable(GL_LIGHT7);
			light7 = false;
		}
	break;
	}
}
}

