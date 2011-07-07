#ifndef __DEMOFW_H__
#define __DEMOFW_H__
#include "demoutil.h"

void demo_main(const char *title, int width, int height,
				float view_x, float view_y, float view_z, float fps);
void demo_destroy(void);
void demo_perspective(bool persp);

#define virtual
virtual void timer(int value);
virtual void render(void);
virtual void init(void);
virtual void keypress(byte key, int x, int y);
#undef virtual

#endif

