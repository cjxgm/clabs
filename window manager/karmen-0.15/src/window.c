/*
 * window.c - window management routines
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "button.h"
#include "global.h"
#include "hints.h"
#include "lib.h"
#include "menu.h"
#include "resizer.h"
#include "title.h"
#include "window.h"

#define NBTN	2

struct window *active = NULL;

static Atom WM_STATE;

static Window front;

static LIST_DEFINE(normallayer);
static LIST_DEFINE(toplayer);
static int needrestack = 0;
static int nwindows = 0;

static Cursor movecurs;

static void client_to_window_geom(XWindowAttributes *, XSizeHints *,
    int *x, int *y, int *width, int *height);
static void window_to_client_geom(struct window *,
    int *x, int *y, int *width, int *height);
static void confevent(struct window *, XConfigureRequestEvent *);
static void windowevent(struct widget *, XEvent *);
static void setgrav(Window, int);
static void restack_transient_windows(struct window *);
static void put_window_group_below(struct window *);
static void selectfrommenu(void *ptr);
static struct window *topmost_window(void);
static void fitwin(struct window *);
static void smartpos(int, int, int *, int *);
static void make_window_visible(struct window *);

static int ismapped(Window client)
{
	XWindowAttributes attr;
	int rval = 0;

	clerr();
	if (XGetWindowAttributes(display, client, &attr))
		rval = attr.map_state != IsUnmapped;
	sterr();

	return rval;
}

void window_init(void)
{
	XSetWindowAttributes attr;
	Window *winlist;
	Window d1, d2;
	unsigned int i, n;

	attr.override_redirect = True;
	front = XCreateWindow(display, root, 0, 0, 1, 1, 0, 0, InputOnly,
	    CopyFromParent, CWOverrideRedirect, &attr);
	movecurs = XCreateFontCursor(display, XC_fleur);

	WM_STATE = XInternAtom(display, "WM_STATE", False);

	if (XQueryTree(display, root, &d1, &d2, &winlist, &n)) {
		for (i = 0; i < n; i++)
			if (find_widget(winlist[i], WIDGET_ANY) == NULL
			    && ismapped(winlist[i]))
				manage_window(winlist[i], 1);
		if (winlist != NULL)
			XFree(winlist);
	}
}

void window_fini(void)
{
	struct window *win;
	LIST *lp;

	while (!LIST_EMPTY(&normallayer)) {
		lp = LIST_TAIL(&normallayer);
		win = LIST_ITEM(lp, struct window, layerlink);
		unmanage_window(win, 0);
	}
	while (!LIST_EMPTY(&toplayer)) {
		lp = LIST_TAIL(&toplayer);
		win = LIST_ITEM(lp, struct window, layerlink);
		unmanage_window(win, 0);
	}

	XDestroyWindow(display, front);
}

void raise_window(struct window *win)
{
	LIST_REMOVE(&win->layerlink);
	LIST_INSERT_HEAD(win->layer, &win->layerlink);
	needrestack = 1;
}

void lower_window(struct window *win)
{
	LIST_REMOVE(&win->layerlink);
	win->layer = &normallayer;
	LIST_INSERT_TAIL(win->layer, &win->layerlink);
	needrestack = 1;
	repaint_window(win);	/* might have changed layer */
}

static void lower_window_within_layer(struct window *win)
{
	LIST_REMOVE(&win->layerlink);
	LIST_INSERT_TAIL(win->layer, &win->layerlink);
	needrestack = 1;
}

void put_window_above(struct window *win, struct window *ref)
{
	LIST_REMOVE(&win->layerlink);
	win->layer = ref->layer;
	LIST_INSERT_BEFORE(&ref->layerlink, &win->layerlink);
	needrestack = 1;
	repaint_window(win);	/* might have changed layer */
}

void put_window_below(struct window *win, struct window *ref)
{
	LIST_REMOVE(&win->layerlink);
	win->layer = ref->layer;
	LIST_INSERT_AFTER(&ref->layerlink, &win->layerlink);
	needrestack = 1;
	repaint_window(win);	/* might have changed layer */
}

void toggle_window_ontop(struct window *win)
{
	win->layer = win->layer == &toplayer ? &normallayer : &toplayer;
	LIST_REMOVE(&win->layerlink);
	LIST_INSERT_HEAD(win->layer, &win->layerlink);
	needrestack = 1;

	restack_transient_windows(win);

	repaint_window(win);	/* changed layer */
}

int window_is_ontop(struct window *win)
{
	return win->layer == &toplayer;
}

void get_window_stack(struct window ***wins_return, int *nwins_return)
{
	LIST *lp;
	int i;

	*wins_return = MALLOC(nwindows * sizeof (struct window *));
	*nwins_return = nwindows;
	i = 0;
	LIST_FOREACH(lp, &toplayer) {
		assert(i < nwindows);
		(*wins_return)[i++] = LIST_ITEM(lp, struct window, layerlink);
	}
	LIST_FOREACH(lp, &normallayer) {
		assert(i < nwindows);
		(*wins_return)[i++] = LIST_ITEM(lp, struct window, layerlink);
	}
	assert(i == nwindows);
}

