/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl, Christophe Conduche - cconduche@apm-online.fr

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

#include <math.h>

#include "layers.h"
#include "rectfill.h"

static SDL_Surface *_rectfill_surface;
static void (*_rectfill_restart)(void);
static Uint8 _rectfill_inner_to_outer;
static SDL_Rect _rectfill_r;
static Uint8 _rectfill_rate;
static float _rectfill_ratio;
static Uint8 _rectfill_color_r;
static Uint8 _rectfill_color_g;
static Uint8 _rectfill_color_b;

void rectfill_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  _rectfill_surface = s;
  _rectfill_restart = restart;
  _rectfill_inner_to_outer = (Uint8)va_arg(parameters, int);
  _rectfill_rate = (Uint8)va_arg(parameters, int);
  _rectfill_color_r = (Uint8)va_arg(parameters, int);
  _rectfill_color_g = (Uint8)va_arg(parameters, int);
  _rectfill_color_b = (Uint8)va_arg(parameters, int);

  if (_rectfill_rate % 2)
    {
      _rectfill_rate++;
    }

  if (_rectfill_inner_to_outer)
    {
      _rectfill_r.x = _rectfill_surface->w >> 1;
      _rectfill_r.y = _rectfill_surface->h >> 1;
      _rectfill_r.w = 0;
      _rectfill_r.h = 0;
    }
  else
    {
      _rectfill_r.x = 0;
      _rectfill_r.y = 0;
      _rectfill_r.w = _rectfill_surface->w;
      _rectfill_r.h = _rectfill_surface->w;
    }

  _rectfill_ratio = _rectfill_surface->h / (float)_rectfill_surface->w;
}

void rectfill_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  _rectfill_surface = s;
  _rectfill_restart = restart;
  _rectfill_inner_to_outer = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _rectfill_rate = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _rectfill_color_r = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _rectfill_color_g = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _rectfill_color_b = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);

  if (_rectfill_rate % 2)
    {
      _rectfill_rate++;
    }

  if (_rectfill_inner_to_outer)
    {
      _rectfill_r.x = _rectfill_surface->w >> 1;
      _rectfill_r.y = _rectfill_surface->h >> 1;
      _rectfill_r.w = 0;
      _rectfill_r.h = 0;
    }
  else
    {
      _rectfill_r.x = 0;
      _rectfill_r.y = 0;
      _rectfill_r.w = _rectfill_surface->w;
      _rectfill_r.h = _rectfill_surface->w;
    }

  _rectfill_ratio = _rectfill_surface->h / (float)_rectfill_surface->w;
}

void rectfill_LTX_draw_effect(void)
{
  if (_rectfill_inner_to_outer)
    {
      if (_rectfill_r.w >= _rectfill_surface->w || _rectfill_r.h >= _rectfill_surface->h)
	{
	  /* ready so call restart callback */
	  if (_rectfill_restart)
	    {
	      (*_rectfill_restart)(); /* call restart callback */
	      return;
	    }
	}
      else
	{
	  SDL_FillRect(_rectfill_surface, &_rectfill_r, SDL_MapRGBA(_rectfill_surface->format,
    _rectfill_color_r, _rectfill_color_g, _rectfill_color_b, 0));
	  _rectfill_r.x -= _rectfill_rate;
	  _rectfill_r.y -= _rectfill_rate * _rectfill_ratio;
	  _rectfill_r.w += _rectfill_rate << 1;
	  _rectfill_r.h += (_rectfill_rate << 1) * _rectfill_ratio;
	}
    }
  else
    {
      if (_rectfill_r.w <= 0 || _rectfill_r.h <= 0)
	{
	  /* ready so call restart callback */
	  if (_rectfill_restart)
	    {
	      (*_rectfill_restart)(); /* call restart callback */
	      return;
	    }
	}
      else
	{
	  SDL_FillRect(_rectfill_surface, &_rectfill_r, SDL_MapRGBA(_rectfill_surface->format, 
        _rectfill_color_r, _rectfill_color_g, _rectfill_color_b, 0));
	  _rectfill_r.x += _rectfill_rate;
	  _rectfill_r.y += _rectfill_rate * _rectfill_ratio;;
	  _rectfill_r.w -= _rectfill_rate << 1;
	  _rectfill_r.h -= (_rectfill_rate << 1) * _rectfill_ratio;
	}
    }
}

void rectfill_LTX_free_effect(void)
{
}

Uint8 rectfill_LTX_is_filter(void)
{
  return TDEC_FILTER;
}
