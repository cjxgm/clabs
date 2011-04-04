/*
 * Support for Extended Window Manager Hints
 * http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
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

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "hints.h"
#include "lib.h"
#include "window.h"

enum {
	NET_SUPPORTED,
	NET_CLIENT_LIST,
	NET_CLIENT_LIST_STACKING,
	NET_NUMBER_OF_DESKTOPS,
	NET_DESKTOP_GEOMETRY,
	NET_DESKTOP_VIEWPORT,
	NET_CURRENT_DESKTOP,
	NET_ACTIVE_WINDOW,
	NET_WORKAREA,
	NET_SUPPORTING_WM_CHECK,
	NET_WM_NAME,
	NET_CLOSE_WINDOW,

	NATOM
};

struct winlist {
	long *win;
	int len;
	int lim;
};

/* EWMH atoms, indexed by the above enum */
static Atom atom[NATOM];

/* The EWMH UTF-8 string type */
static Atom UTF8_STRING;

/* List of managed clients, oldest first */
static struct winlist clientlist = { NULL, 0, 0 };

/* Geometry of the workarea (x, y, width, height) */
static long workarea[4];

/* The window used for announcing EWMH support */
static Window supportwin = None;

static void ewmh_init(void);
static void ewmh_fini(void);
static void ewmh_manage(struct window *);
static void ewmh_unmanage(struct window *);
static void ewmh_activate(struct window *);
static void ewmh_restack(void);
static int ewmh_clientmessage(struct window *, XClientMessageEvent *);
static void addclient(Window);
static void delclient(Window);

struct hints ewmh_hints = {
	.init = ewmh_init,
	.fini = ewmh_fini,
	.manage = ewmh_manage,
	.unmanage = ewmh_unmanage,
	.activate = ewmh_activate,
	.restack = ewmh_restack,
	.clientmessage = ewmh_clientmessage,
};

static void ewmh_init(void)
{
	XSetWindowAttributes attr;
	long ndesk;
	long curdesk;
	long geom[2];
	long viewport[2];
	long active;

	attr.override_redirect = True;
	supportwin = XCreateWindow(display, root, 0, 0, 1, 1, 0,
	    CopyFromParent, InputOnly, CopyFromParent,
	    CWOverrideRedirect, &attr);

	ndesk = 1;
	curdesk = 0;
	geom[0] = DisplayWidth(display, screen);
	geom[1] = DisplayHeight(display, screen);
	viewport[0] = 0;
	viewport[1] = 0;
	active = None;
	workarea[0] = 0; /* x */
	workarea[1] = 0; /* y */
	workarea[2] = DisplayWidth(display, screen);
	workarea[3] = DisplayHeight(display, screen);

	UTF8_STRING = XInternAtom(display, "UTF8_STRING", False);

	atom[NET_SUPPORTED] = XInternAtom(display,
	    "_NET_SUPPORTED", False);
	atom[NET_CLIENT_LIST] = XInternAtom(display,
	    "_NET_CLIENT_LIST", False);
	atom[NET_CLIENT_LIST_STACKING] = XInternAtom(display,
	    "_NET_CLIENT_LIST_STACKING", False);
	atom[NET_NUMBER_OF_DESKTOPS] = XInternAtom(display,
	    "_NET_NUMBER_OF_DESKTOPS", False);
	atom[NET_DESKTOP_GEOMETRY] = XInternAtom(display,
	    "_NET_DESKTOP_GEOMETRY", False);
	atom[NET_DESKTOP_VIEWPORT] = XInternAtom(display,
	    "_NET_DESKTOP_VIEWPORT", False);
	atom[NET_CURRENT_DESKTOP] = XInternAtom(display,
	    "_NET_CURRENT_DESKTOP", False);
	atom[NET_ACTIVE_WINDOW] = XInternAtom(display,
	    "_NET_ACTIVE_WINDOW", False);
	atom[NET_WORKAREA] = XInternAtom(display,
	    "_NET_WORKAREA", False);
	atom[NET_SUPPORTING_WM_CHECK] = XInternAtom(display,
	    "_NET_SUPPORTING_WM_CHECK", False);
	atom[NET_WM_NAME] = XInternAtom(display,
	    "_NET_WM_NAME", False);
	atom[NET_CLOSE_WINDOW] = XInternAtom(display,
	    "_NET_CLOSE_WINDOW", False);

	XChangeProperty(display, root, atom[NET_CLIENT_LIST],
	    XA_WINDOW, 32, PropModeReplace, NULL, 0);
	XChangeProperty(display, root, atom[NET_CLIENT_LIST_STACKING],
	    XA_WINDOW, 32, PropModeReplace, NULL, 0);
	XChangeProperty(display, root, atom[NET_NUMBER_OF_DESKTOPS],
	    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&ndesk, 1);
	XChangeProperty(display, root, atom[NET_DESKTOP_GEOMETRY],
	    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&geom, 2);
	XChangeProperty(display, root, atom[NET_DESKTOP_VIEWPORT],
	    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&viewport, 2);
	XChangeProperty(display, root, atom[NET_CURRENT_DESKTOP],
	    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&curdesk, 1);
	XChangeProperty(display, root, atom[NET_ACTIVE_WINDOW],
	    XA_WINDOW, 32, PropModeReplace, (unsigned char *)&active, 1);
	XChangeProperty(display, root, atom[NET_WORKAREA],
	    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&workarea, 4);
	XChangeProperty(display, root, atom[NET_SUPPORTING_WM_CHECK],
	    XA_WINDOW, 32, PropModeReplace, (unsigned char *)&supportwin, 1);
	XChangeProperty(display, supportwin, atom[NET_SUPPORTING_WM_CHECK],
	    XA_WINDOW, 32, PropModeReplace, (unsigned char *)&supportwin, 1);
	XChangeProperty(display, supportwin, atom[NET_WM_NAME],
	    UTF8_STRING, 8, PropModeReplace,
	    (unsigned char *)"Karmen", 7);

	/* set this last, when everything is set up */
	XChangeProperty(display, root, atom[NET_SUPPORTED], XA_ATOM, 32,
	    PropModeReplace, (unsigned char *)atom, NELEM(atom));
}

