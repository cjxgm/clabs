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
#include <stdio.h>
#include <string.h>

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "list.h"
#include "menu.h"

#define ITEMPAD		MAX(1, (font->ascent + font->descent) / 4)
#define ITEMHEIGHT	(font->ascent + font->descent + 2 * ITEMPAD)
#define MAXWIDTH	(DisplayWidth(display, screen) / 4)

static Cursor cursor = None;

static void menuevent(struct widget *, XEvent *);
static void trim(struct menu *);

/*
 * Perform the action of the currently selected item.
 */
void select_current_menuitem(struct menu *menu)
{
	struct menuitem *ip;
	LIST *lp;
	int i;

	i = 0;
	LIST_FOREACH(lp, &menu->itemlist) {
		if (i == menu->current) {
			ip = LIST_ITEM(lp, struct menuitem, itemlink);
			ip->select(ip->arg);
			return;
		}
		i++;
	}
}

void put_menuitem_first(struct menuitem *item)
{
	if (item->menu != NULL) {
		LIST_REMOVE(&item->itemlink);
		LIST_INSERT_HEAD(&item->menu->itemlist, &item->itemlink);
		REPAINT(item->menu);
	}
}

void put_menuitem_last(struct menuitem *item)
{
	if (item->menu != NULL) {
		LIST_REMOVE(&item->itemlink);
		LIST_INSERT_TAIL(&item->menu->itemlist, &item->itemlink);
		REPAINT(item->menu);
	}
}

static void prepare_repaint(struct widget *widget)
{
	struct menu *menu = (struct menu *)widget;

	XSetWindowBackground(display, WIDGET_XWINDOW(menu),
	    color_menu_bg.normal);
}

static void repaint(struct widget *widget)
{
	struct menu *menu = (struct menu *)widget;
	struct menuitem *ip;
	LIST *lp;
	int i;

	/* clear */
	XSetForeground(display, menu->gc, color_menu_fg.normal);
	XFillRectangle(display, menu->pixmap, menu->gc,
	    0, 0, WIDGET_WIDTH(menu), WIDGET_HEIGHT(menu));
	XSetForeground(display, menu->gc, color_menu_bg.normal);
	XFillRectangle(display, menu->pixmap, menu->gc,
	    1, 1, WIDGET_WIDTH(menu) - 2, WIDGET_HEIGHT(menu) - 2);

	i = 0;
	LIST_FOREACH(lp, &menu->itemlist) {
		ip = LIST_ITEM(lp, struct menuitem, itemlink);
		if (i == menu->current) {
			XSetForeground(display, menu->gc,
			    color_menu_selection_bg.shadow2);
			XDrawLine(display, menu->pixmap, menu->gc,
			    1,
			    1 + ITEMPAD + i * ITEMHEIGHT,
			    WIDGET_WIDTH(menu) - 2,
			    1 + ITEMPAD + i * ITEMHEIGHT);
			XDrawLine(display, menu->pixmap, menu->gc,
			    1,
			    1 + ITEMPAD + i * ITEMHEIGHT + ITEMHEIGHT - 1,
			    WIDGET_WIDTH(menu) - 2,
			    1 + ITEMPAD + i * ITEMHEIGHT + ITEMHEIGHT - 1);

			XSetForeground(display, menu->gc,
			    color_menu_selection_bg.normal);
			XFillRectangle(display, menu->pixmap, menu->gc,
			    1, 1 + ITEMPAD + i * ITEMHEIGHT + 1,
			    WIDGET_WIDTH(menu) - 2, ITEMHEIGHT - 2);

			XSetForeground(display, menu->gc,
			    color_menu_selection_fg.normal);
		} else
			XSetForeground(display, menu->gc,
			    color_menu_fg.normal);
		XDrawString(display, menu->pixmap, menu->gc,
		    1 + ITEMHEIGHT,
		    1 + ITEMPAD + i * ITEMHEIGHT + ITEMPAD + font->ascent,
		    ip->name, strlen(ip->name));
		i++;
	}

	/* show */
	if (WIDGET_MAPPED(menu))
		XCopyArea(display,
		    menu->pixmap, WIDGET_XWINDOW(menu), menu->gc,
		    0, 0, WIDGET_WIDTH(menu), WIDGET_HEIGHT(menu), 0, 0);
}

