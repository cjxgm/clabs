#if !defined(WIDGET_H)
#define WIDGET_H

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

enum widget_type {
	WIDGET_ANY,
	WIDGET_WINDOW,
	WIDGET_TITLE,
	WIDGET_BUTTON,
	WIDGET_MENU,
	WIDGET_RESIZER,
	WIDGET_SIZEWIN
};

struct dim {
	int x;
	int y;
	int width;
	int height;
};

struct widget {
	enum widget_type type;
	Window xwindow;
	int mapped;
	struct dim dim;
	void (*event)(struct widget *, XEvent *);
	void (*prepare_repaint)(struct widget *);
	void (*repaint)(struct widget *);
	LIST repaintlink;
};

#define WIDGET_XWINDOW(ptr)	(((struct widget *)(ptr))->xwindow)
#define WIDGET_X(ptr)		(((struct widget *)(ptr))->dim.x)
#define WIDGET_Y(ptr)		(((struct widget *)(ptr))->dim.y)
#define WIDGET_WIDTH(ptr)	(((struct widget *)(ptr))->dim.width)
#define WIDGET_HEIGHT(ptr)	(((struct widget *)(ptr))->dim.height)
#define WIDGET_MAPPED(ptr)	(((struct widget *)(ptr))->mapped)

#define REPAINT(ptr)	schedule_widget_repaint((struct widget *)(ptr));

void widget_init(void);
void widget_fini(void);
void create_widget(struct widget *, enum widget_type,
    Window, unsigned, int, int, int, int);
void resize_widget(struct widget *, int, int);
void move_widget(struct widget *, int, int);
void moveresize_widget(struct widget *, int, int, int, int);
void map_widget(struct widget *);
void unmap_widget(struct widget *);
void destroy_widget(struct widget *);
struct widget *find_widget(Window, enum widget_type);
void delete_widget_context(Window);
void save_widget_context(struct widget *, Window);
void schedule_widget_repaint(struct widget *);
void repaint_widgets(void);

#endif /* !defined(WIDGET_H) */
