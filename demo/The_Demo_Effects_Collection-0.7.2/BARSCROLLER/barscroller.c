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
#define STOPTIME 3000

static SDL_Surface* font_surface;
static SDL_Surface* scroll_surface;
static SDL_Surface* copy_surface;
static SDL_Surface* flipped_surface;
static SDL_Color colors[5];

static short aSin[360];
static short aSin2[SCREEN_WIDTH + CHARACTER_WIDTH];
static Uint16 sin_index = 0;
static char text[] = " THE DEMO EFFECTS COLLECTION   \"     - CHEERS -           W.P. VAN PAASSEN         \"  -    STARTING OVER AGAIN IN -      9  8  7  6  5  4  3  2  1          ";
static char characters[] = " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static char* text_pointer = text;
static Uint8 stopscroller = 0;
static Uint32 time;

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
  else if (scroll_char =='"')
    {
      stopscroller = 1;
      time = SDL_GetTicks();
      return 0;
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
  static SDL_Rect frect = {0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT};
  static SDL_Rect srect = {SCREEN_WIDTH + CHARACTER_WIDTH, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT};

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
  short centery = SCREEN_HEIGHT >> 1;
  
  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i] = centery + (short)((sin(rad) * 75.0));
    }
  
  for (i = 0; i < SCREEN_WIDTH + CHARACTER_WIDTH; i++)
    {
      rad =  (float)i * 0.0174532 * (360.0 / (SCREEN_WIDTH + CHARACTER_WIDTH)) ; /* spread 360 degrees over SCREEN_WIDTH  + CHARACTER_WIDTH values */ 
      aSin2[i] = (short)((sin(rad) * 100.0));
    }

  /* create scroll surface, this surface must be wider than the screenwidth to print the characters outside the screen */
  
  s = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH + CHARACTER_WIDTH * 2, CHARACTER_HEIGHT, 8, r_mask, g_mask, b_mask, a_mask); 
  
  scroll_surface = SDL_DisplayFormat(s);
  
  SDL_FreeSurface(s);
 
  /* create copy surface */
  
  copy_surface = TDEC_copy_surface(scroll_surface);

  /* create flipped surface */
  
  flipped_surface = TDEC_copy_surface(copy_surface);

  /* load font */
  
  font_surface = IMG_Load("../GFX/font.pcx");

  /* create palette */

  colors[1].g = 64; 
  colors[2].g = 128;
  colors[3].g = 200; 
  colors[4].g = 255; 

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 5); 
  SDL_SetPalette(copy_surface, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 5); 

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
  SDL_Rect srect2 = {0, 0, 1, CHARACTER_HEIGHT};
  SDL_Rect drect = {0, 0, 1, CHARACTER_HEIGHT}; 
  SDL_Rect srect = {2, 0, SCREEN_WIDTH + (CHARACTER_WIDTH * 2), CHARACTER_HEIGHT};
  SDL_Rect frect = {0, SCREEN_HEIGHT / 2 - 75, SCREEN_WIDTH, 182};
  SDL_Rect rs = {0, 0, 1, CHARACTER_HEIGHT};
  SDL_Rect rs2 = {0, 0, 1, CHARACTER_HEIGHT};
  Uint8* image;
  
  if (argc > 1)
    {
      printf("Retro Bar Sine Scroller - W.P. van Paassen - 2002\n");
      return -1;
    }

  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
   quit(1);

  TDEC_init_timer();

  SDL_WM_SetCaption("Retro - Bar Sine Scroller - ", "");
  
  init();

  TDEC_set_fps(50);

  SDL_SetColorKey(copy_surface, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)); 
      
  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();
      
      /* scroll scroll_surface to the left */

      if (!stopscroller)
	{
	  SDL_BlitSurface(scroll_surface, &srect, scroll_surface, 0);
	  displacement += 2;

	  /* print character? */
	  if (displacement > 12)
	    {
	      print_character();
	      displacement = 0;
	    }
	}
      else
	{
	  if (SDL_GetTicks() - time >= STOPTIME)
	    stopscroller = 0;
	}

      
      SDL_FillRect(copy_surface, 0, 0);   
      
      /* fill flipped surface */
      
      SDL_BlitSurface(scroll_surface, 0, flipped_surface, 0);
      TDEC_flipx_image(flipped_surface);      
      
      for (i = 0; i < scroll_surface->w; ++i)
	{
	  if (aSin2[sin_index] > 0)
	    {
	      TDEC_scale_copy_vscanline(scroll_surface, copy_surface, i, aSin2[sin_index]);
	      rs.x = i;
	      SDL_FillRect(screen, &rs2, 0);
	      image = (Uint8*)screen->pixels + 12 * screen->w;
	      *image = 1;
	      image += screen->w;
	      *image = 2;
	      image += screen->w;
	      *image = 3;
	      image += screen->w;
	      *image = 4;
	      image += screen->w;
	      *image = 3;
	      image += screen->w;
	      *image = 2;
	      image += screen->w;
	      *image = 1;
	      SDL_BlitSurface(copy_surface, &rs, screen, &rs2);
	      SDL_BlitSurface(screen, &rs2, copy_surface, &rs);
	    }
	  else
	    {
	      TDEC_scale_copy_vscanline(flipped_surface, copy_surface, i, -aSin2[sin_index]);
	      image = (Uint8*)copy_surface->pixels + 12 * copy_surface->w + i;
	      *image = 1;
	      image += copy_surface->w;
	      *image = 2;
	      image += copy_surface->w;
	      *image = 3;
	      image += copy_surface->w;
	      *image = 4;
	      image += copy_surface->w;
	      *image = 3;
	      image += copy_surface->w;
	      *image = 2;
	      image += copy_surface->w;
	      *image = 1;
	    }
	  sin_index++;
	  sin_index %= SCREEN_WIDTH + CHARACTER_WIDTH;
	}
      
      /* clean sinus area */
      SDL_FillRect(screen, &rs2, 0);
      SDL_FillRect(screen, &frect, 0);
      
      /* create sinus in scroll */
      
      for (i = 0; i < SCREEN_WIDTH; ++i)
	{
	  srect2.x = CHARACTER_WIDTH + i;
	  drect.x = i;
	  drect.y = aSin[(j + i) % 360];
	  SDL_BlitSurface(copy_surface, &srect2, screen, &drect);
	}
      j += 1;
      j %= 360;

      if (TDEC_fps_ok())
	SDL_Flip(screen);
    }
  
  return 0; /* never reached */
}
