
#include "paint.h"
#include "config.h"

void paint(cairo_surface_t * cs)
{
	cairo_t * c = cairo_create(cs);
	cairo_rectangle(c, 10, 10, SIZE_W - 20, SIZE_H - 20);
	cairo_set_source_rgb(c, 0, 0, 0.5);
	cairo_fill(c);
}