void get_client_stack(Window **clients_return, int *nclients_return)
{
	LIST *lp;
	struct window *win;
	int i;

	*clients_return = MALLOC(nwindows * sizeof (Window));
	*nclients_return = nwindows;
	i = 0;
	LIST_FOREACH(lp, &toplayer) {
		assert(i < nwindows);
		win = LIST_ITEM(lp, struct window, layerlink);
		(*clients_return)[i++] = win->client;
	}
	LIST_FOREACH(lp, &normallayer) {
		assert(i < nwindows);
		win = LIST_ITEM(lp, struct window, layerlink);
		(*clients_return)[i++] = win->client;
	}
	assert(i == nwindows);
}

void restack_all_windows(void)
{
	Window *xwins;
	struct window *win;
	LIST *lp;
	int i;

	if (!needrestack)
		return;

	xwins = MALLOC((nwindows + 1) * sizeof (Window));
	xwins[0] = front;
	i = 1;
	LIST_FOREACH(lp, &toplayer) {
		assert(i < nwindows + 1);
		win = LIST_ITEM(lp, struct window, layerlink);
		xwins[i++] = WIDGET_XWINDOW(win);
	}
	LIST_FOREACH(lp, &normallayer) {
		assert(i < nwindows + 1);
		win = LIST_ITEM(lp, struct window, layerlink);
		xwins[i++] = WIDGET_XWINDOW(win);
	}
	assert(i == nwindows + 1);
	XRestackWindows(display, xwins, nwindows + 1);
	FREE(xwins);
	needrestack = 0;

	hints_restack();
}

static struct window *topmost_window(void)
{
	struct window *win;
	LIST *lp;

	LIST_FOREACH(lp, &toplayer) {
		win = LIST_ITEM(lp, struct window, layerlink);
		if (WIDGET_MAPPED(win))
			return win;
	}
	LIST_FOREACH(lp, &normallayer) {
		win = LIST_ITEM(lp, struct window, layerlink);
		if (WIDGET_MAPPED(win))
			return win;
	}
	return NULL;
}

void map_window(struct window *win)
{
	if (WIDGET_MAPPED(win))
		return;

	clerr();
	XMapWindow(display, win->client);
	sterr();

	map_widget((struct widget *)win);

	hints_map(win);
}

void unmap_window(struct window *win)
{
	if (!WIDGET_MAPPED(win))
		return;

	unmap_widget((struct widget *)win);

	if (win == active)
		set_active_window(NULL);

	clerr();
	win->ignoreunmap++;
	XUnmapWindow(display, win->client);
	sterr();

	hints_unmap(win);

	lower_window_within_layer(win);

	if (win->menuitem != NULL)
		put_menuitem_last(win->menuitem);
}

void user_unmap_window(struct window *win)
{
	struct window **wins;
	int i, n, wasactive;

	wasactive = window_is_active(win);

	unmap_window(win);

	get_window_stack(&wins, &n);
	for (i = 0; i < n; i++) {
		if (windows_are_transient_related(wins[i], win)
		    && WIDGET_MAPPED(wins[i]))
			unmap_window(wins[i]);
	}
	FREE(wins);

	if (wasactive)
		set_active_window(topmost_window());
}

static int getwmstate(Window xwindow, long *statp)
{
	unsigned long nitems, bytes_after;
	unsigned char *prop;
	Atom actual_type;
	int actual_format;
	int rval = -1;

	if (XGetWindowProperty(display, xwindow,
	    WM_STATE, 0L, 2L, False, WM_STATE, &actual_type, &actual_format,
	    &nitems, &bytes_after, &prop) == Success) {
		if (nitems > 0) {
			*statp = ((unsigned long *)prop)[0];
			rval = 0;
		}
		XFree(prop);
	}
	return rval;
}

static void client_to_window_geom(XWindowAttributes *attr, XSizeHints *sz,
    int *x, int *y, int *width, int *height)
{
	int north, south, east, west, stat, center;

	north = south = east = west = stat = center = 0;

	if (sz->flags & PWinGravity) {
		switch (sz->win_gravity) {
		case SouthGravity:
			south = 1;
			break;
		case SouthWestGravity:
			south = 1;
			west = 1;
			break;
		case SouthEastGravity:
			south = 1;
			east = 1;
			break;
			break;
		case NorthGravity:
			north = 1;
			break;
		case NorthWestGravity:
			north = 1;
			west = 1;
			break;
		case NorthEastGravity:
			north = 1;
			east = 1;
			break;
		case CenterGravity:
			center = 1;
			break;
		case StaticGravity:
			stat = 1;
			break;
		default:
			north = 1;
			west = 1;
			break;
		}
	} else {
		north = 1;
		west = 1;
	}

	if (north)
		*y = attr->y;
	else if (south)
		*y = attr->y + 2 * attr->border_width
		    - 2 * border_width - button_size;
	else if (center)
		*y = attr->y + attr->border_width
		    + attr->height / 2
		    - (attr->height + 2 * border_width + button_size) / 2;
	else if (stat)
		*y = attr->y + attr->border_width - border_width - button_size;
	else
		*y = attr->y;

	if (west)
		*x = attr->x;
	else if (east)
		*x = attr->x + 2 * attr->border_width
		    - 2 * border_width;
	else if (center)
		*x = attr->x + attr->border_width
		    + attr->width / 2
		    - (attr->width + 2 * border_width) / 2;
	else if (stat)
		*x = attr->x + attr->border_width - border_width;
	else
		*x = attr->x;

	*width = attr->width + 2 * border_width;
	*height = attr->height + 2 * border_width + button_size;
}

