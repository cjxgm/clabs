/*
 * Program initialization and event loop
 *
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
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include "global.h"
#include "lib.h"
#include "hints.h"
#include "menu.h"
#include "window.h"

#include "delete.xbm"
#include "unmap.xbm"

#define BORDERWIDTH_MIN  3

Display *display;
int screen;
Window root;

struct menu *winmenu;

XFontStruct *font;

int border_width = 4;
int button_size;
int title_pad;

struct color color_title_active_fg;
struct color color_title_active_bg;
struct color color_title_inactive_fg;
struct color color_title_inactive_bg;
struct color color_menu_fg;
struct color color_menu_bg;
struct color color_menu_selection_fg;
struct color color_menu_selection_bg;

DEFINE_IMAGE(delete_image, delete);
DEFINE_IMAGE(unmap_image, unmap);

static char *displayname = NULL;

static const char *ftnames[] = {
	/* This will be overwritten by user supplied font name */
	DEFAULT_FONT,

	/* This is the one that looks best */
	DEFAULT_FONT,

	/* This is the one that exists */
	"fixed"
};

static XrmOptionDescRec options[] = {
	/*
	 * Documented options
	 */
	{ "-display", ".display", XrmoptionSepArg, NULL },
	{ "-font", ".font", XrmoptionSepArg, NULL },
	{ "-help", ".showHelp", XrmoptionNoArg, "True" },
	{ "-version", ".showVersion", XrmoptionNoArg, "True" },

	/*
	 * Undocumented options
	 */
	{ "-afg", ".title.active.foreground", XrmoptionSepArg, NULL },
	{ "-abg", ".title.active.background", XrmoptionSepArg, NULL },
	{ "-ifg", ".title.inactive.foreground", XrmoptionSepArg, NULL },
	{ "-ibg", ".title.inactive.background", XrmoptionSepArg, NULL },
	{ "-mfg", ".menu.foreground", XrmoptionSepArg, NULL },
	{ "-mbg", ".menu.background", XrmoptionSepArg, NULL },
	{ "-msfg", ".menu.selection.foreground", XrmoptionSepArg, NULL },
	{ "-msbg", ".menu.selection.background", XrmoptionSepArg, NULL },
	{ "-bw", ".border.width", XrmoptionSepArg, NULL },
	{ "-xrm", NULL, XrmoptionResArg, NULL },
};

#define BLACK		"rgb:00/00/00"
#define WHITE		"rgb:ff/ff/ff"
#define GTK_LIGHTGRAY	"rgb:dc/da/d5"
#define GTK_DARKGRAY	"rgb:ac/aa/a5"
#define GTK_BLUE	"rgb:4b/69/83"
#define FRESH_BLUE	"rgb:c0/d0/e0"

static char *colorname_title_active_fg = BLACK;
static char *colorname_title_active_bg = GTK_LIGHTGRAY;
static char *colorname_title_inactive_fg = BLACK;
static char *colorname_title_inactive_bg = GTK_DARKGRAY;
static char *colorname_menu_fg = BLACK;
static char *colorname_menu_bg = WHITE;
static char *colorname_menu_selection_fg = BLACK;
static char *colorname_menu_selection_bg = GTK_LIGHTGRAY;

/* The signals that we care about */
static const int sigv[] = { SIGHUP, SIGINT, SIGTERM };

/* The self-pipe for delivering signals */
static int sigpipe[2] = { -1, -1 };

/* If greater than zero, don't report X errors */
static int errlev = 0;

static int xerr_report(Display *dpy, XErrorEvent *ep)
{
	static char buf[256];

	if (ep->error_code == BadAccess && ep->resourceid == root) {
		error("another window manager is already running "
		      "on display \"%s\"", XDisplayName(displayname));
		exit(1);
	} else {
		XGetErrorText(dpy, ep->error_code, buf, sizeof buf);
		error("%s", buf);
		return 0;
	}
}

static int xerr_ignore(Display *dpy, XErrorEvent *e)
{
	return 0;
}

void clerr(void)
{
	assert(errlev >= 0);

	if (errlev++ == 0) {
		XGrabServer(display);
		XSetErrorHandler(xerr_ignore);
	}
}

void sterr(void)
{
	assert(errlev >= 1);

	if (--errlev == 0) {
		XSync(display, False);
		XUngrabServer(display);
		XSetErrorHandler(xerr_report);
	}
}

static void sighandler(int signo)
{
	int e = errno;
	char c = signo;

	if (sigpipe[1] != -1)
		write(sigpipe[1], &c, 1);
	errno = e;
}

