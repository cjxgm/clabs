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

/*note that the code has not been fully optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define CHARACTER_WIDTH 16
#define CHARACTER_HEIGHT 32
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static SDL_Surface* font_surface;
static SDL_Surface* scroll_surface;
static SDL_Surface* copy_surface;
static SDL_Surface* flipped_surface;

static short aSin[720];
static short aSin3[720];
static short aSin2[SCREEN_HEIGHT + CHARACTER_HEIGHT];
static Uint16 sin_index = 0;
static char text[] = " The Demo Effects Collection - cheers -    WP van Paassen  -  starting over again in -      9  8  7  6  5  4  3  2  1          ";
static char characters[] = " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static char* text_pointer = text;

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(scroll_surface);
  SDL_FreeSurface(font_surface);
  SDL_FreeSurface(copy_surface);
  SDL_FreeSurface(flipped_surface);

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

  if (scroll_char == '\0')
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

void print_character(void) 
{
  static SDL_Rect frect = {0, 0, CHARACTER_WIDTH - 1, CHARACTER_HEIGHT};
  static SDL_Rect srect = {0, SCREEN_HEIGHT + CHARACTER_HEIGHT, CHARACTER_WIDTH - 1, CHARACTER_HEIGHT};

  /* determine font character according to position in scroll text */
  
  frect.x = compute_font_pos(*text_pointer++);

  /*copy character to scroll_screen */
  SDL_BlitSurface(font_surface, &frect, scroll_surface, &srect);  
}

void init()
{
  SDL_Surface* s;
  float rad;
  Uint16 i;
  short centerx = SCREEN_WIDTH >> 1;
  
  /*create sin lookup table */
  for (i = 0; i < 720; i++)
    {
      rad =  (float)i * 0.0174532 * 0.5; 
      aSin[i] = centerx + (short)((sin(rad) * 100.0));
      aSin3[i] = centerx + (short)((-sin(rad) * 100.0));
    }
  
  for (i = 0; i < SCREEN_HEIGHT + CHARACTER_HEIGHT; i++)
    {
      rad =  (float)i * 0.0174532 * (360.0 / (SCREEN_HEIGHT + CHARACTER_HEIGHT)) ; /* spread 360 degrees over SCREEN_HEIGHT  + CHARACTER_HEIGHT values */ 
      aSin2[i] = (short)((sin(rad) * 70.0));
      if (aSin2[i] >= 0)
	aSin2[i] += 40;
      else 
	aSin2[i] -= 40;
    }

  /* create scroll surface, this surface must be heigher than the screenwidth to print the characters 'below' the screen */
  
  s = SDL_CreateRGBSurface(SDL_HWSURFACE, CHARACTER_WIDTH, SCREEN_HEIGHT + CHARACTER_HEIGHT * 2, 8, r_mask, g_mask, b_mask, a_mask); 
  
  scroll_surface = SDL_DisplayFormat(s);
  
  SDL_FreeSurface(s);
  
  /* create copy surface */
  
  copy_surface = TDEC_copy_surface(scroll_surface);

  /* create flipped surface */
  
  flipped_surface = TDEC_copy_surface(copy_surface);

  /* load font */
  
  font_surface = IMG_Load("../GFX/font.pcx");

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
  Uint32 i, displacement = 0, j = 0;
  SDL_Rect srect2 = {0, 0, CHARACTER_WIDTH, 1};
  SDL_Rect drect = {0, 0, CHARACTER_WIDTH, 1}; 
  SDL_Rect srect = {0, 2, CHARACTER_WIDTH, SCREEN_HEIGHT + CHARACTER_HEIGHT * 2};
  SDL_Rect frect = {SCREEN_WIDTH / 2 - 100 - CHARACTER_WIDTH, 0, 2 * (100 + CHARACTER_WIDTH), SCREEN_HEIGHT};
  
  if (argc > 1)
    {
      printf("Retro Twin Sine Scroller - W.P. van Paassen - 2002\n");
      return -1;
    }

  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
   quit(1);

  TDEC_init_timer();

  SDL_WM_SetCaption("Retro - Twin Sine Scroller - ", "");
  
  init();

  TDEC_set_fps(50);

  SDL_SetColorKey(copy_surface, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)); 

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();
      
      SDL_BlitSurface(scroll_surface, &srect, scroll_surface, 0);
      displacement += 2;
      
      /* print character? */
      if (displacement > 30)
	{
	  print_character();
	  displacement = 0;
	}

      SDL_FillRect(copy_surface, 0, 0);
      
      /* fill flipped surface */
      
      SDL_BlitSurface(scroll_surface, 0, flipped_surface, 0);
      TDEC_flipy_image(flipped_surface);
      
      for (i = 0; i < scroll_surface->h; ++i)
	{
	  if (aSin2[sin_index] > 0)
	    TDEC_scale_copy_hscanline(scroll_surface, copy_surface, i, aSin2[sin_index]);
	  else
	    TDEC_scale_copy_hscanline(flipped_surface, copy_surface, i, -aSin2[sin_index]);

	  sin_index++;
	  sin_index %= SCREEN_HEIGHT + CHARACTER_HEIGHT;
       }
  
      /* clean sinus area */
      
      SDL_FillRect(screen, &frect, 0);
      
      /* create sinus in scroll */
      
      for (i = 0; i < SCREEN_HEIGHT; ++i)
	{
	  /* draw scroller 1 */
	  srect2.y = CHARACTER_HEIGHT + i;
	  drect.y = i;
	  drect.x = aSin[(j + i) % 720];
	  SDL_BlitSurface(copy_surface, &srect2, screen, &drect);
	  
	  /* draw scroller 2 */
	  drect.x = aSin3[(j + i) % 720];
	  SDL_BlitSurface(copy_surface, &srect2, screen, &drect);
	}
      
      j += 6;
      j %= 720;
  
      if (TDEC_fps_ok())
	SDL_Flip(screen);
}
  
  return 0; /* never reached */
}
