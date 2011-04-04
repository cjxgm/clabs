/*
 * widget.c - general window manager widget
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

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include "global.h"
#include "widget.h"

static XContext wmcontext;

static LIST_DEFINE(repaintlist);

void widget_init(void)
{
	wmcontext = XUniqueContext();
}

void widget_fini(void)
{
}

void create_widget(struct widget *widget, enum widget_type type,
    Window xparent, unsigned class, int x, int y, int width, int height)
{
	XSetWindowAttributes attr;

	widget->type = type;
	widget->dim.x = x;
	widget->dim.y = y;
	widget->dim.width = width;
	widget->dim.height = height;
	widget->event = NULL;
	widget->mapped = False;
	attr.override_redirect = True;
	widget->xwindow = XCreateWindow(display, xparent,
	    x, y, width, height, 0,
	    CopyFromParent,
	    class,
	    CopyFromParent,
	    CWOverrideRedirect, &attr);
	XSaveContext(display, widget->xwindow, wmcontext, (XPointer)widget);
	widget->prepare_repaint = NULL;
	widget->repaint = NULL;
	LIST_INIT(&widget->repaintlink);
}

void save_widget_context(struct widget *widget, Window xwindow)
{
	clerr();
	XSaveContext(display, xwindow, wmcontext, (XPointer)widget);
	sterr();
}

struct widget *find_widget(Window xwindow, enum widget_type type)
{
	struct widget *widget;

	if (XFindContext(display, xwindow, wmcontext,
	    (XPointer *)&widget) == 0)
		return (type == WIDGET_ANY || widget->type == type) ?
		    widget : NULL;
	else
		return NULL;
}

void resize_widget(struct widget *widget, int width, int height)
{
	widget->dim.width = width;
	widget->dim.height = height;
	XResizeWindow(display, widget->xwindow, width, height);
}

void move_widget(struct widget *widget, int x, int y)
{
	widget->dim.x = x;
	widget->dim.y = y;
	XMoveWindow(display, widget->xwindow, x, y);
}

void moveresize_widget(struct widget *widget, int x, int y,
    int width, int height)
{
	widget->dim.x = x;
	widget->dim.y = y;
	widget->dim.width = width;
	widget->dim.height = height;
	XMoveResizeWindow(display, widget->xwindow, x, y, width, height);
}

void map_widget(struct widget *widget)
{
	widget->mapped = 1;
	XMapWindow(display, widget->xwindow);
}

void unmap_widget(struct widget *widget)
{
	widget->mapped = 0;
	XUnmapWindow(display, widget->xwindow);
}

void destroy_widget(struct widget *widget)
{
	LIST_REMOVE(&widget->repaintlink);
	XDeleteContext(display, widget->xwindow, wmcontext);
	XDestroyWindow(display, widget->xwindow);
}

void delete_widget_context(Window xwindow)
{
	XDeleteContext(display, xwindow, wmcontext);
}

void schedule_widget_repaint(struct widget *wp)
{
	if (wp->prepare_repaint != NULL)
		wp->prepare_repaint(wp);
	if (!LIST_MEMBER(&wp->repaintlink))
		LIST_INSERT_TAIL(&repaintlist, &wp->repaintlink);
}

void repaint_widgets(void)
{
	struct widget *wp;
	LIST *lp;

	while (!LIST_EMPTY(&repaintlist)) {
		lp = LIST_HEAD(&repaintlist);
		LIST_REMOVE(lp);
		wp = LIST_ITEM(lp, struct widget, repaintlink);
		if (wp->repaint != NULL)
			wp->repaint(wp);
	}
}