static void die(int signo)
{
	struct sigaction sigact;

	window_fini();
	hints_fini();
	destroy_menu(winmenu);
	widget_fini();

	XFreeFont(display, font);

	XSetInputFocus(display, PointerRoot, RevertToPointerRoot, CurrentTime);
	XCloseDisplay(display);

	if (sigpipe[0] != -1)
		close(sigpipe[0]);
	if (sigpipe[1] != -1)
		close(sigpipe[1]);

	if (signo > 0) {
		sigact.sa_handler = SIG_DFL;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = 0;
		sigaction(signo, &sigact, NULL);
		raise(signo);
	} else
		exit(1);
}

static int readsig(int fd)
{
	char c = 42;

	if (read(fd, &c, 1) == -1)
		return 0;
	else
		return c;
}

static void waitevent(void)
{
	struct pollfd pfd[2];
	int res;

	pfd[0].fd = ConnectionNumber(display);
	pfd[0].events = POLLIN;

	pfd[1].fd = sigpipe[0];
	pfd[1].events = POLLIN;

	for (;;) {
		do {
			res = poll(pfd, 2, -1);
		} while (res == -1 && (errno == EINTR || errno == EAGAIN));

		if (res == -1) {
			error("poll: %s", strerror(errno));
			die(0);
		} else {
			int sig;

			if (pfd[1].revents == POLLIN &&
			    (sig = readsig(pfd[1].fd)) != 0) {
				/* Signal received on our self-pipe */
				debug("terminating on signal %d", sig);
				die(sig);
			} else if (pfd[0].revents == POLLIN) {
				/* Event received */
				return;
			} else if (pfd[0].revents == POLLERR) {
				/* X connection broken */
				error("X connection broken");
				die(0);
			}
		}
	}
}

static void nextevent(XEvent *ep)
{
	XFlush(display);
	if (XQLength(display) == 0)
		waitevent();
	XNextEvent(display, ep);
}

static unsigned short scalepixel(unsigned c, double d)
{
	double r;

	r = c + 65535. * d;
	r = MIN(r, 65535.);
	r = MAX(r, 0.);
	return r;
}

static void scalecolor(XColor *rp, const XColor *cp, double d)
{
	rp->red = scalepixel(cp->red, d);
	rp->green = scalepixel(cp->green, d);
	rp->blue = scalepixel(cp->blue, d);
}

static void mkcolor(struct color *color, const char *name)
{
	XColor tc, sc;

	XAllocNamedColor(display, DefaultColormap(display, screen),
	    name, &sc, &tc);

	color->normal = sc.pixel;

	scalecolor(&sc, &tc, -.07);
	XAllocColor(display, DefaultColormap(display, screen), &sc);
	color->shadow1 = sc.pixel;

	scalecolor(&sc, &tc, -.25);
	XAllocColor(display, DefaultColormap(display, screen), &sc);
	color->shadow2 = sc.pixel;

	scalecolor(&sc, &tc, +.07);
	XAllocColor(display, DefaultColormap(display, screen), &sc);
	color->bright1 = sc.pixel;

	scalecolor(&sc, &tc, +.25);
	XAllocColor(display, DefaultColormap(display, screen), &sc);
	color->bright2 = sc.pixel;
}

static void usage(FILE *fp)
{
	fprintf(fp,
"usage: karmen [-display name] [-font name] [-help] [-version]\n");
}

static void loadres(XrmDatabase db)
{
	XrmValue val;
	char *type;

	if (XrmGetResource(db, "karmen.showHelp",
	    "Karmen.ShowHelp", &type, &val)) {
		usage(stdout);
		exit(0);
	}

	if (XrmGetResource(db, "karmen.showVersion",
	    "Karmen.ShowVersion", &type, &val)) {
		printf("%s\n", PACKAGE_STRING);
		exit(0);
	}

	if (XrmGetResource(db, "karmen.display",
	    "Karmen.Display", &type, &val))
		displayname = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.title.active.foreground",
	    "Karmen.Title.Active.Foreground", &type, &val))
		colorname_title_active_fg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.title.active.background",
	    "Karmen.Title.Active.Background", &type, &val))
		colorname_title_active_bg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.title.inactive.foreground",
	    "Karmen.Title.Inactive.Foreground", &type, &val))
		colorname_title_inactive_fg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.title.inactive.background",
	    "Karmen.Title.Inactive.Background", &type, &val))
		colorname_title_inactive_bg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.menu.foreground",
	    "Karmen.Menu.Foreground", &type, &val))
		colorname_menu_fg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.menu.background",
	    "Karmen.Menu.Background", &type, &val))
		colorname_menu_bg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.menu.selection.foreground",
	    "Karmen.Menu.Selection.Foreground", &type, &val))
		colorname_menu_selection_fg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.menu.selection.background",
	    "Karmen.Menu.Selection.Background", &type, &val))
		colorname_menu_selection_bg = STRDUP((char *)val.addr);

	if (XrmGetResource(db, "karmen.border.width",
	    "Karmen.Border.Width", &type, &val))
		border_width = MAX(BORDERWIDTH_MIN, atoi((char *)val.addr));

	if (XrmGetResource(db, "karmen.font", "Karmen.Font", &type, &val))
		 ftnames[0] = STRDUP((char *)val.addr);
}

