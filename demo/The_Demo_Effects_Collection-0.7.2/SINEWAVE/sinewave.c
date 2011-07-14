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

/* 080802 done some optimization WP */
/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360

static Uint16 sin_index = 0;
static Uint8 quiting = 0;
static Uint8 fadein = 1;
static SDL_Surface *image = NULL;

typedef struct
{
  short sine_table[512];
  Uint8 index_add;
  Uint8 effect;
} SINE_EFFECT;

#define MAX_EFFECTS 12

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
    float rad;

    image = IMG_Load("../GFX/tuxblackbg.png");
    if (!image) {
        fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
        quit(3);
    }

    TDEC_blacken_palette(screen);

    EFFECTS.current_effect = 0;

    /*create sin lookup tables in effect chain */

    for (i = 0; i < 512; ++i)
      {
	rad =  (float)i * 0.0174532 * 0.703125; 
	EFFECTS.sine_effects[0].sine_table[i] = (short)((sin(rad) * 8.0));
	EFFECTS.sine_effects[1].sine_table[i] = (short)((sin(rad) * 16.0));
	EFFECTS.sine_effects[2].sine_table[i] = (short)((sin(rad) * 20.0));
	EFFECTS.sine_effects[3].sine_table[i] = (short)((sin(rad) * 32.0));
	EFFECTS.sine_effects[4].sine_table[i] = (short)((sin(rad) * 64.0));      
	EFFECTS.sine_effects[5].sine_table[i] = (short)((sin(rad) * 128.0));
	EFFECTS.sine_effects[6].sine_table[i] = (short)((sin(rad) * 256.0));
	EFFECTS.sine_effects[7].sine_table[i] = (short)((sin(rad) * 128.0));
	EFFECTS.sine_effects[8].sine_table[i] = (short)((sin(rad) * 64.0));
	EFFECTS.sine_effects[9].sine_table[i] = (short)((sin(rad) * 44.0));
	EFFECTS.sine_effects[10].sine_table[i] = (short)((sin(rad) * 32.0));
	EFFECTS.sine_effects[11].sine_table[i] = (short)((sin(rad) * 8.0));
      }

    EFFECTS.sine_effects[0].index_add = 2;
    EFFECTS.sine_effects[0].effect = 0;

    EFFECTS.sine_effects[1].index_add = 4;
    EFFECTS.sine_effects[1].effect = 0;

    EFFECTS.sine_effects[2].index_add = 3;
    EFFECTS.sine_effects[2].effect = 1;

    EFFECTS.sine_effects[3].index_add = 5;
    EFFECTS.sine_effects[3].effect = 0;

    EFFECTS.sine_effects[4].index_add = 2;
    EFFECTS.sine_effects[4].effect = 0;

    EFFECTS.sine_effects[5].index_add = 2;
    EFFECTS.sine_effects[5].effect = 1;

    EFFECTS.sine_effects[6].index_add = 4;
    EFFECTS.sine_effects[6].effect = 0;

    EFFECTS.sine_effects[7].index_add = 1;
    EFFECTS.sine_effects[7].effect = 0;

    EFFECTS.sine_effects[8].index_add = 2;
    EFFECTS.sine_effects[8].effect = 1;

    EFFECTS.sine_effects[9].index_add = 8;
    EFFECTS.sine_effects[9].effect = 0;

    EFFECTS.sine_effects[10].index_add = 3;
    EFFECTS.sine_effects[10].effect = 1; 

    EFFECTS.sine_effects[11].index_add = 2;
    EFFECTS.sine_effects[11].effect = 1; 

    /*disable events */
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
	if (i != SDL_KEYDOWN && i != SDL_QUIT) {
	    SDL_EventState(i, SDL_IGNORE);
	}
    }
  
    SDL_ShowCursor(SDL_DISABLE);
}

void sinewave( void )
{
  Uint16 i, sin_backup = sin_index;
  static SINE_EFFECT* e = EFFECTS.sine_effects;
  SDL_Rect d = {0, 0, SCREEN_WIDTH, 1};
  SDL_Rect r;

  r.x = e->sine_table[sin_backup];
  r.y = 0;
  r.w = SCREEN_WIDTH;
  r.h = 1;
 
 if (e->effect)
   {
     for (i = 0; i < image->h; ++i)
       {
	 SDL_BlitSurface(image, &r, screen, &d);
	 r.y = i;
	 d.y = i;
	 d.x = 0;
	 sin_backup += e->index_add;
	 sin_backup &= 511;

	  if (i % 2)
	    r.x = e->sine_table[sin_backup];
	  else
	    r.x = -e->sine_table[sin_backup];
	}
   }
 else
   {
     for (i = 0; i < image->h; ++i)
       {
	 SDL_BlitSurface(image, &r, screen, &d);
	 r.y = i;
	 d.y = i;
	 d.x = 0;
	 sin_backup += e->index_add;
	 sin_backup &= 511;
	 r.x = e->sine_table[sin_backup];
       }
   }

  sin_index += 6;
  if (sin_index > 511)
    {
      sin_index = 0;
      EFFECTS.current_effect++;
      EFFECTS.current_effect %= MAX_EFFECTS;
      e = EFFECTS.sine_effects + EFFECTS.current_effect;    
    }
}

int main( int argc, char* argv[] )
{
    if (argc > 1) {
	printf("Retro Sinewave - W.P. van Paassen - 2002\n");
	return -1;
    }

    if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE 
/*| SDL_FULLSCREEN*/))
	quit(1);

    TDEC_init_timer();

    SDL_WM_SetCaption("Retro Sinewave effect ", "");
  
    init();

    /* time based demo loop */
    while( 1 ) {
	TDEC_new_time();

	process_events();

	SDL_FillRect(screen, 0, 0);
	sinewave();
 
	if (TDEC_fps_ok()) {
	  SDL_Flip(screen);
	}

	if (fadein && !quiting)
	  fadein = !TDEC_fadein(screen, image->format->palette, 6);	
	
	if (quiting)
	  {
	    if (TDEC_fadeout(screen, 4))
	      quit(0);
	  }
   }
  
    return 0; /* never reached */
}
