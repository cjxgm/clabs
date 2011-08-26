
#include "backend_png.h"
#include "config.h"

cairo_surface_t * backend_create(void)
{
	return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE_W, SIZE_H);
}

void backend_refresh(cairo_surface_t * cs)
{
	cairo_surface_write_to_png(cs, FILENAME);
}

void backend_destroy(cairo_surface_t * cs)
{
	cairo_surface_destroy(cs);
}