static void loadfont(void)
{
	int i;

	for (i = 0; i < NELEM(ftnames); i++) {
		font = XLoadQueryFont(display, ftnames[i]);
		if (font != NULL)
			return;
		error("can't load font \"%s\"", ftnames[i]);
	}
	error("fatal: no more fonts");
	exit(1);
}

static void init(int *argcp, char *argv[])
{
	XrmDatabase adb, db;
	struct sigaction sigact, oldact;
	char *dbstr;
	int i;

	pipe(sigpipe);
	fcntl(sigpipe[0], F_SETFL, O_NONBLOCK);
	fcntl(sigpipe[1], F_SETFL, O_NONBLOCK);

	/*
	 * Set up signal handlers for those that were not ignored.
	 */
	sigact.sa_handler = sighandler;
	sigact.sa_flags = 0;
	sigfillset(&sigact.sa_mask);
	for (i = 0; i < NELEM(sigv); i++) {
		sigaction(sigv[i], NULL, &oldact);
		if (oldact.sa_handler != SIG_IGN)
			sigaction(sigv[i], &sigact, NULL);
	}

	XSetErrorHandler(xerr_report);

	XrmInitialize();

	/* Load command line options before XOpenDisplay to read -display */
	adb = NULL;
	XrmParseCommand(&adb, options, NELEM(options), "karmen", argcp, argv);
	if (*argcp > 1) {
		for (i = 1; i < *argcp; i++)
			error("invalid option: %s", argv[i]);
		usage(stderr);
		exit(1);
	}
	loadres(adb);

	if ((display = XOpenDisplay(displayname)) == NULL) {
		error("can't open display \"%s\"", XDisplayName(displayname));
		exit(1);
	}

	/* Load the default database. */
	db = NULL;
	if ((dbstr = XResourceManagerString(display)) != NULL)
		db = XrmGetStringDatabase(dbstr);
	XrmMergeDatabases(adb, &db);
	loadres(db);

	XrmDestroyDatabase(db);

	screen = DefaultScreen(display);
	root = DefaultRootWindow(display);

	XSelectInput(display, root, ButtonPressMask |
	    SubstructureRedirectMask | SubstructureNotifyMask |
	    KeyPressMask | KeyReleaseMask);

	grabkey(display, XKeysymToKeycode(display, XK_Tab),
	    Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
	grabkey(display, XKeysymToKeycode(display, XK_Tab),
	    ShiftMask | Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

	grabkey(display, XKeysymToKeycode(display, XK_Return),
	    Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

	grabkey(display, XKeysymToKeycode(display, XK_space),
	    Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

	grabkey(display, XKeysymToKeycode(display, XK_Escape),
	    Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

	grabkey(display, XKeysymToKeycode(display, XK_BackSpace),
	    Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
	grabkey(display, XKeysymToKeycode(display, XK_BackSpace),
	    ShiftMask | Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

	loadfont();

	title_pad = 1 + MAX(1, (font->ascent + font->descent) / 10);
	button_size = font->ascent + font->descent + 2 * title_pad;
	if ((button_size & 1) == 1)
		button_size++;
	button_size++;

	mkcolor(&color_title_active_fg, colorname_title_active_fg);
	mkcolor(&color_title_active_bg, colorname_title_active_bg);
	mkcolor(&color_title_inactive_fg, colorname_title_inactive_fg);
	mkcolor(&color_title_inactive_bg, colorname_title_inactive_bg);
	mkcolor(&color_menu_fg, colorname_menu_fg);
	mkcolor(&color_menu_bg, colorname_menu_bg);
	mkcolor(&color_menu_selection_fg, colorname_menu_selection_fg);
	mkcolor(&color_menu_selection_bg, colorname_menu_selection_bg);
}

static void handlekey(XKeyEvent *ep)
{
	static int cycling = 0;

	switch (XKeycodeToKeysym(display, ep->keycode, 0)) {
	case XK_Meta_L:
	case XK_Meta_R:
	case XK_Alt_L:
	case XK_Alt_R:
		if (ep->type == KeyRelease) {
			/* end window cycling */
			if (cycling) {
				cycling = 0;
				XUngrabKeyboard(display, CurrentTime);
				select_current_menuitem(winmenu);
				hide_menu(winmenu);
			}
		}
		break;
	case XK_Tab:
		if (ep->type == KeyPress) {
			cycling = 1;

			/* Listen for Alt/Meta release */
			XGrabKeyboard(display, root, True,
			    GrabModeAsync, GrabModeAsync, CurrentTime);

			if (!WIDGET_MAPPED(winmenu)) {
				int x = DisplayWidth(display, screen) / 2
				    - WIDGET_WIDTH(winmenu) / 2;
				int y = DisplayHeight(display, screen) / 2
				    - WIDGET_HEIGHT(winmenu) / 2;
				show_menu(winmenu, x, y, -1);
			}

			if (winmenu->current == -1)
				winmenu->current = ep->state & ShiftMask ?
				    winmenu->nitems - 1 : 1;
			else
				winmenu->current +=
				    ep->state & ShiftMask ? -1 : 1;

			if (winmenu->current >= winmenu->nitems)
				winmenu->current = 0;
			else if (winmenu->current < 0)
				winmenu->current = winmenu->nitems - 1;

			REPAINT(winmenu);
		}
		break;
	case XK_Return:
		if (ep->type == KeyPress && active != NULL)
			maximize_window(active);
		break;
	case XK_space:
		if (ep->type == KeyPress && active != NULL)
			toggle_window_ontop(active);
		break;
	case XK_Escape:
		if (cycling) {
			cycling = 0;
			hide_menu(winmenu);
			XUngrabKeyboard(display, CurrentTime);
		} else if (ep->type == KeyPress && active != NULL)
			user_unmap_window(active);
		break;
	case XK_BackSpace:
		if (ep->type == KeyPress && active != NULL) {
			if (ep->state & ShiftMask) {
				clerr();
				XKillClient(display, active->client);
				sterr();
			} else
				delete_window(active);
		}
		break;
	default:
		debug("handlekey(): Unhandled key");
		break;
	}
}

static void configrequest(XConfigureRequestEvent *conf)
{
	XWindowChanges wc;

	wc.x = conf->x;
	wc.y = conf->y;
	wc.width = conf->width;
	wc.height = conf->height;
	wc.border_width = conf->border_width;
	wc.sibling = conf->above;
	wc.stack_mode = conf->detail;

	clerr();
	XConfigureWindow(display, conf->window,
	    conf->value_mask, &wc);
	sterr();
}

static Window xeventwindow(XEvent *ep)
{
	switch (ep->type) {
	case ConfigureRequest:
		/*
		 * For some reason, the first configure request
		 * received from a client sometimes has the root
		 * window as its parent even though we have already
		 * managed to reparent it, and since xany.window maps
		 * to xmaprequest.parent we won't be able to figure
		 * out what window it is unless we read the
		 * xconfigurerequest.window member instead.
		 *
		 * I spent so much time finding this out ...
		 */
		return ep->xconfigurerequest.window;

	case MapRequest:
		/*
		 * A map request event's xany.window member maps
		 * to xmaprequest.parent, which will be the root
		 * window when a client maps itself.  We are
		 * interested in the xmaprequest.window member.
		 */
		return ep->xmaprequest.window;

	default:
		/*
		 * For most event types, the xany.window member
		 * is what we're interested in.
		 */
		return ep->xany.window;
	}
}

static void mainloop(void)
{
	XEvent e;
	Window xwindow;
	struct widget *widget;

	for (;;) {
		restack_all_windows();
		repaint_widgets();
		nextevent(&e);
		xwindow = xeventwindow(&e);
		widget = find_widget(xwindow, WIDGET_ANY);
		if (widget != NULL) {
			if (widget->event != NULL)
				widget->event(widget, &e);
		} else {
			switch (e.type) {
			case MapRequest:
				manage_window(xwindow, 0);
				break;
			case ConfigureRequest:
				configrequest(&e.xconfigurerequest);
				break;
			case ButtonPress:
				if (e.xbutton.window == root &&
				    e.xbutton.subwindow == None &&
				    e.xbutton.button == Button3)
					show_menu(winmenu,
					    e.xbutton.x, e.xbutton.y,
					    e.xbutton.button);
				break;
			case KeyPress:
			case KeyRelease:
				if (e.xkey.window == root)
					handlekey(&e.xkey);
				break;
			case ClientMessage:
			case CreateNotify:
			case DestroyNotify:
			case ConfigureNotify:
			case ReparentNotify:
			case MapNotify:
			case UnmapNotify:
				/* ignore */
				break;
			default:
				debug("mainloop(): unhandled event -- %s (%d)",
				    eventname(e.type), e.type);
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	init(&argc, argv);

	widget_init();
	winmenu = create_menu();
	hints_init();
	window_init();
	mainloop();
	return 0;
}
