/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

/* for a correct use of libtool, global symbols are prefixed with <modulename>_LTX_ and internal symbols are prefixed with _<modulename>_ */

#include <stdlib.h>

#include "utils.h"
#include "layers.h"
#include "starfield.h"

#define _star_ _starfield_

static SDL_Surface *_star_surface;
static Uint16 _star_nstars;
static Uint16 _star_centerx, _star_centery;
static SDL_Color _star_colors[256];

typedef struct 
{
  float xpos, ypos;
  short zpos, speed;
  Uint8 color;
} STAR;

static STAR *_star_stars;

static void _star_init_star(STAR* star, int i)
{
  /* randomly init stars, generate them around the center of the screen */
  
  star->xpos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  star->ypos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i;
  star->speed =  2 + (int)(2.0 * (rand()/(RAND_MAX+1.0)));

  star->color = i >> 2; /*the closer to the viewer the brighter*/
}

void starfield_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  Uint16 i;

  _star_surface = s;
  _star_nstars =  (Uint16)va_arg(parameters, int);

  _star_centerx = _star_surface->w >> 1;
  _star_centery = _star_surface->h >> 1;

  /* create and set palette */
  _star_colors[0].r = 0;
  _star_colors[0].g = 0;
  _star_colors[0].b = 0;

  for (i = 0; i < 255; ++i)
    {
      _star_colors[255 - i].r = i;
      _star_colors[255 - i].g = i;
      _star_colors[255 - i].b = i;
    }
  
  if (_star_surface->format->palette)
    {
      SDL_SetPalette(_star_surface, SDL_LOGPAL | SDL_PHYSPAL, _star_colors, 0, 256); 
    }

  /*create stars*/
  _star_stars = (STAR*)malloc(_star_nstars * sizeof(STAR));

  for (i = 0; i < _star_nstars; i++)
    {
      _star_init_star(_star_stars + i, i + 1);
    }
}

void starfield_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  Uint16 i;

  _star_surface = s;
  _star_nstars =  *(Uint16*)TDEC_LIST_get_data_next(&argument_list);

  _star_centerx = _star_surface->w >> 1;
  _star_centery = _star_surface->h >> 1;

  /* create and set palette */
  _star_colors[0].r = 0;
  _star_colors[0].g = 0;
  _star_colors[0].b = 0;

  for (i = 0; i < 255; ++i)
    {
      _star_colors[255 - i].r = i;
      _star_colors[255 - i].g = i;
      _star_colors[255 - i].b = i;
    }
  
  if (_star_surface->format->palette)
    {
      SDL_SetPalette(_star_surface, SDL_LOGPAL | SDL_PHYSPAL, _star_colors, 0, 256); 
    }

  /*create stars*/
  _star_stars = (STAR*)malloc(_star_nstars * sizeof(STAR));

  for (i = 0; i < _star_nstars; i++)
    {
      _star_init_star(_star_stars + i, i + 1);
    }
}

void starfield_LTX_draw_effect(void)
{
  SDL_Color *c;
  int tempx, tempy;  
  Uint16 i;

  /* clear screen */
  
  SDL_FillRect(_star_surface, 0, SDL_MapRGB(_star_surface->format, 0, 0, 0));
    
  /* move and draw stars */
      
  for (i = 0; i < _star_nstars; ++i)
    {
      _star_stars[i].zpos -= _star_stars[i].speed;
      
      if (_star_stars[i].zpos <= 0)
	{
	  _star_init_star(_star_stars + i, i + 1);
	}
      
      /*compute 3D position*/
      tempx = (_star_stars[i].xpos / _star_stars[i].zpos) + _star_centerx;
      tempy = (_star_stars[i].ypos / _star_stars[i].zpos) + _star_centery;

      if (tempx < 0 || tempx > _star_surface->w - 1 || tempy < 0 || tempy > _star_surface->h - 1) /*check if a star leaves the screen*/
	{
	  _star_init_star(_star_stars + i, i + 1);
	  continue;
	}
      
      c = &_star_colors[_star_stars[i].color];
      TDEC_put_pixel(_star_surface, tempx, tempy, SDL_MapRGB(_star_surface->format, c->r, c->g, c->b));
    }
}

void starfield_LTX_free_effect(void)
{
  free(_star_stars);
}

Uint8 starfield_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
