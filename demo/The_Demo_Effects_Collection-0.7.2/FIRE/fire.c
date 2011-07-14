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

static Uint8 fire[SCREEN_WIDTH * SCREEN_HEIGHT];
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
 
  /* create a suitable fire palette, this is crucial for a good effect */
  /* black to blue, blue to red, red to yellow, yellow to white*/
  
 for (i = 0; i < 32; ++i)
    {
      /* black to blue, 32 values*/
      colors[i].b = i << 1;

      /* blue to red, 32 values*/
      colors[i + 32].r = i << 3;
      colors[i + 32].b =  64 - (i << 1);

      /*red to yellow, 32 values*/
      colors[i + 64].r = 255;
      colors[i + 64].g = i << 3;

      /* yellow to white, 162 */
      colors[i + 96].r = 255;
      colors[i + 96].g = 255;
      colors[i + 96].b = i << 2;
      colors[i + 128].r = 255;
      colors[i + 128].g = 255;
      colors[i + 128].b = 64 + (i << 2);
      colors[i + 160].r = 255;
      colors[i + 160].g = 255;
      colors[i + 160].b = 128 + (i << 2);
      colors[i + 192].r = 255;
      colors[i + 192].g = 255;
      colors[i + 192].b = 192 + i;
      colors[i + 224].r = 255;
      colors[i + 224].g = 255;
      colors[i + 224].b = 224 + i;
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
  Uint16 temp;
  Uint8 index;
  int i,j;  

 if (argc > 1)
    {
      printf("Retro Fire Effect - W.P. van Paassen - 2002\n");
      return -1;
    }

 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE/* | SDL_FULLSCREEN;*/))
   quit(1);
 TDEC_init_timer();

  SDL_WM_SetCaption("Retro - Fire - ", "");
  
  init();

  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();

   /* draw random bottom line in fire array*/

      j = SCREEN_WIDTH * (SCREEN_HEIGHT- 1);
      for (i = 0; i < SCREEN_WIDTH - 1; i++)
	{
	  int random = 1 + (int)(16.0 * (rand()/(RAND_MAX+1.0)));
	  if (random > 9) /* the lower the value, the intenser the fire, compensate a lower value with a higher decay value*/
	    fire[j + i] = 255; /*maximum heat*/
	  else
	    fire[j + i] = 0;
	}  
      
      /* move fire upwards, start at bottom*/
      
      for (index = 0; index < 60 ; ++index)
	{
	  for (i = 0; i < SCREEN_WIDTH - 1; ++i)
	    {
	      if (i == 0) /* at the left border*/
		{
		  temp = fire[j];
		  temp += fire[j + 1];
		  temp += fire[j - SCREEN_WIDTH];
		  temp /=3;
		}
	      else if (i == SCREEN_WIDTH - 1) /* at the right border*/
		  {
		    temp = fire[j + i];
		    temp += fire[j - SCREEN_WIDTH + i];
		    temp += fire[j + i - 1];
		    temp /= 3;
		  }
	      else
		{
		  temp = fire[j + i];
		  temp += fire[j + i + 1];
		  temp += fire[j + i - 1];
		  temp += fire[j - SCREEN_WIDTH + i];
		  temp >>= 2;
		}
	      if (temp > 1)
	      	temp -= 1; /* decay */

	      fire[j - SCREEN_WIDTH + i] = temp;
	    }
	  j -= SCREEN_WIDTH;
	}      

      image = (Uint8*)screen->pixels + (screen->pitch * SCREEN_HEIGHT);  /*start in the right bottom corner*/
      
      /* draw fire array to screen from bottom to top + 300*/

      for (i = SCREEN_HEIGHT - 3; i >= 300; --i)
	{
	  for (j = SCREEN_WIDTH - 1; j >= 0; --j)
	    {
	      *image= fire[i * SCREEN_WIDTH + j];
	      image--;
	    }
	}

      if (TDEC_fps_ok())
	SDL_UpdateRect(screen, 0, SCREEN_HEIGHT - 55, SCREEN_WIDTH, 55);
    }
  
  return 0; /* never reached */
}






