static void window_to_client_geom(struct window *win,
    int *x, int *y, int *width, int *height)
{
	int north, south, east, west, stat, center;

	north = south = east = west = stat = center = 0;

	if (win->wmnormalhints->flags & PWinGravity) {
		switch (win->wmnormalhints->win_gravity) {
		case SouthGravity:
			south = 1;
			break;
		case SouthWestGravity:
			south = 1;
			west = 1;
			break;
		case SouthEastGravity:
			south = 1;
			east = 1;
			break;
			break;
		case NorthGravity:
			north = 1;
			break;
		case NorthWestGravity:
			north = 1;
			west = 1;
			break;
		case NorthEastGravity:
			north = 1;
			east = 1;
			break;
		case CenterGravity:
			center = 1;
			break;
		case StaticGravity:
			stat = 1;
			break;
		default:
			north = 1;
			west = 1;
			break;
		}
	} else {
		north = 1;
		west = 1;
	}

	if (north)
		*y = WIDGET_Y(win);
	else if (south)
		*y = WIDGET_Y(win) - 2*win->cborder
		    + 2*border_width + button_size;
	else if (center)
		*y = WIDGET_Y(win) + WIDGET_HEIGHT(win) / 2
		    - (WIDGET_HEIGHT(win) - 2*border_width - button_size) / 2
		    - win->cborder;
	else if (stat)
		*y = WIDGET_Y(win) - win->cborder + border_width + button_size;
	else
		*y = WIDGET_Y(win);

	if (west)
		*x = WIDGET_X(win);
	else if (east)
		*x = WIDGET_X(win) - 2*win->cborder + 2*border_width;
	else if (center)
		*x = WIDGET_X(win) + WIDGET_WIDTH(win) / 2
		    - (WIDGET_WIDTH(win) - 2*border_width) / 2 - win->cborder;
	else if (stat)
		*x = WIDGET_X(win) - win->cborder + border_width;
	else
		*x = WIDGET_X(win);

	*width = WIDGET_WIDTH(win) - 2*border_width;
	*height = WIDGET_HEIGHT(win) - 2*border_width - button_size;
}

void delete_window(struct window *win)
{
	if (!hints_delete(win)) {
		clerr();
		XKillClient(display, win->client);
		sterr();
	}
}

/*
 * See ICCCM 4.1.5 and EWMH "Implementation notes".
 */
static void confevent(struct window *win, XConfigureRequestEvent *ep)
{
	XWindowAttributes attr;
	int x;
	int y;
	int width;
	int height;

	if (ep->value_mask & CWBorderWidth)
		win->cborder = ep->border_width;

	window_to_client_geom(win, &attr.x, &attr.y,
	    &attr.width, &attr.height);
	attr.border_width = win->cborder;

	if (ep->value_mask & CWX)
		attr.x = ep->x;
	if (ep->value_mask & CWY)
		attr.y = ep->y;
	if (ep->value_mask & CWWidth)
		attr.width = ep->width;
	if (ep->value_mask & CWHeight)
		attr.height = ep->height;

	/*
	 * FIXME: handle stacking order
	 */

	client_to_window_geom(&attr, win->wmnormalhints,
	    &x, &y, &width, &height);
	moveresize_window(win, x, y, width, height);
}

static void prepare_repaint(struct widget *widget)
{
	struct window *win = (struct window *)widget;

	if (window_family_is_active(win))
		XSetWindowBackground(display, WIDGET_XWINDOW(win),
		    color_title_active_bg.normal);
	else
		XSetWindowBackground(display, WIDGET_XWINDOW(win),
		    color_title_inactive_bg.normal);
}

static void repaint(struct widget *widget)
{
	struct window *win = (struct window *)widget;
	GC gc = win->title->gc;
	struct color *color =
	    window_family_is_active(win) ?
	    &color_title_active_bg : &color_title_inactive_bg;
	int w = WIDGET_WIDTH(win);
	int h = WIDGET_HEIGHT(win);
	int bw = border_width;

	drawraised(WIDGET_XWINDOW(win), gc, color,
	    0, 0, WIDGET_WIDTH(win), WIDGET_HEIGHT(win));
	drawlowered(WIDGET_XWINDOW(win), gc, color,
	    border_width - 2, border_width - 2,
	    WIDGET_WIDTH(win) - 2 * border_width + 3,
	    WIDGET_HEIGHT(win) - 2 * border_width + 3);

	XSetForeground(display, gc, color->normal);

	XFillRectangle(display, WIDGET_XWINDOW(win), gc,
	    1, 1, w - 3, bw - 3);
	XFillRectangle(display, WIDGET_XWINDOW(win), gc,
	    1, h - bw + 1, w - 3, bw - 3);
	XFillRectangle(display, WIDGET_XWINDOW(win), gc,
	    1, bw - 2, bw - 3, h - 2 * bw + 3);
	XFillRectangle(display, WIDGET_XWINDOW(win), gc,
	    w - bw + 1, bw - 2, bw - 3, h - 2 * bw + 3);

	/* Also paint behind client window in case it is shaped. */
	XFillRectangle(display, WIDGET_XWINDOW(win), gc,
	    bw, bw + button_size, w - 2 * bw, h - 2 * bw - button_size);
}

