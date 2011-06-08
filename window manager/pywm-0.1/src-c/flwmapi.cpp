//@+leo-ver=4
//@+node:@file src-c/flwmapi.cpp
//@@language c

//
// flwmapi.cpp
//
// Due to Pyrex being (presently) unable to handle C++ code, it
// has been necessary to write a 'shim' layer of vanilla C
// functions which call C++ functions within the FLWM engine.
//
// These functions get exposed to the pywm.flwm_ module as vanilla
// C entry points, which the various pywm.flwm_ functions can use
// to take actions.

//@+others
//@+node:includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <FL/filename.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Input.H>

#include "Frame.h"
#include "config.h"

#include "Python.h"


//@-node:includes
//@+node:imported funcs
extern void NextWindow();
extern void PreviousWindow();
extern void NextDesk();
extern void PreviousDesk();
extern void ShowMenu();

void Hotkeys_bindKey(long key);

extern int parse_fl_color(const char* arg);

extern const char* cfg, *cbg, *abg, *bg, *fg;

//@-node:imported funcs
//@+node:dispatchCallbackToPython
//
// generic callback dispatcher
//
// when it receives FLTK callbacks, it invokes the
// corresponding Python callback

static void dispatchCallbackToPython(Fl_Widget *widget, void *arg)
{
   //printf("dispatchCallbackToPython: widget=0x%lx, func=0x%lx\n", widget, arg);
   PyObject_CallObject((PyObject *)arg, NULL);
   //printf("dispatchCallbackToPython: done?\n");
}

//@-node:dispatchCallbackToPython
//@+node:fltk_window_*
class my_Fl_Window : public Fl_Window
{
public:
  my_Fl_Window(int x, int y, int w, int h, char *label,
               PyObject *pyself,
               PyObject *pyOnEnter,
               PyObject *pyOnClick
               );
  ~my_Fl_Window();
  
  int handle(int);
  
  PyObject *pySelf;
  PyObject *pyOnEnter;
  PyObject *pyOnClick;
   
};

my_Fl_Window::my_Fl_Window(int x, int y, int w, int h,
                           char *label,
                           PyObject *pyself,
                           PyObject *pyonenter,
                           PyObject *pyonclick) :
  Fl_Window(x, y, w, h, label)
{
  pySelf = pyself; Py_INCREF(pyself);
  pyOnEnter = pyonenter; Py_INCREF(pyonenter);
  pyOnClick = pyonclick; Py_INCREF(pyonclick);
}

my_Fl_Window::~my_Fl_Window()
{
}

int my_Fl_Window::handle(int e)
{
  int x, y;

  switch (e)
  {

  case FL_SHOW:
  case FL_HIDE:
    return 0; // prevent fltk from messing things up

  case FL_ENTER:
    //printf("Mouse has entered\n");
    Fl::get_mouse(x, y);
    PyObject_CallObject(pyOnEnter,
                        Py_BuildValue("ii", x, y));
    Fl_Window::handle(e);
    
    return 1;

  case FL_PUSH:
    //printf("Mouse click\n");
    Fl::get_mouse(x, y);
    PyObject_CallObject(pyOnClick,
                        Py_BuildValue("iii", Fl::event_button(), x, y));
    Fl_Window::handle(e);
    return 1;

  default:
    return Fl_Window::handle(e);
  }
  return 0;
}


extern "C" long fltk_window_create(int x, int y, int w, int h, char *label,
                                   PyObject *self,
                                   PyObject *onenter,
                                   PyObject *onclick)
{
  printf("fltk_window_create: entered\n");
  my_Fl_Window *win = new my_Fl_Window(x, y, w, h, label,
                                       self, onenter, onclick);
  //printf("fltk_window_create: window=0x%lx\n", win);
  return (long)win;
}

extern "C" void fltk_window_destroy(long pWin)
{
    delete ((my_Fl_Window *)pWin);
}

