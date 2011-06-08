//@+leo
//@+node:0::@file src-c/main.cpp
//@+body
//@@language c

// Define "TEST" and it will compile to make a single fake window so
// you can test the window controls.
//#define TEST 1


//@<< main #includes >>
//@+node:1::<< main #includes >>
//@+body
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xproto.h>
#include <FL/filename.H>

#include "config.h"
#include "Frame.h"

#include "Python.h"
#include "pycallbacks.h"


//@-body
//@-node:1::<< main #includes >>


//@<< main declarations >>
//@+node:2::<< main declarations >>
//@+body
#ifdef SHOW_CLOCK
#include <time.h>
#include <signal.h>
#endif

////////////////////////////////////////////////////////////////

static const char* program_name;
static int initializing;

PyObject *pyClient = NULL;

extern "C" char *getWindowName(Window w);


//@-body
//@-node:2::<< main declarations >>


//@+others
//@+node:3::xerror_handler
//@+body

static int xerror_handler(Display* d, XErrorEvent* e) {
  if (initializing && (e->request_code == X_ChangeWindowAttributes) &&
      e->error_code == BadAccess)
    Fl::fatal("Another window manager is running.  You must exit it before running %s.", program_name);
#ifndef DEBUG
  if (e->error_code == BadWindow) return 0;
  if (e->error_code == BadColor) return 0;
#endif
  char buf1[128], buf2[128];
  sprintf(buf1, "XRequest.%d", e->request_code);
  XGetErrorDatabaseText(d,"",buf1,buf1,buf2,128);
  XGetErrorText(d, e->error_code, buf1, 128);
  Fl::warning("%s: %s: %s 0x%lx", program_name, buf2, buf1, e->resourceid);
  return 0;
}

//@-body
//@-node:3::xerror_handler
//@+node:4::handle
//@+body
//@+at
//  The Fl_Root class looks like a window to fltk but is actually the
// screen's root window. This is done by using set_xid to "show" it
// rather than have fltk create the window.

//@-at
//@@c

class Fl_Root : public Fl_Window {
  int handle(int);
public:
  void *wmobj;
  Fl_Root(void *p_wmobj) : Fl_Window(0,0,Fl::w(),Fl::h()) { wmobj = p_wmobj;}
  void show() {
    if (!shown()) Fl_X::set_xid(this, RootWindow(fl_display, fl_screen));
  }
};
Fl_Window *Root;

extern void ShowMenu();
extern int Handle_Hotkey();
extern void Grab_Hotkeys();

int Fl_Root::handle(int e) {
  if (e == FL_PUSH)
  {
    int button = Fl::event_button();
    if (py_on_buttonPress(pyClient, (long)button))
        return 1;
    ShowMenu();
    return 1;
  }
  //printf("Fl_Root::handle: got other than\n");
  return 0;
}


//@-body
//@-node:4::handle
//@+node:5::flwm_event_handler
//@+body

#if CLICK_RAISES || CLICK_TO_TYPE
extern void click_raise(Frame*);
#endif

// fltk calls this for any events it does not understand:
static int flwm_event_handler(int e)
{
  if (!e)
  { // XEvent that fltk did not understand.
    Window window = fl_xevent->xany.window;
    // unfortunately most of the redirect events put the interesting
    // window id in a different place:
    switch (fl_xevent->type) {
    case CirculateNotify:
    case CirculateRequest:
    case ConfigureNotify:
    case ConfigureRequest:
    case CreateNotify:
    case DestroyNotify:
    case GravityNotify:
    case MapNotify:
    case MapRequest:
    case ReparentNotify:
    case UnmapNotify:
      window = fl_xevent->xmaprequest.window;
    }
    for (Frame* c = Frame::first; c; c = c->next)
      if (c->window() == window || fl_xid(c) == window)
#if CLICK_RAISES || CLICK_TO_TYPE
	if (fl_xevent->type == ButtonPress) {click_raise(c); return 1;}
	else
#endif
	  return c->handle(fl_xevent);
    switch (fl_xevent->type)
    {
    case ButtonPress:
      printf("got a button press in main\n");
      return 0;
    case ConfigureRequest:
      {
      const XConfigureRequestEvent *e = &(fl_xevent->xconfigurerequest);
      XConfigureWindow(fl_display, e->window,
		       e->value_mask&~(CWSibling|CWStackMode),
		       (XWindowChanges*)&(e->x));
      return 1;
      }
    case MapRequest:
      {
      const XMapRequestEvent* e = &(fl_xevent->xmaprequest);
      (void)new Frame(e->window);
      return 1;
      }
    case KeyRelease:
      {
      if (!Fl::grab()) return 0;
      // see if they released the alt key:
      unsigned long keysym =
            XKeycodeToKeysym(fl_display, fl_xevent->xkey.keycode, 0);
      if (keysym == FL_Alt_L || keysym == FL_Alt_R)
      {
            Fl::e_keysym = FL_Enter;
            return Fl::grab()->handle(FL_KEYBOARD);
      }
      return 0;
      }
    }
  }
  else if (e == FL_SHORTCUT)
  {
#if FL_MAJOR_VERSION == 1 && FL_MINOR_VERSION == 0 && FL_PATCH_VERSION < 3
    // make the tab keys work in the menus in older fltk's:
    // (they do not cycle around however, so a new fltk is a good idea)
    if (Fl::grab())
    {
      // make fltk's menus resond to tab + shift+tab:
      if (Fl::event_key() == FL_Tab)
      {
        if (Fl::event_state() & FL_SHIFT)
            goto J1;
        Fl::e_keysym = FL_Down;
      }
      else if (Fl::event_key() == 0xFE20)
      {
        	J1: Fl::e_keysym = FL_Up;
      }
      else
        return 0;
      return Fl::grab()->handle(FL_KEYBOARD);
    }
#endif
    return Handle_Hotkey();
  }
  return 0;
}

