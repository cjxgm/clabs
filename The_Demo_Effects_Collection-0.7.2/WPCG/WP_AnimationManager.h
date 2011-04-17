/* Copyright (C) 2001-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#ifndef WP_ANIMATIONMANAGER_H
#define WP_ANIMATIONMANAGER_H

#include <string>
#include <map>
#include "WPCG.h" //FIXME don't include all headers!

namespace WPCG
{
/**
 * this singleton class is in charge of animating the 3D models\n
 * @author Copyright (C) 2001-2003 W.P. van Paassen   peter@paassen.tmfweb.nl
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
class WP_AnimationManager
{
public:
  ~WP_AnimationManager()
    {
      //empty map
      map<const WP_Object*, WP_ObjectAnimationInfo*>::iterator i = object_info.begin();
      while(i != object_info.end())
	  {
	    delete (*i).second;
	    i++;
	  }
    };

  /**
   * this function is used to obtain a pointer to the only instance of this class (singleton)
   * @return a pointer to the only instance of this class
   */
  static WP_AnimationManager* getInstance()
    {
      if (!om_instance)
	{
	  om_instance = new WP_AnimationManager();
	}
      return om_instance;
    }

  unsigned short getCurrentFrame(const WP_Object* object) 
    {
      if (object_info.count(object))
	{
	  return object_info[object]->currentFrame;
	}
      return 0;
    }

  string getCategory(const WP_Object* object) 
    {
      if (object_info.count(object))
	{
	  return object_info[object]->category;
	}
      return "";
    }

  bool addObject(const WP_Object *object);

  bool isObjectPresent(const WP_Object *object)
    {
      return object_info.count(object);
    }

  bool removeObject(const WP_Object *object)
    {
      if (object_info.count(object))
	return false;

      delete object_info[object];
      object_info.erase(object);

      return true;
    }

  void setInterpolation(float i)
    {
      interpolation = i;
    }

  bool setCategory(const WP_Object *object, const string &category)
    {
      if (object_info.count(object))
	{
	  WP_ObjectAnimationInfo *info = object_info[object];
	  info->category = category;
	  info->currentFrame = 0;
	  info->t = 0.0;
	  return true;
	}
      return false;
    }

  void updateAnimation(const WP_Object* object);
  int getNumberFrames(const WP_Object* object);

  scalar getT(const WP_Object* object);

  bool animateObject(const WP_Object *object, unsigned short *start_frame, unsigned short *next_frame, scalar *interpolation);

private:
  WP_AnimationManager():interpolation(0.2){};

  /**
   * a pointer to the only instance of the WP_ObjectManager class (singleton)
   */
  static WP_AnimationManager* om_instance;	 

  //nested class 
  class WP_ObjectAnimationInfo
    {
    public:
      WP_ObjectAnimationInfo():currentFrame(0), t(0.0f){};
      WP_ObjectAnimationInfo(const string& c): currentFrame(0), t(0.0f), category(c){};
      ~WP_ObjectAnimationInfo(){};

      unsigned short currentFrame; //FIXME it could be faster to store total frame number
      float t;
      string category;
    };

  map<const WP_Object*, WP_ObjectAnimationInfo*> object_info;
  scalar interpolation; 
};

}
#endif


