#ifndef __DEMOFW_H__
#define __DEMOFW_H__
#include "demoutil.h"

extern unsigned int time;		// In microseconds
extern unsigned int time_base;	// In microseconds

void demo_main(const char *title, int width, int height,
				float view_x, float view_y, float view_z, float fps);
void demo_destroy(void);
void demo_perspective(bool persp);

#define virtual
virtual void render(void);
virtual void init(void);
virtual void key_press(byte key, int x, int y);
virtual void special_press(int key, int x, int y);
#undef virtual

/*
   Map value in range [SF, ST] to [DS, DT]
MAP:			Linear mapping
MAP_SMOOTH:		Sine mapping, smooth in and smooth out
MAP_SMOOTH_IN:	Sine mapping, smooth in only
MAP_SMOOTH_OUT:	Sine mapping, smooth out only
 */
#define MAP(V,SF,ST,DS,DT)\
	((float)((V) - (SF)) * ((DT) - (DS)) / ((ST) - (SF)) + (DS))
#define MAP_SMOOTH(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, -PI/2.0, PI/2.0)), -1, 1, DS, DT)
#define MAP_SMOOTH_IN(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, -PI/2.0, 0)), -1, 0, DS, DT)
#define MAP_SMOOTH_OUT(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, 0, PI/2.0)), 0, 1, DS, DT)

#endif

