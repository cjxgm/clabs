/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

/*01-12-2003 drawing directly to screen for speed increase - WP - */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define BLOB_RADIUS 44
#define BLOB_DRADIUS (BLOB_RADIUS * 2)
#define BLOB_SRADIUS (BLOB_RADIUS * BLOB_RADIUS)
#define NUMBER_OF_BLOBS 40
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static SDL_Color colors[256];

Uint8 blob[BLOB_DRADIUS][BLOB_DRADIUS];

/*blob structure*/
typedef struct 
{
  short xpos,ypos;
} BLOB;

static BLOB blobs[NUMBER_OF_BLOBS];

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

void init_blob(BLOB *blob)
{
  blob->xpos =  (SCREEN_WIDTH >> 1) - BLOB_RADIUS;
  blob->ypos =  (SCREEN_HEIGHT >> 1) - BLOB_RADIUS;
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
	
	/* reinit blobs,  generate them in the center of the screen */

	for (i = 0; i < NUMBER_OF_BLOBS; i++)
	  {
	    init_blob(blobs + i);
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
  int i, j;
  Uint32 distance_squared;
  float fraction;

  /* create a suitable palette, this is crucial for a good effect */
  
  for (i = 0; i < 256; ++i)
    {
      colors[i].r = i;
      colors[i].g = i;
      colors[i].b = i;
    } 
 
  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 


  /* create blob */
  for (i = -BLOB_RADIUS; i < BLOB_RADIUS; ++i)
    {
      for (j = -BLOB_RADIUS; j < BLOB_RADIUS; ++j)
	{
	  distance_squared = i * i + j * j;
	  if (distance_squared <= BLOB_SRADIUS)
	    {
	      /* compute density */     
	      fraction = (float)distance_squared / (float)BLOB_SRADIUS;
	      blob[i + BLOB_RADIUS][j + BLOB_RADIUS] = pow((1.0 - (fraction * fraction)),4.0) * 255.0;
	    }
	  else
	    blob[i + BLOB_RADIUS][j + BLOB_RADIUS] = 0;
	}    
    }

  for (i = 0; i < NUMBER_OF_BLOBS; i++)
    {
      init_blob(blobs + i);
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
  Uint8* image;
  Uint32 start;
  int i, j; 
  Uint8 k;

  printf("\nPress Space to reinit the blobs!!\n");
  
  if (argc > 1)
    {
      printf("Retro Blob - W.P. van Paassen - 2002\n");
      return -1;
    }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/))
    quit(1);

  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro Blob ", "");
  
  init();

  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);

  TDEC_set_fps(50);

  image = (Uint8*)screen->pixels;

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();
      
      memset(image, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint8));

      /* move and draw blobs to screen */

      for (i = 0; i < NUMBER_OF_BLOBS; i++)
	{
	  blobs[i].xpos += -2 + (int)(5.0 * (rand()/(RAND_MAX+1.0)));
	  blobs[i].ypos += -2 + (int)(5.0 * (rand()/(RAND_MAX+1.0)));
	}

      for (k = 0; k < NUMBER_OF_BLOBS; ++k)
	{ 
	  if (blobs[k].xpos > 0 && blobs[k].xpos < SCREEN_WIDTH - BLOB_DRADIUS &&
	      blobs[k].ypos > 0 && blobs[k].ypos < SCREEN_HEIGHT - BLOB_DRADIUS)
	      {
		start = blobs[k].xpos + blobs[k].ypos * SCREEN_WIDTH; 
		for (i = 0; i < BLOB_DRADIUS; ++i)
		{
		  for (j = 0; j < BLOB_DRADIUS; ++j)
		  {
		    if (image[start + j] + blob[i][j] > 255)
		    image[start + j] = 255;
		    else 
		    image[start + j] += blob[i][j];     
		  }
		  start += SCREEN_WIDTH;
		}
	      }
	  else
	    init_blob(blobs + k);
	}   
      
      if (TDEC_fps_ok())
	SDL_Flip(screen);
    }
  
  return 0; /* never reached */
}
