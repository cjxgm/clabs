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

#define NUMBER_OF_STARS 1020
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

/*star struct*/
typedef struct 
{
  float xpos, ypos;
  short zpos, speed;
  Uint8 color;
} STAR;

static STAR stars[NUMBER_OF_STARS];

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

void init_star(STAR* star, int i)
{
  /* randomly init stars, generate them around the center of the screen */
  
  star->xpos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  star->ypos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i;
  star->speed =  2 + (int)(2.0 * (rand()/(RAND_MAX+1.0)));

  star->color = i >> 2; /*the closer to the viewer the brighter*/
}

void init()
{
  int i;
  SDL_Color colors[256];
  
  /* create and set palette */

  colors[0].r = 0;
  colors[0].g = 0;
  colors[0].b = 0;

  for (i = 0; i < 255; ++i)
    {
      colors[255 - i].r = i;
      colors[255 - i].g = i;
      colors[255 - i].b = i;
    }
  
  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 

  for (i = 0; i < NUMBER_OF_STARS; i++)
    {
      init_star(stars + i, i + 1);
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

int main( int argc, char* argv[] )
{
  Uint16 centerx, centery;
  int i, tempx, tempy;  

 if (argc > 1)
    {
      printf("Retro Starfield Effect - W.P. van Paassen - 2002\n");
      return -1;
    }

 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN;*/))
   quit(1);
 
 TDEC_init_timer();
 
 SDL_WM_SetCaption("Retro - 3D Starfield - ", "");
  
 init();
  
 /* Lock the screen for direct access to the pixels */
 SDL_LockSurface(screen);

 centerx = SCREEN_WIDTH >> 1;
 centery = SCREEN_HEIGHT >> 1;

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();

      /* clear screen */
      
      memset(screen->pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint8));
      
      /* move and draw stars */
      
      for (i = 0; i < NUMBER_OF_STARS; i++)
	{
	  stars[i].zpos -= stars[i].speed;
	  
	  if (stars[i].zpos <= 0)
	    {
	      init_star(stars + i, i + 1);
	    }

	  /*compute 3D position*/
	  tempx = (stars[i].xpos / stars[i].zpos) + centerx;
	  tempy = (stars[i].ypos / stars[i].zpos) + centery;

	  if (tempx < 0 || tempx > SCREEN_WIDTH - 1 || tempy < 0 || tempy > SCREEN_HEIGHT - 1) /*check if a star leaves the screen*/
	    {
	      init_star(stars + i, i + 1);
	      continue;
	    }
	  
	  *((Uint8*)screen->pixels + (tempy * screen->pitch) + tempx) = stars[i].color;
	}

      if (TDEC_fps_ok())
	SDL_Flip(screen);

    }
  
  return 0; /* never reached */
}


