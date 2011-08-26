
#ifndef __BACKEND_PNG__
#define __BACKEND_PNG__

#include <cairo.h>

cairo_surface_t * backend_create(void);
void backend_refresh(cairo_surface_t * cs);
void backend_destroy(cairo_surface_t * cs);

#endif

