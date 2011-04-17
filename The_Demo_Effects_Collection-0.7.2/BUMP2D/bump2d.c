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

/* This effect was inspired by an article by Sqrt(-1) */
/* 08-22-02 Optimized by WP */
/* note that the code has not been fully optimized */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360

typedef struct
{
  short x, y;
} BUMP;

static short aSin[512];
static float reflectionmap[256][256];
static BUMP bumpmap[SCREEN_WIDTH][SCREEN_HEIGHT];
static Uint16 sin_index = 0;
static Uint16 sin_index2 = 80;
static SDL_Surface* heightmap = 0;

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (heightmap) 
    SDL_FreeSurface(heightmap);

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
	quit(1);
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
      /* Handle quit requests */    
      quit(1);
      break;
    }
  }
}

void init()
{
  Uint16 i;
  short x, y;
  float rad;
  
  heightmap = IMG_Load("../GFX/tuxblackbg.png");
  if (!heightmap) {
    fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
    quit(3);
  }
  
  TDEC_create_heightmap(heightmap);
  
  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, heightmap->format->palette->colors, 0, heightmap->format->palette->ncolors);
  
  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  (float)i * 0.0174532 * 0.703125; 
      aSin[i] = (short)((sin(rad) * 100.0) + 256.0);
    }
  
  /* create reflection map */
  
  for (x = 0; x < 256; ++x)
    {
      for (y = 0; y < 256; ++y)
	{
	  float X = (x - 128) / 128.0;
	  float Y = (y - 128) / 128.0;
	  float Z =  1.0 - sqrt(X * X + Y * Y);
	  Z *= 255.0;
	  if (Z < 0.0)
	    Z = 0.0;
	  reflectionmap[x][y] = Z;
	}
    }

  /* create bump map */

  for (x = 1; x < SCREEN_WIDTH - 1; ++x)
    {
      for (y = 1; y < SCREEN_HEIGHT - 1; ++y)
	{
	  bumpmap[x][y].x = *((Uint8*)heightmap->pixels + y * heightmap->pitch + x + 1) 
	    - *((Uint8*)heightmap->pixels + y * heightmap->pitch + x);
	  bumpmap[x][y].y = *((Uint8*)heightmap->pixels + y * heightmap->pitch + x) 
	    - *((Uint8*)heightmap->pixels + ((y - 1) * heightmap->pitch) + x);
	}
    }
  
  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i) {
    if (i != SDL_KEYDOWN && i != SDL_QUIT) {
      SDL_EventState(i, SDL_IGNORE);
    }
  }
  
  SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
  Uint16 lightx, lighty, temp;
  short normalx, normaly, x, y;
  Uint8 *s1;

  if (argc > 1) {
    printf("Retro 2D Bumpmapping - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| 
														    SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro 2D Bumpmapping effect ", "");
  
  init();
  
  TDEC_set_fps(15);

  SDL_LockSurface(screen);

  /* time based demo loop */
  while( 1 ) 
    {  
      TDEC_new_time();
    
      process_events();

      lightx = aSin[sin_index];
      lighty = aSin[sin_index2];

      s1 = (Uint8*)screen->pixels + screen->pitch + 1;

      for (y = 1; y < SCREEN_HEIGHT - 1; ++y)
	{    
	  temp = lighty - y;
	  for (x = 1; x < SCREEN_WIDTH - 1; ++x)
	    {
	      normalx = bumpmap[x][y].x + lightx - x;
	      normaly = bumpmap[x][y].y + temp;
	  
	      if (normalx < 0)
		normalx = 0;
	      else if (normalx > 255)
		normalx = 0;
	      if (normaly < 0)
		normaly = 0;
	      else if (normaly > 255)
		normaly = 0;	  
	      
	      *s1++ = (Uint8)reflectionmap[normalx][normaly];
	    }
	  s1+= 2;
	}
      
      sin_index += 3;
      sin_index &= 511;
      sin_index2 += 5;
      sin_index2 &= 511;
      
      if (TDEC_fps_ok()) 
	{
	  SDL_Flip(screen);
	}
    }
  
  return 0; /* never reached */
}
