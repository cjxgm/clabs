
#include "backend_x.h"
#include "config.h"
#include <X11/Xlib.h>
#include <cairo-xlib.h>
#include <stdio.h>
#include <stdlib.h>

static Display * dpy;
static void (*exposure)(void);

cairo_surface_t * backend_create(void)
{
	Window root;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display.\n");
		exit(1);
	}

	root = DefaultRootWindow(dpy);
	XSelectInput(dpy, root, ExposureMask);

	return cairo_xlib_surface_create(dpy, root,
			DefaultVisual(dpy, 0), SIZE_W, SIZE_H);
}

void backend_mainloop(void)
{
	XEvent e;

	while(1) {
		XNextEvent(dpy, &e);
		if(e.type == Expose)
			exposure();
		else
			fprintf(stderr, "%d\n", e.type);
	}

}

void backend_set_exposure(void (*expf)(void))
{
	exposure = expf;
}

void backend_destroy(cairo_surface_t * cs)
{
	cairo_surface_destroy(cs);
	XCloseDisplay(dpy);
}

