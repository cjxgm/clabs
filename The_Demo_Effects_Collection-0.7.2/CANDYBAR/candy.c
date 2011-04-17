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
static SDL_Surface* copper_surface;

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(copper_surface);

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
  SDL_Surface* s;
  SDL_Color colors[256];
  SDL_Rect r = {0, 0, SCREEN_WIDTH, 16};

  /*disable events */
  
  for (i = 0; i < SDL_NUMEVENTS; ++i)
    {
      if (i != SDL_KEYDOWN && i != SDL_QUIT)
	{
	  SDL_EventState(i, SDL_IGNORE);
	}
    }
  
  SDL_ShowCursor(SDL_DISABLE);

  /* create palette */
  
  for (i = 0; i < 128; ++i)
    {
      colors[i].r = i << 1;
      colors[i].g = 0; 
      colors[i].b = 0;
      colors[i + 128].r = 0xFF - (i << 1);
      colors[i + 128].g = 0xFF - (i << 1);
      colors[i + 128].b = 0xFF - (i << 1);
    }

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);

  s = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH, 128, 8, r_mask, g_mask, b_mask, a_mask); 
  
  copper_surface = SDL_DisplayFormat(s);
  
  SDL_FreeSurface(s);

  /* create candybar */
  
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0, 0));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0xFF, 0xFF));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0, 0));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0xFF, 0xFF));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0, 0));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0xFF, 0xFF));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0, 0));
  r.y += 16;
  SDL_FillRect(copper_surface, &r, SDL_MapRGB(copper_surface->format, 0xFF, 0xFF, 0xFF));
}

int main( int argc, char* argv[] )
{
  Uint32 i, pitch;
  Uint8 temp;
  SDL_Rect crect1 = {0, 1, SCREEN_WIDTH, 127};
  SDL_Rect crect2 = {0, 0, SCREEN_WIDTH, 127};
  SDL_Rect crect3 = {0, 127, SCREEN_WIDTH, 1};
  SDL_Rect crect4 = {0, (SCREEN_HEIGHT >> 1) - 64, SCREEN_WIDTH, 1};
  SDL_Color col;
 
  if (argc > 1)
    {
      printf("Retro Amiga Candybar - W.P. van Paassen - 2002\n");
      return -1;
    }

  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
   quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro - Amiga Candybar - ", "");
  
  init();

  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();

      /* scroll candy bars up */

      temp = *(Uint8*)copper_surface->pixels;
      SDL_BlitSurface(copper_surface, &crect1, copper_surface, &crect2);
      SDL_FillRect(copper_surface, &crect3, temp);

      /*shade candy*/

      pitch = 0;
 
      for (i = 0; i < 128; ++i)
	{
	  temp = *((Uint8*)copper_surface->pixels + pitch);
	  SDL_GetRGB(temp, screen->format, &col.r, &col.g, &col.b);
	  
	  if (i < 64)
	    {
	      col.r &= i << 2;
	      col.b &= i << 2;
	      col.g &= i << 2;
	    }
	  else
	    {
	      col.r &= 0xFF - (i << 2);
	      col.b &= 0xFF - (i << 2);
	      col.g &= 0xFF - (i << 2);	    
	    }
	  temp = SDL_MapRGB(screen->format, col.r, col.g, col.b);
	  SDL_FillRect(screen, &crect4, temp);
	  crect4.y++;
	  pitch += copper_surface->pitch;
	}

      crect4.y = (SCREEN_HEIGHT >> 1) - 64;

      if (TDEC_fps_ok())
	SDL_UpdateRect(screen, 0, crect4.y, SCREEN_WIDTH, 128);
    }
  
  return 0; /* never reached */
}





