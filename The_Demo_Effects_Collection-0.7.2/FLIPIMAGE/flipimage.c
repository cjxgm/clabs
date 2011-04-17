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

/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360
#define SCALING 3

static Uint8 scaling = 100;
static Uint8 backside = 0;
static Uint8 up = 0;
static Uint8 starting = 1;
static Uint8 quiting = 0;
static SDL_Surface *image = 0;

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (image) 
    SDL_FreeSurface(image);

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
	quiting = 1;
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
      quiting = 1;
      break;
    }
  }
}

void init()
{
    Uint16 i;

    image = IMG_Load("../GFX/tuxblackbg.png");
    if (!image) {
        fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
        quit(3);
    }

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, image->format->palette->colors, 0, image->format->palette->ncolors);

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
  SDL_Rect s, d;
  short i;

  if (argc > 1) {
    printf("Retro FlipImage - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| 
SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro FlipImage effect ", "");
  
  init();

  s.x = d.x = 0;
  s.y = d.y = i = image->h;
  s.w = image->w;
  s.h = d.h = 1;
  d.w = screen->w;
  
  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
    
      process_events();

      if (starting)
	{
	  if (i < 0)
	    starting = 0;
	  else
	    {
	      short j;
	      
	      s.y = d.y = i--;
	      
	      for (j = i; j >= 0; --j)
		{
		  SDL_BlitSurface(image, &s, screen, &d);
		  d.y = j;
		}
	    }
	}
      else 
	{ /* flipping */
	  if (!up)
	    {
	      if (scaling >= SCALING)
		{
		    TDEC_scaley_copy_image(image, screen, scaling);

		  scaling -= SCALING;
		}
	      else
		{
		  backside = !backside;
		    TDEC_flipx_image(image);

		  if (backside)
		    TDEC_fadeout(image, 60);		   
		  else
		    TDEC_fadein(image, screen->format->palette, 60);
		  up = 1;
		}
	    }
	  else
	    {
	      if (scaling <= 100 - SCALING)
		{
		    TDEC_scaley_copy_image(image, screen, scaling);

		  scaling += SCALING;
		}
	      else
		  up = 0;
	    }
	}

      if (TDEC_fps_ok()) 
	{
	  SDL_Flip(screen);
	}
      
      if (quiting)
	{
	  if (TDEC_fadeout(screen, 4))
	      quit(0);	
	}
    }
  
  return 0; /* never reached */
}