static void ewmh_fini(void)
{
	/* delete this first, before we tear things down */
	XDeleteProperty(display, root, atom[NET_SUPPORTED]);

	XDeleteProperty(display, root, atom[NET_CLIENT_LIST]);
	XDeleteProperty(display, root, atom[NET_CLIENT_LIST_STACKING]);
	XDeleteProperty(display, root, atom[NET_NUMBER_OF_DESKTOPS]);
	XDeleteProperty(display, root, atom[NET_DESKTOP_GEOMETRY]);
	XDeleteProperty(display, root, atom[NET_DESKTOP_VIEWPORT]);
	XDeleteProperty(display, root, atom[NET_CURRENT_DESKTOP]);
	XDeleteProperty(display, root, atom[NET_ACTIVE_WINDOW]);
	XDeleteProperty(display, root, atom[NET_WORKAREA]);
	XDeleteProperty(display, root, atom[NET_SUPPORTING_WM_CHECK]);
	XDeleteProperty(display, supportwin, atom[NET_SUPPORTING_WM_CHECK]);
	XDeleteProperty(display, supportwin, atom[NET_WM_NAME]);

	XDestroyWindow(display, supportwin);
}

static void ewmh_manage(struct window *win)
{
	addclient(win->client);
	XChangeProperty(display, root, atom[NET_CLIENT_LIST],
	    XA_WINDOW, 32, PropModeReplace,
	    (unsigned char *)clientlist.win, clientlist.len);
}

static void ewmh_unmanage(struct window *win)
{
	delclient(win->client);
	XChangeProperty(display, root, atom[NET_CLIENT_LIST],
	    XA_WINDOW, 32, PropModeReplace,
	    (unsigned char *)clientlist.win, clientlist.len);
}

static void ewmh_activate(struct window *win)
{
	Window w;

	w = win == NULL ? None : win->client;
	XChangeProperty(display, root, atom[NET_ACTIVE_WINDOW],
	    XA_WINDOW, 32, PropModeReplace, (unsigned char *)&w, 1);
}

static void ewmh_restack(void)
{
	Window *stack, w;
	int i, j, n;

	get_client_stack(&stack, &n);

	/* reverse the stack */
	for (i = 0, j = n - 1; i < j; i++, j--) {
		w = stack[i];
		stack[i] = stack[j];
		stack[j] = w;
	}

	XChangeProperty(display, root, atom[NET_CLIENT_LIST_STACKING],
	    XA_WINDOW, 32, PropModeReplace,
	    (unsigned char *)stack, n);

	FREE(stack);
}

static int ewmh_clientmessage(struct window *win, XClientMessageEvent *ep)
{
	int type = ep->message_type;
	int format = ep->format;

	if (type == atom[NET_ACTIVE_WINDOW] && format == 32) {
		set_active_window(win);
	} else if (type == atom[NET_CLOSE_WINDOW] && format == 32) {
		delete_window(win);
	} else
		return 0;

	return 1;
}

static void addclient(Window w)
{
	if (clientlist.len == clientlist.lim) {
		clientlist.lim += 32;
		clientlist.win = REALLOC(clientlist.win,
		    clientlist.lim * sizeof (long));
	}
	clientlist.win[clientlist.len++] = (long)w;
}

static void delclient(Window w)
{
	int i;

	if (clientlist.len == 0)
		return;

	clientlist.len--;
	for (i = 0; i < clientlist.len && clientlist.win[i] != (long)w; i++)
		;
	if (i < clientlist.len) {
		for (; i < clientlist.len; i++)
			clientlist.win[i] = clientlist.win[i + 1];
	}

	if (clientlist.len == 0) {
		FREE(clientlist.win);
		clientlist.win = NULL;
		clientlist.lim = 0;
	}
}
