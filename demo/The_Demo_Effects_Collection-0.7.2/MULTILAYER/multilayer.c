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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

const Uint16 SCREEN_WIDTH = 480;
const Uint16 SCREEN_HEIGHT = 360;
const Uint8 FADE_FACTOR = 15;
static char fade;
static char copper;
static Uint8 change = 0;
static Uint32 which = 0;
static Uint8 quit_it = 0;

static void init();

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_Quit( );

  TDEC_exit_layer_system();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
	quit_it = 1;
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
  while( SDL_PollEvent( &event ) ) 
    {
      
      switch( event.type ) 
	{
	case SDL_KEYDOWN:
	  /* Handle key presses. */
	  handle_key_down( &event.key.keysym );
	  break;
	case SDL_QUIT:
	  /* Handle quit requests*/
	  quit_it = 1;
	  break;
	}
    }
}

/* effect callbacks */

void fade_in_ready()
{
  /* don't remove effects in the effect callbacks, do it after or before the call to TDEC_draw_layers */
  /* enabling and disabling effects is ok though */
  TDEC_disable_layer(fade);
}

void restart(void)
{
  /* don't remove effects in the effect callbacks, do it after or before the call to TDEC_draw_layers */
  /* enabling and disabling effects is ok though */
   change = 1;
}

void quit_all(void)
{
  /* don't remove effects in the effect callbacks, do it after or before the call to TDEC_draw_layers */
  /* enabling and disabling effects is ok though */
  quit(0);
}

/* effect inits */

void init_effects1()
{
  Uint8 j;
  TDEC_NODE *argument_list;
  char *text = " TdEc  tDeC  ";
  char *text2 = "Run-Time Pluggable Multi Effects and Filter system ";
  Uint8 w = 16, h = 32; 
  Uint16 height = 100, x = SCREEN_WIDTH, y = 100, amplitude = 40;
  
  /* this example uses a TDEC_LIST to pass effect parameters to the effect plugins 
	note that that the list is only a data container so no data is freed
	this means that when you malloc arguments and add them to the list, you've got to free them before destroying the list */

  j = 4; /* number of plasma pixels per color */	
  argument_list = TDEC_LIST_create(&j);
  if (TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/PLASMA/plasma", 
		      TDEC_NO_RESTART_CALLBACK, argument_list) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list); 

  argument_list = TDEC_LIST_create(&text2); /* create new argument list for jumpingscroller effects plugin */
  TDEC_LIST_append(argument_list, &TDEC_FONT1);
  TDEC_LIST_append(argument_list, &TDEC_FONT1_CHARACTERS);
  TDEC_LIST_append(argument_list, &w);
  TDEC_LIST_append(argument_list, &h);
  TDEC_LIST_append(argument_list, &x);
  TDEC_LIST_append(argument_list, &y);
 	
  if (TDEC_add_effect(SCREEN_WIDTH, 200, 0, SCREEN_HEIGHT - (100 + 32) , 0xFF, "../PLUGINS/JUMPINGSCROLLER/jumpingscroller",
		      TDEC_NO_RESTART_CALLBACK, argument_list) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list); 

  argument_list = TDEC_LIST_create(&SCREEN_WIDTH); /* create new argument list for fire effects plugin */
  height = 60;
  TDEC_LIST_append(argument_list, &height);
  x = y = 0;
  TDEC_LIST_append(argument_list, &x);
  TDEC_LIST_append(argument_list, &y);
 
  if (TDEC_add_effect(SCREEN_WIDTH, 60, 0, SCREEN_HEIGHT - 60, 0xFF, "../PLUGINS/FIRE/fire", 
		      TDEC_NO_RESTART_CALLBACK, argument_list) == -1)
    {
      exit(1);
    }

  TDEC_LIST_destroy(argument_list); 
	
  argument_list = TDEC_LIST_create(&text); /* create new argument list for sinescroller effects plugin */
  TDEC_LIST_append(argument_list, &TDEC_FONT1);
  TDEC_LIST_append(argument_list, &TDEC_FONT1_CHARACTERS);
  TDEC_LIST_append(argument_list, &w);
  TDEC_LIST_append(argument_list, &h);
  TDEC_LIST_append(argument_list, &amplitude);
  j = 2; /* number of pixels in sine */
  TDEC_LIST_append(argument_list, &j);
	
  if (TDEC_add_effect(SCREEN_WIDTH, 150, 0, 0, 0xFF, "../PLUGINS/SINESCROLLER/sinescroller",
		      &restart, argument_list) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list); 
	
  /* the lens effect's filter has no arguments at the current time */	
  if (TDEC_add_effect_valist(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/LENS/lens",
    TDEC_NO_RESTART_CALLBACK) == -1)
    {
      exit(1);
    }

	if (which == 0) /* only fade in first time */
	{
	  argument_list = TDEC_LIST_create(&TDEC_FADE_IN); /* create new argument list for fade effects plugin */
	  TDEC_LIST_append(argument_list, &FADE_FACTOR);
	  j = 0;
	  TDEC_LIST_append(argument_list, &j);
	  TDEC_LIST_append(argument_list, &j);
	  TDEC_LIST_append(argument_list, &j);
	 
	  if ((fade = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/FADE/fade",
					  &fade_in_ready, argument_list)) == -1)
		{
		  exit(1);
		}	
		
	 TDEC_LIST_destroy(argument_list); 	
	}
}

