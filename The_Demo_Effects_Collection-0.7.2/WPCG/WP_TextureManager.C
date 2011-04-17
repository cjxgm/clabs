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

#include <iostream>
#include <string>
#include "WP_TextureManager.h"

namespace WPCG
{
WP_TextureManager* WP_TextureManager::tm_instance = 0;

WP_TextureManager::WP_TextureManager():mipmapping(false){}

WP_TextureManager::~WP_TextureManager()
{
  list<WP_Texture*>::iterator i = textures.begin();
  while (i != textures.end())
    {	
      delete *i;
      i++;
    }
  textures.clear();
}

WP_TextureManager* WP_TextureManager::getInstance()
{
  if (!tm_instance)
    {
      tm_instance = new WP_TextureManager();
    }
  return tm_instance;
}

WP_Image* WP_TextureManager::getImage(GLuint id)
{
  list<WP_Texture*>::iterator i = textures.begin();
  while (i != textures.end())
    {	
	if ((*i)->texture_id == id)
		return (*i)->image;
      i++;
    }
  return null;	
}

int WP_TextureManager::getTexture(const string &name, void* owner, bool keepImages)
{
  //search texture list for instance of texture 'name'

  WP_Texture* tex = findInstance(name);
  if (tex)
    {
      tex->owners.push_back(owner); //another object is using this texture so add address to owners list
      return tex->texture_id;
    }

  //texture not in list, so load and add it

  tex = new WP_Texture(name,keepImages);
  if (tex)
    {
      if (tex->texture_ok)
	{
	  tex->owners.push_back(owner);
	  textures.push_back(tex);
	  return tex->texture_id;
	}
      else
	{
	  delete tex;
	}
    }
  cerr << "Error: Unable to get texture " << name << endl;
  return 0;
}

int WP_TextureManager::countTextures(void* owner) const
{
  int counter = 0;
  list<WP_Texture*>::const_iterator i = textures.begin();
  while (i != textures.end())
    {	
      list<void*>::const_iterator j = (*i)->owners.begin();
   
      while (j != (*i)->owners.end())
	{		
	  if ((*j) == owner)
	    {
	      counter++;
	      break;
	    }
	  j++;
       }
      i++;
    }
  return counter;
}

bool WP_TextureManager::removeTextures(void* owner)
{
  int counter = countTextures(owner);
  int j = 0;
  int removed = 0;

  for (; j < counter; j++)
    {
      list<WP_Texture*>::iterator i = textures.begin();

      while (i != textures.end())
	{		
	  if ((*i)->removeInstance(owner))
	    {
	      removed++;
	      if ((*i)->owners.size() == 0)
		{
		  delete *i;
		  textures.erase(i); //remove texture from list
		  break;
		}
	    }
	  i++;
	}
    }
    return removed == counter;
}

WP_TextureManager::WP_Texture* WP_TextureManager::findInstance(const string &name)
{
   list<WP_Texture*>::iterator i = textures.begin();
   
   while (i != textures.end())
     {		
       if ((*i)->name.compare(name) == 0)
	 {
	   return *i; //instance found
	 }
       i++;
       }
   return 0; //instance not found
}

WP_TextureManager::WP_Texture* WP_TextureManager::findInstance(GLuint id)
{
   list<WP_Texture*>::iterator i = textures.begin();
   
   while (i != textures.end())
     {		
       if ((*i)->texture_id == id)
	 {
	   return *i; //instance found
	 }
       i++;
       }
   return 0; //instance not found
}

WP_TextureManager::WP_Texture::WP_Texture(const string &name, bool keepImages):texture_id(0), texture_ok(false)
{  
  try
    {
      const string _name = string("../TEXTURES/") + name;
      
      image = new WP_Image();
      if (image->loadImage(_name))
	{
	  if (WP_TextureManager::getInstance()->mipmapping)
	    {
	      image->setTextureGL(&texture_id, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
	    }
	  else
	    {
	      image->setTextureGL(&texture_id, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
	    }
	}

      if (texture_id != 0)
	{
	  texture_ok = true;
		if (!keepImages)
		{
			delete image;
			image = null;
		}
	}
      else
	{
	  cerr << "Creation of texture " << _name << " failed" << endl;
	}

    }
  catch(...){}
}

WP_TextureManager::WP_Texture::~WP_Texture()
{
  if (texture_id > 0)
    {
      glDeleteTextures(1, &texture_id);
    }
}

bool WP_TextureManager::WP_Texture::removeInstance(void* owner)
{
   list<void*>::iterator i = owners.begin();
   
   while (i != owners.end())
     {		
       if ((*i) == owner)
	 {
	   owners.erase(i); //owner removed
	   return true;
	 }
       i++;
     }
   return false; //nothing removed
}
}

