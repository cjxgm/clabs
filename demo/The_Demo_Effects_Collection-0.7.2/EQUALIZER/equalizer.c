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

/* Mikmod has no easy way (that I know of) to detect the start of a sample in a voice, do you know a better way? */
/* A spectrum analyser would be nice */
/* note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mikmod.h"
#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 110
#define MAX_WIDTH (SCREEN_WIDTH - 80)
#define MIN_WIDTH 20
#define RECT_WIDTH 80
#define MAX_HEIGHT 120
#define DECAY 8
#define TIPSIZE 40

typedef struct
{
  int freq;
  int vol;
  int realvol;
  int width;
} CH;

static SDL_Color copper[76];
static MODULE* module;
static CH channel1 = {0, 0, 0, MIN_WIDTH};
static CH channel2 = {0, 0, 0, MIN_WIDTH};
static CH channel3 = {0, 0, 0, MIN_WIDTH};
static CH channel4 = {0, 0, 0, MIN_WIDTH};

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  Player_Stop();
  Player_Free(module);
  MikMod_Exit();


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
  Uint8 i;

  /* register all the drivers */
  MikMod_RegisterAllDrivers();
     
  /* register all the module loaders */
  MikMod_RegisterAllLoaders();
     
  /* initialize the library */
  md_mixfreq = 22100;
  if (MikMod_Init("")) {
    fprintf(stderr, "Could not initialize sound, reason: %s\n",
	    MikMod_strerror(MikMod_errno));
    return;
  }
     
  /* load module */
  module = Player_Load("../SOUND/MODS/tuneforit.mod", 64, 0);
  if (!module)
    {
      printf("Error loading module!\n");
      quit(1);
    }

  module->wrap = 1; /* make it loop forever */

  copper[0].r = copper[0].g = copper[0].b = 0x00;

  /* green copperbar */
  copper[1].g = 0x22;
  copper[2].g = 0x44;
  copper[3].g = 0x66;
  copper[4].g = 0x88;
  copper[5].g = 0xaa;
  copper[6].g = 0xcc;
  copper[7].g = 0xee;
  copper[8].g = 0xff;
  copper[9].g = 0xee;
  copper[10].g = 0xcc;
  copper[11].g = 0xaa;
  copper[12].g = 0x88;
  copper[13].g = 0x66;
  copper[14].g = 0x44;
  copper[15].g = 0x22;

 /* white copper bar */

  copper[16].r = 0x22;
  copper[16].g = copper[16].r;
  copper[16].b = copper[16].g;
  copper[17].r = 0x44;
  copper[17].g = copper[17].r;
  copper[17].b = copper[17].g;  
  copper[18].r = 0x66;
  copper[18].g = copper[18].r;
  copper[18].b = copper[18].g;
  copper[19].r = 0x88;
  copper[19].g = copper[19].r;
  copper[19].b = copper[19].g;
  copper[20].r = 0xaa;
  copper[20].g = copper[20].r;
  copper[20].b = copper[20].g;
  copper[21].r = 0xcc;
  copper[21].g = copper[21].r;
  copper[21].b = copper[21].g;
  copper[22].r = 0xee;
  copper[22].g = copper[22].r;
  copper[22].b = copper[22].g;
  copper[23].r = 0xff;
  copper[23].g = copper[23].r;
  copper[23].b = copper[23].g;
  copper[24].r = 0xee;
  copper[24].g = copper[24].r;
  copper[24].b = copper[24].g;
  copper[25].r = 0xcc;
  copper[25].g = copper[25].r;
  copper[25].b = copper[25].g;
  copper[26].r = 0xaa;
  copper[26].g = copper[26].r;
  copper[26].b = copper[26].g;
  copper[27].r = 0x88;
  copper[27].g = copper[27].r;
  copper[27].b = copper[27].g;
  copper[28].r = 0x66;
  copper[28].g = copper[28].r;
  copper[28].b = copper[28].g;
  copper[29].r = 0x44;
  copper[29].g = copper[29].r;
  copper[29].b = copper[29].g;
  copper[30].r = 0x22;
  copper[30].g = copper[30].r;
  copper[30].b = copper[30].g;

  /* red copperbar */
  copper[31].r = 0x22;
  copper[32].r = 0x44;
  copper[33].r = 0x66;
  copper[34].r = 0x88;
  copper[35].r = 0xaa;
  copper[36].r = 0xcc;
  copper[37].r = 0xee;
  copper[38].r = 0xff;
  copper[39].r = 0xee;
  copper[40].r = 0xcc;
  copper[41].r = 0xaa;
  copper[42].r = 0x88;
  copper[43].r = 0x66;
  copper[44].r = 0x44;
  copper[45].r = 0x22;

 /* yellow copperbar */
  copper[46].r = copper[46].g = 0x22;
  copper[47].r = copper[47].g = 0x44;
  copper[48].r = copper[48].g = 0x66;
  copper[49].r = copper[49].g = 0x88;
  copper[50].r = copper[50].g = 0xaa;
  copper[51].r = copper[51].g = 0xcc;
  copper[52].r = copper[52].g = 0xee;
  copper[53].r = copper[53].g = 0xff;
  copper[54].r = copper[54].g = 0xee;
  copper[55].r = copper[55].g = 0xcc;
  copper[56].r = copper[56].g = 0xaa;
  copper[57].r = copper[57].g = 0x88;
  copper[58].r = copper[58].g = 0x66;
  copper[59].r = copper[59].g = 0x44;
  copper[60].r = copper[60].g = 0x22;

 /* blue copper bar */
  copper[61].b = 0x22;
  copper[62].b = 0x44;
  copper[63].b = 0x66;
  copper[64].b = 0x88;
  copper[65].b = 0xaa;
  copper[66].b = 0xcc;
  copper[67].b = 0xee;
  copper[68].b = 0xff;
  copper[69].b = 0xee;
  copper[70].b = 0xcc;
  copper[71].b = 0xaa;
  copper[72].b = 0x88;
  copper[73].b = 0x66;
  copper[74].b = 0x44;
  copper[75].b = 0x22;

  SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, copper, 0, 76); 

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

