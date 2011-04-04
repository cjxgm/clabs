/*
 * title.c - window titles
 */

/*
 * Copyright 2006-2007 Johan Veenhuizen
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
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "menu.h"
#include "lib.h"
#include "title.h"
#include "window.h"

static void prepare_repaint(struct widget *widget)
{
	struct title *title = (struct title *)widget;

	if (window_family_is_active(title->window)) {
		title->fg = &color_title_active_fg;
		title->bg = &color_title_active_bg;
	} else {
		title->fg = &color_title_inactive_fg;
		title->bg = &color_title_inactive_bg;
	}

	XSetWindowBackground(display, WIDGET_XWINDOW(title),
	    title->bg->normal);
}

static void repaint(struct widget *widget)
{
	struct title *title = (struct title *)widget;
	struct window *win = title->window;
	struct color *fg = title->fg;
	struct color *bg = title->bg;
	char *buf = NULL;
	int xpad = title_pad + 2 * font->descent; /* this looks reasonable */
	int ypad = MAX(3, 2 * title_pad);
	int maxwidth = window_is_active(win) ?
	    WIDGET_WIDTH(title) - 2 * xpad - ypad - WIDGET_WIDTH(title) / 5 :
	    WIDGET_WIDTH(title) - 2 * xpad;
	int off;

	/* clear */
	XSetForeground(display, title->gc, bg->normal);
	XFillRectangle(display, title->pixmap, title->gc,
	    0, 0, WIDGET_WIDTH(title), WIDGET_HEIGHT(title));

	/* repaint */
	if (window_is_ontop(title->window)) {
		drawdepressed(title->pixmap, title->gc, bg,
		    0, 0, WIDGET_WIDTH(title), WIDGET_HEIGHT(title));
		off = 1;
	} else {
		drawraised(title->pixmap, title->gc, bg,
		    0, 0, WIDGET_WIDTH(title), WIDGET_HEIGHT(title));
		off = 0;
	}

	XSetForeground(display, title->gc, fg->normal);
	if (win->name != NULL && strlen(win->name) > 0) {
		buf = STRDUP(win->name);
		stringfit(buf, maxwidth);
		XDrawString(display, title->pixmap, title->gc,
		    xpad + off,
		    title_pad + font->ascent + off,
		    buf, strlen(buf));
	} 

	if (window_is_active(win)) {
		int x, y;
		int m = 0;

		x = (buf == NULL || strlen(buf) == 0) ?
		    ypad : stringwidth(buf) + 2 * xpad;
		x += off;
		if (x < WIDGET_WIDTH(title) - 1 - ypad) {
			for (y = ypad + off;
			     y < WIDGET_HEIGHT(title) - 1 - ypad + off;
			     y++) {
				XSetForeground(display, title->gc,
				    m ? bg->shadow2 : bg->bright2);
				XDrawLine(display, title->pixmap, title->gc,
				    m + x, y,
				    m + WIDGET_WIDTH(title)-2 - ypad + off, y);
				m = !m;
			}
		}
	}

	/* display */
	if (WIDGET_MAPPED(title))
		XCopyArea(display,
		    title->pixmap, WIDGET_XWINDOW(title), title->gc,
		    0, 0, WIDGET_WIDTH(title), WIDGET_HEIGHT(title), 0, 0);

	FREE(buf);
}

static void titleevent(struct widget *widget, XEvent *ep)
{
	struct title *title = (struct title *)widget;
	Window dummy;

	switch (ep->type) {
	case ButtonPress:
		if (ep->xbutton.button == Button1) {
			if (title->lastclick > ep->xbutton.time - 250) {
				maximize_window(title->window);
			} else {
				XTranslateCoordinates(display,
				    WIDGET_XWINDOW(title),
				    WIDGET_XWINDOW(title->window),
				    ep->xbutton.x,
				    ep->xbutton.y,
				    &title->xoff,
				    &title->yoff,
				    &dummy);
				if (ep->xbutton.state & ShiftMask)
					toggle_window_ontop(title->window);
				set_active_window(title->window);

				beginfastmove(WIDGET_XWINDOW(title));
				title->moving = 1;
			}
			title->lastclick = ep->xbutton.time;
		} else if (ep->xbutton.button == Button3)
			show_menu(winmenu,
			    ep->xbutton.x_root, ep->xbutton.y_root,
			    ep->xbutton.button);
		break;
	case ButtonRelease:
		if (ep->xbutton.button == Button1 && title->moving) {
			title->moving = 0;
			endfastmove();
		}
		break;
	case MotionNotify:
		if (title->moving) {
			if (ep->xmotion.state & ControlMask)
				move_window_family(title->window,
				    ep->xmotion.x_root - title->xoff,
				    ep->xmotion.y_root - title->yoff);
			else
				move_window(title->window,
				    ep->xmotion.x_root - title->xoff,
				    ep->xmotion.y_root - title->yoff);
		}
		break;
	case Expose:
		XCopyArea(display, title->pixmap, WIDGET_XWINDOW(title),
		    title->gc, ep->xexpose.x, ep->xexpose.y,
		    ep->xexpose.width, ep->xexpose.height,
		    ep->xexpose.x, ep->xexpose.y);
		break;
	}
}

struct title *create_title(struct window *window, int x, int y,
    int width, int height)
{
	XGCValues gcval;
	struct title *tp;

	tp = MALLOC(sizeof (struct title));
	create_widget(&tp->widget, WIDGET_TITLE, WIDGET_XWINDOW(window),
	    InputOutput, x, y, width, height);
	tp->pixmap = XCreatePixmap(display, WIDGET_XWINDOW(tp),
	    tp->pixmapwidth = width, tp->pixmapheight = height,
	    DefaultDepth(display, screen));
	gcval.graphics_exposures = False;
	tp->gc = XCreateGC(display, WIDGET_XWINDOW(tp),
	    GCGraphicsExposures, &gcval);
	XSetFont(display, tp->gc, font->fid);
	tp->window = window;
	tp->widget.event = titleevent;
	tp->widget.prepare_repaint = prepare_repaint;
	tp->widget.repaint = repaint;
	XSelectInput(display, WIDGET_XWINDOW(tp),
	    ButtonPressMask | ButtonMotionMask | ButtonReleaseMask |
	    ExposureMask);
	tp->moving = 0;
	tp->lastclick = 0;
	REPAINT(tp);
	map_widget(&tp->widget);
	return tp;
}

void resize_title(struct title *tp, int width, int height)
{
	if (width > tp->pixmapwidth || height > tp->pixmapheight) {
		XFreePixmap(display, tp->pixmap);
		if (width > tp->pixmapwidth)
			tp->pixmapwidth = MAX(LARGER(tp->pixmapwidth),
			                      width);
		if (height > tp->pixmapheight)
			tp->pixmapheight = MAX(LARGER(tp->pixmapheight),
			                       height);
		debug("increasing title pixmap size (%dx%d)",
		    tp->pixmapwidth, tp->pixmapheight);
		tp->pixmap = XCreatePixmap(display, WIDGET_XWINDOW(tp),
		    tp->pixmapwidth, tp->pixmapheight,
		    DefaultDepth(display, screen));
	}

	resize_widget(&tp->widget, width, height);
	REPAINT(tp);
}

void destroy_title(struct title *title)
{
	destroy_widget(&title->widget);
	XFreePixmap(display, title->pixmap);
	XFreeGC(display, title->gc);
	FREE(title);
}
