#ifndef _VWMTERM_H_
#define _VWMTERM_H_

#include <glib.h>
#include <gmodule.h>

#include <vwm.h>
#include <viper.h>
#include <pseudo.h>

#ifdef _VIPER_WIDE
#include <ncursesw/curses.h>
#else
#include <curses.h>
#endif

G_MODULE_EXPORT gchar* g_module_check_init(GModule *module);

WINDOW*	vwmterm_init(gpointer anything);

#endif
