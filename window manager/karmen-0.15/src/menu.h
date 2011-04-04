#if !defined(MENU_H)
#define MENU_H

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

struct menuitem {
	struct menu *menu;
	char *name;
	void (*select)(void *);
	void *arg;
	LIST itemlink;
};

struct menu { struct widget widget;
	GC gc;
	Pixmap pixmap;
	int pixmapwidth;
	int pixmapheight;
	int current;
	int button;
	int nitems;
	LIST itemlist;
};

struct menu *create_menu(void);
void destroy_menu(struct menu *);
struct menuitem *create_menuitem(struct menu *, const char *,
    void (*)(void *), void *);
void destroy_menuitem(struct menuitem *);
void hide_menu(struct menu *);
void put_menuitem_first(struct menuitem *);
void put_menuitem_last(struct menuitem *);
void show_menu(struct menu *, int, int, int);
void rename_menuitem(struct menuitem *, const char *);
void select_current_menuitem(struct menu *);

#endif /* !defined(MENU_H) */
