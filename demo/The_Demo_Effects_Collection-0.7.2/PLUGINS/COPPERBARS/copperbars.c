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

/* for a correct use of libtool, global symbols are prefixed with <modulename>_LTX_ and internal symbols are prefixed with _<modulename>_ */

#include <math.h>
#include "layers.h"
#include "copperbars.h"

#define _copper_ _copperbars_

static SDL_Color _copper_copper[46];
static Uint16 _copper_aSin[360];
static SDL_Surface *_copper_surface;
static Uint16 _copper_red, _copper_red2, _copper_red3, _copper_red4, _copper_red5, _copper_red6, 
  _copper_red7, _copper_red8, _copper_white, _copper_white2, _copper_white3, _copper_white4, 
  _copper_white5, _copper_white6, _copper_white7, _copper_white8, _copper_blue, _copper_blue2, 
  _copper_blue3, _copper_blue4, _copper_blue5, _copper_blue6, _copper_blue7, _copper_blue8;

static void _copper_init_copper(void)
{
  _copper_red = 96; 
  _copper_red2 = 0;
  _copper_red3 = 88;
  _copper_red4 = 0;
  _copper_red5 = 80;
  _copper_red6 = 0;
  _copper_red7 = 72;
  _copper_red8 = 0;
  _copper_white = 64;
  _copper_white2 = 0;
  _copper_white3 = 56;
  _copper_white4 = 0;
  _copper_white5 = 48;
  _copper_white6 = 0;
  _copper_white7 = 40;
  _copper_white8 = 0;
  _copper_blue = 32;
  _copper_blue2 = 0;
  _copper_blue3 = 24;
  _copper_blue4 = 0;
  _copper_blue5 = 16;
  _copper_blue6 = 0;
  _copper_blue7 = 8;
  _copper_blue8 = 0;

/* red copper bar */

  _copper_copper[1].r = 0x22;
  _copper_copper[2].r = 0x44;
  _copper_copper[3].r = 0x66;
  _copper_copper[4].r = 0x88;
  _copper_copper[5].r = 0xaa;
  _copper_copper[6].r = 0xcc;
  _copper_copper[7].r = 0xee;
  _copper_copper[8].r = 0xff;
  _copper_copper[9].r = 0xee;
  _copper_copper[10].r = 0xcc;
  _copper_copper[11].r = 0xaa;
  _copper_copper[12].r = 0x88;
  _copper_copper[13].r = 0x66;
  _copper_copper[14].r = 0x44;
  _copper_copper[15].r = 0x22;

 /* white copper bar */

  _copper_copper[16].r = 0x22;
  _copper_copper[16].g = _copper_copper[16].r;
  _copper_copper[16].b = _copper_copper[16].g;
  _copper_copper[17].r = 0x44;
  _copper_copper[17].g = _copper_copper[17].r;
  _copper_copper[17].b = _copper_copper[17].g;  
  _copper_copper[18].r = 0x66;
  _copper_copper[18].g = _copper_copper[18].r;
  _copper_copper[18].b = _copper_copper[18].g;
  _copper_copper[19].r = 0x88;
  _copper_copper[19].g = _copper_copper[19].r;
  _copper_copper[19].b = _copper_copper[19].g;
  _copper_copper[20].r = 0xaa;
  _copper_copper[20].g = _copper_copper[20].r;
  _copper_copper[20].b = _copper_copper[20].g;
  _copper_copper[21].r = 0xcc;
  _copper_copper[21].g = _copper_copper[21].r;
  _copper_copper[21].b = _copper_copper[21].g;
  _copper_copper[22].r = 0xee;
  _copper_copper[22].g = _copper_copper[22].r;
  _copper_copper[22].b = _copper_copper[22].g;
  _copper_copper[23].r = 0xff;
  _copper_copper[23].g = _copper_copper[23].r;
  _copper_copper[23].b = _copper_copper[23].g;
  _copper_copper[24].r = 0xee;
  _copper_copper[24].g = _copper_copper[24].r;
  _copper_copper[24].b = _copper_copper[24].g;
  _copper_copper[25].r = 0xcc;
  _copper_copper[25].g = _copper_copper[25].r;
  _copper_copper[25].b = _copper_copper[25].g;
  _copper_copper[26].r = 0xaa;
  _copper_copper[26].g = _copper_copper[26].r;
  _copper_copper[26].b = _copper_copper[26].g;
  _copper_copper[27].r = 0x88;
  _copper_copper[27].g = _copper_copper[27].r;
  _copper_copper[27].b = _copper_copper[27].g;
  _copper_copper[28].r = 0x66;
  _copper_copper[28].g = _copper_copper[28].r;
  _copper_copper[28].b = _copper_copper[28].g;
  _copper_copper[29].r = 0x44;
  _copper_copper[29].g = _copper_copper[29].r;
  _copper_copper[29].b = _copper_copper[29].g;
  _copper_copper[30].r = 0x22;
  _copper_copper[30].g = _copper_copper[30].r;
  _copper_copper[30].b = _copper_copper[30].g;

 /* blue copper bar */

  _copper_copper[31].b = 0x22;
  _copper_copper[32].b = 0x44;
  _copper_copper[33].b = 0x66;
  _copper_copper[34].b = 0x88;
  _copper_copper[35].b = 0xaa;
  _copper_copper[36].b = 0xcc;
  _copper_copper[37].b = 0xee;
  _copper_copper[38].b = 0xff;
  _copper_copper[39].b = 0xee;
  _copper_copper[40].b = 0xcc;
  _copper_copper[41].b = 0xaa;
  _copper_copper[42].b = 0x88;
  _copper_copper[43].b = 0x66;
  _copper_copper[44].b = 0x44;
  _copper_copper[45].b = 0x22;
}

