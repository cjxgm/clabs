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
#include <math.h>

#include "scroller.h"
#include "layers.h"
#include "circlescroller.h"

#define _circle_ _circlescroller_

static SDL_Surface *_circle_surface;
static short _circle_aSin[540];
static short _circle_aCos[540];
static short _circle_movement[512];

typedef struct 
{
  Uint16 sin_index;
  Uint16 font_pos;
} LETTER; 

static LETTER *_circle_letters;
static Uint8 _circle_nletters;
static Uint16 _circle_displacement;
static Uint16 _circle_radius;
static Uint16 _circle_rightborder;
static Uint16 _circle_bottomborder;
static SDL_Rect _circle_frect;
static SDL_Rect _circle_srect;
static char _circle_scroll_id;
static Uint16 _circle_x_move, _circle_y_move;
static void (*_circle_restart)(void);

void circlescroller_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  float rad;
  Uint16 i;
  short centerx, centery;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;

  _circle_x_move = 0;
  _circle_y_move = 0;

  _circle_restart = restart;

  _text = va_arg(parameters, char*);
  font = va_arg(parameters, char*);
  _characters = va_arg(parameters, char*);
  character_width = (Uint8)va_arg(parameters, int);
  character_height = (Uint8)va_arg(parameters, int);
  _circle_radius = (Uint16)va_arg(parameters, int);

  if ((_circle_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating circle scroller\n");
      return;
    }

  /* letters can overlap so blit with black as transparant color */

  TDEC_set_font_colorkey(_circle_scroll_id, 0, 0, 0, 0);

  _circle_surface = s;
 
  centerx = _circle_surface->w >> 1;
  centery = _circle_surface->h >> 1;

  _circle_rightborder = _circle_surface->w - character_width;
  _circle_bottomborder = _circle_surface->h - character_height;

  /*create sin lookup table */

  for (i = 0; i < 360; ++i)
    {
      rad =  (float)i * 0.0174532;
      _circle_aSin[i] = centery - (short)((sin(rad) * (float)_circle_radius));
      _circle_aCos[i] = centerx - (short)((cos(rad) * (float)_circle_radius));
    }

  for (i = 0; i < 512; ++i)
    {
      rad =  (float)i * 0.0174532 * 0.703125;
      _circle_movement[i] = sin(rad) * (_circle_surface->w / 3.0); 
    }

  _circle_nletters = (_circle_radius / character_height) * 10;
  _circle_letters = (LETTER*)malloc(_circle_nletters * sizeof(LETTER));
  
  /* reset letters */
  for (i = 0; i < _circle_nletters; ++i)
    {
      _circle_letters[i].sin_index = 0;
    }

  _circle_displacement = 0;

  _circle_frect.w = TDEC_get_character_width(_circle_scroll_id);
  _circle_frect.h = TDEC_get_character_height(_circle_scroll_id);
  _circle_srect.w = _circle_frect.w;
  _circle_srect.h = _circle_frect.h;
}

void circlescroller_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  float rad;
  Uint16 i;
  short centerx, centery;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;

  _circle_x_move = 0;
  _circle_y_move = 0;

  _circle_restart = restart;

  _text = *(char**)TDEC_LIST_get_data_next(&argument_list);
  font = *(char**)TDEC_LIST_get_data_next(&argument_list);
  _characters = *(char**)TDEC_LIST_get_data_next(&argument_list);
  character_width = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  character_height = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _circle_radius = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);

  if ((_circle_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating circle scroller\n");
      return;
    }

  /* letters can overlap so blit with black as transparant color */

  TDEC_set_font_colorkey(_circle_scroll_id, 0, 0, 0, 0);

  _circle_surface = s;
 
  centerx = _circle_surface->w >> 1;
  centery = _circle_surface->h >> 1;

  _circle_rightborder = _circle_surface->w - character_width;
  _circle_bottomborder = _circle_surface->h - character_height;

  /*create sin lookup table */

  for (i = 0; i < 360; ++i)
    {
      rad =  (float)i * 0.0174532;
      _circle_aSin[i] = centery - (short)((sin(rad) * (float)_circle_radius));
      _circle_aCos[i] = centerx - (short)((cos(rad) * (float)_circle_radius));
    }

  for (i = 0; i < 512; ++i)
    {
      rad =  (float)i * 0.0174532 * 0.703125;
      _circle_movement[i] = sin(rad) * (_circle_surface->w / 3.0); 
    }

  _circle_nletters = (_circle_radius / character_height) * 10;
  _circle_letters = (LETTER*)malloc(_circle_nletters * sizeof(LETTER));
  
  /* reset letters */
  for (i = 0; i < _circle_nletters; ++i)
    {
      _circle_letters[i].sin_index = 0;
    }

  _circle_displacement = 0;

  _circle_frect.w = TDEC_get_character_width(_circle_scroll_id);
  _circle_frect.h = TDEC_get_character_height(_circle_scroll_id);
  _circle_srect.w = _circle_frect.w;
  _circle_srect.h = _circle_frect.h;
}

void circlescroller_LTX_draw_effect(void)
{
  short i;

  /* print character? */
  if (_circle_displacement > 20)
    {
      /* init new character */
      
      for (i = 0; i < _circle_nletters; ++i)
	{
	  /* find an unused letter */
	  
	  if (_circle_letters[i].sin_index == 0)
	    {
	      SDL_Rect *r = TDEC_get_font_char(_circle_scroll_id);
	      _circle_letters[i].sin_index = 3;
	      _circle_letters[i].font_pos = r->x;
	      break;
	    }
	}
      
      _circle_displacement = 0;
    }

  if (TDEC_scroller_ready(_circle_scroll_id))
    {
      if (_circle_restart)
	{
	  (*_circle_restart)(); /*call scroller restart callback */
	  return;
	}
    }

  _circle_displacement += 3;

  /* clear screen */

  SDL_FillRect(_circle_surface, 0, SDL_MapRGB(_circle_surface->format, 0, 0, 0));

  /* update letter positions */

  for (i = _circle_nletters - 1; i >= 0; --i)
    {
      if (_circle_letters[i].sin_index > 0)
	{
	  /* blit letter to screen */
		
	  _circle_frect.x = _circle_letters[i].font_pos - 1;
	  _circle_srect.x = _circle_aCos[_circle_letters[i].sin_index] + _circle_movement[_circle_x_move];
	  _circle_srect.y = _circle_aSin[_circle_letters[i].sin_index] + _circle_movement[_circle_y_move];

	  /* check borders */
	  if (_circle_srect.x > _circle_rightborder)
	    {
	      _circle_srect.x = _circle_rightborder;
	    }
	  else if (_circle_srect.x < 0)
	    {
	      _circle_srect.x = 0;
	    }
		
	  if (_circle_srect.y > _circle_bottomborder)
	    {
	      _circle_srect.y = _circle_bottomborder;
	    }
	  else if (_circle_srect.y < 0)
	    {
	      _circle_srect.y = 0;
	    }
	  
	  TDEC_draw_font_char(_circle_scroll_id, &_circle_frect, _circle_surface, &_circle_srect);

	  _circle_letters[i].sin_index += 3;

	  /* spinned round? */

	  _circle_letters[i].sin_index %= 360;
	}
    }

  /* update movement of scroller */

  _circle_x_move += 2;
  _circle_y_move += 3;
  _circle_x_move &= 511;
  _circle_y_move &= 511;
}

void circlescroller_LTX_free_effect(void)
{
  free(_circle_letters);
  TDEC_free_scroller(_circle_scroll_id);
}

Uint8 circlescroller_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
