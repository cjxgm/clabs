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
#include <list>
#include <iostream>
#include "WP_Math.h"
#include "WP_Vertex.h"
#include "WP_Quad.h"
#include "WP_TextureManager.h"
#include "WP_Terrain.h"

namespace WPCG
{
WP_Terrain::WP_Terrain(int width, int height, int number_iterations, scalar resolution, int scale): displayID(0)
{
  //creates a random 3D terrain

  WP_Vertex **height_map = new WP_Vertex*[height];
  if (!height_map)
    {
      cerr << "Unable to allocate memory for height_map" << endl;
      exit(-1);
    }

  int i;

  for (i = 0; i < height; i++)
    {
      height_map[i] = new WP_Vertex[width];
      if (!height_map[i])
	{
	  cerr << "Unable to allocate memory for height_map" << endl;
	  exit(-1);
	}
    }
  
  int k,l;
  
  //create heightmap
  for (i = 0; i < number_iterations; i++)
    {
      //create hill

      //choose random center in height_map

      int center_x = random() % width;
      int center_y = random() % height;
      int radius = random() % (width >> 1);
      int radius_square = radius << 1;

      //FIXME optimize, cycles through all vertices
      
      for (k = 0; k < height; k++)
	{
	  for (l = 0; l < width; l++)
	    {
	      //raise hill
	      int x = center_x - l;
	      x *= x;
	      int z = center_y - k;
	      z *= z;

	      float y = radius_square - (x + z);
	      if (y > 0.0)
		{
		  if (height_map[k][l].point.data[1] > 0.0)
		    {
		      height_map[k][l].point.data[1] += y;
		    }
		  else
		    {
		      height_map[k][l].point.data[1] = y;
		    }
		}
	    }
	}
    }

  //normalize terrain

#define INT_MAX	0x7fffffff
  int min = INT_MAX;
  int max = 0;

  for (k = 0; k < height; k++)
    {
      for (l = 0; l < width; l++)
	{
	  if (height_map[k][l].point.data[1] > max)
	    {
	      max = (int)height_map[k][l].point.data[1];
	    }
	  else if (height_map[k][l].point.data[1] < min)
	    {
	      min = (int)height_map[k][l].point.data[1];
	    }
	}
    }

  scalar norm = max - min;
  if (norm == 0.0)
    {
      norm = 0.0001;
    }

  for (k = 0; k < height; k++)
    {
      for (l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[1] = (height_map[k][l].point.data[1] - min) / norm;
	}
    }

  //flatten terrain
  
  for (k = 0; k < height; k++)
    {
      for (l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[1] *= height_map[k][l].point.data[1];
	}
    }
  
  //finalize height map (by adding x and z values and scaling by spread and resolution
  
  for (k = 0; k < height; k++)
    {
      for (l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[0] = k * resolution;
	  height_map[k][l].point.data[1] *= scale;
	  height_map[k][l].point.data[2] = l * resolution;
	}
    }
    
  //create quad list for vertex normal calculation (for per vertex lighting)
  list<WP_Quad*> quads;

 for (k = 0; k < height - 1; k++)
    {
      for (l = 0; l < width - 1; l++)
	{
	  WP_Quad* quad = new WP_Quad();
	  quad->vertices[0] = &height_map[k + 1][l + 1];
	  quad->vertices[1] = &height_map[k][l + 1];
	  quad->vertices[2] = &height_map[k][l];
	  quad->vertices[3] = &height_map[k + 1][l];

	  // compute normals

	  WP_Vector3D v1 = quad->vertices[0]->point - quad->vertices[3]->point;
	  WP_Vector3D v2 = quad->vertices[2]->point - quad->vertices[1]->point;
	  if (!v1.crossProduct(v2))
	    {
	      v1 = WP_Vector3D(1.0, 1.0, 1.0);
	    }
	  else
	    {
	      v1.normalize();
	    }
	  quad->normal = v1;
	  quads.push_back(quad);
	}
    }
  
 list<WP_Quad*>::const_iterator q;

 // calculate normal for each individual vertex

 for (k = 0; k < height - 1; k++)
    {
      for (l = 0; l < width - 1; l++)
	{
	  WP_Vector3D normal_sum(1.0, 1.0, 1.0);
	  q = quads.begin();
	  WP_Vertex* v = &height_map[k][l];
	  while (q != quads.end())
	    {	
	      if ((*q)->vertices[0] == v || (*q)->vertices[1] == v || (*q)->vertices[2] == v || (*q)->vertices[3] == v)
	      {
	        normal_sum += (*q)->normal;
	      }
	      q++;
	    }

	  normal_sum.normalize();

	  // find vertex in height map

	  q = quads.begin();
	  while (q != quads.end())
	    {	
	      if ((*q)->vertices[0] == v)
	      {
		(*q)->vertices[0]->normal = normal_sum;
		break;
	      }
	      else if ((*q)->vertices[1] == v)
	      {
		(*q)->vertices[1]->normal = normal_sum;
		break;
	      }
	      else if ((*q)->vertices[2] == v)
	      {
		(*q)->vertices[2]->normal = normal_sum;
		break;
	      }
	      else if ((*q)->vertices[3] == v)
	      {
		(*q)->vertices[3]->normal = normal_sum;
		break;
	      }
	      q++;
	    }
	}
    }
 
 WP_TextureManager* tex_manager = WP_TextureManager::getInstance();
 tex_manager->mipmapping = true;
 GLuint ft_id = tex_manager->getTexture("terrain.pcx", this);

  //create display list

  displayID = glGenLists(1);
  glNewList(displayID, GL_COMPILE);
  glPushMatrix();
  WP_GLState::getInstance()->enableTexture2D(); 
  glBindTexture(GL_TEXTURE_2D, ft_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high

  q = quads.begin();
  while (q != quads.end())
    {
     glBegin(GL_POLYGON);
     glColor4f(1.0, 1.0, 1.0, 1.0);
     glTexCoord2f(0.0, 0.0);
     glNormal3fv((*q)->vertices[3]->normal.data);
     glVertex3fv((*q)->vertices[3]->point.data);
     glColor4f(1.0, 1.0, 1.0, 1.0);
     glTexCoord2f(0.0, 1.0);
     glNormal3fv((*q)->vertices[2]->normal.data);
     glVertex3fv((*q)->vertices[2]->point.data);
     glColor4f(1.0, 1.0, 1.0, 1.0);
     glTexCoord2f(1.0, 1.0);
     glNormal3fv((*q)->vertices[1]->normal.data);
     glVertex3fv((*q)->vertices[1]->point.data);
     glColor4f(1.0, 1.0, 1.0, 1.0);
     glTexCoord2f(1.0, 0.0);
     glNormal3fv((*q)->vertices[0]->normal.data);
     glVertex3fv((*q)->vertices[0]->point.data);
     glEnd();
     ++q;
   }

  WP_GLState::getInstance()->disableTexture2D(); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
  glPopMatrix();
  glEndList();

  //clear quads
  q = quads.begin();
  while (q != quads.end())
    {	
      delete (*q);
      q++;
    }

  for (i = 0; i < height; i++)
    {
      delete[] height_map[i];
    }
  delete[] height_map;
  
  displacement_x = (width * resolution) / 2;
  displacement_y = (height * resolution) / 2;
}

WP_Terrain::~WP_Terrain()
{
  if (displayID != 0)
    {
      glDeleteLists(displayID, 1);
    }
}

void WP_Terrain::drawTerrain() const
{
  glPushMatrix();
  glTranslatef(middle.data[0] - displacement_x, middle.data[1], middle.data[2] - displacement_y);
  glCallList(displayID);
  glPopMatrix();
}
}
