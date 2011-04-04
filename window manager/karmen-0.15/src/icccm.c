/*
 * Inter-Client Communication Conventions Manual (ICCCM) hints
 *
 * NOTE: Since the ICCCM is so closely tied to window management, much
 *       of the standard is hardwired into "window.c".
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "hints.h"
#include "lib.h"
#include "window.h"

static Atom WM_CHANGE_STATE;
static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
static Atom WM_STATE;

static void icccm_init(void);
static void icccm_manage(struct window *);
static void icccm_map(struct window *);
static void icccm_unmap(struct window *);
static void icccm_withdraw(struct window *);
static void icccm_activate(struct window *);
static void icccm_deactivate(struct window *);
static void icccm_move(struct window *);
static int icccm_clientmessage(struct window *, XClientMessageEvent *);
static int icccm_propertynotify(struct window *, XPropertyEvent *);
static int icccm_delete(struct window *);
static int knowsproto(struct window *, Atom);
static void sendmesg(struct window *, Atom, long);
static void sendconf(struct window *);
static void setwmstate(struct window *, long);

struct hints icccm_hints = {
	.name = "Inter-Client Communication Conventions Manual (ICCCM)",

	.init = icccm_init,
	.manage = icccm_manage,
	.map = icccm_map,
	.unmap = icccm_unmap,
	.withdraw = icccm_withdraw,
	.activate = icccm_activate,
	.deactivate = icccm_deactivate,
	.move = icccm_move,
	.clientmessage = icccm_clientmessage,
	.propertynotify = icccm_propertynotify,
	.delete = icccm_delete,
};

static void icccm_init(void)
{
	WM_CHANGE_STATE = XInternAtom(display, "WM_CHANGE_STATE", False);
	WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
	WM_PROTOCOLS = XInternAtom(display, "WM_PROTOCOLS", False);
	WM_STATE = XInternAtom(display, "WM_STATE", False);
}

static void icccm_manage(struct window *win)
{
	sendconf(win);
}

static void icccm_map(struct window *win)
{
	setwmstate(win, NormalState);
}

static void icccm_unmap(struct window *win)
{
	setwmstate(win, IconicState);
}

static void icccm_withdraw(struct window *win)
{
	setwmstate(win, WithdrawnState);
}

static void icccm_activate(struct window *win)
{
	if (win == NULL)
		XInstallColormap(display, DefaultColormap(display, screen));
	else
		XInstallColormap(display, win->colormap);
}

static void icccm_deactivate(struct window *win)
{
	XUninstallColormap(display, win->colormap);
}

static void icccm_move(struct window *win)
{
	sendconf(win);
}

static int icccm_clientmessage(struct window *win, XClientMessageEvent *ep)
{
	if (ep->message_type == WM_CHANGE_STATE && ep->format == 32) {
		switch (ep->data.l[0]) {
		case IconicState:
			unmap_window(win);
			return 1;
		case NormalState:
			map_window(win);
			return 1;
		}
	}
	return 0;
}

static int icccm_propertynotify(struct window *win, XPropertyEvent *ep)
{
	switch (ep->atom) {
	case XA_WM_NAME:
		if (ep->state != PropertyDelete)
			fetch_window_name(win);
		return 1;
	case XA_WM_ICON_NAME:
		if (ep->state != PropertyDelete)
			fetch_icon_name(win);
		return 1;
	case XA_WM_NORMAL_HINTS:
		fetch_wm_normal_hints(win);
		return 1;
	case XA_WM_HINTS:
		fetch_wm_hints(win);
		return 1;
	case XA_WM_TRANSIENT_FOR:
		fetch_wm_transient_for_hint(win);
		return 1;
	}
	return 0;
}

static int icccm_delete(struct window *win)
{
	if (knowsproto(win, WM_DELETE_WINDOW)) {
		sendmesg(win, WM_PROTOCOLS, WM_DELETE_WINDOW);
		return 1;
	} else
		return 0;
}

static int knowsproto(struct window *win, Atom proto)
{
	Atom *protocols;
	int i, n;
	int found;

	found = 0;
	clerr();
	if (XGetWMProtocols(display, win->client, &protocols, &n)) {
		for (i = 0; !found && i < n; i++) {
			if (protocols[i] == proto)
				found = 1;
		}
		if (protocols != NULL)
			XFree(protocols);
	}
	sterr();
	return found;
}

static void sendmesg(struct window *win, Atom type, long value)
{
	XEvent ev;

	memset(&ev, 0, sizeof ev);
	ev.xclient.type = ClientMessage;
	ev.xclient.window = win->client;
	ev.xclient.message_type = type;
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = value;
	ev.xclient.data.l[1] = CurrentTime;

	clerr();
	XSendEvent(display, win->client, False, 0L, &ev);
	sterr();
}

static void sendconf(struct window *win)
{
	XConfigureEvent conf;

	conf.type = ConfigureNotify;
	conf.event = win->client;
	conf.window = win->client;
	conf.x = WIDGET_X(win) + border_width - win->cborder;
	conf.y = WIDGET_Y(win) + border_width + button_size - win->cborder;
	conf.width = WIDGET_WIDTH(win) - 2 * border_width;
	conf.height = WIDGET_HEIGHT(win) - (2 * border_width + button_size);
	conf.border_width = win->cborder;
	conf.above = None;
	conf.override_redirect = False;

	clerr();
	XSendEvent(display, win->client, False, StructureNotifyMask,
	    (XEvent *)&conf);
	sterr();
}

static void setwmstate(struct window *win, long state)
{
	long data[2];

	data[0] = state;
	data[1] = (long)None;

	clerr();
	XChangeProperty(display, win->client, WM_STATE, WM_STATE, 32,
	    PropModeReplace, (unsigned char *)data, 2);
	sterr();
}