/*
 * Pop up and activate the menu at position (x, y).
 */
void show_menu(struct menu *menu, int x, int y, int button)
{
	int dw = DisplayWidth(display, screen);
	int dh = DisplayHeight(display, screen);

	if (LIST_EMPTY(&menu->itemlist))
		return;

	if (x + WIDGET_WIDTH(menu) >= dw)
		x = MAX(0, x - WIDGET_WIDTH(menu) + 1);

	if (y + WIDGET_HEIGHT(menu) >= dh)
		y = MAX(0, y - WIDGET_HEIGHT(menu) + 1);

	menu->button = button;

	move_widget(&menu->widget, x, y);
	XRaiseWindow(display, WIDGET_XWINDOW(menu));
	map_widget(&menu->widget);
	if (button != -1) {
		XGrabPointer(display, WIDGET_XWINDOW(menu), False,
		    ButtonPressMask | ButtonReleaseMask |
		    ButtonMotionMask | PointerMotionMask,
		    GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	}
}

/*
 * Hide and inactivate the menu.
 */
void hide_menu(struct menu *menu)
{
	unmap_widget(&menu->widget);
	if (menu->button != -1)
		XUngrabPointer(display, CurrentTime);
	menu->current = -1;
	REPAINT(menu);
}

/*
 * Handle menu events.
 */
static void menuevent(struct widget *widget, XEvent *ep)
{
	struct menu *mp = (struct menu *)widget;
	int tmp;

	switch (ep->type) {
	case MotionNotify:
		tmp = mp->current;
		if (ep->xmotion.x < 0 ||
		    ep->xmotion.x > WIDGET_WIDTH(mp) - 1 ||
		    ep->xmotion.y < 0 ||
		    ep->xmotion.y > WIDGET_HEIGHT(mp) - 1) {
			/* Outside menu window */
			if (mp->current != -2)
				mp->current = -1;
		} else if (ep->xmotion.x < 1 ||
		           ep->xmotion.x > WIDGET_WIDTH(mp) - 2 ||
		           ep->xmotion.y < 1 + ITEMPAD ||
		           ep->xmotion.y > WIDGET_HEIGHT(mp) - 2 - ITEMPAD) {
			/* Inside menu window but not on an item */
			mp->current = -1;
		} else {
			/* On an item */
			mp->current =
			    (ep->xmotion.y - ITEMPAD - 1) / ITEMHEIGHT;
		}
		if (tmp != mp->current)
			REPAINT(mp);
		break;
	case ButtonPress:
		if (mp->current == -1 &&
		    (ep->xbutton.x <= 0 ||
		     ep->xbutton.x >= WIDGET_WIDTH(mp) - 1 ||
		     ep->xbutton.y <= 0 ||
		     ep->xbutton.y >= WIDGET_HEIGHT(mp) - 1))
			mp->current = -2;
		break;
	case ButtonRelease:
		if (mp->current == -2)
			hide_menu(mp);
		else if (mp->current != -1 &&
		         (ep->xbutton.button == mp->button ||
		          ep->xbutton.button == 1)) {
			select_current_menuitem(mp);
			hide_menu(mp);
		}
		break;
	case Expose:
		XCopyArea(display, mp->pixmap, WIDGET_XWINDOW(mp),
		    mp->gc, ep->xexpose.x, ep->xexpose.y,
		    ep->xexpose.width, ep->xexpose.height,
		    ep->xexpose.x, ep->xexpose.y);
		break;
	default:
		debug("menuevent: %s (%d)", eventname(ep->type), ep->type);
		break;
	}
}

/*
 * Create an empty menu.
 */
struct menu *create_menu(void)
{
	XSetWindowAttributes attr;
	XGCValues gcval;
	struct menu *mp;

	mp = MALLOC(sizeof (struct menu));

	create_widget(&mp->widget, WIDGET_MENU, root, InputOutput,
	    0, 0, 1, 1);
	attr.save_under = True;
	XChangeWindowAttributes(display, WIDGET_XWINDOW(mp),
	    CWSaveUnder, &attr);

	if (cursor == None)
		cursor = XCreateFontCursor(display, XC_hand2);
	XDefineCursor(display, WIDGET_XWINDOW(mp), cursor);

	mp->pixmap = XCreatePixmap(display, WIDGET_XWINDOW(mp),
	    mp->pixmapwidth = WIDGET_WIDTH(mp),
	    mp->pixmapheight = WIDGET_HEIGHT(mp),
	    DefaultDepth(display, screen));

	gcval.font = font->fid;
	gcval.graphics_exposures = False;
	mp->gc = XCreateGC(display, WIDGET_XWINDOW(mp),
	    GCFont | GCGraphicsExposures, &gcval);

	LIST_INIT(&mp->itemlist);
	mp->nitems = 0;
	mp->current = -1;

	mp->widget.event = menuevent;
	XSelectInput(display, WIDGET_XWINDOW(mp), ExposureMask);

	mp->widget.prepare_repaint = prepare_repaint;
	mp->widget.repaint = repaint;

	REPAINT(mp);

	return mp;
}

void destroy_menu(struct menu *mp)
{
	struct menuitem *ip;
	LIST *lp;

	LIST_FOREACH(lp, &mp->itemlist) {
		ip = LIST_ITEM(lp, struct menuitem, itemlink);
		LIST_REMOVE(lp);
		ip->menu = NULL;
	}
	XFreeGC(display, mp->gc);
	XFreePixmap(display, mp->pixmap);
	destroy_widget(&mp->widget);
	FREE(mp);
}

/*
 * Resize the menu so that all items fit.
 */
static void trim(struct menu *mp)
{
	struct menuitem *ip;
	LIST *lp;
	int width;
	int height;

	width = 2 + 2 * ITEMHEIGHT;
	height = 2 + 2 * ITEMPAD;
	LIST_FOREACH(lp, &mp->itemlist) {
		ip = LIST_ITEM(lp, struct menuitem, itemlink);
		height += ITEMHEIGHT;
		width = MAX(width,
		    stringwidth(ip->name) + 2 * (1 + ITEMHEIGHT));
	}

	resize_widget(&mp->widget, width, height);

	if (width > mp->pixmapwidth || height > mp->pixmapheight) {
		XFreePixmap(display, mp->pixmap);
		if (width > mp->pixmapwidth)
			mp->pixmapwidth = MAX(LARGER(mp->pixmapwidth),
			                      width);
		if (height > mp->pixmapheight)
			mp->pixmapheight = MAX(LARGER(mp->pixmapheight),
			                       height);
		debug("increasing menu pixmap size (%dx%d)",
		    mp->pixmapwidth, mp->pixmapheight);
		mp->pixmap = XCreatePixmap(display, WIDGET_XWINDOW(mp),
		    mp->pixmapwidth, mp->pixmapheight,
		    DefaultDepth(display, screen));
	}
}

/*
 * Add an item and automatically resize + repaint.
 */
struct menuitem *create_menuitem(struct menu *menu, const char *name,
    void (*select)(void *), void *arg)
{
	struct menuitem *ip;

	assert(name != NULL);

	ip = MALLOC(sizeof (struct menuitem));

	ip->name = STRDUP(name);
	stringfit(ip->name, MAXWIDTH);

	ip->select = select;
	ip->arg = arg;

	if (menu == NULL) {
		ip->menu = NULL;
		LIST_INIT(&ip->itemlink);
	} else {
		ip->menu = menu;
		LIST_INSERT_HEAD(&menu->itemlist, &ip->itemlink);
		menu->nitems++;
		trim(menu);
		REPAINT(menu);
	}

	return ip;
}

void rename_menuitem(struct menuitem *item, const char *name)
{
	assert(name != NULL);

	FREE(item->name);
	item->name = STRDUP(name);
	stringfit(item->name, MAXWIDTH);

	if (item->menu != NULL) {
		trim(item->menu);
		REPAINT(item->menu);
	}
}

void destroy_menuitem(struct menuitem *item)
{
	if (item->menu != NULL) {
		LIST_REMOVE(&item->itemlink);
		item->menu->nitems--;
		trim(item->menu);
		REPAINT(item->menu);
	}

	FREE(item->name);
	FREE(item);
}