static void windowevent(struct widget *widget, XEvent *ep)
{
	struct window *win = (struct window *)widget;

	switch (ep->type) {
	case ButtonPress:
		set_active_window(win);
		if (ep->xbutton.state & Mod1Mask) {
			win->altmove.xoff = ep->xbutton.x;
			win->altmove.yoff = ep->xbutton.y;
			win->altmove.moving = 1;
			beginfastmove(WIDGET_XWINDOW(win));
		}
		break;
	case MotionNotify:
		if (win->altmove.moving) {
			if (ep->xmotion.state & ControlMask)
				move_window_family(win,
				    ep->xmotion.x_root - win->altmove.xoff,
				    ep->xmotion.y_root - win->altmove.yoff);
			else
				move_window(win,
				    ep->xmotion.x_root - win->altmove.xoff,
				    ep->xmotion.y_root - win->altmove.yoff);
		}
		break;
	case ButtonRelease:
		win->altmove.moving = 0;
		endfastmove();
		break;
	case MapRequest:
		set_active_window(win);
		break;
	case UnmapNotify:
		/* This can be a real or synthetic unmap event. */
		if (ep->xunmap.window != win->client)
			break;
		if (win->ignoreunmap > 0)
			win->ignoreunmap--;
		else if (win->ignoreunmap == 0) {
			hints_withdraw(win);
			unmanage_window(win, 1);
		} else
			abort();
		break;
	case ConfigureRequest:
		confevent(win, &ep->xconfigurerequest);
		break;
	case ClientMessage:
		hints_clientmessage(win, &ep->xclient);
		break;
	case PropertyNotify:
		hints_propertynotify(win, &ep->xproperty);
		break;
	case DestroyNotify:
		if (ep->xdestroywindow.window == win->client)
			unmanage_window(win, 1);
		break;
	case Expose:
		if (ep->xexpose.count == 0)
			repaint(widget);
		break;
	case GravityNotify:
	case CreateNotify:
	case MapNotify:
	case ReparentNotify:
	case ConfigureNotify:
		/* ignore */
		break;
	default:
		debug("windowevent(): unhandled event -- %s (%d)",
		    eventname(ep->type), ep->type);
		break;
	}
}

void low_limit_size(int *width, int *height)
{
	*width = MAX(*width, 2 * border_width + 1);
	*height = MAX(*height, 2 * border_width + button_size + 1);
}

void window_calcsize(struct window *win, int width, int height,
    int *rwidth, int *rheight, int *rxdim, int *rydim)
{
	int decwidth = 2 * border_width;
	int decheight = 2 * border_width + button_size;
	int havemin = 0;
	int minwidth = 0;
	int minheight = 0;
	int wmminwidth = 0;
	int wmminheight = 0;

	low_limit_size(&width, &height);
	low_limit_size(&wmminwidth, &wmminheight);

	width -= decwidth;
	height -= decheight;

	if (win->wmnormalhints->flags & PMaxSize) {
		width = MIN(width, win->wmnormalhints->max_width);
		height = MIN(height, win->wmnormalhints->max_height);
	}

	havemin = 0;
	if (win->wmnormalhints->flags & PMinSize) {
		minwidth = win->wmnormalhints->min_width;
		minheight = win->wmnormalhints->min_height;
		havemin = 1;
	} else if (win->wmnormalhints->flags & PBaseSize) {
		minwidth = win->wmnormalhints->base_width;
		minheight = win->wmnormalhints->base_height;
		havemin = 1;
	}
	if (havemin) {
		width = MAX(width, minwidth);
		height = MAX(height, minheight);
	}

	if (win->wmnormalhints->flags & PResizeInc) {
		if (win->wmnormalhints->width_inc != 0) {
			int wb;
			if (win->wmnormalhints->flags & PBaseSize)
				wb = win->wmnormalhints->base_width;
			else if (win->wmnormalhints->flags & PMinSize)
				wb = win->wmnormalhints->min_width;
			else
				wb = 0;
			width -= wb;
			width -= width % win->wmnormalhints->width_inc;
			if (havemin)
				width = MAX(width, minwidth - wb);
			while (wb + width + decwidth < wmminwidth)
				width += win->wmnormalhints->width_inc;
			if (rxdim != NULL)
				*rxdim = width / win->wmnormalhints->width_inc;
			width += wb;
		} else if (rxdim != NULL)
			*rxdim = width;
		if (win->wmnormalhints->height_inc != 0) {
			int hb;
			if (win->wmnormalhints->flags & PBaseSize)
				hb = win->wmnormalhints->base_height;
			else if (win->wmnormalhints->flags & PMinSize)
				hb = win->wmnormalhints->min_height;
			else
				hb = 0;
			height -= hb;
			height -= height % win->wmnormalhints->height_inc;
			if (havemin)
				height = MAX(height, minheight - hb);
			while (hb + height + decheight < wmminheight)
				height += win->wmnormalhints->height_inc;
			if (rydim != NULL)
				*rydim =
				    height / win->wmnormalhints->height_inc;
			height += hb;
		} else if (rydim != NULL)
			*rydim = height;
	} else {
		if (rxdim != NULL)
			*rxdim = width;
		if (rydim != NULL)
			*rydim = height;
	}

	width += 2 * border_width;
	height += 2 * border_width + button_size;

	if (rwidth != NULL)
		*rwidth = width;
	if (rheight != NULL)
		*rheight = height;
}

static void setgrav(Window xwin, int grav)
{
	XSetWindowAttributes attr;

	attr.win_gravity = grav;
	XChangeWindowAttributes(display, xwin, CWWinGravity, &attr);
}

