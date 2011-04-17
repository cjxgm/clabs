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

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 180

static Uint8 blur[SCREEN_WIDTH * SCREEN_HEIGHT];
static SDL_Surface *image = NULL;
static SDL_Surface *bwimage = NULL;
static int xpath[512];
static int ypath[512];
static Uint8 quiting = 0;
static Uint16 blocksize = 2;

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_FreeSurface(bwimage);

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
            /* Handle quit requests*/
            quit( 0 );
            break;
	}
    }
}

void init()
{
  int i, hw, hh;
  
  image = IMG_Load("../GFX/tuxblackbg.png");
  if (!image) {
    fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
    quit(3);
  }
  
  TDEC_create_heightmap(image);
  
  bwimage = TDEC_scale_image_new(image, 20);

  SDL_FreeSurface(image);

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, bwimage->format->palette->colors, 0, bwimage->format->palette->ncolors);

  hw = (SCREEN_WIDTH >> 1) - (bwimage->w >> 1);
  hh = (SCREEN_HEIGHT >> 1) - (bwimage->h >> 1) - 1;
  
  /*create x and y values */
  for (i = 0; i < 512; ++i) 
    {
      double rad = ((float)i * 0.703125) * 0.0174532; /* spread 360 degrees over 512 values and convert to rad */
      xpath[i] = sin(rad) * hw + hw;
      ypath[i] = cos(rad) * hh + (hh + 1);
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
  Uint32 temp, start, buf, index, l = 0, m = 40, pos;
  short i, j, startx, starty; 

  if (argc > 1)
    {
      printf("Tux Trail - W.P. van Paassen - 2002\n");
      return -1;
    }
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/))
    quit(1);

  TDEC_init_timer();
  
  SDL_WM_SetCaption("Tux Trail ", "");
  
  init();

  /* Lock the screen for direct access to the pixels */
  SDL_LockSurface(screen);

  image_start = (Uint8*)screen->pixels + (screen->pitch * SCREEN_HEIGHT);  /*start in the right bottom corner*/

  /* fill blur array with black */
  memset(blur, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint8));

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();

      process_events();
             

      if (!quiting)
	{
	  startx = xpath[l];
	  starty = ypath[m];
	  
	  /* draw bwlogo into blur array */
	  
	  for (i = 1; i < SCREEN_HEIGHT - 2; i++)
	    {
	      index = (i  - 1) * SCREEN_WIDTH;
	      
	      for (j = 1; j < SCREEN_WIDTH - 2; j++)
		{
		  buf = index + j;
		  
		  temp = blur[buf];
		  temp += blur[buf + 1];
		  temp += blur[buf - 1];
		  buf += SCREEN_WIDTH;
		  temp += blur[buf - 1];
		  temp += blur[buf + 1];
		  buf += SCREEN_WIDTH;
		  temp += blur[buf];
		  temp += blur[buf + 1];
		  temp += blur[buf - 1];
		  
		  temp >>= 3;
		  
		  if (temp > 4)
		    {
		      temp -= 4;
		    }
		  else
		    temp = 0;
		  
		  blur[buf - SCREEN_WIDTH] = temp;
		}
	    }
	  
	  for (i = 0; i < bwimage->w; ++i)
	    {
	      start = i;
	      pos = startx + starty * SCREEN_WIDTH + i;
	      for (j = 0; j < bwimage->h; ++j)
		{

		  if (*((Uint8*)bwimage->pixels + start) >  blur[pos])
		    {
		      blur[pos] = *((Uint8*)bwimage->pixels + start);
		    }
		  pos += SCREEN_WIDTH;
		  start += bwimage->pitch;
		}
	    }
	  
	  image = image_start;
	  
	  /* draw blur array to screen from bottom to top*/
	  
	  for (i = SCREEN_HEIGHT - 1; i >= 0; --i)
	    {
	      temp = i * SCREEN_WIDTH;
	      for (j = SCREEN_WIDTH - 1; j >= 0; --j)
		{
		  *image= blur[temp + j];
		  image --;
		}
	    }
	  
	  l+=3;
	  l &= 511;
	  m+=2;
	  m &= 511;
	}
      else
	{
	  TDEC_mozaiek_surface(screen, blocksize);
	  blocksize <<= 1;
	  if (blocksize > 256)
	    quit(1);
	}
      
      if (TDEC_fps_ok())
	SDL_Flip(screen);

      if (quiting)
	SDL_Delay(200);
    }
  
  return 0; /* never reached */
}
