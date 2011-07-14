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

#ifndef WP_TEXTUREMANAGER_H
#define WP_TEXTUREMANAGER_H

#include <list>
#include "WPCG.h"

namespace WPCG
{
/**
 * this singleton class is used for managing and storing all textures in such a way that only necessary textures are resident and textures are shared between models (objects). It makes use internally of WP_Texture objects. These texture objects are able to generate a texture from the following image formats:\n (.bmp), Cut (.cut), Doom flats and textures, Icon (.ico), Jpeg (.jpg, .jpe, .jpeg), Lbm (.lbm), Pcd (.pcd), ZSoft Pcx (.pcx), Pic (.pic), Portable Anymap (.pbm, .pgm, .ppm), Portable Network Graphics (.png), Sgi (.sgi, .bw, .rgb, .rgba), Truevision Targa (.tga) and Tiff (.tif, .tiff) images\n 
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
class WP_TextureManager
{
public:
  ~WP_TextureManager();
	
  /**
   * this function is used to obtain a pointer to the only instance of this class (singleton)
   * @return a pointer to the only instance of this class
   */
  static WP_TextureManager* getInstance();

  /**
   * this function returns the texture ID (an OpenGL texture ID) belonging to the queried texture. If the texture is currently not maintained by the texture manager, the manager will load it.
   * @param name the name of the texture, for example <i>texture1.bmp</i> or <i>roof.pcs</i>.  For now only the 24 bit .bmp and 24 bit compressed/uncompressed .pcx format are supported\n
   * @param owner a void pointer to the owner of the texture (e.g. an object). This is used to internally keep track of how many objects use a particular texture
   * @return a texture ID with can be directly used in OpenGL's glBindTexture call, 0 in case of failure
   */
  int getTexture(const string &name, void* owner, bool keepImages=false);

  WP_Image* getImage(GLuint id);

  /**
   * this function removes all textures belonging to this object. In fact it only removes this object from the textures' internal owner list. Only if this list is empty, the texture is removed from the texture manager. It should be called when an object is destroyed
   * @param owner a void pointer to an object of which all textures are removed
   * @return a boolean indicating the success or failure of this function
   */
  bool removeTextures(void* owner);

  /**
   * this boolean indicates if mipmapping should be used for textures
   */
  bool mipmapping;

private:
	WP_TextureManager();

	/**
	 * this nested class represents a texture. It is able to load the following image files according to their extensions\nFor now only the 24 bit .bmp and 24 bit compressed/uncompressed .pcx format are supported\n
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
	class WP_Texture
	{
	public:
	  WP_Texture():texture_id(0), texture_ok(false){};

	  /**
	   * @param name the name of the image, for example <i>texture1.bmp</i> or <i>roof.pcx</i>.  For now only the 24 bit .bmp and 24 bit compressed/uncompressed .pcx format are supported\n
	   */
	  WP_Texture(const string &name, bool keepImages=false);
	  ~WP_Texture();

	  /**
	   * this function removes an owner of the texture's internal list of owners
	   * @param owner a pointer to an owner
	   * @return a boolean indicating if the owner was removed or not
	   */
	  bool removeInstance(void* owner);
	  
	  /**
	   * this variable stores texture name
	   */
	  string name;

	  /**
	   * this variable represents an OpenGL texture ID
	   */
	  GLuint texture_id;

	  /**
	   * a list of void pointers, holding all objects using this texture
	   */
	  list<void*> owners;

	  /**
	   * a boolean indicating if texture creation was succesfull
	   */
	  bool texture_ok;

	  WP_Image* image;
	};
	
	/**
	 * this function retrieves a specific texture from the texture manager
	 * @param name the name of the texture
	 * @return a pointer to a WP_Texture object, the wanted texture
	 */
	WP_Texture* findInstance(const string &name);

	WP_Texture* findInstance(GLuint id);

	/**
	 * this function counts the number of textures belonging to an object 
	 * @param owner the object of which the textures are counted
	 * @param the number of textures beloning to an object
	 */
	int countTextures(void* owner) const;

	/**
	 * all the textures stored in the texture manager are stored in the textures list
	 */
	list<WP_Texture*> textures;

	/**
	 * a pointer to the only instance of the WP_TextureManager class (singleton)
	 */
	static WP_TextureManager* tm_instance;	 
};
}
#endif
