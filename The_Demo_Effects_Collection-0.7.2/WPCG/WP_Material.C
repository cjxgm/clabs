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

#include "WP_Material.h"

namespace WPCG
{
WP_Material::WP_Material()
{ 
	setDefault();
}

WP_Material::~WP_Material(){}

/* ASSIGNMENT OPERATOR */
WP_Material& WP_Material::operator=(const WP_Material& m)
{
  if (this == &m)
    return *this;
  
  ambient = m.ambient;
  diffuse = m.diffuse;
  specular = m.specular;
  emissive = m.emissive;
  specularExponent = m.specularExponent;
  
  return *this;
}

void WP_Material::setDefault()
{
  ambient.components[0] = 0.11;
  ambient.components[1] = 0.11;
  ambient.components[2] = 0.11;
  
  diffuse.components[0] = 0.75;
  diffuse.components[1] = 0.75;
  diffuse.components[2] = 0.75;
  
  specular.components[0] = 0.0;
  specular.components[1] = 0.0;
  specular.components[2] = 0.0;
  
  emissive.components[0] = 0;
  emissive.components[1] = 0;
  emissive.components[2] = 0;
  specularExponent = 0.0;
}

void WP_Material::initMaterialGL() const
{
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient.components);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse.components);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular.components);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emissive.components);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, specularExponent);
}
}

