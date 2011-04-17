/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl, Christophe Conduche - cconduche@apm-online.fr

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
#include "utils.h"
#include "fade.h"

static SDL_Surface *_fade_surface;
static void (*_fade_restart)(void);
static Uint8 _fade_type;
static Uint8 _fade_factor;
static Uint8 _fade_alpha;
static Uint8 _fade_color_r;
static Uint8 _fade_color_g;
static Uint8 _fade_color_b;

void fade_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  _fade_surface = s;
  _fade_restart = restart;
  _fade_type = (Uint8)va_arg(parameters, int);
  _fade_factor = (Uint8)va_arg(parameters, int);
  _fade_color_r = (Uint8)va_arg(parameters, int);
  _fade_color_g = (Uint8)va_arg(parameters, int);
  _fade_color_b = (Uint8)va_arg(parameters, int);

  _fade_alpha = TDEC_get_layer_alpha( TDEC_get_layer_id(_fade_surface) );
  TDEC_set_layer_colorkey(TDEC_get_layer_id(_fade_surface), 0);
  SDL_FillRect(_fade_surface, 0, SDL_MapRGBA(_fade_surface->format, _fade_color_r, _fade_color_g, _fade_color_b, 0));
}

void fade_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  _fade_surface = s;
  _fade_restart = restart;
  _fade_type = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _fade_factor = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _fade_color_r = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _fade_color_g = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _fade_color_b = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);

  _fade_alpha = TDEC_get_layer_alpha( TDEC_get_layer_id(_fade_surface) );
  TDEC_set_layer_colorkey(TDEC_get_layer_id(_fade_surface), 0);
  SDL_FillRect(_fade_surface, 0, SDL_MapRGBA(_fade_surface->format, _fade_color_r, _fade_color_g, _fade_color_b, 0));	
}

void fade_LTX_draw_effect(void)
{
  if( _fade_type )
  {
    if( _fade_alpha < _fade_factor )
    {
      if (_fade_restart)
	{
	  (*_fade_restart)();
	  return;
	}
    }
    _fade_alpha -= _fade_factor;
 }
  else
  {
    if( _fade_alpha >(0xFF - _fade_factor) )
    {
      if (_fade_restart)
	{
	  (*_fade_restart)();
	  return;
	}
    }
     _fade_alpha += _fade_factor;
  }

  SDL_SetAlpha(_fade_surface, SDL_SRCALPHA, _fade_alpha);
}

void fade_LTX_free_effect(void)
{
}

Uint8 fade_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
