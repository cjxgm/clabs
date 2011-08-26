
#ifndef __BACKEND_X__
#define __BACKEND_X__

#include <cairo.h>

cairo_surface_t * backend_create(void);
void backend_mainloop(void);
void backend_set_exposure(void (*expf)(void));
void backend_destroy(cairo_surface_t * cs);

#endif