//@-body
//@-node:5::flwm_event_handler
//@+node:6::flwm_update_clock
//@+body

#if DESKTOPS
extern void init_desktops();
extern Atom _win_workspace;
extern Atom _win_workspace_count;
extern Atom _win_workspace_names;
#endif

extern Atom _win_state;
extern Atom _win_hints;

#ifdef SHOW_CLOCK
int clock_period = 1;
int clock_oldmin = 61;
int clock_alarm_on = 0;
char clock_buf[80];

struct sigaction flwm_clock_alarm_start = {0,}, flwm_clock_alarm_stop = {0,};

void flwm_update_clock(void*) {
    time_t newtime;
    struct tm *tm_p;

    // get current time
    time(&newtime);
    tm_p = localtime(&newtime);

    // Update a window frame if necessary
    if (Frame::activeFrame() && tm_p->tm_min != clock_oldmin) {
	if (clock_oldmin != 61)
	    clock_period = 60;  // now that we're in sync, only update 1/minute
	clock_oldmin = tm_p->tm_min;
	strftime(clock_buf, 80, SHOW_CLOCK, tm_p);
	Frame::activeFrame()->redraw_clock();
    }
    // Now reschedule the timeout
    Fl::remove_timeout(flwm_update_clock);
    Fl::add_timeout(clock_period, flwm_update_clock);
}

//@-body
//@-node:6::flwm_update_clock
//@+node:7::flwm_clock_alarm_on
//@+body

void flwm_clock_alarm_on(int) {
    clock_alarm_on = 1;
    Frame::activeFrame()->redraw_clock();
}

//@-body
//@-node:7::flwm_clock_alarm_on
//@+node:8::flwm_clock_alarm_off
//@+body

void flwm_clock_alarm_off(int) {
    clock_alarm_on = 0;
    Frame::activeFrame()->redraw_clock();
}

//@-body
//@-node:8::flwm_clock_alarm_off
//@+node:9::color_setup
//@+body
#endif

const char* cfg, *cbg, *abg, *bg, *fg;

static int cursor = FL_CURSOR_ARROW;

static void color_setup(Fl_Color slot, const char* arg, ulong value) {
  if (arg) {
    XColor x;
    if (XParseColor(fl_display, fl_colormap, arg, &x))
      value = ((x.red>>8)<<24)
            | ((x.green>>8)<<16)
            | ((x.blue));
  }
  Fl::set_color(slot, value);
}

//@-body
//@-node:9::color_setup
//@+node:10::parse_fl_color
//@+body
int parse_fl_color(const char* arg)
{
  int value = 0;
  XColor x;
  if (arg)
  {
    if (XParseColor(fl_display, fl_colormap, arg, &x))
        value = fl_color_cube(
            (x.red >> 8) * FL_NUM_RED/256,
            (x.green >> 8) * FL_NUM_GREEN/256,
            (x.blue >> 8) * FL_NUM_BLUE/256
            );
  }
  printf("parse_fl_color: %s -> 0x%x\n", arg, value);
  
  return value;
}



//@-body
//@-node:10::parse_fl_color
//@+node:11::initialize
//@+body

