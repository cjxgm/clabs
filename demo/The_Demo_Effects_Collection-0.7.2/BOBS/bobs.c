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

#define NUMBER_OF_BOBS 14
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static SDL_Surface* eye_surface;
static int xpath[512];
static int ypath[512];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(eye_surface);

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
  int i, hw, hh;
  
  hw = (SCREEN_WIDTH - 60) >> 1;
  hh = (SCREEN_HEIGHT - 60) >> 1;
  
  /*create x and y values for the bobs */
  for (i = 0; i < 512; ++i) 
    {
      double rad = ((float)i * 0.703125) * 0.0174532; /* spread 360 degrees over 512 values and convert to rad */
      xpath[i] = sin(rad) * hw + hw;
      ypath[i] = cos(rad) * hh + hh;
    }
  
    /* load eye bob */
  
  eye_surface = (SDL_Surface*)IMG_Load("../GFX/eye.png");

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, eye_surface->format->palette->colors, 0, eye_surface->format->palette->ncolors);

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
  Uint32 i = 0, j = 40, k, l, m;
  SDL_Rect rects[NUMBER_OF_BOBS];
  SDL_Rect oldrects[NUMBER_OF_BOBS];

  if (argc > 1) {
    printf("Retro Bobs Effect - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE/* | 
SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();

  SDL_WM_SetCaption("Retro - Bobs - ", "");
  
  init();
  
  SDL_SetColorKey(eye_surface, SDL_SRCCOLORKEY, 0);

  l = i;
  m = j;
  
  for (k = 0; k < NUMBER_OF_BOBS; ++k)
    {
      rects[k].w = oldrects[k].w = eye_surface->w;
      rects[k].h = oldrects[k].h = eye_surface->h;
      rects[k].x = xpath[l & 511];
      rects[k].y = ypath[m & 511];
      l += 20;
      m += 20;
    }
  
  TDEC_set_fps(50);

  /* time based demo loop */
  while( 1 ) {
    
    TDEC_new_time();
    
    process_events();
    
    l = i;
    m = j;

    /* clear and compute bob positions */

    for (k = 0; k < NUMBER_OF_BOBS; ++k)
      {
	oldrects[k].x = rects[k].x;
	rects[k].x = xpath[l & 511];
	oldrects[k].y = rects[k].y;
	rects[k].y = ypath[m & 511];
	
	SDL_FillRect(screen, oldrects + k, 0);
	
	l += 20;
	m += 20;
      }	

    /* blit bobs */

    for (k = 0; k < NUMBER_OF_BOBS; ++k)
      {
	SDL_BlitSurface(eye_surface, 0 , screen, rects + k);
      }	
    
    /* update positions in x and y pos table */
    
    i += 2;
    j += 3;
    i &= 511; 
    j &= 511;
    
    if (TDEC_fps_ok())
      {
	SDL_Flip(screen);
      }
  }
  
  return 0; /* never reached */
}

