/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl, Byron Ellacott - bje@apnic.net

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

#include <math.h>

#include "layers.h"
#include "utils.h"
#include "lens.h"

#define LENS_WIDTH 150
#define LENS_ZOOM  40

typedef struct
{
  int x;
  int y;
} PIXEL;

static PIXEL _lens_lens[LENS_WIDTH][LENS_WIDTH];
static SDL_Surface *_lens_surface;
static Uint32 _lens_x, _lens_y, _lens_xd, _lens_yd;

void _lens_apply_lens(int ox, int oy)
{
  int x, y;

  /* draw in stages to avoid overdrawing needed pixels */

  for (y = 0; y < LENS_WIDTH/2; ++y) 
    {
      for (x = LENS_WIDTH - 1; x >= LENS_WIDTH/2; --x) 
	{
	  if (!(_lens_lens[y][x].x == 0 && _lens_lens[y][x].y == 0))
	    {
	      TDEC_put_pixel(_lens_surface, ox + x, y + oy, 
			     TDEC_get_pixel(_lens_surface, ox + x + _lens_lens[y][x].x, oy + y + _lens_lens[y][x].y));
	    }
	}

      for (x = 0; x < LENS_WIDTH/2; ++x) 
	{
	  if (!(_lens_lens[y][x].x == 0 && _lens_lens[y][x].y == 0))
	    {
	      TDEC_put_pixel(_lens_surface, ox + x, y + oy, 
			     TDEC_get_pixel(_lens_surface, ox + x + _lens_lens[y][x].x, oy + y + _lens_lens[y][x].y));
	    }
	}
    }
  
  for (y = LENS_WIDTH - 1; y >= LENS_WIDTH / 2; --y) 
    {
      for (x = 0; x < LENS_WIDTH/2; ++x) 
	{
	  if (!(_lens_lens[y][x].x == 0 && _lens_lens[y][x].y == 0))
	    {
	      TDEC_put_pixel(_lens_surface, ox + x, y + oy, 
			     TDEC_get_pixel(_lens_surface, ox + x + _lens_lens[y][x].x, oy + y + _lens_lens[y][x].y));
	    }
	}

      for (x = LENS_WIDTH - 1; x >= LENS_WIDTH/2; --x) 
	{
	  if (!(_lens_lens[y][x].x == 0 && _lens_lens[y][x].y == 0))
	    {
	      TDEC_put_pixel(_lens_surface, ox + x, y + oy, 
			     TDEC_get_pixel(_lens_surface, ox + x + _lens_lens[y][x].x, oy + y + _lens_lens[y][x].y));
	    }
	}
    }
}

void lens_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
   int x, y, r, d;

   _lens_surface = s;
   _lens_x = 60; 
   _lens_y = 60; 
   _lens_xd = 1;
   _lens_yd = 1;
 
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
     * the plane to the surface of the sphere.  
    
     * it is sufficient to generate 1/4 of the lens and reflect this
     * around; 
     * a sphere is mirrored on both the x and y axes 
     */

    for (y = 0; y < LENS_WIDTH/2; ++y) 
      {
        for (x = 0; x < LENS_WIDTH/2; ++x) 
	  {
            int tix, tiy;
            if ((x * x + y * y) <= (r * r)) 
	      {
                float shift = d/sqrt(d*d - (x*x + y*y - r*r));
                tix = x * shift - x;
                tiy = y * shift - y;
	      } 
	    else 
	      {
		tix = 0;
		tiy = 0;
	      }

            _lens_lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 - x].x = -tix;
            _lens_lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 - x].y = -tiy;

            _lens_lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 + x].x = tix;
            _lens_lens[LENS_WIDTH/2 - y][LENS_WIDTH/2 + x].y = -tiy;

            _lens_lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 + x].x = tix;
            _lens_lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 + x].y = tiy;

            _lens_lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 - x].x = -tix;
            _lens_lens[LENS_WIDTH/2 + y][LENS_WIDTH/2 - x].y = tiy;
	  }
      }
}

void lens_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
	lens_LTX_init_effect_valist(s, 0, 0);	
}

void lens_LTX_draw_effect(void)
{
    _lens_apply_lens(_lens_x, _lens_y);
    
    /* shift the coordinates around */

    _lens_x += _lens_xd; _lens_y += _lens_yd;

    if (_lens_x > (_lens_surface->w - LENS_WIDTH - 15) || _lens_x < 15)
      {
	_lens_xd = -_lens_xd;
      }
    if (_lens_y > (_lens_surface->h - LENS_WIDTH - 15) || _lens_y < 15)
      {
	_lens_yd = -_lens_yd;
      }
}

void lens_LTX_free_effect(void)
{
}

Uint8 lens_LTX_is_filter(void)
{
  return TDEC_FILTER;
}
