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

#ifndef WP_GLSTATE_H
#define WP_GLSTATE_H

#include "GL/gl.h"
#include "WP_Def.h"

namespace WPCG
{

/**
 * this singleton class keeps track of OpenGL's internal state and prevents redundant and expensive state changes\n
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
class WP_GLState
{
public:
	~WP_GLState(){};

	/**
	 * this static function is used to obtain an instance of this class
	 * @return a pointer to a WP_GLState object by which OpenGL's internal state must be changed
	 */
	static WP_GLState* getInstance()
	{	if (!_instance)
			_instance = new WP_GLState();
		return _instance;
	}

	/**
	 * this function enables fog in OpenGL
	 */
	void enableFog()
	  {
	    if (!fog)
	      {
		glEnable(GL_FOG);
		fog = true;
	      }
	  }

	/**
	 * this function disables fog in OpenGL
	 */
	void disableFog()
	  {
	    if (fog)
	      {
		glDisable(GL_FOG);
		fog = false;
	      }
	  }

	/**
	 * this function enables lighting in OpenGL
	 */
	void enableLighting()
	{
		if (!lighting)
		{
			glEnable(GL_LIGHTING);
			lighting = true;
		}

	}

	/**
	 * this function disables lighting in OpenGL
	 */
	void disableLighting()
	{
		if (lighting)
		{
			glDisable(GL_LIGHTING);
			lighting = false;
		}
	}

	/**
	 * this function enables depth testing (z-buffering) in OpenGL
	 */
	void enableDepthTest()
	{
		if (!depthTest)
		{
			glEnable(GL_DEPTH_TEST);
			depthTest = true;
		}
	}

	/**
	 * this function disables depth testing (z-buffering) in OpenGL
	 */
	void disableDepthTest()
	{
		if (depthTest)
		{
			glDisable(GL_DEPTH_TEST);
			depthTest = false;
		}
	}

	/**
	 * this function enables (polygon) culling in OpenGL
	 */
	void enableCulling()
	{
		if (!culling)
		{
			glEnable(GL_CULL_FACE);
			culling = true;
		}
	}

	/**
	 * this function disables (polygon) culling in OpenGL
	 */
	void disableCulling()
	{
		if (culling)
		{
			glDisable(GL_CULL_FACE);
			culling = false;
		}
	}

	/**
	 * this function enables normalizing of normal vectors in OpenGL
	 */
	void enableNormalize()
	{
		if (!normalize)
		{
			glEnable(GL_NORMALIZE);
			normalize = true;
		}
	}

	/**
	 * this function disables normalizing of normal vectors in OpenGL
	 */
	void disableNormalize()
	{
		if (normalize)
		{
			glDisable(GL_NORMALIZE);
			normalize = false;
		}
	}

	/**
	 * this function enables light 0 in OpenGL
	 */
	void enableLight0()
	{
		if (!light0)
		{
			glEnable(GL_LIGHT0);
			light0 = true;
		}
	}

	/**
	 * this function disables light 0 in OpenGL
	 */
	void disableLight0()
	{
		if (light0)
		{
			glDisable(GL_LIGHT0);
			light0 = false;
		}
	}

	/**
	 * this function enables light 1 in OpenGL
	 */
	void enableLight1()
	{
		if (!light1)
		{
			glEnable(GL_LIGHT1);
			light1 = true;
		}
	}

	/**
	 * this function disables light 1 in OpenGL
	 */
	void disableLight1()
	{
		if (light1)
		{
			glDisable(GL_LIGHT1);
			light1 = false;
		}
	}

	/**
	 * this function enables light 2 in OpenGL
	 */
	void enableLight2()
	{
		if (!light2)
		{
			glEnable(GL_LIGHT2);
			light2 = true;
		}
	}

	/**
	 * this function disables light 2 in OpenGL
	 */
	void disableLight2()
	{
		if (light2)
		{
			glDisable(GL_LIGHT2);
			light2 = false;
		}
	}

	/**
	 * this function enables light 3 in OpenGL
	 */
	void enableLight3()
	{
		if (!light3)
		{
			glEnable(GL_LIGHT3);
			light3 = true;
		}
	}

	/**
	 * this function disables light 3 in OpenGL
	 */
	void disableLight3()
	{
		if (light3)
		{
			glDisable(GL_LIGHT3);
			light3 = false;
		}
	}

	/**
	 * this function enables light 4 in OpenGL
	 */
	void enableLight4()
	{
		if (!light4)
		{
			glEnable(GL_LIGHT4);
			light4 = true;
		}
	}

	/**
	 * this function disables light 4 in OpenGL
	 */
	void disableLight4()
	{
		if (light4)
		{
			glDisable(GL_LIGHT4);
			light4 = false;
		}
	}

	/**
	 * this function enables light 5 in OpenGL
	 */
	void enableLight5()
	{
		if (!light5)
		{
			glEnable(GL_LIGHT5);
			light5 = true;
		}
	}

	/**
	 * this function disables light 5 in OpenGL
	 */
	void disableLight5()
	{
		if (light5)
		{
			glDisable(GL_LIGHT5);
			light5 = false;
		}
	}

	/**
	 * this function enables light 6 in OpenGL
	 */
	void enableLight6()
	{
		if (!light6)
		{
			glEnable(GL_LIGHT6);
			light6 = true;
		}
	}

	/**
	 * this function disables light 6 in OpenGL
	 */
	void disableLight6()
	{
		if (light6)
		{
			glDisable(GL_LIGHT6);
			light6 = false;
		}
	}