void init_effects2()
{
  TDEC_NODE *argument_list;
  char *text2 = "Run-Time Pluggable Multi Effects and Filter system ";
  Uint8 w = 16, h = 32; 
  Uint16 amplitude = 100, count = 750;

  /* this example uses a TDEC_LIST to pass effect parameters to the effect plugins 
	note that that the list is only a data container so no data is freed
	this means that when you malloc arguments and add them to the list, you've got to free them before destroying the list */
  
  argument_list = TDEC_LIST_create(&count); /* create new argument list for starfield effects plugin */
 	
  if (TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/STARFIELD/starfield",
		      TDEC_NO_RESTART_CALLBACK, argument_list) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list);
	
  argument_list = TDEC_LIST_create(&amplitude); /* create new argument list for copperbar effects plugin */	
  if ((copper = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0x80, "../PLUGINS/COPPERBARS/copperbars",
		      TDEC_NO_RESTART_CALLBACK, argument_list)) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list);
	
  /* the lens effect's filter has no arguments at the current time */	
  if (TDEC_add_effect_valist(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/LENS/lens",
    TDEC_NO_RESTART_CALLBACK) == -1)
    {
      exit(1);
    }
	
  argument_list = TDEC_LIST_create(&text2); /* create new argument list for circlescroller effects plugin */
  TDEC_LIST_append(argument_list, &TDEC_FONT1);
  TDEC_LIST_append(argument_list, &TDEC_FONT1_CHARACTERS);
  TDEC_LIST_append(argument_list, &w);
  TDEC_LIST_append(argument_list, &h);
  TDEC_LIST_append(argument_list, &amplitude);
 	
  if (TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/CIRCLESCROLLER/circlescroller",
				&restart, argument_list) == -1)
    {
      exit(1);
    }
	
  TDEC_LIST_destroy(argument_list);	
}


void init()
{
 Uint32 i;

  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i)
    {
      if (i != SDL_KEYDOWN && i != SDL_QUIT)
	{
	  SDL_EventState(i, SDL_IGNORE);
	}
    }
  
    SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
 if (argc > 1)
    {
      printf("Run-Time Pluggable Multi Effects and Filter system - W.P. van Paassen - 2003\n");
      return -1;
    }
 
 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | SDL_SRCALPHA/* |SDL_FULLSCREEN*/))
   quit(1);

 TDEC_init_timer();
 TDEC_set_fps(60);

 SDL_WM_SetCaption("Run-Time Pluggable Multi Effects and Filter system", "");

 TDEC_init_layer_system();
  
 init();
 init_effects1();
 
 while( 1 ) 
   {
     TDEC_new_time();
     
     process_events();
     
     if (quit_it)
       {
		    Uint8 i = 8, j = 0;
 			TDEC_NODE *argument_list = TDEC_LIST_create(&TDEC_INNER_OUTER_FILL); /* create new argument list for rectfill effects filter plugin */
 			TDEC_LIST_append(argument_list, &i);
  			TDEC_LIST_append(argument_list, &j);
  			TDEC_LIST_append(argument_list, &j);
  			TDEC_LIST_append(argument_list, &j);
 
  			if (TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/RECTFILL/rectfill",
				    &quit_all, argument_list) == -1)
    		{
      			exit(1);
    		}
	
 			TDEC_LIST_destroy(argument_list); 	
			quit_it = 0;
      }
     
     TDEC_draw_layers();
	 
	 /* this is the place to remove effects/layers, just after or before the TDEC_draw_layers call, not in the effects callbacks! */   
	   
	if ( change)
  	{
		change = 0;
		TDEC_free_layers(); /* instead of freeing all effects and adding new ones, it is also possible to load all effects at once and*/
							/* to use the TDEC_disable_layer and TDEC_enable_layer calls to enable/disable needed effects, */
							/* in this way effects won't be loaded and reinited again */
		if (which++ % 2)
		{
			init_effects1();
		}
		else
		{
			init_effects2();
		}
	}

	if (which % 2)
	{
     	TDEC_set_layer_alpha(copper, TDEC_get_layer_alpha(copper) + 1);
	}
	
    TDEC_fps_ok();
   }
}
