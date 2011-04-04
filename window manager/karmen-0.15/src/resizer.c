/*
 * resizer.c - resizable window borders
 */

/*
 * Copyright 2006-2008 Johan Veenhuizen
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "menu.h"
#include "resizer.h"
#include "window.h"

#define PADDING		MAX(1, (font->ascent + font->descent) / 4)

static Cursor c1, c2, c3, c4, c5, c6, c7, c8;

struct sizewin { struct widget widget;
	struct window *window;
	GC gc;
};

static void update_sizewin(struct sizewin *sizewin, int xdim, int ydim)
{
	static char buf[256];
	struct widget *widget;
	struct window *win;
	int width, height;

	widget = (struct widget *)sizewin;
	win = sizewin->window;

	sprintf(buf, "%dx%d", xdim, ydim);
	width = 2 * (PADDING + font->descent) + stringwidth(buf);
	height = 2 * PADDING + font->ascent + font->descent;
	moveresize_widget(widget,
	    WIDGET_X(win) + WIDGET_WIDTH(win) / 2 - (width + 2) / 2,
	    WIDGET_Y(win) + border_width + button_size
	     + (WIDGET_HEIGHT(win) - 2*border_width - button_size) / 2
	     - (height + 2) / 2,
	    width, height);
	if (!WIDGET_MAPPED(sizewin))
		map_widget(&sizewin->widget);
	XClearWindow(display, WIDGET_XWINDOW(sizewin));
	XSetForeground(display, sizewin->gc, color_title_active_fg.normal);
	XDrawString(display, WIDGET_XWINDOW(sizewin), sizewin->gc,
	    PADDING + font->descent, PADDING + font->ascent,
	    buf, strlen(buf));
	drawraised(WIDGET_XWINDOW(sizewin), sizewin->gc,
	    &color_title_active_bg,
	    0, 0, WIDGET_WIDTH(sizewin), WIDGET_HEIGHT(sizewin));
}

static struct sizewin *create_sizewin(struct window *win, int xdim, int ydim)
{
	XSetWindowAttributes attr;
	XGCValues gcval;
	struct sizewin *sizewin;

	sizewin = MALLOC(sizeof *sizewin);
	create_widget(&sizewin->widget, WIDGET_SIZEWIN,
	    root, InputOutput, 0, 0, 1, 1);
	XSetWindowBackground(display, WIDGET_XWINDOW(sizewin),
	    color_title_active_bg.normal);
	attr.save_under = True;
	XChangeWindowAttributes(display, WIDGET_XWINDOW(sizewin),
	    CWSaveUnder, &attr);
	gcval.font = font->fid;
	sizewin->gc = XCreateGC(display, WIDGET_XWINDOW(sizewin),
	    GCFont, &gcval);
	sizewin->window = win;
	update_sizewin(sizewin, xdim, ydim);
	return sizewin;
}

static void destroy_sizewin(struct sizewin *sizewin)
{
	XFreeGC(display, sizewin->gc);
	destroy_widget(&sizewin->widget);
	FREE(sizewin);
}

static void press(struct resizer *resizer, XButtonEvent *ep)
{
	struct window *win = resizer->window;
	int xdim, ydim;

	if (ep->button == Button1) {
		set_active_window(win);
		window_calcsize(win, WIDGET_WIDTH(win), WIDGET_HEIGHT(win),
		    NULL, NULL, &xdim, &ydim);
		if (resizer->sizewin != NULL)
			destroy_sizewin(resizer->sizewin);
		resizer->sizewin = create_sizewin(win, xdim, ydim);
	} else if (ep->button == Button3) {
		if (resizer->sizewin != NULL) {
			destroy_sizewin(resizer->sizewin);
			resizer->sizewin = NULL;
		}
		show_menu(winmenu, ep->x_root, ep->y_root, ep->button);
	}
}

static void release(struct resizer *resizer, XButtonEvent *ep)
{
	if (ep->button == Button1 && resizer->sizewin != NULL) {
		destroy_sizewin(resizer->sizewin);
		resizer->sizewin = NULL;
	}
}

static void motion(struct resizer *resizer, XMotionEvent *ep)
{
	struct window *win = resizer->window;
	int x, y;
	int width, height;
	int rwidth, rheight;
	int xdim, ydim;

	if (resizer->sizewin == NULL)
		return;

	switch (resizer->dir) {
	case NORTHWEST:
		width = WIDGET_X(win) + WIDGET_WIDTH(win) - ep->x_root;
		height = WIDGET_Y(win) + WIDGET_HEIGHT(win) - ep->y_root;
		break;
	case NORTH:
		width = WIDGET_WIDTH(win);
		height = WIDGET_Y(win) + WIDGET_HEIGHT(win) - ep->y_root;
		break;
	case NORTHEAST:
		width = 1 + ep->x_root - WIDGET_X(win);
		height = WIDGET_Y(win) + WIDGET_HEIGHT(win) - ep->y_root;
		break;
	case WEST:
		width = WIDGET_X(win) + WIDGET_WIDTH(win) - ep->x_root;
		height = WIDGET_HEIGHT(win);
		break;
	case EAST:
		width = 1 + ep->x_root - WIDGET_X(win);
		height = WIDGET_HEIGHT(win);
		break;
	case SOUTHWEST:
		width = WIDGET_X(win) + WIDGET_WIDTH(win) - ep->x_root;
		height = 1 + ep->y_root - WIDGET_Y(win);
		break;
	case SOUTH:
		width = WIDGET_WIDTH(win);
		height = 1 + ep->y_root - WIDGET_Y(win);
		break;
	case SOUTHEAST:
		width = 1 + ep->x_root - WIDGET_X(win);
		height = 1 + ep->y_root - WIDGET_Y(win);
		break;
	default:
		abort();
		break;
	}

	window_calcsize(win, width, height, &rwidth, &rheight, &xdim, &ydim);

	switch (resizer->dir) {
	case NORTHWEST:
		x = WIDGET_X(win) + WIDGET_WIDTH(win) - rwidth;
		y = WIDGET_Y(win) + WIDGET_HEIGHT(win) - rheight;
		break;
	case NORTHEAST:
		x = WIDGET_X(win);
		y = WIDGET_Y(win) + WIDGET_HEIGHT(win) - rheight;
		break;
	case NORTH:
		x = WIDGET_X(win);
		y = WIDGET_Y(win) + WIDGET_HEIGHT(win) - rheight;
		break;
	case WEST:
		x = WIDGET_X(win) + WIDGET_WIDTH(win) - rwidth;
		y = WIDGET_Y(win);
		break;
	case SOUTHWEST:
		x = WIDGET_X(win) + WIDGET_WIDTH(win) - rwidth;
		y = WIDGET_Y(win);
		break;
	default:
		x = WIDGET_X(win);
		y = WIDGET_Y(win);
		break;
	}

	if (rwidth != WIDGET_WIDTH(win) || rheight != WIDGET_HEIGHT(win)) {
		resizer->window->maximized = 0;
		moveresize_window(resizer->window, x, y, rwidth, rheight);
		update_sizewin(resizer->sizewin, xdim, ydim);
	}
}

static void resizerevent(struct widget *widget, XEvent *ep)
{
	switch (ep->type) {
	case ButtonPress:
		press((struct resizer *)widget, &ep->xbutton);
		break;
	case ButtonRelease:
		release((struct resizer *)widget, &ep->xbutton);
		break;
	case MotionNotify:
		motion((struct resizer *)widget, &ep->xmotion);
		break;
	}
}

struct resizer *create_resizer(struct window *win, int dir)
{
	struct resizer *resizer;
	static int initialized = 0;

	if (!initialized) {
		initialized = 1;
		c1 = XCreateFontCursor(display, XC_top_left_corner);
		c2 = XCreateFontCursor(display, XC_top_side);
		c3 = XCreateFontCursor(display, XC_top_right_corner);
		c4 = XCreateFontCursor(display, XC_left_side);
		c5 = XCreateFontCursor(display, XC_right_side);
		c6 = XCreateFontCursor(display, XC_bottom_left_corner);
		c7 = XCreateFontCursor(display, XC_bottom_side);
		c8 = XCreateFontCursor(display, XC_bottom_right_corner);
	}

	resizer = MALLOC(sizeof (struct resizer));
	create_widget(&resizer->widget, WIDGET_RESIZER,
	    WIDGET_XWINDOW(win), InputOnly, 0, 0, 1, 1);
	resizer->widget.event = resizerevent;
	XSelectInput(display, WIDGET_XWINDOW(resizer),
	    ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
	resizer->window = win;
	resizer->sizewin = NULL;
	resizer->dir = dir;
	fit_resizer(resizer);

	switch (resizer->dir) {
	case NORTHWEST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c1);
		break;
	case NORTH:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c2);
		break;
	case NORTHEAST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c3);
		break;
	case WEST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c4);
		break;
	case EAST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c5);
		break;
	case SOUTHWEST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c6);
		break;
	case SOUTH:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c7);
		break;
	case SOUTHEAST:
		XDefineCursor(display, WIDGET_XWINDOW(resizer), c8);
		break;
	default:
		abort();
	}

	XLowerWindow(display, WIDGET_XWINDOW(resizer));
	map_widget(&resizer->widget);
	return resizer;
}

void destroy_resizer(struct resizer *resizer)
{
	if (resizer->sizewin != NULL)
		destroy_sizewin(resizer->sizewin);
	destroy_widget(&resizer->widget);
	FREE(resizer);
}

void fit_resizer(struct resizer *resizer)
{
	struct window *window = resizer->window;
	int x, y;
	int ux, uy;
	int width, height;

	ux = MIN(button_size, WIDGET_WIDTH(window) / 3);
	uy = MIN(button_size, WIDGET_HEIGHT(window) / 3);

	switch (resizer->dir) {
	case NORTHWEST:
		x = 0;
		y = 0;
		width = ux;
		height = uy;
		break;
	case NORTH:
		x = ux;
		y = 0;
		width = WIDGET_WIDTH(window) - 2 * ux;
		height = border_width;
		break;
	case NORTHEAST:
		x = WIDGET_WIDTH(window) - ux;
		y = 0;
		width = ux;
		height = uy;
		break;
	case WEST:
		x = 0;
		y = uy;
		width = border_width;
		height = WIDGET_HEIGHT(window) - 2 * uy;
		break;
	case EAST:
		x = WIDGET_WIDTH(window) - border_width;
		y = uy;
		width = border_width;
		height = WIDGET_HEIGHT(window) - 2 * uy;
		break;
	case SOUTHWEST:
		x = 0;
		y = WIDGET_HEIGHT(window) - uy;
		width = ux;
		height = uy;
		break;
	case SOUTH:
		x = ux;
		y = WIDGET_HEIGHT(window) - border_width;
		width = WIDGET_WIDTH(window) - 2 * ux;
		height = border_width;
		break;
	case SOUTHEAST:
		x = WIDGET_WIDTH(window) - ux;
		y = WIDGET_HEIGHT(window) - uy;
		width = ux;
		height = uy;
		break;
	default:
		abort();
	}
	moveresize_widget(&resizer->widget, x, y, width, height);
}
