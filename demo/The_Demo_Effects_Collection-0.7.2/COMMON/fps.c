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

#include "fps.h"

static Uint32 /*Ticks = 0, Drawn_Frames = 0,*/ next, now;
static unsigned char fps = 50;
static Uint16 Tick_Interval = 20 ;

void TDEC_set_fps(unsigned char _fps)
{
	fps = _fps;
	Tick_Interval = 1000 / fps;
}

void TDEC_print_fps()
{
  printf("FPS was: %i\n", fps);
}

void TDEC_new_time()
{
     next = SDL_GetTicks() + Tick_Interval;
}

int TDEC_get_fps()
{
  /*return 1000 / (Ticks / Drawn_Frames);*/
  return fps;
}

int TDEC_fps_ok()
{
  now = SDL_GetTicks();
  
  if (now < next)
    {
      /*frame rate is met*/
      SDL_Delay(next - now);

    }
  else
  {
	  /* frame rate is too high, lower it */
	  TDEC_set_fps(fps-1);
	  return 0;
  }

  return 1;
}

void TDEC_init_timer()
{
  SDL_InitSubSystem(SDL_INIT_TIMER);
}

