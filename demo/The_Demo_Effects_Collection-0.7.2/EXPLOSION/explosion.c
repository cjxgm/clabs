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

#define NUMBER_OF_PARTICLES 500
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static Uint8 fire[SCREEN_WIDTH * SCREEN_HEIGHT];
static SDL_Color colors[256];

/*particle structure*/
typedef struct 
{
  Uint16 xpos,ypos,xdir,ydir;
  Uint8 colorindex;
  Uint8 dead;
} PARTICLE;

static PARTICLE particles[NUMBER_OF_PARTICLES];

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

void init_particle(PARTICLE* particle)
{
  /* randomly init particles, generate them in the center of the screen */

  particle->xpos =  (SCREEN_WIDTH >> 1) - 20 + (int)(40.0 * (rand()/(RAND_MAX+1.0)));
  particle->ypos =  (SCREEN_HEIGHT >> 1) - 20 + (int)(40.0 * (rand()/(RAND_MAX+1.0)));
  particle->xdir =   -10 + (int)(20.0 * (rand()/(RAND_MAX+1.0)));
  particle->ydir =   -17 + (int)(19.0 * (rand()/(RAND_MAX+1.0)));
  particle->colorindex = 255;
  particle->dead = 0;
}

void handle_key_down( SDL_keysym* keysym )
{
  int i;

    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
        quit( 0 );
        break;
      case SDLK_SPACE:

	/* randomly init particles, generate them in the center of the screen */

	for (i = 0; i < NUMBER_OF_PARTICLES; i++)
	  {
	    init_particle(particles + i);
	  }
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
  Uint32 i;

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

  for (i = 0; i < NUMBER_OF_PARTICLES; i++)
    {
      init_particle(particles + i);
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
  Uint8* image, *image_start;
  Uint32 buf, index, temp;
  int i, j; 

  printf("\nPress Space to relive the experience!!\n");
  
  if (argc > 1)
    {
      printf("Retro Particle Explosion Effect - W.P. van Paassen - 2002\n");
      return -1;
    }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN;*/))
    quit(1);

  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro - Particle Explosion - ", "");
  
  init();

  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);

  image_start = (Uint8*)screen->pixels + (screen->pitch * SCREEN_HEIGHT);  /*start in the right bottom corner*/

  /* fill fire array with black */

  memset(fire, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint8));

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();
      
      /* move and draw particles into fire array */

      for (i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
	  if (!particles[i].dead)
	    {
	      particles[i].xpos += particles[i].xdir;
	      particles[i].ypos += particles[i].ydir;
	  
	      /* is particle dead? */
	  
	      if ((particles[i].ypos >= SCREEN_HEIGHT - 3) || particles[i].colorindex == 0 || 
		  particles[i].xpos <= 1 || particles[i].xpos >= SCREEN_WIDTH - 3)
		{
		  particles[i].dead = 1;
		  continue;
		}
	  
	      /* gravity takes over */
	  
	      particles[i].ydir++;
	  
	      /* particle cools off */
	  
	      particles[i].colorindex--;
	  
	      /* draw particle */
	  
	      temp = particles[i].ypos * SCREEN_WIDTH + particles[i].xpos;
	  
	      fire[temp] = particles[i].colorindex;
	      fire[temp - 1] = particles[i].colorindex;
	      fire[temp + SCREEN_WIDTH] = particles[i].colorindex;
	      fire[temp - SCREEN_WIDTH] = particles[i].colorindex;
	      fire[temp + 1] = particles[i].colorindex;
	    }
	}
  
      /* create fire effect */
  
      for (i = 1; i < SCREEN_HEIGHT - 2; i++)
	{
	  index = (i  - 1) * SCREEN_WIDTH;
      
	  for (j = 1; j < SCREEN_WIDTH - 2; j++)
	    {
	      buf = index + j;
	  
	      temp = fire[buf];
	      temp += fire[buf + 1];
	      temp += fire[buf - 1];
	      buf += SCREEN_WIDTH;
	      temp += fire[buf - 1];
	      temp += fire[buf + 1];
	      buf += SCREEN_WIDTH;
	      temp += fire[buf];
	      temp += fire[buf + 1];
	      temp += fire[buf - 1];
	  
	      temp >>= 3;
	  
	      if (temp > 4)
		{
		  temp -= 4;
		}
	      else
		{
		  temp = 0;
		}
	      
	      fire[buf - SCREEN_WIDTH] = temp;
	    }
	}
      
      image = image_start;
      
      /* draw fire array to screen from bottom to top*/
      
      for (i = SCREEN_HEIGHT - 1; i >= 0; --i)
	{
	  temp = i * SCREEN_WIDTH;
	  for (j = SCREEN_WIDTH - 1; j >= 0; --j)
	    {
	      *image= fire[temp + j];
	      image --;
	    }
	}

      if (TDEC_fps_ok())
	SDL_Flip(screen);
    }
  
  return 0; /* never reached */
}







