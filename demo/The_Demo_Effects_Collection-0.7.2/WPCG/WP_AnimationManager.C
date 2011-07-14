/* Copyright (C) 2001-2004 W.P. van Paassen - peter@paassen.tmfweb.nl

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
#include <cmath>
#include "WP_Model.h"
#include "WP_AnimationManager.h"

namespace WPCG
{
WP_AnimationManager* WP_AnimationManager::om_instance = 0;

bool
WP_AnimationManager::animateObject(const WP_Object *object, unsigned short *start_frame, unsigned short *next_frame, scalar *interpolate)
{
  bool tweening = false;
  if (object_info.count(object))
    {
      WP_ObjectAnimationInfo *info = object_info[object];
      WP_AnimatedModel *model = dynamic_cast<WP_AnimatedModel*>(object->model);
      
      if (model)
	{
	  if (info->t != 0.0)
	    {
	      tweening = true;
	      *next_frame = model->getCategoryStartFrame(info->category) + ((info->currentFrame + 1) % model->getCategoryNumberFrames(info->category));
	    }
	  
	  *start_frame = model->getCategoryStartFrame(info->category) + info->currentFrame;
	  *interpolate = info->t;
	}
    }
  return tweening;
}

scalar WP_AnimationManager::getT(const WP_Object* object)
{
  if (object_info.count(object))
    {
	  WP_ObjectAnimationInfo *info = object_info[object];
	  return info->t;
    }
  return -1.0;
}
  
void
WP_AnimationManager::updateAnimation(const WP_Object* object)
{
  if (object_info.count(object))
    {
      WP_AnimatedModel *model = dynamic_cast<WP_AnimatedModel*>(object->model);
      
      if (model)
	{
	  WP_ObjectAnimationInfo *info = object_info[object];
	  info->t  += interpolation;
	  if (info->t >= 1.0)
	    {
	      info->t -= 1.0;
	      info->currentFrame++;
	      info->currentFrame %= model->getCategoryNumberFrames(info->category);
	    }
	}
    }
}
  int WP_AnimationManager::getNumberFrames(const WP_Object* object)
  {
      WP_ObjectAnimationInfo *info = object_info[object];
      WP_AnimatedModel *model = dynamic_cast<WP_AnimatedModel*>(object->model);
      return model->getCategoryNumberFrames(info->category);
  }

bool 
WP_AnimationManager::addObject(const WP_Object *object)
{
  if (object_info.count(object))
    return false;
  
  //set model's first category as a default
  WP_AnimatedModel *model = dynamic_cast<WP_AnimatedModel*>(object->model);
  if (model)
    {
      //create and add ObjectAnimationInfo
      WP_ObjectAnimationInfo *info = new WP_ObjectAnimationInfo(model->getCategoryName(0));
      object_info.insert(map<const WP_Object*, WP_ObjectAnimationInfo*>::value_type(object, info));
      return true;
    }
  return false;
}

}