extern "C" void fltk_window_end(long pWin)
{
    //printf("fltk_window_end: window=0x%lx\n", pWin);
    ((my_Fl_Window *)pWin)->end();
}

extern "C" void fltk_window_show(long pWin)
{
    ((my_Fl_Window *)pWin)->show();
    Fl::check();
}

extern "C" void fltk_window_hide(long pWin)
{
    //printf("fltk_window_hide: pWin=0x%lx\n", pWin);
    ((my_Fl_Window *)pWin)->hide();
    //printf("fltk_window_hide: done\n");
}

extern "C" void fltk_window_activate(long pWin)
{
    ((my_Fl_Window *)pWin)->activate();
    XSetInputFocus(fl_display, PointerRoot, RevertToPointerRoot,
		     fl_event_time);
    Fl::check();
}

extern "C" void fltk_window_set_modal(long pWin)
{
    ((my_Fl_Window *)pWin)->set_modal();
    Fl::check();
}

extern "C" void fltk_window_move(long pWin, long x, long y)
{
    //printf("fltk_window_move: pWin=0x%lx\n", pWin);
    ((my_Fl_Window *)pWin)->position(x, y);
    //printf("fltk_window_move: done\n");
}

extern "C" void fltk_window_stayOnTop(long pWin)
{
    //printf("fltk_window_stayOnTop: pWin=0x%lx\n", pWin);
    ((my_Fl_Window *)pWin)->set_non_modal();
    //printf("fltk_window_stayOnTop: done\n");
    Fl::check();
}
//@-node:fltk_window_*
//@+node:fltk_button_*
extern "C" long fltk_button_create(int x, int y, int w, int h, char *label)
{
    return (long)(new Fl_Button(x, y, w, h, label));
}

extern "C" void fltk_button_destroy(long pBut)
{
    delete ((Fl_Button *)pBut);
}

extern "C" void fltk_button_show(long pBut)
{
    ((Fl_Button *)pBut)->show();
}

extern "C" void fltk_button_hide(long pBut)
{
    ((Fl_Button *)pBut)->hide();
}

extern "C" void fltk_button_callback(long pBut, long arg)
{
    Py_INCREF((PyObject *)arg);
    //printf("fltk_button_callback: pBut=0x%lx, arg=0x%lx\n", pBut, arg);
    ((Fl_Button *)pBut)->callback(
        dispatchCallbackToPython, (void *)arg);
}

extern "C" void fltk_button_symbollabel(long pBut)
{
    ((Fl_Button *)pBut)->labeltype(FL_SYMBOL_LABEL);
}

//@-node:fltk_button_*
//@+node:fltk_repeat_button_*
extern "C" long fltk_repeat_button_create(int x, int y, int w, int h, char *label)
{
    return (long)(new Fl_Repeat_Button(x, y, w, h, label));
}

extern "C" void fltk_repeat_button_destroy(long pBut)
{
    delete ((Fl_Repeat_Button *)pBut);
}

extern "C" void fltk_repeat_button_show(long pBut)
{
    ((Fl_Repeat_Button *)pBut)->show();
}

extern "C" void fltk_repeat_button_hide(long pBut)
{
    ((Fl_Repeat_Button *)pBut)->hide();
}

extern "C" void fltk_repeat_button_callback(long pBut, long arg)
{
    Py_INCREF((PyObject *)arg);
    //printf("fltk_button_callback: pBut=0x%lx, arg=0x%lx\n", pBut, arg);
    ((Fl_Repeat_Button *)pBut)->callback(
        dispatchCallbackToPython, (void *)arg);
}

extern "C" void fltk_repeat_button_symbollabel(long pBut)
{
    ((Fl_Repeat_Button *)pBut)->labeltype(FL_SYMBOL_LABEL);
}

//@-node:fltk_repeat_button_*
//@+node:fltk_select_browser_*
extern "C" long fltk_select_browser_create(int x, int y, int w, int h, char *label)
{
    return (long)(new Fl_Select_Browser(x, y, w, h, label));
}

