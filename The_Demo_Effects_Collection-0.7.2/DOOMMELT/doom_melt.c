/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl, 
                      Ed Sinjiashvili  - slimb@swes.saren.ru
		      dekoder          - dekoder81@users.sourceforge.net

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

/* 07-31-02 Optimizations done by WP */
/* 03-20-04 Improvement by Dekoder */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360
#define COL_WIDTH 2
#define NUM_COLS SCREEN_WIDTH / COL_WIDTH

typedef struct column
{
  short x, y;
} column_t;

static SDL_Surface *image;
static column_t columns[NUM_COLS];

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

int rand_fun ()
{
    return rand() % 256;
}

void init_columns ()
{
  int i, start_x = 1;

  columns[0].y = -(rand_fun() % 16);
  columns[0].x = 0;

  for (i = 1; i < NUM_COLS; i++)
    {
      int r = (rand_fun() % 3) - 1;
      columns[i].y = columns[i-1].y + r;
      if (columns[i].y > 0)
      	columns[i].y = 0;
      else if (columns[i].y == -16)
	columns[i].y = -15;
      
      columns[i].x = start_x;
      
      start_x += COL_WIDTH;
    }
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
        quit( 0 );
        break;
      case SDLK_SPACE:
	init_columns();
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

  image = IMG_Load ("../GFX/tuxblackbg.png");
  if (!image)
    quit(1);

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, image->format->palette->colors, 0, image->format->palette->ncolors);

  /*disable events */
  
  for (i = 0; i < SDL_NUMEVENTS; ++i)
    {
      if (i != SDL_KEYDOWN && i != SDL_QUIT)
	{
	  SDL_EventState(i, SDL_IGNORE);
	}
    }
  
  SDL_ShowCursor(SDL_DISABLE);

  init_columns ();
}

void column_draw (column_t *column)
{
  static SDL_Rect image_rect = {0, 0, COL_WIDTH, };
  static SDL_Rect dest_rect = {0, 0, COL_WIDTH, SCREEN_HEIGHT};

  int tmp = column->y;
  if (tmp < 0) 
    tmp = 0;
  
  dest_rect.x = column->x;
  dest_rect.y = tmp;

  image_rect.x = column->x;
  image_rect.h = image->h - tmp;

  SDL_BlitSurface(image, &image_rect, screen, &dest_rect);
}

void column_think (column_t *column)
{
  static int grow = 0;
 
  if (column->y < 0)
    {
      grow = 1;
    }
  else if (column->y < SCREEN_HEIGHT)
    {
      if (column->y < 8)
	grow = column->y+1;
      else
	{
	  grow = 4; 
	}
    }
  
    column->y += grow;
}
 
void main_loop ()
{
  int i;

  SDL_BlitSurface(image, 0, screen, 0);
  
  for (i = 0; i < NUM_COLS; i++)
    {
      column_draw (columns + i);
      column_think (&columns[i]); 
    }

  SDL_Flip(screen);
}

int main( int argc, char* argv[] )
{
  if (argc > 1)
    {
      printf("Retro Doom Melting Effect, Ed Sinjiashvili & W.P. van Paassen\n");
      return -1;
    }

  printf("\nPress Space to melt DOOM style\n");
  
  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, 
		 SDL_HWACCEL | SDL_HWSURFACE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN;*/))
   {
     quit(1);
   }

  TDEC_init_timer();
 
  SDL_WM_SetCaption("Retro - DOOM Melt - ", "");
  
  init();

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
      
      process_events();

      if (TDEC_fps_ok())
	main_loop ();
    }
  
  return 0; /* never reached */
}
