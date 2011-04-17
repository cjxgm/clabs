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

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
static SDL_Surface* eye_surface;

/* the animation screens */
static SDL_Surface* screen1;
static SDL_Surface* screen2;
static SDL_Surface* screen3;
static SDL_Surface* screen4;
static SDL_Surface* screen5;
static SDL_Surface* screen6;

static int xpath[2048];
static int ypath[2048];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(eye_surface);
  SDL_FreeSurface(screen1); 
  SDL_FreeSurface(screen2); 
  SDL_FreeSurface(screen3);
  SDL_FreeSurface(screen4);
  SDL_FreeSurface(screen5);
  SDL_FreeSurface(screen6);

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
  
  /*create x and y values for the bob */
  for (i = 0; i < 512; ++i) 
    {
      double rad = ((float)i * 0.703125) * 0.0174532; /* spread 360 degrees over 512 values and convert to rad */
      xpath[i] = sin(rad) * hw + hw;
      ypath[i] = cos(rad) * hh + hh;
      xpath[i + 512] = sin(rad * 2.0) * hw + hw;
      ypath[i + 512] = cos(rad) * hh + hh;
      xpath[i + 1024] = sin(rad) * hw + hw;
      ypath[i + 1024] = cos(rad * 2.0) * hh + hh;
      xpath[i + 1536] = sin(rad * 2.0) * hw + hw;
      ypath[i + 1536] = cos(rad * 2.0) * hh + hh;
    }
  
    /* load eye bob */
  
  eye_surface = (SDL_Surface*)IMG_Load("../GFX/eye.png");

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, eye_surface->format->palette->colors, 0, eye_surface->format->palette->ncolors);

  /* create screen buffers */

  screen1 = TDEC_copy_surface(screen);
  screen2 = TDEC_copy_surface(screen);
  screen3 = TDEC_copy_surface(screen);
  screen4 = TDEC_copy_surface(screen);
  screen5 = TDEC_copy_surface(screen);
  screen6 = TDEC_copy_surface(screen);

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
  Uint32 i = 0, j = 0;
  Uint8 screen_index = 0;
  SDL_Rect rect;

  if (argc > 1) {
    printf("Retro Uunlimited Bobs Effect - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWACCEL | SDL_HWPALETTE/* | 
SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro - Unlimited Bobs - ", "");
  
  init();

  TDEC_set_fps(50);

  SDL_SetColorKey(eye_surface, SDL_SRCCOLORKEY, 0);

  rect.w = eye_surface->w;
  rect.h = eye_surface->h;
  
  /* time based demo loop */
  while( 1 ) {
    TDEC_new_time();
    
    process_events();
    
    rect.x = xpath[i];
    rect.y = ypath[j];
	
    /* blit bob to the screenbuffer identified by screen_index */

    switch(screen_index)
      {
      case 0:
	SDL_BlitSurface(eye_surface, 0 , screen1, &rect);
	SDL_BlitSurface(screen1, 0, screen, 0);
	break;
      case 1:
	SDL_BlitSurface(eye_surface, 0 , screen2, &rect);
	SDL_BlitSurface(screen2, 0, screen, 0);
	break;
      case 2:
	SDL_BlitSurface(eye_surface, 0 , screen3, &rect);
	SDL_BlitSurface(screen3, 0, screen, 0);
	break;
      case 3:
	SDL_BlitSurface(eye_surface, 0 , screen4, &rect);
	SDL_BlitSurface(screen4, 0, screen, 0);
	break; 
      case 4:
	SDL_BlitSurface(eye_surface, 0 , screen5, &rect);
	SDL_BlitSurface(screen5, 0, screen, 0);
	break;
      case 5:
	SDL_BlitSurface(eye_surface, 0 , screen6, &rect);
	SDL_BlitSurface(screen6, 0, screen, 0);
	break;
      }

    screen_index++;
    screen_index %= 6;
    
    /* update positions in x and y pos table */
    
    i += 1;
    j += 1;

    i &= 2047; 
    j &= 2047;

    if (TDEC_fps_ok())
      {
	SDL_Flip(screen);
      }
  }
  
  return 0; /* never reached */
}