extern "C" void fltk_select_browser_destroy(long pBut)
{
    delete ((Fl_Select_Browser *)pBut);
}

extern "C" void fltk_select_browser_show(long pBut)
{
    ((Fl_Select_Browser *)pBut)->show();
}

extern "C" void fltk_select_browser_hide(long pBut)
{
    ((Fl_Select_Browser *)pBut)->hide();
}

extern "C" void fltk_select_browser_callback(long pBut, long arg)
{
    Py_INCREF((PyObject *)arg);
    //printf("fltk_select_browser_callback: pBut=0x%lx, arg=0x%lx\n", pBut, arg);
    ((Fl_Select_Browser *)pBut)->callback(
        dispatchCallbackToPython, (void *)arg);
}

extern "C" void fltk_select_browser_add(long pBut, char *item)
{
    ((Fl_Select_Browser *)pBut)->add(item);
}

extern "C" void fltk_select_browser_remove(long pBut, int item)
{
    ((Fl_Select_Browser *)pBut)->remove(item);
}

extern "C" void fltk_select_browser_clear(long pBut)
{
    ((Fl_Select_Browser *)pBut)->clear();
}

extern "C" void fltk_select_browser_color(long pBut, int color)
{
    //printf("fltk_Select_browser_color: col=%06lx\n", color);
    ((Fl_Select_Browser *)pBut)->color(color);
}

extern "C" void fltk_select_browser_textcolor(long pBut, int color)
{
    //printf("fltk_select_browser_textcolor: col=%06lx\n", color);
    ((Fl_Select_Browser *)pBut)->textcolor(color);
}

extern "C" void fltk_select_browser_selectioncolor(long pBut, int color)
{
    //printf("fltk_Select_browser_selectioncolor: col=%06lx\n", color);
    ((Fl_Select_Browser *)pBut)->selection_color(color);
}

extern "C" int fltk_select_browser_value(long pLst, int val)
{
    if (val == -2)
        return ((Fl_Select_Browser *)pLst)->value();
    else
    {
        ((Fl_Select_Browser *)pLst)->value(val);
        return 0;
    }
}
//@-node:fltk_select_browser_*
//@+node:fltk_hold_browser_*
extern "C" long fltk_hold_browser_create(int x, int y, int w, int h, char *label)
{
    return (long)(new Fl_Hold_Browser(x, y, w, h, label));
}

extern "C" void fltk_hold_browser_destroy(long pBut)
{
    delete ((Fl_Hold_Browser *)pBut);
}

extern "C" void fltk_hold_browser_show(long pBut)
{
    ((Fl_Hold_Browser *)pBut)->show();
}

extern "C" void fltk_hold_browser_hide(long pBut)
{
    ((Fl_Hold_Browser *)pBut)->hide();
}

extern "C" void fltk_hold_browser_callback(long pBut, long arg)
{
    Py_INCREF((PyObject *)arg);
    //printf("fltk_select_browser_callback: pBut=0x%lx, arg=0x%lx\n", pBut, arg);
    ((Fl_Hold_Browser *)pBut)->callback(
        dispatchCallbackToPython, (void *)arg);
}

extern "C" void fltk_hold_browser_add(long pBut, char *item)
{
    ((Fl_Hold_Browser *)pBut)->add(item);
}

extern "C" void fltk_hold_browser_remove(long pBut, int item)
{
    ((Fl_Hold_Browser *)pBut)->remove(item);
}

extern "C" void fltk_hold_browser_clear(long pBut)
{
    ((Fl_Hold_Browser *)pBut)->clear();
}

extern "C" void fltk_hold_browser_color(long pBut, int color)
{
    //printf("fltk_hold_browser_color: col=%06lx\n", color);
    ((Fl_Hold_Browser *)pBut)->color(color);
}

