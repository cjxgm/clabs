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

#include <stdlib.h>
#include <stdarg.h>

#include "ltdl.h"
#include "layers.h"
#include "video.h"
#include "utils.h"

typedef struct
{
  void (*init_valist)(SDL_Surface *s, void (*restart)(void), va_list parameters);
  void (*init)(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list);
  void (*draw)(void);
  void (*free)(void);
  Uint8 (*is_filter)(void);
  lt_dlhandle effect_module;
} EFFECT;

typedef struct
{
  SDL_Surface* surface;
  SDL_Rect r;
  Uint8 alpha;
  EFFECT *effect;
  Uint8 visible;
} LAYER;

static LAYER* layers[NLAYERS];
static Uint8 nlayers = 0;

int TDEC_init_layer_system()
{
  nlayers = 0;
  return lt_dlinit();
}
int TDEC_exit_layer_system()
{
  TDEC_free_layers();
  return lt_dlexit();
}

void TDEC_free_layers(void)
{
  Uint8 i;
  
  for (i = 0; i < nlayers; ++i)
    {
      (*layers[i]->effect->free)();
      if ((lt_dlclose(layers[i]->effect->effect_module)) != 0)
      {
	printf("TDEC: Error closing plugin\n");
      }
      free(layers[i]->effect);
      if (layers[i]->surface != screen)
	{
	  SDL_FreeSurface(layers[i]->surface);
	}
      free(layers[i]);
      layers[i] = (LAYER*)0;
    }
  nlayers = 0;
}

SDL_Surface* TDEC_get_background_layer(void)
{
  return layers[TDEC_BACKGROUND_LAYER]->surface;
}

char TDEC_add_effect_valist(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha,
			    const char *module, void (*restart_callback)(void), ...)
{
  char res = -1;
  va_list parameters;
  EFFECT *effect;
  Uint8 is_filter;

  /* dynamically load effects-plugin, attach it to a layer and init it */

  effect = (EFFECT*)malloc(sizeof(EFFECT));

  effect->effect_module = lt_dlopenext(module);
  if (!effect->effect_module)
    {
      printf("error, unable to load effects plugin %s\n", module);
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }
  
  effect->init_valist = (void(*)(SDL_Surface*, void(*)(void), va_list))lt_dlsym(effect->effect_module, "init_effect_valist");
  if (!effect->init_valist)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->init = (void(*)(SDL_Surface*, void(*)(void), TDEC_NODE *argument_list))0;

  effect->draw = (void(*)(void))lt_dlsym(effect->effect_module, "draw_effect");
  if (!effect->draw)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->free = (void(*)(void))lt_dlsym(effect->effect_module, "free_effect");
  if (!effect->free)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->is_filter = (Uint8(*)(void))lt_dlsym(effect->effect_module,"is_filter");
  if (!effect->is_filter)
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  /* check if effects module is a filter by invoking its is_filter function and checking the result */

  is_filter = (*effect->is_filter)();
  
  if (nlayers + 1 <= NLAYERS && nlayers != 0)
    {
      LAYER *l = (LAYER*)malloc(sizeof(LAYER));

      if (is_filter == TDEC_NO_FILTER)
	{
	  SDL_Surface *s = SDL_CreateRGBSurface(screen->flags, width, height, screen->format->BitsPerPixel, 
					    screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, 
					    screen->format->Amask);
 
	  l->surface = SDL_ConvertSurface(s, screen->format, screen->flags);

	  /* set palette if any */
	  if (l->surface->format->palette)
	    {
	      SDL_SetPalette(l->surface, SDL_LOGPAL | SDL_PHYSPAL, l->surface->format->palette->colors, 0, 
			     l->surface->format->palette->ncolors);
	    }
	  
	  /* set transparant pixel which is black */
	  SDL_SetColorKey(l->surface, SDL_SRCCOLORKEY/* | SDL_RLEACCEL*/, SDL_MapRGBA(l->surface->format, 0, 0, 0, 0xFF)); 

	  /* set surface alpha value if appropriate*/
	  l->alpha = alpha;
	  if (alpha != 0xFF)
	    {
	      SDL_SetAlpha(l->surface, SDL_SRCALPHA/* | SDL_RLEACCEL*/, alpha);
	    }

#ifdef DEBUG
	  printf("Added layer %i\n", nlayers);
#endif
	  SDL_FreeSurface(s);
	}
      else
	{
	  /* effect is a filter, for instance a blur or a lens or a transition effect, so use background as surface */
	  l->surface = TDEC_get_background_layer();
	}
      
      /* set surface dimensions and position */
      l->r.x = xstart;
      l->r.y = ystart;
      l->r.w = width;
      l->r.h = height;
      l->visible = 1;
      layers[nlayers] = l;

      res = nlayers;

      layers[nlayers++]->effect = effect;

      va_start(parameters, restart_callback);

      /*init effects plugin */
      (*effect->init_valist)(l->surface, restart_callback, parameters);

      va_end(parameters);
    }
  else if (nlayers == 0)
    {
      LAYER *background;

      /* first layer so init layering system, the first effect can also be a filter but we've got to create a background surface anyway */

      /* check if video was set */
      if (!screen)
	{
	  printf("error, video not set, call TDEC_set_video first please\n");
	  return -1;
	}

      /* screen is the display buffer and therefore the background */
  
      background = (LAYER*)malloc(sizeof(LAYER));
      background->surface = screen;
      background->r.x = xstart;
      background->r.y = ystart;
      background->r.w = width;
      background->r.h = height;
      background->alpha = 0xFF;
      background->visible = 1;
      res = TDEC_BACKGROUND_LAYER;

      background->effect = effect;
      layers[TDEC_BACKGROUND_LAYER] = background;

      va_start(parameters, restart_callback);
	  
      /*init effects plugin */
      (*effect->init_valist)(background->surface, restart_callback, parameters);

      va_end(parameters);
      
      nlayers = 1;
    }

  return res;
}

