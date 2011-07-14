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
#include "jumpingscroller.h"

#define _jump_ _jumpingscroller_

static SDL_Surface *_jump_surface;
static short _jump_aSin[540];

typedef struct 
{
  short xpos;
  Uint16 sin_index;
  Uint16 font_pos;
} LETTER; 

static LETTER *_jump_letters;
static Uint8 _jump_nletters;
static Uint16 _jump_displacement;
static Uint16 _jump_width;
static Uint16 _jump_height;
static SDL_Rect _jump_frect;
static SDL_Rect _jump_srect;
static char _jump_scroll_id;
static void (*_jump_restart)(void);

void jumpingscroller_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  float rad;
  Uint16 i, j;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;
  short centery;

  _jump_restart = restart;

  _text = va_arg(parameters, char*);
  font = va_arg(parameters, char*);
  _characters = va_arg(parameters, char*);
  character_width = (Uint8)va_arg(parameters, int);
  character_height = (Uint8)va_arg(parameters, int);
  _jump_width = (Uint16)va_arg(parameters, int);
  _jump_height = (Uint16)va_arg(parameters, int);;
  centery = _jump_height / 2;

  if ((_jump_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating jumping scroller\n");
      return;
    }

  _jump_surface = s;
 
  /*create sin lookup table */

  for (i = 0, j = 0; i < 180; i++)
    {
      rad =  (float)j * 0.0174532; 
      _jump_aSin[i] = centery - (short)((sin(rad) * centery));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    } 
  for (i = 90, j = 90; i < 270; i++)
    {
      rad =  (float)j * 0.0174532; 
      _jump_aSin[i + 90] = centery - (short)((sin(rad) * centery));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    }
 
  for (i = 180; i < 270; i++)
    {
      rad =  (float)i * 0.0174532; 
      _jump_aSin[i + 180] = centery - (short)((sin(rad) * centery));
    } 
  for (i = 270; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _jump_aSin[i + 180] = centery - (short)((sin(rad) * centery));
    }

  _jump_nletters = _jump_width / character_width;
  _jump_letters = (LETTER*)malloc(_jump_nletters * sizeof(LETTER));
  
  /* reset letters */
  for (i = 0; i < _jump_nletters; ++i)
    {
      _jump_letters[i].xpos = -character_width;
    }

  _jump_displacement = 0;

  _jump_frect.h = TDEC_get_character_height(_jump_scroll_id);
  _jump_srect.w = TDEC_get_character_width(_jump_scroll_id);
  _jump_srect.h = _jump_frect.h;
}

void jumpingscroller_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  float rad;
  Uint16 i, j;
  char *_text, *font, *_characters;
  Uint8 character_width, character_height;
  short centery;

  _jump_restart = restart;

  _text = *(char**)TDEC_LIST_get_data_next(&argument_list);
  font = *(char**)TDEC_LIST_get_data_next(&argument_list);
  _characters = *(char**)TDEC_LIST_get_data_next(&argument_list);
  character_width = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  character_height = *(Uint8*)TDEC_LIST_get_data_next(&argument_list);
  _jump_width = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  _jump_height = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  centery = _jump_height / 2;

  if ((_jump_scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating jumping scroller\n");
      return;
    }

  _jump_surface = s;
 
  /*create sin lookup table */

  for (i = 0, j = 0; i < 180; i++)
    {
      rad =  (float)j * 0.0174532; 
      _jump_aSin[i] = centery - (short)((sin(rad) * centery));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    } 
  for (i = 90, j = 90; i < 270; i++)
    {
      rad =  (float)j * 0.0174532; 
      _jump_aSin[i + 90] = centery - (short)((sin(rad) * centery));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    }
 
  for (i = 180; i < 270; i++)
    {
      rad =  (float)i * 0.0174532; 
      _jump_aSin[i + 180] = centery - (short)((sin(rad) * centery));
    } 
  for (i = 270; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _jump_aSin[i + 180] = centery - (short)((sin(rad) * centery));
    }

  _jump_nletters = _jump_width / character_width;
  _jump_letters = (LETTER*)malloc(_jump_nletters * sizeof(LETTER));
  
  /* reset letters */
  for (i = 0; i < _jump_nletters; ++i)
    {
      _jump_letters[i].xpos = -character_width;
    }

  _jump_displacement = 0;

  _jump_frect.h = TDEC_get_character_height(_jump_scroll_id);
  _jump_srect.w = TDEC_get_character_width(_jump_scroll_id);
  _jump_srect.h = _jump_frect.h;
}

void jumpingscroller_LTX_draw_effect(void)
{
  Uint16 i;
  
  /* print character? */
  if (_jump_displacement > 20)
    {
      /* init new character */
      
      for (i = 0; i < _jump_nletters; ++i)
	{
	  /* find an unused letter */
	  
	  if (_jump_letters[i].xpos < -TDEC_get_character_width(_jump_scroll_id))
	    {
	      SDL_Rect *r = TDEC_get_font_char(_jump_scroll_id);
	      _jump_letters[i].xpos = _jump_width;
	      _jump_letters[i].sin_index = 0;
	      _jump_letters[i].font_pos = r->x;
	      break;
	    }
	}
      
      _jump_displacement = 0;
    }

  if (TDEC_scroller_ready(_jump_scroll_id))
    {
      if (_jump_restart)
	{
	  (*_jump_restart)(); /*call scroller restart callback */
	  return;
	}
    }
  
  _jump_displacement += 2;
  
  /* clear screen */
  
  SDL_FillRect(_jump_surface, 0, SDL_MapRGB(_jump_surface->format, 0, 0, 0));
  
  /* update letter positions */
  
  for (i = 0; i < _jump_nletters; ++i)
    {
      _jump_letters[i].xpos -= 3;
      if (_jump_letters[i].xpos  > -TDEC_get_character_width(_jump_scroll_id))
	{
	  _jump_letters[i].sin_index += 8;
	  _jump_letters[i].sin_index %= 540;
	  
	  if (_jump_letters[i].xpos < 0)
	    {
	      /* correct font and position when reaching left border */
	      
	      short diff = _jump_letters[i].xpos;
	      _jump_frect.x = _jump_letters[i].font_pos - 1 - diff;
	      _jump_frect.w = TDEC_get_character_width(_jump_scroll_id) +  diff;
	      _jump_srect.x = 0;
	    }
	  else
	    {
	      _jump_frect.x = _jump_letters[i].font_pos - 1;
	      _jump_frect.w = TDEC_get_character_width(_jump_scroll_id);
	      _jump_srect.x = _jump_letters[i].xpos;
	    }

	  _jump_srect.y = _jump_aSin[_jump_letters[i].sin_index];
	  
	  TDEC_draw_font_char(_jump_scroll_id, &_jump_frect, _jump_surface, &_jump_srect);
	}
    }
}

void jumpingscroller_LTX_free_effect(void)
{
  free(_jump_letters);
  TDEC_free_scroller(_jump_scroll_id);
}

Uint8 jumpingscroller_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
