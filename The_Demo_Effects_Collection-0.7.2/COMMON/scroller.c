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

#include <stdlib.h>
#include <string.h>

#include "SDL/SDL_image.h"
#include "scroller.h"

typedef struct
{
  SDL_Surface *font_surface;
  char *text;
  char *characters;
  char *text_pointer;
  char *font_name;
  Uint8 cwidth;
  Uint8 cheight;
  SDL_Rect frect;
  Uint8 restarted;
  Uint8 shared_font;
} SCROLLER;

#define MAX_SCROLLERS 50

static SCROLLER scrollers[MAX_SCROLLERS];
static Uint8 scroller_system_initiated = 0;

static Uint16 TDEC_compute_font_pos(Uint8 scroll_id, char scroll_char);

char TDEC_add_scroller(char *_text, char *font, char *_characters,
			Uint8 character_width, Uint8 character_height)
{
  Uint8 scroller_index = 0;

  if (!_text || !font)
    {
      printf("TDEC_scroller error, supply scroller text and font name please\n");
      return -1;
    }

  if (!scroller_system_initiated)
    {
      /* initiate */

      Uint8 i;
      for (i = 0; i < MAX_SCROLLERS; ++i)
	{
	  scrollers[i].text = (char*)0;
	  scrollers[i].font_name = (char*)0;
	}
      scroller_system_initiated = 1;
    }

  for (; scroller_index < MAX_SCROLLERS; ++scroller_index)
    {
      if (scrollers[scroller_index].text == (char*)0)
	{
	  Uint8 i;

	  scrollers[scroller_index].text = _text;
	  scrollers[scroller_index].text_pointer = _text;

	  /* check if a font is already in use */

	  for (i = 0; i < MAX_SCROLLERS; ++i)
	    {
	      if (scrollers[i].font_name)
		{
		  if (!strcmp(scrollers[i].font_name, font))
		    {
#ifdef DEBUG
		      printf("Sharing font\n");
#endif
		      
		      /* font found, so share it, also copy characters and size information so a user has only to supply the font name in case of a shared font */
		      scrollers[scroller_index].font_surface = scrollers[i].font_surface;
		      scrollers[scroller_index].font_name = scrollers[i].font_name;
		      scrollers[scroller_index].characters = scrollers[i].characters;
		      scrollers[scroller_index].cwidth = scrollers[i].cwidth;
		      scrollers[scroller_index].cheight = scrollers[i].cheight;
		      scrollers[scroller_index].frect.x = 0;
		      scrollers[scroller_index].frect.y = 0;
		      scrollers[scroller_index].frect.w = scrollers[i].cwidth - 1;
		      scrollers[scroller_index].frect.h = scrollers[i].cheight;
		      scrollers[scroller_index].shared_font = 1;
		      break;
		    }
		}
	    }

	  if (!scrollers[scroller_index].font_name)
	    {
	      size_t l;
#ifdef DEBUG
	      printf("New font\n");
#endif
	      /* new font, so load it */
	      scrollers[scroller_index].font_surface = (SDL_Surface*)(IMG_Load(font));
	      scrollers[scroller_index].characters = _characters;
	      scrollers[scroller_index].cwidth = character_width;
	      scrollers[scroller_index].cheight = character_height;
	      scrollers[scroller_index].frect.x = 0;
	      scrollers[scroller_index].frect.y = 0;
	      scrollers[scroller_index].frect.w = character_width - 1;
	      scrollers[scroller_index].frect.h = character_height;
	      scrollers[scroller_index].shared_font = 0;

	      /* store font name */

	      l = strlen(font);
	      scrollers[scroller_index].font_name = malloc((l + 1) * sizeof(char));
	      strcpy(scrollers[scroller_index].font_name, font);
	    }
	  return scroller_index;
	}
    }

  printf("Unable to add scroller, maximum of %i scrollers reached\n",MAX_SCROLLERS);
  return -1;
}

/* determine the font character */
Uint16 TDEC_compute_font_pos(Uint8 scroll_id, char scroll_char)
{
  char* p = scrollers[scroll_id].characters;
  Uint16 pos = 0;

  if (scroll_char == '\0')
    {
      scrollers[scroll_id].text_pointer = scrollers[scroll_id].text;
      scroll_char = *scrollers[scroll_id].text_pointer++;
      scrollers[scroll_id].restarted = 1;
    }

  while (*p++ != scroll_char)
    {
      pos += scrollers[scroll_id].cwidth;
    }

  if (pos > 0)
    return pos - 1;

  return 0;
}

SDL_Rect* TDEC_get_font_char(Uint8 scroll_id) 
{
  /* determine font character according to position in scroll text */
  
  scrollers[scroll_id].frect.x = TDEC_compute_font_pos(scroll_id, *scrollers[scroll_id].text_pointer++);

  return &scrollers[scroll_id].frect;
}

void TDEC_free_scroller(Uint8 scroll_id)
{
  if (scroll_id < MAX_SCROLLERS)
    {
      if (!scrollers[scroll_id].shared_font)
	{
	  SDL_FreeSurface(scrollers[scroll_id].font_surface);
	  free(scrollers[scroll_id].font_name);
	  scrollers[scroll_id].font_name = (char*)0;
	}
      scrollers[scroll_id].text = (char*)0;
    }
}

Uint8 TDEC_scroller_ready(Uint8 scroll_id)
{
  if(scrollers[scroll_id].restarted)
    {
      scrollers[scroll_id].restarted = 0;
      return 1;
    }
  return 0;
}

Uint8 TDEC_get_character_width(Uint8 scroll_id)
{
  return scrollers[scroll_id].cwidth;
}

Uint8 TDEC_get_character_height(Uint8 scroll_id)
{
  return scrollers[scroll_id].cheight;
}

void TDEC_draw_font_char(Uint8 scroll_id, SDL_Rect *font_rect, SDL_Surface *destination, SDL_Rect *dest_rect)
{
  SDL_BlitSurface(scrollers[scroll_id].font_surface, font_rect, destination, dest_rect);
}

void TDEC_free_all_scrollers(void)
{
  Uint8 i;
  for (i = 0; i < MAX_SCROLLERS; ++i)
    {
      if (scrollers[i].text != (char*)0)
	{
	  TDEC_free_scroller(i);
	}
    }
}

void TDEC_set_font_colorkey(Uint8 scroll_id, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  SDL_SetColorKey(scrollers[scroll_id].font_surface, SDL_SRCCOLORKEY/* | SDL_RLEACCEL*/, 
		  SDL_MapRGBA(scrollers[scroll_id].font_surface->format, r, g, b, a));
}