void maximize_window(struct window *win)
{
	int x, y, rwidth, rheight;

	if (win->maximized) {
		moveresize_window(win, win->odim.x, win->odim.y,
		    win->odim.width, win->odim.height);
		win->maximized = 0;
	} else {
		win->odim = win->widget.dim;
		window_calcsize(win,
		    DisplayWidth(display, screen),
		    DisplayHeight(display, screen),
		    &rwidth, &rheight, NULL, NULL);
		x = (DisplayWidth(display, screen) - rwidth) / 2;
		y = (DisplayHeight(display, screen) - rheight) / 2;
		moveresize_window(win, x, y, rwidth, rheight);
		win->maximized = 1;
	}
}

struct window *manage_window(Window client, int wmstart)
{
	struct window *win;
	XWindowAttributes attr;
	XSizeHints *sz;
	XWMHints *wmhints;
	long state;
	long dummy;
	int x, y, width, height;

	clerr();
	if (!XGetWindowAttributes(display, client, &attr)) {
		sterr();
		return NULL;
	}
	sterr();

	if (attr.override_redirect)
		return NULL;

	if (find_widget(client, WIDGET_ANY) != NULL) {
		debug("XXX: Trying to remanage a window!");
		return NULL;
	}

	clerr();
	if (getwmstate(client, &state) == -1 || state == WithdrawnState) {
		wmhints = XGetWMHints(display, client);
		if (wmhints == NULL)
			state = NormalState;
		else {
			if (wmhints->flags & StateHint)
				state = wmhints->initial_state;
			else
				state = NormalState;
			XFree(wmhints);
		}
	}
	sterr();

	if (state == WithdrawnState) {
		debug("skipping withdrawn window");
		return NULL;
	}

	while ((sz = XAllocSizeHints()) == NULL)
		sleep(1);
	clerr();
	XGetWMNormalHints(display, client, sz, &dummy);
	sterr();
	client_to_window_geom(&attr, sz, &x, &y, &width, &height);
	low_limit_size(&width, &height);
	if (!wmstart && ~sz->flags & USPosition && ~sz->flags & PPosition)
		smartpos(width, height, &x, &y);
	XFree(sz);

	win = MALLOC(sizeof (struct window));
	create_widget(&win->widget, WIDGET_WINDOW,
	    root, InputOutput, x, y, width, height);

	/*
	 * Initialize struct members
	 */

	win->name = NULL;
	win->iconname = NULL;
	win->title = NULL;
	win->deletebtn = NULL;
	win->unmapbtn = NULL;
	win->rsz_northwest = NULL;
	win->rsz_north = NULL;
	win->rsz_northeast = NULL;
	win->rsz_west = NULL;
	win->rsz_east = NULL;
	win->rsz_southwest = NULL;
	win->rsz_south = NULL;
	win->rsz_southeast = NULL;
	win->menuitem = NULL;
	win->color = &color_title_active_bg;
	win->client = client;
	win->colormap = attr.colormap;
	win->wmnormalhints = NULL;
	win->wmhints = NULL;
	win->wmtransientfor = None;
	win->cborder = attr.border_width;
	win->altmove.moving = 0;
	win->altmove.xoff = 0;
	win->altmove.yoff = 0;
	win->ignoreunmap = 0;
	win->maximized = 0;
	win->odim.x = attr.x;
	win->odim.y = attr.y;
	win->odim.width = attr.width;
	win->odim.height = attr.height;
	win->layer = &normallayer;
	LIST_INIT(&win->layerlink);

	/*
	 * Everything initialized. Time to get some work done.
	 */

	LIST_INSERT_HEAD(win->layer, &win->layerlink);
	nwindows++;
	needrestack = 1;

	save_widget_context(&win->widget, client);

	grabbutton(display, Button1, Mod1Mask,
	    WIDGET_XWINDOW(win), False,
	    ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
	    GrabModeAsync, GrabModeAsync, None, movecurs);
	grabbutton(display, Button1, Mod1Mask | ControlMask,
	    WIDGET_XWINDOW(win), False,
	    ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
	    GrabModeAsync, GrabModeAsync, None, movecurs);

	XSelectInput(display, WIDGET_XWINDOW(win),
	    ExposureMask | SubstructureRedirectMask | SubstructureNotifyMask);

	win->widget.event = windowevent;
	win->widget.prepare_repaint = prepare_repaint;
	win->widget.repaint = repaint;

	win->deletebtn = create_button(win,
	    width - border_width - button_size,
	    border_width, button_size, button_size);
	setgrav(WIDGET_XWINDOW(win->deletebtn), NorthEastGravity);
	set_button_image(win->deletebtn, &delete_image);
	set_button_handler(win->deletebtn, delete_window);

	win->unmapbtn = create_button(win,
	    width - border_width - 2 * button_size,
	    border_width, button_size, button_size);
	setgrav(WIDGET_XWINDOW(win->unmapbtn), NorthEastGravity);
	set_button_image(win->unmapbtn, &unmap_image);
	set_button_handler(win->unmapbtn, user_unmap_window);

	win->title = create_title(win,
	    border_width, border_width,
	    width - 2 * border_width - NBTN * button_size, button_size);

	clerr();
	XAddToSaveSet(display, client);
	XSetWindowBorderWidth(display, client, 0);
	XReparentWindow(display, client, WIDGET_XWINDOW(win),
	    border_width, border_width + button_size);
	XLowerWindow(display, client);
	XSelectInput(display, client, PropertyChangeMask);
	setgrav(client, NorthWestGravity);
	grabbutton(display, AnyButton, 0, client, True,
	    ButtonPressMask, GrabModeSync, GrabModeSync, None, None);
	sterr();

