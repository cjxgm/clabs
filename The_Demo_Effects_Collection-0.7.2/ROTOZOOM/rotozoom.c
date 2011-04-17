/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl, Byron Ellacott - bje@apnic.net

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

/* 07-14-02 Major speed optimizations done by WP (it's really fast now :-)). Uses an 8 bit indexed image*/
/* note that the code has not been fully optimized */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
static SDL_Surface *tile = NULL;
static int roto[256];
static int roto2[256];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (tile) {
      SDL_FreeSurface(tile);
  }

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

void draw_tile(int stepx, int stepy, int zoom)
{
  Uint8 *image = (Uint8 *)screen->pixels;
  Uint8 *texture = (Uint8 *)tile->pixels;
  int x, y, i, j, xd, yd, a, b, sx, sy;
    
  sx = sy = 0;
  xd = (stepx * zoom) >> 12;
  yd = (stepy * zoom) >> 12;
    
  /* Stepping across and down the screen, each screen row has a
   * starting coordinate in the texture: (sx, sy).  As each screen
   * row is traversed, the current texture coordinate (x, y) is
   * modified by (xd, yd), which are (sin(rot), cos(rot)) multiplied
   * by the current zoom factor.  For each vertical step, (xd, yd)
   * is rotated 90 degrees, to become (-yd, xd).
   *
   * More fun can be had by playing around with x, y, xd, and yd as
   * you move about the image.
   */

  for (j = 0; j < SCREEN_HEIGHT; j++) {
    x = sx; y = sy;   
    for (i = 0; i < SCREEN_WIDTH; i++) {
      a = x >> 12 & 255;
      b = y >> 12 & 255;
      *image++ = texture[b * tile->w + a];
      x += xd; y += yd;
    }
    sx -= yd; sy += xd;
  }
}

void init()
{
    int i;
 
    /* load the background image */
    tile = IMG_Load("../GFX/tile.png");
    if (!tile) {
        fprintf(stderr, "Cannot open file tile.png: %s\n", SDL_GetError());
        quit(3);
    }
     
    if (tile->w != 256 || tile->h != 256) {
        fprintf(stderr, "Tile dimensions must be 256x256!\n");
        quit(3);
    }

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, tile->format->palette->colors, 0, 256);

    for (i = 0; i < 256; i++) {
        float rad =  (float)i * 1.41176 * 0.0174532;
	float c = sin(rad);
        roto[i] = (c + 0.8) * 4096.0;
	roto2[i] = (2.0 * c) * 4096.0;
    }

    /*disable events */
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
	if (i != SDL_KEYDOWN && i != SDL_QUIT) {
	    SDL_EventState(i, SDL_IGNORE);
	}
    }

    SDL_LockSurface(tile);
    SDL_LockSurface(screen);
  
    SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
    unsigned path = 0, zpath = 0;

    if (argc > 1) {
	printf("Retro Rotozoom Effect - B. Ellacott, W.P. van Paassen - 2002\n");
	return -1;
    }

    if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE))
	quit(1);
    TDEC_init_timer();

    SDL_WM_SetCaption("Retro - Rotozoom - ", "");
  
    TDEC_set_fps(30);
    init();

    /* time based demo loop */
    while( 1 ) {
	TDEC_new_time();

	process_events();

        /* draw the tile at current zoom and rotation */
	draw_tile(roto[path], roto[(path + 128) & 255], roto2[zpath]);

        path = (path - 1) & 255;
        zpath = (zpath + 1) & 255;

	/* update changed portions */
	if (TDEC_fps_ok()) {
            SDL_Flip(screen);
        } 
    }
  
    return 0; /* never reached */
}