	/**
	 * this function enables light 7 in OpenGL
	 */
	void enableLight7()
	{
		if (!light7)
		{
			glEnable(GL_LIGHT7);
			light7 = true;
		}
	}

	/**
	 * this function disables light 7 in OpenGL
	 */
	void disableLight7()
	{
		if (light7)
		{
			glDisable(GL_LIGHT7);
			light7 = false;
		}
	}

	/**
	 * this function enables 2D texture mapping in OpenGL
	 */
	void enableTexture2D()
	{
		if (!texture2D)
		{
			glEnable(GL_TEXTURE_2D);
			texture2D = true;
		}
	}

	/**
	 * this function disables 2D texture mapping in OpenGL
	 */
	void disableTexture2D()
	{
		if (texture2D)
		{
			glDisable(GL_TEXTURE_2D);
			texture2D = false;
		}
	}

	/**
	 * this function enables blending in OpenGL
	 */
	void enableBlending()
	{
		if (!blending)
		{
			glEnable(GL_BLEND);
			blending = true;
		}
	}

	/**
	 * this function disables blending in OpenGL
	 */
	void disableBlending()
	{
		if (blending)
		{
			glDisable(GL_BLEND);
			blending = false;
		}
	}

	/**
	 * this function enables the smooting of polygons in OpenGL
	 */
	void enablePolygonSmooth()
	{
		if (!polygonSmooth)
		{
			glEnable(GL_POLYGON_SMOOTH);
			polygonSmooth = true;
		}
	}

	/**
	 * this function disables the smoothing of polygons in OpenGL
	 */
	void disablePolygonSmooth()
	{
		if (polygonSmooth)
		{
			glDisable(GL_POLYGON_SMOOTH);
			polygonSmooth = false;
		}
	}

	/**
	 * this function changes the matrixmode to GL_MODELVIEW in OpenGL
	 */
	void modelview()
	  {
	    if (!_modelview)
	      {
		glMatrixMode(GL_MODELVIEW);
		_modelview = true;
		_projection = false;
	      }
	  }

	/**
	 * this function changes the matrixmode to GL_PROJECTION in OpenGL
	 */
	void projection()
	  {
	    if (!_projection)
	      {
		glMatrixMode(GL_PROJECTION);
		_projection = true;
		_modelview = false;
	      }
	  }

	/**
	 * this function changes the rendering mode to selection mode. This is used for rendering into the selection buffer instead into the frame_buffer
	 */
	void selection_mode()
	  {
	    if (!_selection)
	      {
		_selection = true;
		glRenderMode(GL_SELECT);
	      }
	  }
	
	/**
	 * this function changes the rendering mode to framebuffer mode (GL_RENDER). This is used for the actual rendering to the screen
	 * @return the number of hits (objects) by drawing into the selection buffer, 0 if previous mode was not selection mode
	 */
	int render_mode()
	  {
	    if (_selection)
	      {
		_selection = false;
		return glRenderMode(GL_RENDER);
	      }
	    return 0;
	  }

	/**
	 * a helper function for enabling the use of light <i>number</i> in OpenGL
	 * @param number the number of the to be enabled light. Values from 0 - 7 are valid
	 */
	void enableLighti(GLuint number);

	/**
	 * a helper function for disabling the use of light <i>number</i> in OpenGL
	 * @param number the number of the to be disabled light. Values from 0 - 7 are valid
	 */
	void disableLighti(GLuint number);

	/**
	 * a static WP_GLState pointer to the only instance (singleton) of this class
	 */
	static WP_GLState* _instance;

	/**
	 * a boolean indicating if lighting is enabled in OpenGL
	 */
	bool lighting;

	/**
	 * a boolean indicating if culling is enabled in OpenGL
	 */
	bool culling;

	/**
	 * a boolean indicating if 2D texture mapping is enabled in OpenGL
	 */
	bool texture2D;

	/**
	 * a boolean indicating if blending is enabled in OpenGL
	 */
	bool blending;

	/**
	 * a boolean indicating if depth testing (z-buffering) is enabled in OpenGL
	 */
	bool depthTest;

	/**
	 * a boolean indicating if every normal will be normalized by OpenGL
	 */
	bool normalize;

	/**
	 * a boolean indicating if light 0 is enabled in OpenGL
	 */
	bool light0;
	
	/**
	 * a boolean indicating if light 1 is enabled in OpenGL
	 */
	bool light1;

	/**
	 * a boolean indicating if light 2 is enabled in OpenGL
	 */
	bool light2;
	
	/**
	 * a boolean indicating if light 3 is enabled in OpenGL
	 */
	bool light3;
	
	/**
	 * a boolean indicating if light 4 is enabled in OpenGL
	 */
	bool light4;
	
	/**
	 * a boolean indicating if light 5 is enabled in OpenGL
	 */
	bool light5;

	/**
	 * a boolean indicating if light 6 is enabled in OpenGL
	 */
	bool light6;
	
	/**
	 * a boolean indicating if light 7 is enabled in OpenGL
	 */
	bool light7;

	/**
	 * a boolean indicating if polygon smoothing is enabled in OpenGL
	 */
	bool polygonSmooth;

	/**
	 * a boolean indicating if fog is enabled in OpenGL
	 */
	bool fog;

	/**
	 * a boolean indicating if the current matrix mode is GL_MODELVIEW in OpenGL
	 */
	bool _modelview;
	
	/**
	 * a boolean indicating if the current matrix mode is GL_PROJECTION in OpenGL
	 */
	bool _projection;

	/**
	 * a boolean indicating if the current rendering mode is selection buffer rendering
	 */
	bool _selection;

 private:
	WP_GLState();
};
}
#endif



