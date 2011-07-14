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

#ifndef VIDEO_H
#define VIDEO_H

#include "SDL/SDL.h"

SDL_Surface* screen;

/* 2D */

/* this function will first call TDEC_init_video */
extern int TDEC_set_video(Uint16 width, Uint16 height, int bpp, int flags);

/* 3D */
/* call TDEC_init_video, set SDL_GL_Attributes, then call this function */
extern int TDEC_set_video_GL(Uint16 width, Uint16 height, int bpp, int flags);

/* global functions */
extern int TDEC_init_video();

#endif
