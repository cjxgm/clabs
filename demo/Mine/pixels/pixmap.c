
#include "pixmap.h"
#include "demolino.h"
#include <assert.h>
#include <stdio.h>

PIXMAP * pixmap_new(unsigned int w, unsigned int h)
{
	PIXMAP * pm = malloc(sizeof(PIXMAP)
				+ sizeof(unsigned int) * w * h);
	assert(pm);

	pm->w = w;
	pm->h = h;

	return pm;
}

void pixmap_draw(PIXMAP * pm, float x, float y, float w, float h)
{
	assert(pm);
	assert(pm->w);
	assert(pm->h);

	unsigned int u, v, clr;
	float x1, y1;
	float x2, y2;

	for (v=0; v<pm->h; v++)
		for (u=0; u<pm->w; u++) {
			x1 = MAP(u, 0, pm->w, x, x+w);
			y1 = MAP(v, 0, pm->h, y+h, y);
			x2 = MAP(u+1, 0, pm->w, x, x+w);
			y2 = MAP(v+1, 0, pm->h, y+h, y);
			clr = pm->data[v*pm->w + u];

			glColor3f((clr & 0xFF) / 255.0,
					((clr>>8) & 0xFF) / 255.0,
					((clr>>16) & 0xFF) / 255.0);
			glRectf(x1, y1, x2, y2);
		}
}

void pixmap_put(PIXMAP * pm, unsigned int x, unsigned int y, unsigned int clr)
{
	assert(pm);

	if (x < 0 || y < 0
		|| x >= pm->w || y >= pm->h)
		return;
	pm->data[y*pm->w + x] = clr;
}

