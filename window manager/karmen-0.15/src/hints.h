#if !defined(HINTS_H)
#define HINTS_H

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

struct window;

struct hints {
	/* The name of the hints */
	char *name;

	/* Called at program start */
	void (*init)(void);

	/* Called before shutting down */
	void (*fini)(void);

	/* Called when a new window is managed */
	void (*manage)(struct window *);

	/* Called when a window is unmanaged */
	void (*unmanage)(struct window *);

	/* Called when a window is mapped */
	void (*map)(struct window *);

	/* Called when a window is unmapped */
	void (*unmap)(struct window *);

	/* Called when a client withdraws (unmaps) itself */
	void (*withdraw)(struct window *);

	/* Called when a window becomes active, NULL if none */
	void (*activate)(struct window *);

	/* Called when a window loses focus */
	void (*deactivate)(struct window *);

	/* Called when a window was moved but not resized */
	void (*move)(struct window *);

	/* Called when a window was resized but not moved */
	void (*resize)(struct window *);

	/* Called when a window was moved AND resized */
	void (*moveresize)(struct window *);

	/*
	 * Called when a client message arrives for a window.
	 * A hint should return nonzero if the message was understood.
	 */
	int (*clientmessage)(struct window *, XClientMessageEvent *);

	/*
	 * Called when a property event arrives for a window.
	 * A hint should return nonzero if the property was understood.
	 */
	int (*propertynotify)(struct window *, XPropertyEvent *);

	/*
	 * Called when a hint should try to gracefully delete the
	 * window.  A hint should return nonzero if it knows how
	 * to delete the window.
	 */
	int (*delete)(struct window *);

	/* Called after restacking windows */
	void (*restack)(void);
};

void hints_init(void);
void hints_fini(void);
void hints_manage(struct window *);
void hints_unmanage(struct window *);
void hints_map(struct window *);
void hints_unmap(struct window *);
void hints_withdraw(struct window *);
void hints_activate(struct window *);
void hints_deactivate(struct window *);
void hints_move(struct window *);
void hints_resize(struct window *);
void hints_moveresize(struct window *);
void hints_clientmessage(struct window *, XClientMessageEvent *);
void hints_propertynotify(struct window *, XPropertyEvent *);
int hints_delete(struct window *);
void hints_restack(void);

#endif /* !defined(HINTS_H) */
