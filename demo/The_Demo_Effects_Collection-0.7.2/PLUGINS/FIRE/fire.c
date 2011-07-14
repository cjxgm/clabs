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

#include <stdlib.h>

#include "utils.h"
#include "layers.h"
#include "fire.h"

static SDL_Surface *_fire_surface;
static SDL_Color _fire_colors[256];
static Uint16 _fire_width;
static Uint16 _fire_height;
static Uint16 _fire_xpos;
static Uint16 _fire_ypos;
static Uint8 *_fire_fire;

void fire_LTX_init_effect_valist(SDL_Surface* s, void (*restart)(void), va_list parameters)
{
  unsigned int i;

  _fire_surface = s;
  _fire_width = (Uint16)va_arg(parameters, int);
  _fire_height = (Uint16)va_arg(parameters, int);
  _fire_xpos = (Uint16)va_arg(parameters, int);
  _fire_ypos = (Uint16)va_arg(parameters, int);

  if (_fire_width > _fire_surface->w || _fire_width == 0)
    {
      _fire_width = _fire_surface->w;
    }

  if (_fire_height > _fire_surface->h || _fire_height == 0)
    {
      _fire_height = _fire_surface->h;
    }

  if (_fire_xpos > _fire_surface->w || _fire_xpos + _fire_width >= _fire_surface->w)
    {
      _fire_xpos = 0;
    }

  if (_fire_ypos > _fire_surface->h || _fire_ypos + _fire_height >= _fire_surface->h)
    {
      _fire_ypos = 0;
    }

  /* create fire array */

  _fire_fire = (Uint8*)calloc(_fire_width * _fire_height, sizeof(Uint8));
 
  /* create a suitable fire palette, this is crucial for a good effect */
  /* black to blue, blue to red, red to yellow, yellow to white*/
  
 for (i = 0; i < 32; ++i)
    {
      /* black to blue, 32 values*/
      _fire_colors[i].b = i << 1;

      /* blue to red, 32 values*/
      _fire_colors[i + 32].r = i << 3;
      _fire_colors[i + 32].b =  64 - (i << 1);

      /*red to yellow, 32 values*/
      _fire_colors[i + 64].r = 255;
      _fire_colors[i + 64].g = i << 3;

      /* yellow to white, 162 */
      _fire_colors[i + 96].r = 255;
      _fire_colors[i + 96].g = 255;
      _fire_colors[i + 96].b = i << 2;
      _fire_colors[i + 128].r = 255;
      _fire_colors[i + 128].g = 255;
      _fire_colors[i + 128].b = 64 + (i << 2);
      _fire_colors[i + 160].r = 255;
      _fire_colors[i + 160].g = 255;
      _fire_colors[i + 160].b = 128 + (i << 2);
      _fire_colors[i + 192].r = 255;
      _fire_colors[i + 192].g = 255;
      _fire_colors[i + 192].b = 192 + i;
      _fire_colors[i + 224].r = 255;
      _fire_colors[i + 224].g = 255;
      _fire_colors[i + 224].b = 224 + i;
    } 
 
 if (_fire_surface->format->palette)
   {
     SDL_SetPalette(_fire_surface, SDL_LOGPAL | SDL_PHYSPAL, _fire_colors, 0, 256); 
   }
}

