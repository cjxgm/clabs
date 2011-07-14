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

#include <math.h>
#include "scroller.h"
#include "layers.h"
#include "sinescroller.h"

#define _sine_ _sinescroller_

static SDL_Surface *_sine_scroll_surface;
static short _sine_aSin[360];
static SDL_Surface *_sine_surface;
static Uint16 _sine_displacement;
static SDL_Rect _sine_srect2;
static SDL_Rect _sine_drect;
static SDL_Rect _sine_srect;
static SDL_Rect _sine_frect;
static Uint16 _sine_sine_index;
static Uint8 _sine_pixels;
static char _sine_scroll_id;
static void (*_sine_restart)(void);

void sinescroller_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  float rad;
  Uint16 i, centery;
  SDL_Surface *temp;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;
  Uint16 amplitude;

  _sine_restart = restart;

  _sine_sine_index = 0;

 _text = va_arg(parameters, char*);
  font = va_arg(parameters, char*);
  _characters = va_arg(parameters, char*);
  character_width = (Uint8)va_arg(parameters, int);
  character_height = (Uint8)va_arg(parameters, int);
  amplitude = (Uint16)va_arg(parameters, int);
  _sine_pixels = (Uint8)va_arg(parameters, int);

  if ((_sine_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating sine scroller\n");
      return;
    }

  _sine_surface = s;
  centery = _sine_surface->h >> 1;

  _sine_srect2.x = _sine_srect2.y = _sine_drect.x = _sine_drect.y = _sine_srect.y = _sine_frect.y = 0;
  _sine_srect2.w = _sine_drect.w = _sine_pixels;
  _sine_srect2.h = _sine_drect.h = _sine_frect.h = _sine_srect.h = character_height;
  _sine_srect.x = 2;
  _sine_srect.w = _sine_surface->w + character_width;
  _sine_frect.x = _sine_surface->w;
  _sine_frect.w = character_width;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _sine_aSin[i] = centery + (short)((sin(rad) * (float)amplitude));
    }
  
  /* create scroll surface, this surface must be wider than the surface width to be able to place the characters */
  temp = SDL_CreateRGBSurface(_sine_surface->flags, _sine_surface->w + character_width, character_height,
			      _sine_surface->format->BitsPerPixel, 
			      _sine_surface->format->Rmask, _sine_surface->format->Gmask, _sine_surface->format->Bmask, 
			      _sine_surface->format->Amask); 
  _sine_scroll_surface = SDL_ConvertSurface(temp, _sine_surface->format, _sine_surface->flags);
  SDL_FreeSurface(temp);

  _sine_displacement = 0;
}

void sinescroller_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  float rad;
  Uint16 i, centery;
  SDL_Surface *temp;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;
  Uint16 amplitude;

  _sine_restart = restart;

  _sine_sine_index = 0;

  _text = *(char**)TDEC_LIST_get_data_next(&argument_list);
  font = *(char**)TDEC_LIST_get_data_next(&argument_list);
  _characters = *(char**)TDEC_LIST_get_data_next(&argument_list);
  character_width = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  character_height = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  amplitude = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  _sine_pixels = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);

  if ((_sine_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating sine scroller\n");
      return;
    }

  _sine_surface = s;
  centery = _sine_surface->h >> 1;

  _sine_srect2.x = _sine_srect2.y = _sine_drect.x = _sine_drect.y = _sine_srect.y = _sine_frect.y = 0;
  _sine_srect2.w = _sine_drect.w = _sine_pixels;
  _sine_srect2.h = _sine_drect.h = _sine_frect.h = _sine_srect.h = character_height;
  _sine_srect.x = 2;
  _sine_srect.w = _sine_surface->w + character_width;
  _sine_frect.x = _sine_surface->w;
  _sine_frect.w = character_width;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _sine_aSin[i] = centery + (short)((sin(rad) * (float)amplitude));
    }
  
  /* create scroll surface, this surface must be wider than the surface width to be able to place the characters */
  temp = SDL_CreateRGBSurface(_sine_surface->flags, _sine_surface->w + character_width, character_height,
			      _sine_surface->format->BitsPerPixel, 
			      _sine_surface->format->Rmask, _sine_surface->format->Gmask, _sine_surface->format->Bmask, 
			      _sine_surface->format->Amask); 
  _sine_scroll_surface = SDL_ConvertSurface(temp, _sine_surface->format, _sine_surface->flags);
  SDL_FreeSurface(temp);

  _sine_displacement = 0;
}

void sinescroller_LTX_draw_effect(void)
{
  Uint16 i;
  
  /* scroll scroll_surface to the left */
  SDL_BlitSurface(_sine_scroll_surface, &_sine_srect, _sine_scroll_surface, 0);

  _sine_displacement += 2;
      
  if (_sine_displacement > 30)
    {
      TDEC_draw_font_char(_sine_scroll_id, TDEC_get_font_char(_sine_scroll_id), _sine_scroll_surface, &_sine_frect);
      _sine_displacement = 0;
    }

  if (TDEC_scroller_ready(_sine_scroll_id))
    {
      if (_sine_restart)
	{
	  (*_sine_restart)(); /*call scroller restart callback */
	  return;
	}
    }
    
  /* clean sinus area */
  SDL_FillRect(_sine_surface, 0, SDL_MapRGB(_sine_surface->format, 0, 0, 0));
   
  /* create sinus in scroll */
  for (i = 0; i < _sine_surface->w; i += _sine_pixels)
    {
      _sine_srect2.x = _sine_drect.x = i;
      _sine_drect.y = _sine_aSin[(_sine_sine_index + i) % 360];
      SDL_BlitSurface(_sine_scroll_surface, &_sine_srect2, _sine_surface, &_sine_drect);
    }
  _sine_sine_index += 6;
  _sine_sine_index %= 360;
}

void sinescroller_LTX_free_effect(void)
{
  SDL_FreeSurface(_sine_scroll_surface);
  TDEC_free_scroller(_sine_scroll_id);
}

Uint8 sinescroller_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