static void initialize() {

  Display* d = fl_display;

#ifdef TEST
  Window w = XCreateSimpleWindow(d, root,
				 100, 100, 200, 300, 10,
				 BlackPixel(fl_display, 0),
//				 WhitePixel(fl_display, 0));
				 0x1234);
  Frame* frame = new Frame(w);
  XSelectInput(d, w,
	       ExposureMask | StructureNotifyMask |
	       KeyPressMask | KeyReleaseMask | FocusChangeMask |
	       KeymapStateMask |
	       ButtonPressMask | ButtonReleaseMask |
	       EnterWindowMask | LeaveWindowMask /*|PointerMotionMask*/
	       );
#else

  Fl::add_handler(flwm_event_handler);

  // setting attributes on root window makes me the window manager:
  initializing = 1;
  XSelectInput(d, fl_xid(Root),
	       SubstructureRedirectMask | SubstructureNotifyMask |
	       ColormapChangeMask | PropertyChangeMask |
	       ButtonPressMask | ButtonReleaseMask | 
	       EnterWindowMask | LeaveWindowMask |
	       KeyPressMask | KeyReleaseMask | KeymapStateMask |
           PointerMotionMask);
  color_setup(CURSOR_FG_SLOT, cfg, CURSOR_FG_COLOR<<8);
  color_setup(CURSOR_BG_SLOT, cbg, CURSOR_BG_COLOR<<8);
  Root->cursor((Fl_Cursor)cursor, CURSOR_FG_SLOT, CURSOR_BG_SLOT);

#ifdef TITLE_FONT
  Fl::set_font(TITLE_FONT_SLOT, TITLE_FONT);
#endif
#ifdef MENU_FONT
  Fl::set_font(MENU_FONT_SLOT, MENU_FONT);
#endif
#ifdef ACTIVE_COLOR
  //Fl::set_color(FL_SELECTION_COLOR, ACTIVE_COLOR<<8);
  color_setup(FL_SELECTION_COLOR, abg, ACTIVE_COLOR<<8);
#endif

  // Gnome crap:
  // First create a window that can be watched to see if wm dies:
  Atom a = XInternAtom(d, "_WIN_SUPPORTING_WM_CHECK", False);
  Window win = XCreateSimpleWindow(d, fl_xid(Root), -200, -200, 5, 5, 0, 0, 0);
  CARD32 val = win;
  XChangeProperty(d, fl_xid(Root), a, XA_CARDINAL, 32, PropModeReplace, (uchar*)&val, 1);
  XChangeProperty(d, win, a, XA_CARDINAL, 32, PropModeReplace, (uchar*)&val, 1);
  // Next send a list of Gnome stuff we understand:
  a = XInternAtom(d, "_WIN_PROTOCOLS", 0);
  Atom list[10]; unsigned int i = 0;
//list[i++] = XInternAtom(d, "_WIN_LAYER", 0);
  list[i++] = _win_state = XInternAtom(d, "_WIN_STATE", 0);
  list[i++] = _win_hints = XInternAtom(d, "_WIN_HINTS", 0);
//list[i++] = XInternAtom(d, "_WIN_APP_STATE", 0);
//list[i++] = XInternAtom(d, "_WIN_EXPANDED_SIZE", 0);
//list[i++] = XInternAtom(d, "_WIN_ICONS", 0);
#if DESKTOPS
  list[i++] = _win_workspace = XInternAtom(d, "_WIN_WORKSPACE", 0);
  list[i++] = _win_workspace_count = XInternAtom(d, "_WIN_WORKSPACE_COUNT", 0);
  list[i++] = _win_workspace_names = XInternAtom(d, "_WIN_WORKSPACE_NAMES", 0);
#endif
//list[i++] = XInternAtom(d, "_WIN_FRAME_LIST", 0);
  XChangeProperty(d, fl_xid(Root), a, XA_ATOM, 32, PropModeReplace, (uchar*)list, i);

  Grab_Hotkeys();

#ifdef SHOW_CLOCK
  Fl::add_timeout(clock_period, flwm_update_clock);
  flwm_clock_alarm_start.sa_handler = &flwm_clock_alarm_on;
  flwm_clock_alarm_stop.sa_handler = &flwm_clock_alarm_off;
  sigaction(SIGALRM, &flwm_clock_alarm_start, NULL);
  sigaction(SIGCONT, &flwm_clock_alarm_stop, NULL);
#endif

  XSync(d, 0);
  initializing = 0;

#if DESKTOPS
  init_desktops();
#endif

  // find all the windows and create a Frame for each:
  unsigned int n;
  Window w1, w2, *wins;
  XWindowAttributes attr;
  XQueryTree(d, fl_xid(Root), &w1, &w2, &wins, &n);
  for (i = 0; i < n; ++i) {
    XGetWindowAttributes(d, wins[i], &attr);
    if (attr.override_redirect || !attr.map_state) continue;
    (void)new Frame(wins[i],&attr);
  }
  XFree((void *)wins);

#endif
}