extern "C" void fltk_hold_browser_textcolor(long pBut, int color)
{
    //printf("fltk_hold_browser_textcolor: col=%06lx\n", color);
    ((Fl_Hold_Browser *)pBut)->textcolor(color);
}

extern "C" void fltk_hold_browser_selectioncolor(long pBut, int color)
{
    //printf("fltk_hold_browser_selectioncolor: col=%06lx\n", color);
    ((Fl_Hold_Browser *)pBut)->selection_color(color);
}

extern "C" int fltk_hold_browser_value(long pLst, int val)
{
    if (val == -2)
        return ((Fl_Hold_Browser *)pLst)->value();
    else
    {
        ((Fl_Hold_Browser *)pLst)->value(val);
        return 0;
    }
}
//@-node:fltk_hold_browser_*
//@+node:fltk_input_*
extern "C" long fltk_input_create(int x, int y, int w, int h, char *label)
{
    Fl_Input *fld = new Fl_Input(x, y, w, h, label);
    fld->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    fld->take_focus();
    return (long)fld;
}

extern "C" void fltk_input_callback(long pFld, long arg)
{
    Py_INCREF((PyObject *)arg);
    //printf("fltk_input_callback: pBut=0x%lx, arg=0x%lx\n", pBut, arg);
    ((Fl_Input *)pFld)->callback(
        dispatchCallbackToPython, (void *)arg);
}

extern "C" void fltk_input_color(long pFld, int color)
{
    //printf("fltk_input_color: col=%06lx\n", color);
    ((Fl_Input *)pFld)->color(color);
}

extern "C" void fltk_input_textcolor(long pFld, int color)
{
    //printf("fltk_input_textcolor: col=%06lx\n", color);
    ((Fl_Input *)pFld)->textcolor(color);
}

extern "C" void fltk_input_selectioncolor(long pFld, int color)
{
    //printf("fltk_input_selectioncolor: col=%06lx\n", color);
    ((Fl_Input *)pFld)->selection_color(color);
}

extern "C" char *fltk_input_value(long pFld, char *val)
{
    //printf("fltk_input_value: val=0x%lx\n", val);
    if (val != NULL)
        ((Fl_Input *)pFld)->value(val);
    return (char *)(((Fl_Input_ *)pFld)->value());
}

extern "C" void fltk_input_take_focus(long pFld)
{
    //printf("fltk_input_selectioncolor: col=%06lx\n", color);
    ((Fl_Input *)pFld)->take_focus();
}

//@-node:fltk_input_*
//@+node:fltk_color
extern "C" long fltk_color(int red, int green, int blue)
{
    long col = (long)fl_color_cube(red * FL_NUM_RED/256,
                               green * FL_NUM_GREEN/256,
                               blue * FL_NUM_BLUE/256);
    //printf("flwmapi.fltk_color: %x %x %x -> %x\n",
    //       red, green, blue, col);
    return col;
}
//@-node:fltk_color
//@+node:flwm_keyCode

struct flwm_keyTab
{
  char *name;
  long val;
}
flwm_keyNames[] =
{
    { "tab", FL_Tab },
    { "enter", FL_Enter },
    { "escape", FL_Escape },
    { "insert", FL_Insert },
    { "delete", FL_Delete },
    { "home", FL_Home },
    { "end", FL_End },
    { "pageup", FL_Page_Up },
    { "pagedown", FL_Page_Down },
    { "up", FL_Up },
    { "down", FL_Down },
    { "left", FL_Left },
    { "right", FL_Right },
    { "kp0", FL_KP },
    { "kp1", FL_KP+1 },
    { "kp2", FL_KP+2 },
    { "kp3", FL_KP+3 },
    { "kp4", FL_KP+4 },
    { "kp5", FL_KP+5 },
    { "kp6", FL_KP+6 },
    { "kp7", FL_KP+7 },
    { "kp8", FL_KP+8 },
    { "kp9", FL_KP+9 },
    { "kpenter", FL_KP_Enter },
    { (char *)0, 0 }
};