void draw_copper(SDL_Rect r, int add)
{
  int i;
  
  for (i = 0; i < 15; i++)
    {
      SDL_FillRect(screen, &r, i + add);
      r.y++;
    }
}

unsigned short calc_size(SBYTE voice, CH* channel)
{
  int freq = Voice_GetFrequency(voice);
  int vol = Voice_GetVolume(voice);
  int realvol = Voice_RealVolume(voice);

  if (freq != channel->freq || vol != channel->vol || realvol > channel->realvol)
    {
      channel->width = MAX_WIDTH;
    }
  else
    {
    channel->width -= DECAY;
    if (channel->width < MIN_WIDTH)
      channel->width = MIN_WIDTH;
    }  

  channel->vol = vol;
  channel->freq = freq;
  channel->realvol = realvol;
  
  return channel->width;
}

int main( int argc, char* argv[] )
{
 SDL_Rect drect, rrect, copy1, copy2, copy3, copy4;

  if (argc > 1)
    {
      printf("Retro Equalizer - W.P. van Paassen - 2002\n");
      return -1;
    }

  if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWSURFACE | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
   quit(1);
  TDEC_init_timer();

 init();

 SDL_WM_SetCaption("Retro - Equalizer - ", "");

 drect.h = 1;
 rrect.w = RECT_WIDTH;
  
 /* start module */
 Player_Start(module);

 /* time based demo loop */
 while( 1 ) 
   {
      TDEC_new_time();
      
      process_events();

      SDL_FillRect(screen, 0, 0);

      drect.y = 40;
      drect.w = calc_size(0, &channel1);
      drect.x = 0;
      copy1 = drect;
      rrect.h = drect.w;
      rrect.y = SCREEN_HEIGHT - (rrect.h >> 2); 
      drect.x = drect.w;
      drect.w = TIPSIZE;
      copy2 = drect;
      rrect.x = 50;
      SDL_FillRect(screen, &rrect, 14);

      drect.y = 88;
      drect.w = calc_size(3, &channel4);
      drect.x = SCREEN_WIDTH - drect.w;
      copy3 = drect;
      rrect.h = drect.w;
      rrect.y = SCREEN_HEIGHT - (rrect.h >> 2);
      drect.x = SCREEN_WIDTH - drect.w - TIPSIZE;
      drect.w = TIPSIZE;
      copy4 = drect;
      rrect.x = 350;
      SDL_FillRect(screen, &rrect, 14);

      draw_copper(copy1, 31);
      draw_copper(copy3, 46);
      draw_copper(copy2, 16);
      draw_copper(copy4, 16);
   
      drect.y = 56;
      drect.w = calc_size(1, &channel2);
      drect.x = SCREEN_WIDTH - drect.w;
      copy1 = drect;   
      rrect.h = drect.w;
      rrect.y = SCREEN_HEIGHT - (rrect.h >> 2);
      drect.x = SCREEN_WIDTH - drect.w - TIPSIZE;
      drect.w = TIPSIZE;
      copy2 = drect;
      rrect.x = 150;
      SDL_FillRect(screen, &rrect, 14);
   
      drect.y = 72;
      drect.w = calc_size(2, &channel3);
      drect.x = 0;
      copy3 = drect;
      rrect.h = drect.w;
      rrect.y = SCREEN_HEIGHT - (rrect.h >> 2);
      drect.x = drect.w;
      drect.w = TIPSIZE;
      copy4 = drect;
      rrect.x = 250;
      SDL_FillRect(screen, &rrect, 14);  
     
      draw_copper(copy1, 1);
      draw_copper(copy3, 61);  
      draw_copper(copy2, 16);
      draw_copper(copy4, 16);
      
      if (TDEC_fps_ok())
	{
	  SDL_UpdateRect(screen, 0, 0, 0, 0);	
	  MikMod_Update();	
	}
    }
  
  return 0; /* never reached */
}