char TDEC_add_effect(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha,
			    const char *module, void (*restart_callback)(void), TDEC_NODE *argument_list)
{
  char res = -1;
  EFFECT *effect;
  Uint8 is_filter;

  /* dynamically load effects-plugin, attach it to a layer and init it */

  effect = (EFFECT*)malloc(sizeof(EFFECT));

  effect->effect_module = lt_dlopenext(module);
  if (!effect->effect_module)
    {
      printf("error, unable to load effects plugin %s\n", module);
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }
  
  effect->init = (void(*)(SDL_Surface*, void(*)(void), TDEC_NODE *argument_list))lt_dlsym(effect->effect_module, "init_effect");
  if (!effect->init)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->init_valist = (void(*)(SDL_Surface*, void(*)(void), va_list))0;

  effect->draw = (void(*)(void))lt_dlsym(effect->effect_module, "draw_effect");
  if (!effect->draw)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->free = (void(*)(void))lt_dlsym(effect->effect_module, "free_effect");
  if (!effect->free)  
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  effect->is_filter = (Uint8(*)(void))lt_dlsym(effect->effect_module,"is_filter");
  if (!effect->is_filter)
    {
      fprintf (stderr, "%s\n", lt_dlerror());
      return -1;
    }

  /* check if effects module is a filter by invoking its is_filter function and checking the result */

  is_filter = (*effect->is_filter)();
  
  if (nlayers + 1 <= NLAYERS && nlayers != 0)
    {
      LAYER *l = (LAYER*)malloc(sizeof(LAYER));

      if (is_filter == TDEC_NO_FILTER)
	{
	  SDL_Surface *s = SDL_CreateRGBSurface(screen->flags, width, height, screen->format->BitsPerPixel, 
					    screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, 
					    screen->format->Amask);
 
	  l->surface = SDL_ConvertSurface(s, screen->format, screen->flags);

	  /* set palette if any */
	  if (l->surface->format->palette)
	    {
	      SDL_SetPalette(l->surface, SDL_LOGPAL | SDL_PHYSPAL, l->surface->format->palette->colors, 0, 
			     l->surface->format->palette->ncolors);
	    }
	  
	  /* set transparant pixel which is black */
	  SDL_SetColorKey(l->surface, SDL_SRCCOLORKEY/* | SDL_RLEACCEL*/, SDL_MapRGBA(l->surface->format, 0, 0, 0, 0xFF)); 

	  /* set surface alpha value if appropriate*/
	  l->alpha = alpha;
	  if (alpha != 0xFF)
	    {
	      SDL_SetAlpha(l->surface, SDL_SRCALPHA/* | SDL_RLEACCEL*/, alpha);
	    }

#ifdef DEBUG
	  printf("Added layer %i\n", nlayers);
#endif
	  SDL_FreeSurface(s);
	}
      else
	{
	  /* effect is a filter, for instance a blur or a lens or a transition effect, so use background as surface */
	  l->surface = TDEC_get_background_layer();
	}
      
      /* set surface dimensions and position */
      l->r.x = xstart;
      l->r.y = ystart;
      l->r.w = width;
      l->r.h = height;
      l->visible = 1;
      layers[nlayers] = l;

      res = nlayers;

      layers[nlayers++]->effect = effect;

      /*init effects plugin */
      (*effect->init)(l->surface, restart_callback, argument_list);
    }
  else if (nlayers == 0)
    {
      LAYER *background;

      /* first layer so init layering system, the first effect can also be a filter but we've got to create a background surface anyway */

      /* check if video was set */
      if (!screen)
	{
	  printf("error, video not set, call TDEC_set_video first please\n");
	  return -1;
	}

      /* screen is the display buffer and therefore the background */
  
      background = (LAYER*)malloc(sizeof(LAYER));
      background->surface = screen;
      background->r.x = xstart;
      background->r.y = ystart;
      background->r.w = width;
      background->r.h = height;
      background->alpha = 0xFF;
      background->visible = 1;
      res = TDEC_BACKGROUND_LAYER;

      background->effect = effect;
      layers[TDEC_BACKGROUND_LAYER] = background;

      /*init effects plugin */
      (*effect->init)(background->surface, restart_callback, argument_list);

      nlayers = 1;
    }

  return res;
}

