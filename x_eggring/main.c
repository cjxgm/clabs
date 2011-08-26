
#include <cairo.h>
#include <stdio.h>
#include <unistd.h>
#include "backend_x.h"
#include "paint.h"

static cairo_surface_t * cs;

void exposure_cb(void)
{
	paint(cs);
}

int main(void)
{
	cs = backend_create();
	backend_set_exposure(&exposure_cb);
	backend_mainloop();
	backend_destroy(cs);

	return 0;
}

