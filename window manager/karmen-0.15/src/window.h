#if !defined(WINDOW_H)
#define WINDOW_H

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

#include <X11/Xlib.h>

#include "list.h"
#include "widget.h"

struct button;
struct menuitem;
struct resizer;
struct title;

struct window { struct widget widget;
	char *name;
	char *iconname;

	struct title *title;

	struct button *deletebtn;
	struct button *unmapbtn;

	struct resizer *rsz_northwest, *rsz_north, *rsz_northeast,
	    *rsz_west, *rsz_east, *rsz_southwest, *rsz_south, *rsz_southeast;

	struct menuitem *menuitem;

	struct color *color;

	Window client;

	Colormap colormap;

	/* ICCCM hints */
	XSizeHints *wmnormalhints;
	XWMHints *wmhints;
	Window wmtransientfor;

	int cborder;	/* client's initial border width */

	/* Meta-Button1 moving of window */
	struct {
		int moving;
		int xoff;
		int yoff;
	} altmove;

	int ignoreunmap;
	int maximized;

	struct dim odim;	/* remembered dim while maximized */

	LIST *layer;
	LIST layerlink;
};

void window_init(void);
void window_fini(void);
struct window *manage_window(Window, int);
int window_is_active(struct window *);
int window_is_transient_active(struct window *);
int window_group_is_active(struct window *);
int window_family_is_active(struct window *);
int windows_are_related(struct window *, struct window *);
int windows_are_transient_related(struct window *, struct window *);
int windows_are_group_related(struct window *, struct window *);
void window_calcsize(struct window *, int, int, int *, int *, int *, int *);
void delete_window(struct window *);
void fetch_icon_name(struct window *);
void fetch_window_name(struct window *);
void fetch_wm_hints(struct window *);
void fetch_wm_normal_hints(struct window *);
void fetch_wm_transient_for_hint(struct window *);
void raise_window(struct window *);
void lower_window(struct window *);
void toggle_window_ontop(struct window *);
int window_is_ontop(struct window *);
void map_window(struct window *);
void maximize_window(struct window *);
void move_window(struct window *, int, int);
void move_window_family(struct window *, int, int);
void moveresize_window(struct window *, int, int, int, int);
void repaint_window(struct window *);
void resize_window(struct window *, int, int);
void set_active_window(struct window *);
void unmanage_window(struct window *, int);
void unmap_window(struct window *);
void user_unmap_window(struct window *);
void restack_all_windows(void);
void get_client_stack(Window **, int *);

#endif /* !defined(WINDOW_H) */
