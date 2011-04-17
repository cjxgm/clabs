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

/* Optimized it further - WP - 07-13-02 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static Uint16 pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, tpos1, tpos2, tpos3, tpos4;
static int aSin[512];
static SDL_Color colors[256];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
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

void init()
{
  int i;
  float rad;
 
  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
      aSin[i] = sin(rad) * 1024; /*using fixed point math with 1024 as base*/
    }
  
  /* create palette */
  for (i = 0; i < 64; ++i)
    {
      colors[i].r = i << 2;
      colors[i].g = 255 - ((i << 2) + 1); 
      colors[i+64].r = 255;
      colors[i+64].g = (i << 2) + 1;
      colors[i+128].r = 255 - ((i << 2) + 1);
      colors[i+128].g = 255 - ((i << 2) + 1);
      colors[i+192].g = (i << 2) + 1; 
    } 

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 

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
  Uint8* image;
  Uint16 i,j;
  Uint8 index;
  int x;  

  if (argc > 1)
    {
      printf("Retro Plasma Effect - W.P. van Paassen - 2002\n");
      return -1;
    }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE/* | SDL_FULLSCREEN;*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro - Plasma - ", "");
  
  init();
  
  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);
  
  TDEC_set_fps(30);
  
  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
      
      process_events();

      /*this is where it all happens */

      /* draw plasma */

      tpos4 = pos4;
      tpos3 = pos3;

      image = screen->pixels;
      
      for (i = 0; i < screen->h; ++i)
	{
	  tpos1 = pos1 + 5;
	  tpos2 = pos2 + 3;
	  
	  tpos3 &= 511;
	  tpos4 &= 511;
	  
	  for (j = 0; j < screen->w; ++j)
	    {
	      tpos1 &= 511;
	      tpos2 &= 511;
	      
	      x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4]; /*actual plasma calculation*/
	      
	      index = 128 + (x >> 4); /*fixed point multiplication but optimized so basically it says (x * (64 * 1024) / (1024 * 1024)), x is already multiplied by 1024*/
	      
	      *image++ = index;
	      
	      tpos1 += 5; 
	      tpos2 += 3; 
	    }
	  
	  tpos4 += 3;
	  tpos3 += 1;
	}
           
      /* move plasma */
      
      pos1 +=9;
      pos3 +=8;

      if (TDEC_fps_ok())
	SDL_Flip(screen);
    }
  
  return 0; /* never reached */
}