	win->rsz_northwest = create_resizer(win, NORTHWEST);
	win->rsz_north = create_resizer(win, NORTH);
	win->rsz_northeast = create_resizer(win, NORTHEAST);
	win->rsz_west = create_resizer(win, WEST);
	win->rsz_east = create_resizer(win, EAST);
	win->rsz_southwest = create_resizer(win, SOUTHWEST);
	win->rsz_south = create_resizer(win, SOUTH);
	win->rsz_southeast = create_resizer(win, SOUTHEAST);

	fetch_window_name(win);
	fetch_icon_name(win);
	fetch_wm_hints(win);
	fetch_wm_normal_hints(win);
	fetch_wm_transient_for_hint(win);

	repaint_window(win);

#if 0
	if (win->wmtransientfor != None) {
		struct window *w;
		w = (struct window *)find_widget(win->wmtransientfor,
		                                 WIDGET_WINDOW);
		if (w != NULL) {
			raise_window(w);
			restack_transient_windows(w);
		}
	}
#endif

	hints_manage(win);

	map_window(win);

	if (state == IconicState)
		unmap_window(win);
	else
		set_active_window(win);

	XSync(display, False);

	debug("manage \"%s\" (Window=0x%x)", win->name, (int)win->client);

	clerr();
	if (!XGetWindowAttributes(display, client, &attr)) {
		sterr();
		debug("Oops, client window disappeared in manage_window()");
		unmanage_window(win, 1);
		return NULL;
	}
	sterr();

	return win;
}

static void ranpos(int width, int height, int *rx, int *ry)
{
	int dx, dy;

	dx = DisplayWidth(display, screen) - width;
	dy = DisplayHeight(display, screen) - height;
	*rx = dx > 0 ? rand() % dx : 0;
	*ry = dy > 0 ? rand() % dy : 0;
}

static long badness(int x, int y, int width, int height,
                    struct window **wins, int n)
{
	long x1, x2, y1, y2;
	long w, h;
	long area;
	int nwin;
	int i;

	nwin = 0;
	area = 0;
	for (i = 0; i < n; i++)
		if (WIDGET_MAPPED(wins[i])) {
			x1 = WIDGET_X(wins[i]);
			x2 = x1 + WIDGET_WIDTH(wins[i]);
			y1 = WIDGET_Y(wins[i]);
			y2 = y1 + WIDGET_HEIGHT(wins[i]);
			if (x + width > x1 && x < x2 &&
			    y + height > y1 && y < y2) {
				w = MIN(x + width, x2) - MAX(x, x1);
				h = MIN(y + height, y2) - MAX(y, y1);
				if (LONG_MAX - area < w * h)
					area = LONG_MAX;
				else
					area += w * h;
				nwin++;
			}
		}
	if (nwin != 0 && LONG_MAX / nwin < area) {
		nwin = 1;
		area = LONG_MAX;
	}
	return area * nwin;
}

static void smartpos(int width, int height, int *rx, int *ry)
{
	struct window **wins;
	int n;
	int x, y;
	long best = -1;
	long score;
	int i;

	get_window_stack(&wins, &n);
	for (i = 0; best != 0 && i < 100; i++) {
		ranpos(width, height, &x, &y);
		score = badness(x, y, width, height, wins, n);
		if (best == -1 || score < best) {
			best = score;
			*rx = x;
			*ry = y;
		}
	}
	FREE(wins);
}

void fetch_wm_normal_hints(struct window *win)
{
	long dummy;

	if (win->wmnormalhints != NULL)
		XFree(win->wmnormalhints);
	while ((win->wmnormalhints = XAllocSizeHints()) == NULL)
		sleep(1);
	clerr();
	XGetWMNormalHints(display, win->client, win->wmnormalhints, &dummy);
	sterr();
}

void fetch_wm_hints(struct window *win)
{
	if (win->wmhints != NULL)
		XFree(win->wmhints);
	clerr();
	win->wmhints = XGetWMHints(display, win->client);
	sterr();
}

void fetch_window_name(struct window *win)
{
	if (win->name != NULL) {
		XFree(win->name);
		win->name = NULL;
	}
	clerr();
	XFetchName(display, win->client, &win->name);
	sterr();
	REPAINT(win->title);
}

void fetch_icon_name(struct window *win)
{
	char *name;

	if (win->iconname != NULL) {
		XFree(win->iconname);
		win->iconname = NULL;
	}

	clerr();
	XGetIconName(display, win->client, &win->iconname);
	if (win->iconname == NULL || strlen(win->iconname) == 0) {
		if (win->iconname != NULL) {
			XFree(win->iconname);
			win->iconname = NULL;
		}
		XFetchName(display, win->client, &win->iconname);
	}
	sterr();

	if (win->iconname == NULL || strlen(win->iconname) == 0)
		name = "<< Anonymous >>";
	else
		name = win->iconname;

	if (win->menuitem == NULL)
		win->menuitem = create_menuitem(winmenu, name,
		    selectfrommenu, win);
	else
		rename_menuitem(win->menuitem, name);
}

static void selectfrommenu(void *ptr)
{
	set_active_window(ptr);
}

void fetch_wm_transient_for_hint(struct window *win)
{
	win->wmtransientfor = None;
	clerr();
	XGetTransientForHint(display, win->client, &win->wmtransientfor);
	sterr();

	fitwin(win);
}

