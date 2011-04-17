/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#ifndef FIRE_H
#define FIRE_H

#include <stdarg.h>
#include "SDL/SDL.h"
#include "list.h"

extern void fire_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters);
extern void fire_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list);
extern void fire_LTX_draw_effect(void);
extern void fire_LTX_free_effect(void);
extern Uint8 fire_LTX_is_filter(void);

#endif
