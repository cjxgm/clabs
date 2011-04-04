/*
 * lib.c - various routines
 */

/*
 * Copyright 2006 Johan Veenhuizen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "lib.h"
#include "widget.h"

void error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "karmen: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(ap);
}

void debug(const char *fmt, ...)
{
#if defined(DEBUG)
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "karmen: DEBUG: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(ap);
#endif
}

void putimage(Display *display, Drawable d, GC gc, IMAGE *image,
    int x, int y)
{
	if (image->pixmap == None)
		image->pixmap = XCreateBitmapFromData(display, d,
		    (char *)image->data, image->width, image->height);
	XCopyPlane(display, image->pixmap, d, gc,
	    0, 0, image->width, image->height, x, y, 1);
}

void drawraised(Drawable d, GC gc, struct color *color,
                int x, int y, int w, int h)
{
	XSetForeground(display, gc, color->bright2);
	XDrawLine(display, d, gc, x, y, x+w-3, y);
	XDrawLine(display, d, gc, x, y+1, x, y+h-3);

	XSetForeground(display, gc, BlackPixel(display, screen));
	XDrawLine(display, d, gc, x+w-1, y, x+w-1, y+h-1);
	XDrawLine(display, d, gc, x, y+h-1, x+w-2, y+h-1);

	XSetForeground(display, gc, color->shadow2);
	XDrawLine(display, d, gc, x+w-2, y+1, x+w-2, y+h-2);
	XDrawLine(display, d, gc, x+1, y+h-2, x+w-3, y+h-2);

	XSetForeground(display, gc, color->normal);
	XDrawPoint(display, d, gc, x+w-2, y);
	XDrawPoint(display, d, gc, x, y+h-2);
}

void drawlowered(Drawable d, GC gc, struct color *color,
                 int x, int y, int w, int h)
{
	XSetForeground(display, gc, color->shadow2);
	XDrawLine(display, d, gc, x, y, x+w-2, y);
	XDrawLine(display, d, gc, x, y+1, x, y+h-2);

	XSetForeground(display, gc, BlackPixel(display, screen));
	XDrawLine(display, d, gc, x+1, y+1, x+w-2, y+1);
	XDrawLine(display, d, gc, x+1, y+2, x+1, y+h-2);

	XSetForeground(display, gc, color->bright2);
	XDrawLine(display, d, gc, x+w-1, y+1, x+w-1, y+h-1);
	XDrawLine(display, d, gc, x+1, y+h-1, x+w-2, y+h-1);

	XSetForeground(display, gc, color->normal);
	XDrawPoint(display, d, gc, x+w-1, y);
	XDrawPoint(display, d, gc, x, x+h-1);
}

void drawdepressed(Drawable d, GC gc, struct color *color,
                   int x, int y, int w, int h)
{
	XSetForeground(display, gc, BlackPixel(display, screen));
	XDrawLine(display, d, gc, x, y, x+w-1, y);
	XDrawLine(display, d, gc, x, y+1, x, y+h-1);

	XSetForeground(display, gc, color->bright1);
	XDrawLine(display, d, gc, x+1, y+1, x+w-2, y+1);
	XDrawLine(display, d, gc, x+1, y+2, x+1, y+h-2);

	XSetForeground(display, gc, color->shadow2);
	XDrawLine(display, d, gc, x+w-1, y+2, x+w-1, y+h-1);
	XDrawLine(display, d, gc, x+2, y+h-1, x+w-2, y+h-1);

	XSetForeground(display, gc, color->normal);
	XDrawPoint(display, d, gc, x+w-1, y+1);
	XDrawPoint(display, d, gc, x+1, x+h-1);
}

int stringwidth(const char *str)
{
        XCharStruct ch;
        int direction;
        int ascent;
        int descent;

        XTextExtents(font, str, strlen(str), &direction,
            &ascent, &descent, &ch);
        return ch.width;
}

char *stringfit(char *str, int width)
{
	int len;

	len = strlen(str);
	while (stringwidth(str) > width) {
		if (len < 3) {
			str[0] = '\0';
			break;
		}
		strcpy(str + len - 3, "...");
		len--;
	}
	return str;
}

void *MALLOC(size_t size)
{
	void *ptr;

	while ((ptr = malloc(size)) == NULL && size != 0)
		sleep(1);
	return ptr;
}

void *REALLOC(void *optr, size_t size)
{
	void *nptr;

	while ((nptr = realloc(optr, size)) == NULL && size != 0)
		sleep(1);
	return nptr;
}

void FREE(void *ptr)
{
	free(ptr);
}

char *STRDUP(const char *str)
{
	char *new;

	assert(str != NULL);
	new = MALLOC(strlen(str) + 1);
	return strcpy(new, str);
}

static Window fastmovewin = None;

void beginfastmove(Window xwin)
{
	XSetWindowAttributes attr;

	if (fastmovewin != None)
		return;

	attr.override_redirect = True;
	fastmovewin = XCreateWindow(display, root,
	    0, 0, DisplayWidth(display, screen),
	    DisplayHeight(display, screen), 0, CopyFromParent, InputOnly,
	    CopyFromParent, CWOverrideRedirect, &attr);
	XMapWindow(display, fastmovewin);
	XGrabPointer(display, xwin, False,
	    ButtonMotionMask | ButtonReleaseMask,
	    GrabModeAsync, GrabModeAsync,
	    fastmovewin, None, CurrentTime);
}

void endfastmove(void)
{
	if (fastmovewin == None)
		return;

	XUngrabPointer(display, CurrentTime);
	XDestroyWindow(display, fastmovewin);
	fastmovewin = None;
}

#define CASE(type)	case type: return #type
const char *eventname(int type)
{
	switch (type) {
	CASE(KeyPress);
	CASE(KeyRelease);
	CASE(ButtonPress);
	CASE(ButtonRelease);
	CASE(MotionNotify);
	CASE(EnterNotify);
	CASE(LeaveNotify);
	CASE(FocusIn);
	CASE(FocusOut);
	CASE(KeymapNotify);
	CASE(Expose);
	CASE(GraphicsExpose);
	CASE(NoExpose);
	CASE(VisibilityNotify);
	CASE(CreateNotify);
	CASE(DestroyNotify);
	CASE(UnmapNotify);
	CASE(MapNotify);
	CASE(MapRequest);
	CASE(ReparentNotify);
	CASE(ConfigureNotify);
	CASE(ConfigureRequest);
	CASE(GravityNotify);
	CASE(ResizeRequest);
	CASE(CirculateNotify);
	CASE(CirculateRequest);
	CASE(PropertyNotify);
	CASE(SelectionClear);
	CASE(SelectionRequest);
	CASE(SelectionNotify);
	CASE(ColormapNotify);
	CASE(ClientMessage);
	CASE(MappingNotify);
	default:
		return "INVALID EVENT";
	}
}