void fire_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  unsigned int i;

  _fire_surface = s;
  _fire_width = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  _fire_height = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  _fire_xpos =*(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  _fire_ypos = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);

  if (_fire_width > _fire_surface->w || _fire_width == 0)
    {
      _fire_width = _fire_surface->w;
    }

  if (_fire_height > _fire_surface->h || _fire_height == 0)
    {
      _fire_height = _fire_surface->h;
    }

  if (_fire_xpos > _fire_surface->w || _fire_xpos + _fire_width >= _fire_surface->w)
    {
      _fire_xpos = 0;
    }

  if (_fire_ypos > _fire_surface->h || _fire_ypos + _fire_height >= _fire_surface->h)
    {
      _fire_ypos = 0;
    }

  /* create fire array */

  _fire_fire = (Uint8*)calloc(_fire_width * _fire_height, sizeof(Uint8));
 
  /* create a suitable fire palette, this is crucial for a good effect */
  /* black to blue, blue to red, red to yellow, yellow to white*/
  
 for (i = 0; i < 32; ++i)
    {
      /* black to blue, 32 values*/
      _fire_colors[i].b = i << 1;

      /* blue to red, 32 values*/
      _fire_colors[i + 32].r = i << 3;
      _fire_colors[i + 32].b =  64 - (i << 1);

      /*red to yellow, 32 values*/
      _fire_colors[i + 64].r = 255;
      _fire_colors[i + 64].g = i << 3;

      /* yellow to white, 162 */
      _fire_colors[i + 96].r = 255;
      _fire_colors[i + 96].g = 255;
      _fire_colors[i + 96].b = i << 2;
      _fire_colors[i + 128].r = 255;
      _fire_colors[i + 128].g = 255;
      _fire_colors[i + 128].b = 64 + (i << 2);
      _fire_colors[i + 160].r = 255;
      _fire_colors[i + 160].g = 255;
      _fire_colors[i + 160].b = 128 + (i << 2);
      _fire_colors[i + 192].r = 255;
      _fire_colors[i + 192].g = 255;
      _fire_colors[i + 192].b = 192 + i;
      _fire_colors[i + 224].r = 255;
      _fire_colors[i + 224].g = 255;
      _fire_colors[i + 224].b = 224 + i;
    } 
 
 if (_fire_surface->format->palette)
   {
     SDL_SetPalette(_fire_surface, SDL_LOGPAL | SDL_PHYSPAL, _fire_colors, 0, 256); 
   }
}

void fire_LTX_draw_effect()
{
  Uint16 temp;
  int i,j;  

  /* draw random bottom line in fire array */
  
  j = _fire_width * (_fire_height - 1);
  for (i = 0; i < _fire_width; ++i)
    {
      int random = 1 + (int)(16.0 * (rand()/(RAND_MAX+1.0)));
      if (random > 9) /* the lower the value, the intenser the fire, compensate a lower value with a higher decay value*/
	_fire_fire[j + i] = 255; /*maximum heat*/
      else
	_fire_fire[j + i] = 0;
    }  
  
  /* move fire upwards, start at bottom*/

  while (j >= _fire_width)
    {
      for (i = 0; i < _fire_width; ++i)
	{
	  if (i == 0) /* at the left border */
	    {
	      temp = _fire_fire[j];
	      temp += _fire_fire[j + 1];
	      temp += _fire_fire[j - _fire_width];
	      temp /=3;
	    }
	  else if (i == _fire_width - 1) /* at the right border */
	    {
	      temp = _fire_fire[j + i];
	      temp += _fire_fire[j - 1];
	      temp += _fire_fire[j + i - 1];
	      temp /= 3;
	    }
	  else
	    {
	      temp = _fire_fire[j + i];
	      temp += _fire_fire[j + i + 1];
	      temp += _fire_fire[j + i - 1];
	      temp += _fire_fire[j - _fire_width + i];
	      temp >>= 2;
	    }

	  if (temp > 28)
	    {
	      temp -= 1; /* decay */
	    }
	  else
	    {
	      temp = 0;
	    }
	  
	  _fire_fire[j - _fire_width + i] = temp;
	}
      j -= _fire_width;
    }      

  /* draw fire array to surface */

  for (i = 0; i < _fire_height; ++i)
    {
      for (j = 0; j < _fire_width; ++j)
	{
	  Uint16 temp = _fire_width * i;
	  TDEC_put_pixel(_fire_surface, _fire_xpos + j , _fire_ypos + i, SDL_MapRGB(_fire_surface->format,
										    _fire_colors[_fire_fire[temp + j]].r, 
										    _fire_colors[_fire_fire[temp + j]].g, 
										    _fire_colors[_fire_fire[temp + j]].b));
	}
    }
}

void fire_LTX_free_effect()
{
  free(_fire_fire);
}

Uint8 fire_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
