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

#include <cmath>
#include <iostream>
#include "WP_Math.h"
#include "WP_Vertex.h"
#include "WP_GLState.h"
#include "WP_TextureManager.h"
#include "WP_SkyBox.h"

namespace WPCG
{
WP_SkyBox::WP_SkyBox(const char* front_texture,
		     const char* right_texture, 
		     const char* back_texture,
		     const char* left_texture,
		     const char* top_texture,
		     const char* bottom_texture):displayID(0)
{
  //FIXME create a collision model for each side so frustum culling can be applied

  //creates a skybox

  WP_TextureManager* tex_manager = WP_TextureManager::getInstance();
  tex_manager->mipmapping = false;
  GLuint ft_id = tex_manager->getTexture(front_texture, this);
  GLuint rg_id = tex_manager->getTexture(right_texture, this);
  GLuint bk_id = tex_manager->getTexture(back_texture, this);
  GLuint lf_id = tex_manager->getTexture(left_texture, this);
  GLuint up_id;
  if (top_texture)
    up_id = tex_manager->getTexture(top_texture, this);
  GLuint dn_id;
  if (bottom_texture)
    dn_id = tex_manager->getTexture(bottom_texture, this);

   //create display list 
  displayID = glGenLists(1);
  glNewList(displayID, GL_COMPILE);
  glPushMatrix();
  WP_GLState::getInstance()->disableDepthTest();
  WP_GLState::getInstance()->disableLighting();
  WP_GLState::getInstance()->disableCulling();
  WP_GLState::getInstance()->enableTexture2D(); 
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDepthMask(false);    
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBindTexture(GL_TEXTURE_2D, ft_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high
  
  glBegin(GL_QUADS);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1.0, -1.0, -1.0);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(1.0, -1.0, -1.0);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(1.0, 1.0, -1.0);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1.0, 1.0, -1.0);
  
  glEnd();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
  glBindTexture(GL_TEXTURE_2D, rg_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

  glBegin(GL_QUADS);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0, -1.0, -1.0);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(1.0, -1.0, 1.0);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(1.0, 1.0, 1.0);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0, 1.0, -1.0);

  glEnd();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
  glBindTexture(GL_TEXTURE_2D, bk_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

  glBegin(GL_QUADS);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0, -1.0, 1.0);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0, -1.0, 1.0);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, 1.0, 1.0);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(1, 1.0, 1.0);

  glEnd();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
  glBindTexture(GL_TEXTURE_2D, lf_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

  glBegin(GL_QUADS);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1.0, -1.0, 1.0);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0, -1.0, -1.0);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, 1.0, -1.0);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1, 1.0, 1.0);

  glEnd();

  if (top_texture)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
      glBindTexture(GL_TEXTURE_2D, up_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

      glBegin(GL_QUADS);

      glTexCoord2f(1.0, 1.0);
      glVertex3f(1.0, 1.0, -1.0);

      glTexCoord2f(0.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);

      glTexCoord2f(0.0, 0.0);
      glVertex3f(-1.0, 1.0, 1.0);

      glTexCoord2f(1.0, 0.0);
      glVertex3f(-1, 1.0, -1.0);

      glEnd();
    }

  if (bottom_texture)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
      glBindTexture(GL_TEXTURE_2D, dn_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

      glBegin(GL_QUADS);

      glTexCoord2f(1.0, 1.0);
      glVertex3f(-1.0, -1.0, -1.0);

      glTexCoord2f(0.0, 1.0);
      glVertex3f(-1.0, -1.0, 1.0);

      glTexCoord2f(0.0, 0.0);
      glVertex3f(1.0, -1.0, 1.0);

      glTexCoord2f(1.0, 0.0);
      glVertex3f(1, -1.0, -1.0);

      glEnd();
    }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
  WP_GLState::getInstance()->enableDepthTest();
  WP_GLState::getInstance()->disableTexture2D();
  WP_GLState::getInstance()->enableLighting();
  WP_GLState::getInstance()->enableCulling();
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glDepthMask(true);
  glPopMatrix();
  glEndList();
  tex_manager->mipmapping = true;

}

WP_SkyBox::~WP_SkyBox()
{
  if (displayID != 0)
    {
      glDeleteLists(displayID, 1);
    }
}

void WP_SkyBox::drawSkyBox(const WP_Point3D& p) const
{
  glPushMatrix();
  glTranslatef(p.data[0], p.data[1], p.data[2]);
  glCallList(displayID);
  glPopMatrix();
}
}
