#ifndef __DEMOFW_H__
#define __DEMOFW_H__

void demo_main(const char *title, int width, int height, float fps);

#define virtual
virtual void timer(int value);
virtual void render(void);
virtual void init(void);
#undef virtual

#endif