/* draw each effect attached to every surface and flatten layer*/
void TDEC_draw_layers(void)
{
  Uint8 i;

  if (layers[TDEC_BACKGROUND_LAYER]->visible)
    {
      (layers[TDEC_BACKGROUND_LAYER]->effect->draw)();
    }
  else
    {
      TDEC_clear_layer(TDEC_BACKGROUND_LAYER);
    }

  for (i = 1; i < nlayers; ++i)
    {
      if (layers[i]->visible)
	{
	  (*layers[i]->effect->draw)();
	  if (layers[i]->surface != screen)
	    {
	      SDL_BlitSurface(layers[i]->surface, 0, screen, &layers[i]->r);
	    }
	}
    }
  SDL_Flip(screen);
}

void TDEC_set_layer_colorkey(Uint8 id, Uint8 on)
{
  if (on)
    {
      SDL_SetColorKey(layers[id]->surface, SDL_SRCCOLORKEY/* | SDL_RLEACCEL*/, SDL_MapRGBA(layers[id]->surface->format, 0, 0, 0, 0xFF)); 
    }
  else
    {
      SDL_SetColorKey(layers[id]->surface, 0, SDL_MapRGBA(layers[id]->surface->format, 0, 0, 0, 0xFF)); 
    }
}

char TDEC_get_layer_id(SDL_Surface *s)
{
  Uint8 i;
  for (i = 0; i < nlayers; ++i)
    {
      if (layers[i]->surface == s)
	{
	  return i;
	}
    }
  return -1; /* no such surface in a layer */
}

SDL_Surface* TDEC_get_layer(Uint8 index)
{
  if (index < nlayers)
    {
	  return (layers[index])->surface;
    }
  return (SDL_Surface*)0;
}

void TDEC_flatten_layers()
{
  if (nlayers > 1)
    {
      Uint8 i;
      for (i = 1; i < nlayers; ++i)
	{
	  if (layers[i]->visible && layers[i]->surface != screen)
	    {
	      SDL_BlitSurface(layers[i]->surface, 0, screen, &layers[i]->r);
	    }
	}
    }
}

void TDEC_clear_layer(Uint8 id)
{
  SDL_Surface *s = layers[id]->surface;
  SDL_FillRect(s, 0, SDL_MapRGB(s->format, 0, 0, 0));
}

void TDEC_remove_layer(void)
{
  if (nlayers > 1)
    {
      (*layers[nlayers - 1]->effect->free)();
      lt_dlclose(layers[nlayers - 1]->effect->effect_module);
      free(layers[nlayers - 1]->effect);
      SDL_FreeSurface(layers[nlayers - 1]->surface);
      free(layers[nlayers - 1]);
      nlayers--;
#ifdef DEBUG
      printf("Removed layer %i\n", nlayers);
#endif
    }
}

void TDEC_enable_layer(Uint8 id)
{
  layers[id]->visible = 1;
}

void TDEC_disable_layer(Uint8 id)
{
  layers[id]->visible = 0;
}

void TDEC_set_layer_alpha(Uint8 id, Uint8 alpha)
{
  layers[id]->alpha = alpha;
  SDL_SetAlpha(layers[id]->surface, SDL_SRCALPHA/* | SDL_RLEACCEL*/, alpha);
}

Uint8 TDEC_get_layer_alpha(Uint8 id)
{
  return layers[id]->alpha;
}

