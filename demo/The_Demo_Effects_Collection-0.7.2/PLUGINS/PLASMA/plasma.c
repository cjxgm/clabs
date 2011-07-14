/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#include <math.h>
#include <stdarg.h>

#include "SDL/SDL.h"

#include "utils.h"
#include "layers.h"
#include "plasma.h"

static SDL_Surface *_plasma_surface;
static Uint16 _plasma_pos1, _plasma_pos2, _plasma_pos3, _plasma_pos4, 
  _plasma_tpos1, _plasma_tpos2, _plasma_tpos3, _plasma_tpos4;
static int _plasma_aSin[512];
static SDL_Color _plasma_colors[256];
static Uint8 _plasma_pixelspcolor;

void plasma_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  int i;
  float rad;

  _plasma_pixelspcolor = (Uint8)va_arg(parameters, int);
 
  _plasma_surface = s;

  _plasma_pos1 = 0;
  _plasma_pos2 = 0;
  _plasma_pos3 = 0;
  _plasma_pos4 = 0;

  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
      _plasma_aSin[i] = sin(rad) * 1024; /*using fixed point math with 1024 as base*/
    }
      
  /* create color palette */
  for (i = 0; i < 64; ++i)
    {
      _plasma_colors[i].r = i << 2;
      _plasma_colors[i].g = 255 - ((i << 2) + 1); 
      _plasma_colors[i+64].r = 255;
      _plasma_colors[i+64].g = (i << 2) + 1;
      _plasma_colors[i+128].r = 255 - ((i << 2) + 1);
      _plasma_colors[i+128].g = 255 - ((i << 2) + 1);
      _plasma_colors[i+192].g = (i << 2) + 1; 
    } 

  if (_plasma_surface->format->palette)
    {
      SDL_SetPalette(_plasma_surface, SDL_LOGPAL | SDL_PHYSPAL, _plasma_colors, 0, 256); 
    }
}

void plasma_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  int i;
  float rad;

  _plasma_pixelspcolor = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
 
  _plasma_surface = s;

  _plasma_pos1 = 0;
  _plasma_pos2 = 0;
  _plasma_pos3 = 0;
  _plasma_pos4 = 0;

  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
      _plasma_aSin[i] = sin(rad) * 1024; /*using fixed point math with 1024 as base*/
    }
      
  /* create color palette */
  for (i = 0; i < 64; ++i)
    {
      _plasma_colors[i].r = i << 2;
      _plasma_colors[i].g = 255 - ((i << 2) + 1); 
      _plasma_colors[i+64].r = 255;
      _plasma_colors[i+64].g = (i << 2) + 1;
      _plasma_colors[i+128].r = 255 - ((i << 2) + 1);
      _plasma_colors[i+128].g = 255 - ((i << 2) + 1);
      _plasma_colors[i+192].g = (i << 2) + 1; 
    } 

  if (_plasma_surface->format->palette)
    {
      SDL_SetPalette(_plasma_surface, SDL_LOGPAL | SDL_PHYSPAL, _plasma_colors, 0, 256); 
    }
}

void plasma_LTX_draw_effect()
{
  Uint16 i,j;
  Uint8 index, k, l;
  int x;  
  
  _plasma_tpos4 = _plasma_pos4;
  _plasma_tpos3 = _plasma_pos3;
  
  for (i = 0; i + _plasma_pixelspcolor - 1 < _plasma_surface->h; i += _plasma_pixelspcolor)
    {
      _plasma_tpos1 = _plasma_pos1 + 5;
      _plasma_tpos2 = _plasma_pos2 + 3;
      
      _plasma_tpos3 &= 511;
      _plasma_tpos4 &= 511;
      
      for (j = 0; j  + _plasma_pixelspcolor - 1 < _plasma_surface->w;)
	{
	  _plasma_tpos1 &= 511;
	  _plasma_tpos2 &= 511;
	  
	  x = _plasma_aSin[_plasma_tpos1] + _plasma_aSin[_plasma_tpos2] 
	    + _plasma_aSin[_plasma_tpos3] + _plasma_aSin[_plasma_tpos4]; /* actual plasma calculation */
	  
	  index = 128 + (x >> 4); /*fixed point multiplication but optimized so basically it says (x * (64 * 1024) / (1024 * 1024)), x is already multiplied by 1024 */
	  
	  for (k = 0; k < _plasma_pixelspcolor; ++j, ++k)
	    {
	      for (l = 0; l < _plasma_pixelspcolor; ++l)
		{
		  TDEC_put_pixel(_plasma_surface, j, i + l, SDL_MapRGB(_plasma_surface->format, _plasma_colors[index].r, 
							       _plasma_colors[index].g, _plasma_colors[index].b));
		}
	    }
	  
	  _plasma_tpos1 += 5; 
	  _plasma_tpos2 += 3; 
	}
      
      _plasma_tpos4 += 3;
      _plasma_tpos3 += 1;
    }
  
  /* move plasma */
  
  _plasma_pos1 +=9;
  _plasma_pos3 +=8;
}
  
void plasma_LTX_free_effect(void)
{
}

Uint8 plasma_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
