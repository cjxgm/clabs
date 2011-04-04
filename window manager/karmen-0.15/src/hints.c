/*
 * Modular hinting support
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "hints.h"
#include "lib.h"

extern struct hints icccm_hints;

#if CONFIG_EWMH
extern struct hints ewmh_hints;
#endif

static struct hints *hints[] = {
	&icccm_hints,

#if CONFIG_EWMH
	&ewmh_hints,
#endif
};

void hints_init(void)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->init != NULL)
			hints[i]->init();
}

void hints_fini(void)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->fini != NULL)
			hints[i]->fini();
}

void hints_manage(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->manage != NULL)
			hints[i]->manage(win);
}

void hints_unmanage(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->unmanage != NULL)
			hints[i]->unmanage(win);
}

void hints_map(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->map != NULL)
			hints[i]->map(win);
}

void hints_unmap(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->unmap != NULL)
			hints[i]->unmap(win);
}

void hints_withdraw(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->withdraw != NULL)
			hints[i]->withdraw(win);
}

void hints_activate(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->activate != NULL)
			hints[i]->activate(win);
}

void hints_deactivate(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->deactivate != NULL)
			hints[i]->deactivate(win);
}

void hints_move(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->move != NULL)
			hints[i]->move(win);
}

void hints_resize(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->resize != NULL)
			hints[i]->resize(win);
}

void hints_moveresize(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->moveresize != NULL)
			hints[i]->moveresize(win);
}

void hints_clientmessage(struct window *win, XClientMessageEvent *ep)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->clientmessage != NULL)
			if (hints[i]->clientmessage(win, ep))
				return;
}

void hints_propertynotify(struct window *win, XPropertyEvent *ep)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->propertynotify != NULL)
			if (hints[i]->propertynotify(win, ep))
				return;
}

int hints_delete(struct window *win)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->delete != NULL)
			if (hints[i]->delete(win))
				return 1;
	return 0;
}

void hints_restack(void)
{
	int i;

	for (i = 0; i < NELEM(hints); i++)
		if (hints[i]->restack != NULL)
			hints[i]->restack();
}
