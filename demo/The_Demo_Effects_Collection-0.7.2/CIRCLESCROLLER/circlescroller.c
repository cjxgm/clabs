/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl

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

/* note that the code has not been optimized*/
/* for instance instead of clearing and updating the complete scroll area */
/* it would be nicer to remember each letter its coordinates and clearing these and updating the new ones instead */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
#define CHARACTER_WIDTH 16
#define CHARACTER_HEIGHT 32
#define RIGHTBORDER SCREEN_WIDTH - CHARACTER_WIDTH
#define BOTTOMBORDER SCREEN_HEIGHT - CHARACTER_HEIGHT
#define NLETTERS 20

static SDL_Surface* font_surface;

static short aSin[360];
static short aCos[360];
static short Movement[512];
static char text[] = " A circle scroller, part of ThE DeMo EffEcTs ColLeCtiOn - pretty retro isn't it? Watch it hit the borders!!! - cheers - W.P. van Paassen - starting over again in - 9  8  7  6  5  4  3  2  1 \"";
static char characters[] = " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static char* text_pointer = text;
static Uint16 x_move = 0, y_move = 0;

typedef struct 
{
  Uint16 sin_index;
  Uint16 font_pos;
} LETTER; 

LETTER letters[NLETTERS];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(font_surface);

  SDL_Quit( );

  TDEC_print_fps();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
        quit( 0 );
        break;
      default:
        break;
      }
}

void process_events( void )
{
    /* Our SDL event placeholder. */
    SDL_Event event;

    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests*/
            quit( 0 );
            break;
	}
    }
}

/* determine the print character */
Uint16 compute_font_pos(char scroll_char)
{
  char* p = characters;
  Uint16 pos = 0;

  if (scroll_char == '"')
    {
      text_pointer = text;
      scroll_char = *text_pointer++;
    }

  while (*p++ != scroll_char)
    {
      pos += CHARACTER_WIDTH;
    }

  if (pos > 0)
    return pos - 1;

  return 0;
}

Uint16 init_character(void) 
{
  /* determine font character according to position in scroll text */
  
  return compute_font_pos(*text_pointer++);
}

void init()
{
  float rad;
  Uint16 i;
  short centerx = SCREEN_WIDTH >> 1;
  short centery = SCREEN_HEIGHT >> 1;

  /*create sin lookup table */

  for (i = 0; i < 360; ++i)
    {
      rad =  (float)i * 0.0174532;
      aSin[i] = centery - (short)((sin(rad) * 100.0));
      aCos[i] = centerx - (short)((cos(rad) * 100.0));
    }

  for (i = 0; i < 512; ++i)
    {
      rad =  (float)i * 0.0174532 * 0.703125;
      Movement[i] = sin(rad) * 200.0;
    }

  /* load font */
  
  font_surface = IMG_Load("../GFX/font.pcx");

  SDL_SetColorKey(font_surface, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

  /* reset letters */
  for (i = 0; i < NLETTERS; ++i)
    {
      letters[i].sin_index = 0;
    }
  
  /*disable events */

  for (i = 0; i < SDL_NUMEVENTS; ++i)
    {
      if (i != SDL_KEYDOWN && i != SDL_QUIT)
	{
	  SDL_EventState(i, SDL_IGNORE);
	}
    }

  SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
  int i, displacement = 0;
  SDL_Rect frect = {0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT};
  SDL_Rect srect = {0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT};
  
  if (argc > 1)
    {
      printf("Retro Circle Scroller - W.P. van Paassen - 2002\n");
      return -1;
    }

  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| 
SDL_FULLSCREEN*/))
   quit(1);
  
  TDEC_init_timer();

  SDL_WM_SetCaption("Retro - Circle Scroller - ", "");
  
  init();

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();

      /* print character? */
      if (displacement > 20)
	{
	  /* init new character */

	  for (i = 0; i < NLETTERS; ++i)
	    {
	      /* find an unused letter */

	      if (letters[i].sin_index == 0)
		{
		  letters[i].sin_index = 3;
		  letters[i].font_pos = init_character();
		  break;
		}
	    }

	  displacement = 0;
	}

      displacement += 3;

      /* clear screen */

      SDL_FillRect(screen, 0, 0);
  
      /* update letter positions */

      for (i = NLETTERS - 1; i >= 0; --i)
	{
	  if (letters[i].sin_index > 0)
	      {
		/* blit letter to screen */
		
		frect.x = letters[i].font_pos - 1;
		srect.x = aCos[letters[i].sin_index] + Movement[x_move];
		srect.y = aSin[letters[i].sin_index] + Movement[y_move];

		/* check borders */
		if (srect.x > RIGHTBORDER)
		  srect.x = RIGHTBORDER;
		else if (srect.x < 0)
		  srect.x = 0;
		
		if (srect.y > BOTTOMBORDER)
		  srect.y = BOTTOMBORDER;
		else if (srect.y < 0)
		  srect.y = 0;

		SDL_BlitSurface(font_surface, &frect, screen, &srect);

		letters[i].sin_index += 3;

		/* spinned round? */

		letters[i].sin_index %= 360;
	      }
	}

      /* update movement of scroller */

      x_move += 2;
      y_move += 3;
      x_move &= 511;
      y_move &= 511;

      if (TDEC_fps_ok())
	{
	SDL_Flip(screen);
	}
    }
  
  return 0; /* never reached */
}