//@-body
//@-node:11::initialize
//@+node:12::arg
//@+body

////////////////////////////////////////////////////////////////

extern int exit_flag;
extern int max_w_switch;
extern int max_h_switch;

// consume a switch from argv.  Returns number of words eaten, 0 on error:
int arg(int argc, char **argv, int &i) {
  const char *s = argv[i];
  if (s[0] != '-') return 0;
  s++;

  // do single-word switches:
  if (!strcmp(s,"x")) {
    exit_flag = 1;
    i++;
    return 1;
  }

  // do switches with a value:
  const char *v = argv[i+1];
  if (i >= argc-1 || !v)
    return 0;	// all the rest need an argument, so if missing it is an error

  if (!strcmp(s, "cfg")) {
    cfg = v;
  }

  else if (!strcmp(s, "fg"))
  {
    fg = v;
    return 0;
  }
  else if (!strcmp(s, "bg"))
  {
    bg = v;
    return 0;
  }

  else if (!strcmp(s, "cbg"))
  {
    cbg = v;
  }
  else if (!strcmp(s, "abg"))
  {
    abg = v;
  }
  else if (*s == 'c')
  {
    cursor = atoi(v);
  }
  else if (*s == 'v')
  {
    int visid = atoi(v);
    fl_open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    fl_visual = XGetVisualInfo(fl_display, VisualIDMask, &templt, &num);
    if (!fl_visual) Fl::fatal("No visual with id %d",visid);
    fl_colormap = XCreateColormap(fl_display, 
                                  RootWindow(fl_display,fl_screen),
                                  fl_visual->visual, AllocNone);
  }
  else if (*s == 'm')
  {
    max_w_switch = atoi(v);
    while (*v && *v++ != 'x');
    max_h_switch = atoi(v);
  }
  else
    return 0; // unrecognized
  // return the fact that we consumed 2 switches:
  i += 2;
  return 2;
}


//@-body
//@-node:12::arg
//@+node:13::main
//@+body

int main0(int argc, char** argv, PyObject *wmobj) {
  int i;

  pyClient = wmobj;
  Py_INCREF(wmobj);
  
  //program_name = filename_name(argv[0]);

  printf("*********** LAUNCHING FLWM *******************\n");
  //printf("main: argc=%d\n", argc);
  //for (i = 0; i < argc; i++)
  //{
  //    printf("main: argv[%d] = %s\n", i, argv[i]);
  //}

  if (Fl::args(argc, argv, i, arg) < argc) Fl::error(
"options are:\n"
" -d[isplay] host:#.#\tX display & screen to use\n"
" -v[isual] #\t\tvisual to use\n"
" -g[eometry] WxH+X+Y\tlimits windows to this area\n"
" -m[aximum] WxH\t\tsize of maximized windows\n"
" -x\t\t\tmenu says Exit instead of logout\n"
" -bg color\t\tFrame color\n"
" -fg color\t\tLabel color\n"
" -bg2 color\t\tText field color\n"
" -c[ursor] #\t\tCursor number for root\n"
" -cfg color\t\tCursor color\n"
" -abg color\t\tActive window frame color\n"
" -cbg color\t\tCursor outline color"
);
#ifndef FL_NORMAL_SIZE // detect new versions of fltk where this is a variable
  FL_NORMAL_SIZE = 12;
#endif
  Fl::set_color(FL_SELECTION_COLOR,0,0,128);
  Root = new Fl_Root(wmobj);
  Root->show(argc,argv); // fools fltk into using -geometry to set the size
  XSetErrorHandler(xerror_handler);
  initialize();

  Fl::check();

  // ****************************************
  py_on_startup(wmobj);
  // ****************************************

  return Fl::run();
}

extern "C" int flwm_main(int argc, char **argv, PyObject *wmobj)
{
  return main0(argc, argv, wmobj);
}



//@-body
//@-node:13::main
//@+node:14::getWindowName
//@+body
extern "C" char *getWindowName(Window w)
{
	char * name;
	char * machine;
	Atom actual_type;
	int format;
	unsigned long n;
	unsigned long extra;
	
	if (XGetWindowProperty(
                          fl_display,
                          w,
                          XA_WM_NAME,
                          0L,
                          100L,
                          False,
                          AnyPropertyType,
                          &actual_type,
                          &format,
                          &n,
                          &extra,
                          (unsigned char **) &name) == Success && name && *name != '\0' && n != 0)
        return name;
    else
        return NULL;
}


//@-body
//@-node:14::getWindowName
//@-others


//@-body
//@-node:0::@file src-c/main.cpp
//@-leo
