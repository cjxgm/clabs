
#ifndef __PIXMAP_H__
#define __PIXMAP_H__

#include <stdlib.h>

#ifndef NEW
#define NEW(T)		malloc(sizeof(T))
#endif

typedef struct _pixmap
{
	unsigned int	w, h;
	unsigned int	data[0];
}
PIXMAP;

#define PIXMAP_RGB(R,G,B)		(((int)(R))&0xFF)\
								| ((((int)(G))&0xFF)<<8)\
								| ((((int)(B))&0xFF)<<16)

PIXMAP * pixmap_new(unsigned int w, unsigned int h);
#define pixmap_free(pm)			free(pm);
void pixmap_put(PIXMAP * pm, unsigned int x, unsigned int y, unsigned int clr);
void pixmap_draw(PIXMAP * pm, float x, float y, float w, float h);
#define pixmap_draw_full(pm)	pixmap_draw(pm, -100, -100, 200, 200);

#endif

