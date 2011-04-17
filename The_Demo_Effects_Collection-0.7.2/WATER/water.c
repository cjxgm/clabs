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

/* The code was inspired by articles of Tom Hammersley and Roy Willemse */
/* note that the code has not been fully optimized */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 300

static const float max_array = SCREEN_WIDTH * SCREEN_HEIGHT;
static const short amplitudes[4] = { -250, -425, -350, -650};
static SDL_Surface *image = 0;
static short wavemap[SCREEN_WIDTH * SCREEN_HEIGHT];
static short old_wavemap[SCREEN_WIDTH * SCREEN_HEIGHT];
static short *p_old = old_wavemap;
static short *p_new = wavemap;

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
    Uint32 i;

    image = IMG_Load("../GFX/TDEC_2.png");
    if (!image) {
        fprintf(stderr, "Cannot open file TDEC_2.png: %s\n", SDL_GetError());
        quit(3);
    }

    /* set screen palette */

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, image->format->palette->colors, 0, image->format->palette->ncolors);

    /* init wavemaps */

    for (i = 0; i < max_array; ++i)
      {
	wavemap[i] = 0;
	old_wavemap[i] = 0;
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

void start_drop()
{
  Uint32 v,w;
  static const Uint16 b = SCREEN_WIDTH - 10;
  static const Uint16 c = SCREEN_WIDTH * 10;
  static const Uint32 d = (SCREEN_WIDTH * SCREEN_HEIGHT) - (SCREEN_WIDTH * 10);
  static Uint8 amp_index = 0;
  
  /* borders are invalid so keep trying till valid value*/
  do
    {
      v = (int)(max_array * (rand()/(RAND_MAX+1.0))); 
      w = v % SCREEN_WIDTH;
    }
  while (w < 10 || w > b || v < c || v > d);

  wavemap[v] = amplitudes[amp_index++];
  amp_index &= 4;
}

int main( int argc, char* argv[] )
{
  Uint32 i, j, t;
  short *address_new, *address_old, *temp;
  short height, xdiff;
  Uint8 *pscreen, *pimage;

  if (argc > 1) {
    printf("Retro Water (Rain) effect - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| 
SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
 
  SDL_WM_SetCaption("Retro Water (Rain) effect ", "");
  
  init();

  TDEC_set_fps(20);

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
    
      process_events();

      start_drop();

      /* compute waves */
      t = SCREEN_WIDTH + 1;
      address_new = p_new + t;
      address_old = p_old + t;

      for (i = 1; i < SCREEN_HEIGHT - 1; ++i)
	{
	  for (j = 1; j < SCREEN_WIDTH - 1; ++j)
	    {
	      height = 0;
	      height += *(address_new + SCREEN_WIDTH);
	      height += *(address_new - 1);
	      height += *(address_new + 1);
	      height += *(address_new - SCREEN_WIDTH);
	      height >>= 1;
	      height -= *address_old;
	      height -= height >> 5;
	      *address_old = height;
	      address_new++;
	      address_old++;
	    }
	  address_new += 2; /* next scanline starting at pos 1 */
	  address_old += 2;
	}

      t = SCREEN_WIDTH + 1;
      address_old = p_old + t;
      pscreen = (Uint8*)screen->pixels + t;
      pimage = (Uint8*)image->pixels + t;
      
      /* draw waves */
      for (i = 1; i < SCREEN_HEIGHT - 1; ++i)
	{
	  for (j = 1; j < SCREEN_WIDTH - 1; ++j)
	    {	  
	      xdiff = *(address_old + 1) - *(address_old);
	      *pscreen = *(pimage + xdiff);
	      address_old++;
	      pscreen++;
	      pimage++;
	    }
	  address_old += 2;
	  pscreen += 2; /* next scanline starting at pos 1 */
	  pimage += 2;
	}

      /* swap wave tables */

      temp = p_new;
      p_new = p_old;
      p_old = temp;
      
      if (TDEC_fps_ok()) 
	{
	  SDL_Flip(screen);
	}
    }
  
  return 0; /* never reached */
}