static void fitwin(struct window *win)
{
	int nbtn = 0;

	if (win->wmtransientfor == None
	    && WIDGET_WIDTH(win) >= 2 * border_width + 2 * button_size + 1) {
		map_widget((struct widget *)win->unmapbtn);
	} else
		unmap_widget((struct widget *)win->unmapbtn);

	if (WIDGET_WIDTH(win) >= 2 * border_width + button_size + 1)
		map_widget((struct widget *)win->deletebtn);
	else
		unmap_widget((struct widget *)win->deletebtn);

	if (WIDGET_MAPPED(win->deletebtn))
		nbtn++;
	if (WIDGET_MAPPED(win->unmapbtn))
		nbtn++;

	resize_title(win->title,
	    MAX(1, WIDGET_WIDTH(win) - 2 * border_width - nbtn * button_size),
	    button_size);
}

void moveresize_window(struct window *win, int x, int y, int width, int height)
{
	int move;
	int resize;

	low_limit_size(&width, &height);

	move = x != WIDGET_X(win) || y != WIDGET_Y(win);
	resize = width != WIDGET_WIDTH(win) || height != WIDGET_HEIGHT(win);

	if (resize) {
		clerr();
		XResizeWindow(display, win->client,
		    width - 2 * border_width,
		    height - 2 * border_width - button_size);
		sterr();
	}

	moveresize_widget((struct widget *)win, x, y, width, height);
	win->maximized = 0;

	if (resize) {
		fitwin(win);
		fit_resizer(win->rsz_northwest);
		fit_resizer(win->rsz_north);
		fit_resizer(win->rsz_northeast);
		fit_resizer(win->rsz_west);
		fit_resizer(win->rsz_east);
		fit_resizer(win->rsz_southwest);
		fit_resizer(win->rsz_south);
		fit_resizer(win->rsz_southeast);
	}

	if (move && !resize)
		hints_move(win);
	else if (!move && resize)
		hints_resize(win);
	else if (move && resize)
		hints_moveresize(win);
}

void move_window(struct window *win, int x, int y)
{
	moveresize_window(win, x, y, WIDGET_WIDTH(win), WIDGET_HEIGHT(win));
}

void move_window_family(struct window *win, int x, int y)
{
	struct window *wp;
	LIST *lp;
	int dx, dy;

	dx = x - WIDGET_X(win);
	dy = y - WIDGET_Y(win);

	LIST_FOREACH(lp, &toplayer) {
		wp = LIST_ITEM(lp, struct window, layerlink);
		if (WIDGET_MAPPED(wp) && windows_are_related(wp, win))
			move_window(wp, WIDGET_X(wp) + dx, WIDGET_Y(wp) + dy);
	}
	LIST_FOREACH(lp, &normallayer) {
		wp = LIST_ITEM(lp, struct window, layerlink);
		if (WIDGET_MAPPED(wp) && windows_are_related(wp, win))
			move_window(wp, WIDGET_X(wp) + dx, WIDGET_Y(wp) + dy);
	}
}

void resize_window(struct window *win, int width, int height)
{
	moveresize_window(win, WIDGET_X(win), WIDGET_Y(win), width, height);
}

void unmanage_window(struct window *win, int clientquit)
{
	int x, y, width, height;
	int wasactive;

	debug("unmanage \"%s\" (Window=0x%x)",win->name, (int)win->client);

	wasactive = win == active;

	if (wasactive)
		set_active_window(NULL);

	if (WIDGET_MAPPED(win))
		unmap_widget(&win->widget);

	hints_unmanage(win);

	/*
	 * Begin teardown.
	 * Not safe to call WM related functions from now on.
	 */

	if (win->menuitem != NULL)
		destroy_menuitem(win->menuitem);
	LIST_REMOVE(&win->layerlink);
	nwindows--;

	window_to_client_geom(win, &x, &y, &width, &height);
	delete_widget_context(win->client);

	clerr();
	XReparentWindow(display, win->client, root, x, y);
	if (!clientquit)
		XMapWindow(display, win->client);
	ungrabbutton(display, AnyButton, AnyModifier, win->client);
	XSelectInput(display, win->client, 0);
	XSetWindowBorderWidth(display, win->client, win->cborder);
	if (win->wmnormalhints->flags & PWinGravity)
		setgrav(win->client, win->wmnormalhints->win_gravity);
	XRemoveFromSaveSet(display, win->client);
	sterr();

	destroy_title(win->title);
	destroy_button(win->deletebtn);
	destroy_button(win->unmapbtn);

	destroy_resizer(win->rsz_northwest);
	destroy_resizer(win->rsz_north);
	destroy_resizer(win->rsz_northeast);
	destroy_resizer(win->rsz_west);
	destroy_resizer(win->rsz_east);
	destroy_resizer(win->rsz_southwest);
	destroy_resizer(win->rsz_south);
	destroy_resizer(win->rsz_southeast);

	if (win->wmhints != NULL)
		XFree(win->wmhints);
	assert(win->wmnormalhints != NULL);
	XFree(win->wmnormalhints);

	destroy_widget(&win->widget);
	if (win->name != NULL)
		XFree(win->name);
	if (win->iconname != NULL)
		XFree(win->iconname);
	FREE(win);

	XSync(display, False);

	/*
	 * Teardown finished
	 */

	if (wasactive)
		set_active_window(topmost_window());

}

void repaint_window(struct window *win)
{
	REPAINT(win->title);
	REPAINT(win->deletebtn);
	REPAINT(win->unmapbtn);
	REPAINT(win);
}