extern "C" long flwm_keyCode(char *keystr,
                             int shift, int ctrl, int alt, int meta)
{
    long keycode = 0;

    if (strlen(keystr) == 1)
      // literal key character
      keycode = (long)keystr[0];
    else if (strlen(keystr) == 2 && tolower(keystr[0]) == 'f')
    {
      // possible F key
      int fKey = atoi(&keystr[1]);
      
      if (fKey >= 1 && fKey <= 12)
         keycode = FL_F + fKey;
    }
    else
    {
       int i;
       for (i = 0; flwm_keyNames[i].name; i++)
          if (!strcasecmp(keystr, flwm_keyNames[i].name))
          {
             keycode = flwm_keyNames[i].val;
             break;
          }
       if (keycode == 0)
          return 0;
    }

    // add shift/ctrl/alt/meta modifiers
    if (shift)
      keycode += FL_SHIFT;
    if (ctrl)
      keycode += FL_CTRL;
    if (alt)
      keycode += FL_ALT;
    if (meta)
      keycode += FL_META;

    return keycode;
}

//@-node:flwm_keyCode
//@+node:flwm_bindKey
// flwm_bindKey - tell FLWM engine to watch out for a key

extern "C" void flwm_bindKey(long key)
{
    //printf("flwm_bindKey: want to bind key 0x%lx\n", key);
    Hotkeys_bindKey(key);
}


//@-node:flwm_bindKey
//@+node:flwm_closeWindow
extern "C" void flwm_closeWindow(Frame *frm)
{
  //printf("flwm_closeWindow: trying to close\n");
  frm->close();
}
//@-node:flwm_closeWindow
//@+node:flwm_goNextWindow
extern "C" void flwm_goNextWindow()
{
   NextWindow();
}

//@-node:flwm_goNextWindow
//@+node:flwm_goPrevWindow
extern "C" void flwm_goPrevWindow()
{
   PreviousWindow();
}

//@-node:flwm_goPrevWindow
//@+node:flwm_goNextDesk
extern "C" void flwm_goNextDesk()
{
   NextDesk();
}
//@-node:flwm_goNextDesk
//@+node:flwm_goPrevDesk
extern "C" void flwm_goPrevDesk()
{
   PreviousDesk();
}
//@-node:flwm_goPrevDesk
//@+node:flwm_showMenu
extern "C" void flwm_showMenu()
{
  printf("flwm_showMenu: trying...\n");
   ShowMenu();
}
//@-node:flwm_showMenu
//@+node:flwm_raiseWindow
extern "C" void flwm_raiseWindow(Frame *f)
{
  if (f)
    f->raise();
}
//@-node:flwm_raiseWindow
//@+node:flwm_lowerWindow
extern "C" void flwm_lowerWindow(Frame *f)
{
  if (f)
    f->lower();
}
//@-node:flwm_lowerWindow
//@+node:flwm_iconiseWindow
extern "C" void flwm_iconiseWindow(Frame *f)
{
  if (f)
    f->iconize();
}
//@-node:flwm_iconiseWindow
//@+node:flwm_activateWindow
extern "C" void flwm_activateWindow(Frame *f)
{
  if (f)
    f->activate();
}
//@-node:flwm_activateWindow
//@+node:flwm_deactivateWindow
extern "C" void flwm_deactivateWindow(Frame *f)
{
  if (f)
    f->deactivate();
}
//@-node:flwm_deactivateWindow
//@+node:flwm_geLlabel
extern "C" char *flwm_getLabel(Frame *frm)
{
  //printf("flwm_getLabel: frm=0x%lx\n", frm);
  char *name = (char *)frm->label();
  //printf("flwm_getLabel: name=%s\n", name);
  return name;
}

