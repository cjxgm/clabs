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

#ifndef LAYERS_H
#define LAYERS_H

#include "SDL/SDL.h"
#include "list.h"

#define NLAYERS 25
#define TDEC_BACKGROUND_LAYER 0
#define TDEC_NO_RESTART_CALLBACK 0
#define TDEC_NO_FILTER 0
#define TDEC_FILTER 1

extern int TDEC_init_layer_system();
extern int TDEC_exit_layer_system();

extern char TDEC_add_effect_valist(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha, 
				   const char *module, void (*restart_callback)(void),  ...);
extern char TDEC_add_effect(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha, 
				   const char *module, void (*restart_callback)(void), TDEC_NODE *argument_list);
extern SDL_Surface* TDEC_get_backgroundlayer(void);
extern void TDEC_draw_layers(void);
extern void TDEC_free_layers(void);
extern SDL_Surface* TDEC_get_layer(Uint8 id);
extern void TDEC_flatten_layers(void);
extern void TDEC_clear_layer(Uint8 id);
extern void TDEC_remove_layer(void);
extern void TDEC_enable_layer(Uint8 id);
extern void TDEC_disable_layer(Uint8 id);
extern void TDEC_set_layer_alpha(Uint8 id, Uint8 alpha);
extern Uint8 TDEC_get_layer_alpha(Uint8 id);
extern void TDEC_set_layer_colorkey(Uint8 id, Uint8 on);
extern char TDEC_get_layer_id(SDL_Surface *s);

#endif
