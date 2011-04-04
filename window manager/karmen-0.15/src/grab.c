/*
 * grab.c - key/button grabbing
 *
 * The routines in this files are equivalent to their X counterparts,
 * except that they also grab/ungrab all combinations of Num Lock,
 * Caps Lock and Scroll Lock.
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

#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "lib.h"

/*
 * All possible combinations of the lock masks.
 */
static unsigned lockmasks[] = {
	0,
	LockMask,	/* Caps Lock */
	Mod2Mask,	/* Num Lock */
	Mod5Mask,	/* Scroll Lock */
	LockMask | Mod2Mask,
	LockMask | Mod5Mask,
	Mod2Mask | Mod5Mask,
	LockMask | Mod2Mask | Mod5Mask
};

/*
 * XGrabKey()
 */
void grabkey(Display *display, int keycode, unsigned modifiers,
    Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode)
{
	int i;

	if (modifiers == AnyModifier)
		XGrabKey(display, keycode, AnyModifier,
		    grab_window, owner_events, pointer_mode, keyboard_mode);
	else
		for (i = 0; i < NELEM(lockmasks); i++)
			XGrabKey(display, keycode, lockmasks[i] | modifiers,
			    grab_window, owner_events, pointer_mode,
			    keyboard_mode);
}

/*
 * XUngrabKey()
 */
void ungrabkey(Display *display, int keycode, unsigned modifiers,
    Window grab_window)
{
	int i;

	if (modifiers == AnyModifier)
		XUngrabKey(display, keycode, AnyModifier, grab_window);
	else
		for (i = 0; i < NELEM(lockmasks); i++)
			XUngrabKey(display, keycode, lockmasks[i] | modifiers,
			    grab_window);
}

/*
 * XGrabButton()
 */
void grabbutton(Display *display, unsigned button, unsigned modifiers,
    Window grab_window, Bool owner_events, unsigned event_mask,
    int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor)
{
	int i;

	if (modifiers == AnyModifier)
		XGrabButton(display, button, AnyModifier,
		    grab_window, owner_events, event_mask, pointer_mode,
		    keyboard_mode, confine_to, cursor);
	else
		for (i = 0; i < NELEM(lockmasks); i++)
			XGrabButton(display, button, lockmasks[i] | modifiers,
			    grab_window, owner_events, event_mask,
			    pointer_mode, keyboard_mode, confine_to, cursor);
}

/*
 * XUngrabButton()
 */
void ungrabbutton(Display *display, unsigned button, unsigned modifiers,
    Window grab_window)
{
	int i;

	if (modifiers == AnyModifier)
		XUngrabButton(display, button, AnyModifier, grab_window);
	else
		for (i = 0; i < NELEM(lockmasks); i++)
			XUngrabButton(display, button,
			    lockmasks[i] | modifiers, grab_window);
}
