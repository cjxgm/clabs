//@+leo
//@+node:0::@file src-c/fltkshim.cpp
//@+body
//@@language c
//
// fltk-shim.cpp
//
// exposes FLTK C++ methods as C
//


//@+others
//@+node:1::includes
//@+body
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Python.h"

#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <FL/filename.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>



//@-body
//@-node:1::includes
//@+node:2::flwmapi FLTK wrappers
//@+node:1::dispatchCallbackToPython
//@+body
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


//@-body
//@-node:1::dispatchCallbackToPython
//@+node:2::fltk_window_*
//@+body
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

//@-body
//@-node:2::fltk_window_*
//@+node:3::fltk_button_*
//@+body
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


//@-body
//@-node:3::fltk_button_*
//@+node:4::fltk_repeat_button_*
//@+body
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


//@-body
//@-node:4::fltk_repeat_button_*
//@+node:5::fltk_select_browser_*
//@+body
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

//@-body
//@-node:5::fltk_select_browser_*
//@+node:6::fltk_hold_browser_*
//@+body
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

//@-body
//@-node:6::fltk_hold_browser_*
//@+node:7::fltk_input_*
//@+body
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


//@-body
//@-node:7::fltk_input_*
//@+node:8::fltk_color
//@+body
extern "C" long fltk_color(int red, int green, int blue)
{
    long col = (long)fl_color_cube(red * FL_NUM_RED/256,
                               green * FL_NUM_GREEN/256,
                               blue * FL_NUM_BLUE/256);
    //printf("flwmapi.fltk_color: %x %x %x -> %x\n",
    //       red, green, blue, col);
    return col;
}

//@-body
//@-node:8::fltk_color
//@-node:2::flwmapi FLTK wrappers
//@-others


//@-body
//@-node:0::@file src-c/fltkshim.cpp
//@-leo
