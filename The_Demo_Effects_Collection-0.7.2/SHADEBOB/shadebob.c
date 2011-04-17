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

/* WP - added little performance optimizations */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

static Uint8 shades[SCREEN_WIDTH * SCREEN_HEIGHT];
static SDL_Color colors[256];
static int xpath[512];
static int ypath[512];
static int pathpath[1024];

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
    int i, hw, hh, aw, ah;
 
    hw = (SCREEN_WIDTH - 150);
    hh = (SCREEN_HEIGHT - 180);
    aw = 67;
    ah = 82;

    bzero(shades, sizeof(shades));

    /* initialise a movement path for the bob */
    for (i = 0; i < 512; i++) {
      double rad = ((float)i * 0.703125) * 0.0174532; /* spread 360 degrees over 512 values and convert to rad */
	xpath[i] = sin(rad * 2) * hw / 2 + hw / 2 + aw;
	ypath[i] = sin(rad) * hh / 2 + hh / 2 + ah;
    }

    for (i = 0; i < 1024; i++) {
	double rad = ((float)i * 0.3515625) * 0.0174532; /* spread 360 degrees over 1024 values and convert to rad */
	pathpath[i] = sin(rad) * 15;
    }

    /* create a suitable shadebob palette, this is crucial for a good effect */
    /* black to blue, blue to red, red to white*/
  
    for (i = 0; i < 64; ++i) {
	colors[i].r = 0;
	colors[i].g = 0;
	colors[i].b = i << 1;
	colors[i + 64].r = i << 1;
	colors[i + 64].g = 0;
	colors[i + 64].b = 128 - (i << 1);
	colors[i + 128].r = 128 + (i << 1);
	colors[i + 128].g = 0;
	colors[i + 128].b = 128 - (i << 1);
	colors[i + 192].r = 255;
	colors[i + 192].g = i << 2;
	colors[i + 192].b = i << 2;
    } 
 
    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 

    /*disable events */
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
	if (i != SDL_KEYDOWN && i != SDL_QUIT) {
	    SDL_EventState(i, SDL_IGNORE);
	}
    }
  
    SDL_ShowCursor(SDL_DISABLE);
}

/* the shadebob image to apply */
Uint8 heat[16][16] = {
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    { 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0},
    { 0, 0, 1, 1, 2, 2, 2, 3, 3, 2, 2, 2, 1, 1, 0, 0},
    { 0, 0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 2, 2, 1, 0, 0},
    { 0, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 1, 0},
    { 0, 1, 2, 2, 3, 3, 3, 4, 4, 3, 3, 3, 2, 2, 1, 0},
    { 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 2, 1, 1},
    { 1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 2, 1, 1},
    { 0, 1, 2, 2, 3, 3, 3, 4, 4, 3, 3, 3, 2, 2, 1, 0},
    { 0, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 1, 0},
    { 0, 0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 2, 2, 1, 0, 0},
    { 0, 0, 1, 1, 2, 2, 2, 3, 3, 2, 2, 2, 1, 1, 0, 0},
    { 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0},
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
};

int get_bob_x_location(int index)
{
    return xpath[index & 511] + pathpath[index & 1023];
}

int get_bob_y_location(int index)
{
    return ypath[index & 511] + pathpath[index & 1023];
}

int main( int argc, char* argv[] )
{
    int trail = 0;

    if (argc > 1) {
	printf("Retro Shadebob Effect - B. Ellacott, W.P. van Paassen - 2002\n");
	return -1;
    }

    if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
	quit(1);
    
    TDEC_init_timer();

    SDL_WM_SetCaption("Retro - Shadebob - ", "");
  
    init();

    /* Lock the screen for direct access to the pixels */
    SDL_LockSurface(screen);

    /* time based demo loop */
    while( 1 ) {
	Uint8 *image;
	int remx = 0, remy = 0;
	int drawx = 0, drawy = 0;
	int i, j, tmp;

	TDEC_new_time();

	process_events();

	/* Get a copy of the screen data */
	image = (Uint8*)screen->pixels;

	/* remove heat from tail */
	if (trail >= 500) {
	  tmp = trail - 500;
	  remx = get_bob_x_location(tmp);
	  remy = get_bob_y_location(tmp);
	  for (i = 0; i < 16; i++)
	    {
	      tmp = (remy + i) * SCREEN_WIDTH + remx;
	      for (j = 0; j < 16; j++) {
		int val = image[tmp + j];
		val -= heat[i][j] * 8;
		if (val < 0) val = 0;
		image[tmp + j] = val;
	      }
	    }
	}

	/* add heat at new head */
	drawx = get_bob_x_location(trail);
	drawy = get_bob_y_location(trail);
	for (i = 0; i < 16; i++)
	  {
	    tmp = (drawy + i) * SCREEN_WIDTH + drawx;
	    for (j = 0; j < 16; j++) {
		int val = image[tmp + j];
		val += heat[i][j] * 8;
		if (val > 255) val = 255;
		image[tmp + j] = val;
	    }
	  }

	/* update changed portions */
	if (TDEC_fps_ok()) {
	    SDL_UpdateRect(screen, remx, remy, 16, 16);
	    SDL_UpdateRect(screen, drawx, drawy, 16, 16);
	}
	  trail++;
    }
  
    return 0; /* never reached */
}

