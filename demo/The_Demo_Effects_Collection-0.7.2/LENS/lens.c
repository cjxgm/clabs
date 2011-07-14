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

/* 07-17-02 Fixed a bug in the image loading, tux image is now displayed ok - WP - */
/* 07-14-02 Optimizations by WP */
/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 360
#define LENS_WIDTH 150
#define LENS_ZOOM  40

static SDL_Rect dst = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
static SDL_Surface *backing = NULL;
static Uint32 lens[LENS_WIDTH][LENS_WIDTH];

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (backing) SDL_FreeSurface(backing);

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

void apply_lens(int ox, int oy)
{
  Uint8* image = (Uint8*)screen->pixels;
  Uint8* back = (Uint8*)backing->pixels;
  int x, y, temp, pos;

  for (y = 0; y < LENS_WIDTH; y++) {
    temp = (y + oy) * SCREEN_WIDTH + ox;
    for (x = 0; x < LENS_WIDTH; x++) {
      pos = temp + x;
      image[pos] = back[pos + lens[y][x]];
    }
  }
}

void init()
{
   int i, x, y, r, d;
 
    /* load the background image */
    backing = IMG_Load("../GFX/tuxblackbg.png");
    if (!backing) {
        fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
        quit(3);
    }
    
    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, backing->format->palette->colors, 0, backing->format->palette->ncolors);

    /* blit it to the screen */
    SDL_BlitSurface(backing, NULL, screen, &dst);
    SDL_UpdateRects(screen, 1, &dst);
    SDL_LockSurface(screen);

    /* generate the lens distortion */
    r = LENS_WIDTH/2;
    d = LENS_ZOOM;

    /* the shift in the following expression is a function of the
     * distance of the current point from the center of the sphere.
     * If you imagine:
     *
     *       eye
     *
     *   .-~~~~~~~-.    sphere surface
     * .`           '.
     * ---------------  viewing plane
     *        .         center of sphere
     *
     * For each point across the viewing plane, draw a line from the
     * point on the sphere directly above that point to the center of
     * the sphere.  It will intersect the viewing plane somewhere
     * closer to the center of the sphere than the original point.
     * The shift function below is the end result of the above math,
     * given that the height of the point on the sphere can be derived
     * from:
     *
     * x^2 + y^2 + z^2 = radius^2
     *
     * x and y are known, z is based on the height of the viewing
     * plane.
     *
     * The radius of the sphere is the distance from the center of the
     * sphere to the edge of the viewing plane, which is a neat little
     * triangle.  If d = the distance from the center of the sphere to
     * the center of the plane (aka, LENS_ZOOM) and r = half the width
     * of the plane (aka, LENS_WIDTH/2) then radius^2 = d^2 + r^2.
     *
     * Things become simpler if we take z=0 to be at the plane's
     * height rather than the center of the sphere, turning the z^2 in
     * the expression above to (z+d)^2, since the center is now at
     * (0, 0, -d).
     *
     * So, the resulting function looks like:
     *
     * x^2 + y^2 + (z+d)^2 = d^2 + r^2
     *
     * Expand the (z-d)^2:
     *
     * x^2 + y^2 + z^2 + 2dz + d^2 = d^2 + r^2
     *
     * Rearrange things to be a quadratic in terms of z:
     *
     * z^2 + 2dz + x^2 + y^2 - r^2 = 0
     *
     * Note that x, y, and r are constants, so apply the quadratic
     * formula:
     *
     * For ax^2 + bx + c = 0,
     * 
     * x = (-b +- sqrt(b^2 - 4ac)) / 2a
     *
     * We can ignore the negative result, because we want the point at
     * the top of the sphere, not at the bottom.
     *
     * x = (-2d + sqrt(4d^2 - 4 * (x^2 + y^2 - r^2))) / 2
     *
     * Note that you can take the -4 out of both expressions in the
     * square root to put -2 outside, which then cancels out the
     * division:
     *
     * z = -d + sqrt(d^2 - (x^2 + y^2 - r^2))
     *
     * This now gives us the height of the point on the sphere
     * directly above the equivalent point on the plane.  Next we need
     * to find where the line between this point and the center of the
     * sphere at (0, 0, -d) intersects the viewing plane at (?, ?, 0).
     * This is a matter of the ratio of line below the plane vs the
     * total line length, multiplied by the (x,y) coordinates.  This
     * ratio can be worked out by the height of the line fragment
     * below the plane, which is d, and the total height of the line,
     * which is d + z, or the height above the plane of the sphere
     * surface plus the height of the plane above the center of the
     * sphere.
     *
     * ratio = d/(d + z)
     *
     * Subsitute in the formula for z:
     *
     * ratio = d/(d + -d + sqrt(d^2 - (x^2 + y^2 - r^2))
     *
     * Simplify to:
     *
     * ratio = d/sqrt(d^2 - (x^2 + y^2 - r^2))
     *
     * Since d and r are constant, we now have a formula we can apply
     * for each (x,y) point within the sphere to give the (x',y')
     * coordinates of the point we should draw to project the image on
     * the plane to the surface of the sphere.  I subtract the
     * original (x,y) coordinates to give an offset rather than an
     * absolute coordinate, then convert that offset to the image
     * dimensions, and store the offset in a matrix the size of the
     * intersecting circle.  Drawing the lens is then a matter of:
     *
     * screen[coordinate] = image[coordinate + lens[y][x]]
     *
     */

    /* it is sufficient to generate 1/4 of the lens and reflect this
     * around; a sphere is mirrored on both the x and y axes */
    for (y = 0; y < LENS_WIDTH >> 1; y++) {
        for (x = 0; x < LENS_WIDTH >> 1; x++) {
            int ix, iy, offset;
            if ((x * x + y * y) < (r * r)) {
                float shift = d/sqrt(d*d - (x*x + y*y - r*r));
                ix = x * shift - x;
                iy = y * shift - y;
            } else {
                ix = 0;
                iy = 0;
            }
            offset = (iy * SCREEN_WIDTH + ix);
            lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 - x] = -offset;
            lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 + x] = offset;
            offset = (-iy * SCREEN_WIDTH + ix);
            lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 - x] = -offset;
            lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 + x] = offset;
        }
    }

    /*disable events */
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
	if (i != SDL_KEYDOWN && i != SDL_QUIT) {
	    SDL_EventState(i, SDL_IGNORE);
	}
    }
  
    SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
    int x = 16, y = 16;
    int xd = 1, yd = 1;

    if (argc > 1) {
	printf("Retro Lens Effect - B. Ellacott - 2002\n");
	return -1;
    }

    if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
	quit(1);
    
    TDEC_init_timer();

    SDL_WM_SetCaption("Retro - Lens - ", "");
  
    init();

    /* time based demo loop */
    while( 1 ) {
	TDEC_new_time();

	process_events();

        /* blit the whole backing image across */
        SDL_UnlockSurface(screen);
        SDL_BlitSurface(backing, NULL, screen, &dst);
        SDL_LockSurface(screen);

        /* apply the lens */
        apply_lens(x, y);

        /* shift the coordinates around */
        x += xd; y += yd;
        if (x > (SCREEN_WIDTH - LENS_WIDTH - 15) || x < 15) xd = -xd;
        if (y > (SCREEN_HEIGHT - LENS_WIDTH - 15) || y < 15) yd = -yd;

	/* update changed portions */
	if (TDEC_fps_ok()) {
            SDL_UpdateRects(screen, 1, &dst);
	}
    }
  
    return 0; /* never reached */
}

