#if !defined(BUTTON_H)
#define BUTTON_H

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

#include <X11/Xlib.h>

#include "lib.h"
#include "widget.h"

struct window;

struct button { struct widget widget;
	struct window *window;
	struct color *fg;
	struct color *bg;
	Pixmap pixmap;
	GC gc;
	IMAGE *image;
	int acting;
	int depressed;
	int hover;
	void (*handler)(struct window *);
};

struct button *create_button(struct window *, int, int, int, int);
void destroy_button(struct button *);
void move_button(struct button *, int, int);
void repaint_button(struct button *);
void set_button_handler(struct button *, void (*)(struct window *));
void set_button_image(struct button *, IMAGE *);

#endif /* !defined(BUTTON_H) */
