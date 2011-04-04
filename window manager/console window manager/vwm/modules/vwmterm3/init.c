/*************************************************************************
 * All portions of code are copyright by their respective author/s.
 * Copyright (C) 2007      Bryan Christ <bryan.christ@hp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *----------------------------------------------------------------------*/

#include <unistd.h>
#include <fcntl.h>

#include <viper.h>
#include <vterm.h>

#include "vwmterm.h"
#include "events.h"
#include "psthread.h"
#include "signals.h"

G_MODULE_EXPORT gchar* g_module_check_init(GModule *module)
{
	gchar			*libfilename;

	// preload libutil for use with this module.
	if(g_module_open("libutil.so",G_MODULE_BIND_LAZY)==NULL)
 		return "\ncould not preload libutil.so";

   // preload libvterm for use with this module
   if(g_module_open("libvterm.so",G_MODULE_BIND_LAZY)==NULL)
      return "\ncould not preload libvterm.so";

	/*	register with vwm 	*/
	libfilename=(gchar*)g_module_name(module);
	vwm_module_add("Tools","VTerm",vwmterm_init,NULL,libfilename);

	return NULL;
}

WINDOW* vwmterm_init(gpointer anything)
{
   extern WINDOW  	*SCREEN_WINDOW;
	extern ps_runq_t	*vwm_runq;
   vterm_t           *vterm;
	WINDOW	      	*window;
	gint		      	width,height;
   int               master_fd;
   int               fflags;

   getmaxyx(SCREEN_WINDOW,height,width);
   if(height>30 && width>84)
   {
      height=25;
      width=80;
   }
   else
   {
      /* calculate scaled window size */
	   window_get_size_scaled(NULL,&width,&height,0.85,0.65);
	   if(width>80) width=80;
	   if(height>25) height=25;
   }

   vterm=vterm_create(width,height,0);
   vterm_set_colors(vterm,COLOR_WHITE,COLOR_BLACK);
   master_fd=vterm_get_pty_fd(vterm);

   // configure SIGIO acceleration
#ifdef SIGPOLL
   vwmterm_sigset(SIGPOLL,vwmterm_SIGIO);
#else
   vwmterm_sigset(SIGIO,vwmterm_SIGIO);
#endif
	fcntl(master_fd,F_SETOWN,getpid());
   fflags=fcntl(master_fd,F_GETFL);
   fcntl(master_fd,F_SETFL,fflags | FASYNC);

   viper_thread_enter();

   // create window
	window=viper_window_create(" VTerm ",0.5,0.5,width,height,TRUE);
   viper_window_set_state(window,STATE_UNSET | STATE_NORESIZE);
	viper_window_set_limits(window,15,2,WSIZE_UNCHANGED,WSIZE_UNCHANGED);

   // libviper set the default bkgd OR to WHITE on BLACK.  undo it.
   wbkgdset(window,0);

   // init terminal
   vterm_wnd_set(vterm,window);
   vterm_erase(vterm);

   // attache event handlers
	viper_event_set(window,"window-resized",vwmterm_ON_RESIZE,(gpointer)vterm);
	viper_event_set(window,"window-close",vwmterm_ON_CLOSE,(gpointer)vterm);
	viper_event_set(window,"window-destroy",vwmterm_ON_DESTROY,
		(gpointer)vterm);
	viper_window_set_key_func(window,vwmterm_ON_KEYSTROKE);
	viper_window_set_userptr(window,(gpointer)vterm);

   // push pseudo-thread onto run queue
	psthread_add(vwm_runq,vwmterm_psthread,(gpointer)window);

	viper_thread_leave();

	return window;
}


