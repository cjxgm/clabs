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

#ifndef UTILS_H
#define UTILS_H

#include "SDL/SDL.h"

/* copying */
extern SDL_Surface* TDEC_copy_surface(SDL_Surface *surface);

/* palette fading */
extern Uint8 TDEC_fadeout( SDL_Surface *s, Uint8 rate);
extern Uint8 TDEC_fadein( SDL_Surface *s, SDL_Palette *d, Uint8 rate );
extern void TDEC_blacken_palette(SDL_Surface *s);

/* mozaiek effect */
extern void TDEC_mozaiek_surface( SDL_Surface *s, Uint16 blocksize);

/* image scaling */
extern void TDEC_scalex_copy_image(SDL_Surface *original, SDL_Surface *copy, Uint8 percentage);
extern void TDEC_scaley_copy_image(SDL_Surface *original, SDL_Surface *copy, Uint8 percentage);
extern void TDEC_scale_copy_image(SDL_Surface *original, SDL_Surface *copy, Uint8 percentage);
extern void TDEC_scalex_image(SDL_Surface *surface, Uint8 percentage);
extern void TDEC_scaley_image(SDL_Surface *surface, Uint8 percentage);
extern void TDEC_scale_image(SDL_Surface *surface, Uint8 percentage);
extern SDL_Surface* TDEC_scale_image_new(SDL_Surface *source, Uint8 percentage); /* creates a new image surface with new width and height */
extern void TDEC_scale_copy_hscanline(SDL_Surface *original, SDL_Surface *copy, Uint16 scanline_index, Uint8 percentage);
extern void TDEC_scale_copy_vscanline(SDL_Surface *original, SDL_Surface *copy, Uint16 scanline_index, Uint8 percentage);
extern void TDEC_scale_hscanline(SDL_Surface *surface, Uint16 scanline_index, Uint8 percentage);
extern void TDEC_scale_vscanline(SDL_Surface *surface, Uint16 scanline_index, Uint8 percentage);

/* image flipping */
extern void TDEC_flipx_image(SDL_Surface *surface);
extern void TDEC_flipy_image(SDL_Surface *surface);
extern void TDEC_flipx_copy_image(SDL_Surface *original, SDL_Surface *copy);
extern void TDEC_flipy_copy_image(SDL_Surface *original, SDL_Surface *copy);

/*various */
extern void TDEC_create_heightmap(SDL_Surface *source);
extern void TDEC_create_blackandwhite(SDL_Surface *surface);

/* putting/getting pixels */
extern Uint32 TDEC_get_pixel(SDL_Surface *surface, int x, int y);
extern void TDEC_put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/* variables */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
static const Uint32 r_mask = 0xFF000000; 
static const Uint32 g_mask = 0x00FF0000;
static const Uint32 b_mask = 0x0000FF00;
static const Uint32 a_mask = 0x000000FF;
#else
static const Uint32 r_mask = 0x000000FF; 
static const Uint32 g_mask = 0x0000FF00;
static const Uint32 b_mask = 0x00FF0000;
static const Uint32 a_mask = 0xFF000000;
#endif


#endif
