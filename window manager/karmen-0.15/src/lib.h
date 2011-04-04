#if !defined(LIB_H)
#define LIB_H

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

#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define MIN(a, b)	((a) < (b) ? (a) : (b))

#define ABS(x)		((x) < 0 ? -(x) : (x))

#define NELEM(v)	(sizeof v / sizeof v[0])

#define LARGER(i)	((i) + 128)

typedef struct {
	unsigned char *data;
	int width;
	int height;
	Pixmap pixmap;
} IMAGE;

#define DEFINE_IMAGE(name, xbm) \
	IMAGE name = { xbm##_bits, xbm##_width, xbm##_height, None }

struct color {
	unsigned long bright2;
	unsigned long bright1;
	unsigned long normal;
	unsigned long shadow1;
	unsigned long shadow2;
};

void *MALLOC(size_t);
void *REALLOC(void *, size_t);
void FREE(void *);

char *STRDUP(const char *);

void clerr(void);
void sterr(void);

void grabkey(Display *display, int keycode, unsigned modifiers,
    Window grab_window, Bool owner_events, int pointer_mode,
    int keyboard_mode);
void ungrabkey(Display *display, int keycode, unsigned modifiers,
    Window grab_window);
void grabbutton(Display *display, unsigned button, unsigned modifiers,
    Window grab_window, Bool owner_events, unsigned event_mask,
    int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor);
void ungrabbutton(Display *display, unsigned button, unsigned modifiers,
    Window grab_window);

void putimage(Display *display, Drawable d, GC gc, IMAGE *image,
              int x, int y);
void drawraised(Drawable, GC, struct color *, int, int, int, int);
void drawlowered(Drawable, GC, struct color *, int, int, int, int);
void drawdepressed(Drawable, GC, struct color *, int, int, int, int);
int stringwidth(const char *);
char *stringfit(char *str, int width);

void beginfastmove(Window);
void endfastmove(void);

void error(const char *, ...);
void debug(const char *, ...);
const char *eventname(int);

#endif /* !defined(LIB_H) */