//@-node:flwm_geLlabel
//@+node:flwm_getPos
extern "C" void flwm_getPos(Frame *frm, int *x, int *y)
{
  //printf("flwm_getPos: start\n");
  *x = frm->x();
  *y = frm->y();
  //printf("flwm_getPos: done\n");
}
//@-node:flwm_getPos
//@+node:flwm_setPos
extern "C" void flwm_setPos(Frame *frm, long x, long y)
{
  //printf("flwm_setPos: x=%d, y=%d\n", x, y);
  frm->set_size(x, y, frm->w(), frm->h(), 0, 1);
  //printf("flwm_setPos: done\n");
}
//@-node:flwm_setPos
//@+node:flwm_getSize
extern "C" void flwm_getSize(Frame *frm, int *w, int *h)
{
  *w = frm->w();
  *h = frm->h();
}
//@-node:flwm_getSize
//@+node:flwm_setSize
extern "C" void flwm_setSize(Frame *frm, long w, long h)
{
  //printf("flwm_setSize: w=%d, h=%d\n", w, h);
  frm->set_size(frm->x(), frm->y(), w, h, 0, 1);
  //printf("flwm_setSize: done\n");
}
//@-node:flwm_setSize
//@+node:flwm_getMousePos
extern "C" void flwm_getMousePos(int *x, int *y)
{
  int x1, y1;
  Fl::get_mouse(x1, y1);
  *x = x1;
  *y = y1;
}

//@-node:flwm_getMousePos
//@+node:flwm_getScreenSize
extern "C" void flwm_getScreenSize(int *x, int *y)
{
  *x = Fl::w();
  *y = Fl::h();
}

//@-node:flwm_getScreenSize
//@+node:flwm_test
Fl_Window *testwin = NULL;
Fl_Browser *testbrows = NULL;

static void cb_mybut(Fl_Widget *, void *)
{
    testwin->hide();
}


extern "C" void flwm_test()
{
    int x = 0, y = 0;
    
    Fl::get_mouse(x, y);
    
    printf("flwm_test: entered\n");

    if (testwin == NULL)
    {
        testwin = new Fl_Window(x, y, 100, 200,
                               "A Window");

        testwin->color(
           parse_fl_color(fg),
           parse_fl_color(bg)
           );
        
        testbrows = new Fl_Select_Browser(
             5, 5, 90, 190, "Choose");

        testbrows->color(parse_fl_color(bg)
                     //,
                     //parse_fl_color(fg)
                     );
        printf("fg=%s\n", fg);
        printf("bg=%s\n", bg);
        printf("cbg=%s\n", cbg);
        printf("cfg=%s\n", cfg);
        printf("abg=%s\n", abg);

        testbrows->add((char *)"Click me", (char *)"First");
        testbrows->callback(cb_mybut);
        testwin->end();
    }
    else
        testbrows->deselect();

    testwin->position(x, y);
    testwin->show();
}


//@-node:flwm_test
//@+node:flwm_setTimer

static void cbTimer(void *pyfunc)
{
    //printf("cbTimer: about to fire python callback\n");
    PyObject_CallObject((PyObject *)pyfunc, NULL);
    //printf("cbTimer: returned from python callback\n");
    Py_DECREF((PyObject *)pyfunc);
}


extern "C" void flwm_setTimer(double secs, PyObject *func)
{
    //printf("flwm_setTimer: func=0x%lx\n", func);
    Py_INCREF(func);
    Fl::add_timeout(secs, cbTimer, (void *)func);
}
 
extern "C" void flwm_repeatTimer(double secs, PyObject *func)
{
    //printf("flwm_repeatTimer: func=0x%lx\n", func);
    Py_INCREF(func);
    Fl::repeat_timeout(secs, cbTimer, (void *)func);
}
//@-node:flwm_setTimer
//@+node:flwm_setBorder
extern "C" void flwm_setBorder(Frame *frm, long mode)
{
  //printf("flwm_setBorder: mode=%d\n", mode);
  frm->setBorder(mode);
  //printf("flwm_setBorder: done\n");
}
//@-node:flwm_setBorder
//@-others
//@-node:@file src-c/flwmapi.cpp
//@-leo