void copperbars_LTX_init_effect_valist(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  int i, centery;
  float rad;
  Uint16 sine_amplitude;

  _copper_surface = s;

  sine_amplitude = (Uint16)va_arg(parameters, int);

  centery = _copper_surface->h >> 1;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _copper_aSin[i] = centery + (sin(rad) * (float)sine_amplitude);
    }

  _copper_init_copper();
	
  if (_copper_surface->format->palette)
    {
      SDL_SetPalette(_copper_surface, SDL_LOGPAL | SDL_PHYSPAL, _copper_copper, 0, 46); 
    }
}

void copperbars_LTX_init_effect(SDL_Surface *s, void (*restart)(void), TDEC_NODE *argument_list)
{
  int i, centery;
  float rad;
  Uint16 sine_amplitude;

  _copper_surface = s;
 
  sine_amplitude = *(Uint16*)TDEC_LIST_get_data_next(&argument_list);
  
  centery = _copper_surface->h >> 1;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      _copper_aSin[i] = centery + (sin(rad) * (float)sine_amplitude);
    }
	
  _copper_init_copper();
	  
  if (_copper_surface->format->palette)
    {
      SDL_SetPalette(_copper_surface, SDL_LOGPAL | SDL_PHYSPAL, _copper_copper, 0, 46); 
    }	
}

static void _copper_draw(SDL_Rect* r, Uint8 add)
{
  Uint8 i;
  
  for (i = 0; i < 15; ++i)
    {
      SDL_FillRect(_copper_surface, r, 
		   SDL_MapRGB(_copper_surface->format, _copper_copper[i + add].r, _copper_copper[i + add].g, _copper_copper[i + add].b));
      r->y++;
    }
}

void copperbars_LTX_draw_effect(void)
{
  SDL_Rect drect, crect;

  drect.x = 0;
  drect.w = _copper_surface->w;
  drect.h = 1;
  
  crect.x = 0;
  crect.w = _copper_surface->h;
  crect.h = 15;

  SDL_FillRect(_copper_surface, 0, SDL_MapRGB(_copper_surface->format, 0, 0, 0));
  
  /*draw copperbars back to front*/
  
  drect.y = _copper_aSin[_copper_blue7];
  _copper_blue8 = drect.y;
  _copper_blue7 += 2;
  _copper_blue7 %= 360;
  
  _copper_draw(&drect, 31);
  
  drect.y = _copper_aSin[_copper_blue5];
  _copper_blue6 = drect.y;
  _copper_blue5 += 2;
  _copper_blue5 %= 360;
  
  _copper_draw(&drect, 31);
  
  drect.y = _copper_aSin[_copper_blue3];
  _copper_blue4 = drect.y;
  _copper_blue3 += 2;
  _copper_blue3 %= 360;
  
  _copper_draw(&drect, 31);
  
  drect.y = _copper_aSin[_copper_blue];
  _copper_blue2 = drect.y;
  _copper_blue += 2;
  _copper_blue %= 360;
  
  _copper_draw(&drect, 31);
  
  drect.y = _copper_aSin[_copper_white7];
  _copper_white8 = drect.y;
  _copper_white7 += 2;
  _copper_white7 %= 360;
  
  _copper_draw(&drect, 16);
  
  drect.y = _copper_aSin[_copper_white5];
  _copper_white6 = drect.y;
  _copper_white5 += 2;
  _copper_white5 %= 360;
  
  _copper_draw(&drect, 16);
  
  drect.y = _copper_aSin[_copper_white3];
  _copper_white4 = drect.y;
  _copper_white3 += 2;
  _copper_white3 %= 360;
  
  _copper_draw(&drect, 16);
  
  drect.y = _copper_aSin[_copper_white];
  _copper_white2 = drect.y;
  _copper_white += 2;
  _copper_white %= 360;
  
  _copper_draw(&drect, 16);
  
  drect.y = _copper_aSin[_copper_red7];
  _copper_red8 = drect.y;
  _copper_red7 += 2;
  _copper_red7 %= 360;
  
  _copper_draw(&drect, 1);
  
  drect.y = _copper_aSin[_copper_red5];
  _copper_red6 = drect.y;
  _copper_red5 += 2;
  _copper_red5 %= 360;
  
  _copper_draw(&drect, 1);
  
  drect.y = _copper_aSin[_copper_red3];
  _copper_red4 = drect.y;
  _copper_red3 += 2;
  _copper_red3 %= 360;
  
  _copper_draw(&drect, 1);
  
  drect.y = _copper_aSin[_copper_red];
  _copper_red2 = drect.y;
  _copper_red += 2;
  _copper_red %= 360;
  
  _copper_draw(&drect, 1);
}


void copperbars_LTX_free_effect(void)
{
}

Uint8 copperbars_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
