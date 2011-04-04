#ifndef _H_VWM_BKGD_
#define _H_VWM_BKGD_

#include <signal.h>

#include <glib.h>

#ifdef _VIPER_WIDE
#include <ncursesw/curses.h>
#else
#include <curses.h>
#endif

gint  vwm_bkgd_simple(WINDOW *window,gpointer arg);
gint  vwm_bkgd_bricks(WINDOW *window,gpointer arg);

#endif
