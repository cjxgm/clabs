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

/* 080802 Added vertical scanline effect. WP */
/* 241202 Added moziaek transition effect at end WP */
/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360

static short aSin[512];
static Uint16 sin_index = 0;
static Uint16 sin_global_index = 0;
static SDL_Surface* image = 0;
static SDL_Surface* flippedx_image = 0;
static SDL_Surface* flippedy_image = 0;
static Uint8 quiting = 0;
static Uint16 blocksize = 2;

typedef struct
{
  Uint8 index_add;
  Uint8 effect;
} SINE_EFFECT;

#define MAX_EFFECTS 4

struct 
{
  SINE_EFFECT sine_effects[MAX_EFFECTS];
  Uint8 current_effect;
} EFFECTS;


void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (image) 
    SDL_FreeSurface(image);
  if (flippedx_image) 
    SDL_FreeSurface(flippedx_image);
  if (flippedx_image)
    SDL_FreeSurface(flippedy_image);

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
      quit(1);
      break;
    }
  }
}

void init()
{
    Uint16 i;
    float rad;

    image = IMG_Load("../GFX/tuxblackbg.png");
    if (!image) {
        fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
        quit(3);
    }

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, image->format->palette->colors, 0, image->format->palette->ncolors);

    flippedx_image = TDEC_copy_surface(image);
    TDEC_flipx_image(flippedx_image);

    flippedy_image = TDEC_copy_surface(image);
    TDEC_flipy_image(flippedy_image);

    /*create sin lookup table */
    for (i = 0; i < 512; i++)
      {
	rad =  (float)i * 0.0174532 * 0.703125; 
	aSin[i] = (short)((sin(rad) * 100.0));
      }

    /* create effects */

    EFFECTS.current_effect = 0;

    EFFECTS.sine_effects[0].index_add = 2;
    EFFECTS.sine_effects[0].effect = 0;

    EFFECTS.sine_effects[1].index_add = 1;
    EFFECTS.sine_effects[1].effect = 1;

    EFFECTS.sine_effects[2].index_add = 1;
    EFFECTS.sine_effects[2].effect = 0;

    EFFECTS.sine_effects[3].index_add = 2;
    EFFECTS.sine_effects[3].effect = 1;

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
  Uint16 i;
  SINE_EFFECT* e = EFFECTS.sine_effects + EFFECTS.current_effect;

  if (argc > 1) {
    printf("Retro Spiraltwist - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| 
SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro Spiraltwist effect ", "");
  
  init();
  
  TDEC_set_fps(30);

  /* time based demo loop */
  while( 1 ) 
    {
      
      TDEC_new_time();
    
      process_events();

      if (!quiting)
	{
	  sin_index = sin_global_index;
	  
	  SDL_FillRect(screen, 0, 0);
	  
	  if (e->effect)
	    {
	      for (i = 0; i < image->w; ++i)
		{
		  if (aSin[sin_index] < 0)
		    TDEC_scale_copy_vscanline(flippedx_image, screen, i, -aSin[sin_index]);
		  else
		    TDEC_scale_copy_vscanline(image, screen, i, aSin[sin_index]);
		  
		  sin_index += e->index_add;
		  sin_index &= 511;
		}
	    }
	  else
	    {
	      for (i = 0; i < image->h; ++i)
		{
		  if (aSin[sin_index] < 0) 
		    TDEC_scale_copy_hscanline(flippedy_image, screen, i, -aSin[sin_index]);
		  else
		    TDEC_scale_copy_hscanline(image, screen, i, aSin[sin_index]);
		  
		  sin_index += e->index_add;
		  sin_index &= 511;
		}
	    }
	  
	  sin_global_index += 4;
	  if (sin_global_index > 511)
	    {
	      sin_global_index = 0;
	      EFFECTS.current_effect++;
	      EFFECTS.current_effect %= MAX_EFFECTS;
	      e = EFFECTS.sine_effects + EFFECTS.current_effect;
	    }
	}
      else
	{
	  TDEC_mozaiek_surface(screen, blocksize);
	  blocksize <<= 1;
	  if (blocksize > 256)
	    quit(1);
	}
      
      if (TDEC_fps_ok()) 
	{
	  SDL_Flip(screen);
	}

      if (quiting)
	SDL_Delay(200);
    }
  
  return 0; /* never reached */
}