void repaint_window_family(struct window *win)
{
	struct window *wp;
	LIST *lp;

	LIST_FOREACH(lp, &toplayer) {
		wp = LIST_ITEM(lp, struct window, layerlink);
		if (windows_are_related(wp, win))
			repaint_window(wp);
	}
	LIST_FOREACH(lp, &normallayer) {
		wp = LIST_ITEM(lp, struct window, layerlink);
		if (windows_are_related(wp, win))
			repaint_window(wp);
	}
}

/*
 * This function assumes that 'win' already has the desired stacking.
 */
static void restack_transient_windows(struct window *win)
{
	struct window *leader;
	struct window **wins;
	int i, n;

	leader = NULL;
	if (win->wmtransientfor != None)
		leader = (struct window *)find_widget(win->wmtransientfor,
		                                      WIDGET_WINDOW);
	if (leader == NULL)
		leader = win;

	if (win != leader)
		put_window_below(leader, win);

	get_window_stack(&wins, &n);
	for (i = 0; i < n; i++) {
		if (wins[i]->wmtransientfor == leader->client)
			put_window_above(wins[i], leader);
	}
	FREE(wins);
}

static void map_transient_windows(struct window *win)
{
	struct window *leader;
	struct window **wins;
	int i, n;

	leader = NULL;
	if (win->wmtransientfor != None)
		leader = (struct window *)find_widget(win->wmtransientfor,
		                                      WIDGET_WINDOW);
	if (leader == NULL)
		leader = win;

	get_window_stack(&wins, &n);
	for (i = 0; i < n; i++)
		if (wins[i]->wmtransientfor == leader->client)
			make_window_visible(wins[i]);
	FREE(wins);
	make_window_visible(leader);
}

static void put_window_group_below(struct window *win)
{
	struct window **wins, *wp;
	int n;

	if (win->wmhints == NULL
	    || ~win->wmhints->flags & WindowGroupHint
	    || win->wmhints->window_group == None
	    || win->wmhints->window_group == root)
		return;

	get_window_stack(&wins, &n);
	while (n > 1 && wins[--n] != win) {
		wp = wins[n];
		if (wp->wmhints != NULL
		    && wp->wmhints->flags & WindowGroupHint
		    && wp->wmhints->window_group == win->wmhints->window_group)
			put_window_below(wp, win);
	}
	FREE(wins);
}

int window_is_active(struct window *win)
{
	return win == active;
}

int window_is_transient_active(struct window *win)
{
	return window_is_active(win)
	    || (active != NULL && windows_are_transient_related(win, active));
}

int window_group_is_active(struct window *win)
{
	return window_is_active(win)
	    || (active != NULL && windows_are_group_related(win, active));
}

int window_family_is_active(struct window *win)
{
	return window_is_active(win)
	    || window_is_transient_active(win) || window_group_is_active(win);
}

int windows_are_related(struct window *win1, struct window *win2)
{
	return win1 == win2
	    || windows_are_group_related(win1, win2)
	    || windows_are_transient_related(win1, win2);
}

int windows_are_transient_related(struct window *win1, struct window *win2)
{
	return win1 == win2
	    || win1->wmtransientfor == win2->client
	    || win2->wmtransientfor == win1->client
	    || (win1->wmtransientfor != None
	        && win1->wmtransientfor == win2->wmtransientfor);
}

int windows_are_group_related(struct window *win1, struct window *win2)
{
	return win1 == win2
	    || (win1->wmhints != NULL && win2->wmhints != NULL
	        && win1->wmhints->flags & WindowGroupHint
	        && win2->wmhints->flags & WindowGroupHint
	        && win1->wmhints->window_group == win2->wmhints->window_group);
}

/*
 * Activate window.
 */
void set_active_window(struct window *win)
{
	struct window *old;

	if (win != NULL) {
		raise_window(win);
		if (!window_group_is_active(win))
			put_window_group_below(win);
		restack_transient_windows(win);
	}

	if (win == active)
		return;

	old = active;
	active = win;

	if (old != NULL) {
		clerr();
		grabbutton(display, AnyButton, 0, old->client, True,
		    ButtonPressMask, GrabModeSync, GrabModeSync, None, None);
		sterr();
		repaint_window_family(old);
		hints_deactivate(old);
	}

	if (win != NULL) {
		repaint_window_family(win);
		map_transient_windows(win);
		make_window_visible(win);

		clerr();
		XSetInputFocus(display, win->client, RevertToPointerRoot,
		    CurrentTime);
		ungrabbutton(display, AnyButton, 0, win->client);
		XAllowEvents(display, ReplayPointer, CurrentTime);
		sterr();

		if (win->menuitem != NULL)
			put_menuitem_first(win->menuitem);
	} else {
		XSetInputFocus(display, PointerRoot, RevertToPointerRoot,
		    CurrentTime);
	}

	hints_activate(win);
}

static void make_window_visible(struct window *win)
{
	int x, y;

	if (WIDGET_X(win) >= DisplayWidth(display, screen) - border_width ||
	    WIDGET_Y(win) >= DisplayHeight(display, screen) - border_width ||
	    WIDGET_X(win) + WIDGET_WIDTH(win) < border_width ||
	    WIDGET_Y(win) + WIDGET_HEIGHT(win) < border_width) {
		smartpos(WIDGET_WIDTH(win), WIDGET_HEIGHT(win), &x, &y);
		move_window(win, x, y);
	}

	if (!WIDGET_MAPPED(win))
		map_window(win);
}
