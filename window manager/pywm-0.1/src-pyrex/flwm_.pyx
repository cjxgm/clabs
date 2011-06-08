#@+leo
#@+node:0::@file src-pyrex/flwm_.pyx
#@+body
"""
flwm_ is a low-level Pyrex-generated module which talks to
the FLWM engine, setting config items, receiving callbacks
and taking actions.

You should never have reason to access anything in this
module directly - instead, use the default pywm.WM and
pywm.window methods.
"""

#@+others
#@+node:1::C header imports
#@+body
#@+others
#@+node:1::System
#@+body
#@+others
#@+node:1::string.h
#@+body
cdef extern from "string.h":

    cdef void *memset(void *s, int c, int n)
    cdef void *memcpy(void *dest, void *src, int n)


#@-body
#@-node:1::string.h
#@+node:2::stdio.h
#@+body
cdef extern from "stdio.h":
    int printf(char *format,...)


#@-body
#@-node:2::stdio.h
#@+node:3::stdlib.h
#@+body
cdef extern from "stdlib.h":
    void *malloc(int size)
    void *realloc(void *ptr, int size)
    void free(void *ptr)


#@-body
#@-node:3::stdlib.h
#@+node:4::math.h
#@+body
cdef extern from "math.h":
    double fabs(double x)


#@-body
#@-node:4::math.h
#@-others


#@-body
#@-node:1::System
#@+node:2::X
#@+body
cdef extern from "X11/X.h":
    cdef enum XEventConstants:
        KeyPress, KeyRelease, ButtonPress, ButtonRelease, MotionNotify,
        EnterNotify, LeaveNotify, FocusIn, FocusOut, KeymapNotify,
        Expose, GraphicsExpose, NoExpose, VisibilityNotify,
        CreateNotify, DestroyNotify, UnmapNotify, MapNotify, MapRequest,
        ReparentNotify, ConfigureNotify, ConfigureRequest, GravityNotify,
        ResizeRequest, CirculateNotify, CirculateRequest, PropertyNotify,
        SelectionClear, SelectionRequest, SelectionNotify, ColormapNotify,
        ClientMessage, MappingNotify, LASTEvent

    cdef void Xfree(char *pre)

    ctypedef unsigned long Window
    ctypedef unsigned long Display

    cdef int XQueryPointer( Display *display,
                            Window w,
                            Window *root_return,
                            Window *child_return,
                            int *root_x_return,
                            int *root_y_return,
                            int *win_x_return,
                            int *win_y_return,
                            unsigned int *mask_return
                            )

cdef extern from "X11/keysym.h":
    cdef enum Xkeyconstants:
        XK_a
        XK_b
        XK_c
        XK_d
        XK_e
        XK_f
        XK_g
        XK_h
        XK_i
        XK_j
        XK_k
        XK_l
        XK_m
        XK_n
        XK_o
        XK_p
        XK_q
        XK_r
        XK_s
        XK_t
        XK_u
        XK_v
        XK_w
        XK_x
        XK_y
        XK_z
        XK_0
        XK_1
        XK_2
        XK_3
        XK_4
        XK_5
        XK_6
        XK_7
        XK_8
        XK_9

#@-body
#@-node:2::X
#@+node:3::Python
#@+body
#@+others
#@+node:1::python.h
#@+body
# Python-specific imports

cdef extern from "Python.h":
    object PyString_FromStringAndSize(char *, int)
    object PyString_FromString(char *)
    char* PyString_AsString(object string)
    object PyLong_FromLong(long v)
    unsigned long PyLong_AsLong(object pylong)
    object PyInt_FromLong(long v)
    unsigned long PyInt_AsLong(object pylong)
    double PyFloat_AsDouble(object pyFloat)

#@-body
#@-node:1::python.h
#@-others


#@-body
#@-node:3::Python
#@+node:4::flwm c/c++ imports
#@+body
cdef extern int flwm_main(int argc, char **argv, object wmobj)
cdef extern void flwm_closeWindow(long hWin)
cdef extern void flwm_activateWindow(long hWin)
cdef extern void flwm_deactivateWindow(long hWin)
cdef extern void flwm_raiseWindow(long hWin)
cdef extern char *flwm_getLabel(long hWin)
cdef extern int flwm_getPos(long hWin, int *x, int *y)
cdef extern void flwm_getSize(long hWin, int *w, int *h)
cdef extern void flwm_setBorder(long hWin, int mode)
cdef extern void flwm_setPos(long hWin, long x, long y)
cdef extern void flwm_setSize(long hWin, long w, long h)
cdef extern void flwm_getMousePos(int *x, int *y)
cdef extern void flwm_getScreenSize(int *x, int *y)

cdef extern long flwm_keyCode(char *keystr,
                              int shift, int ctrl, int alt, int meta)
cdef extern void flwm_bindKey(long key)

cdef extern void flwm_goNextWindow()
cdef extern void flwm_goPrevWindow()
cdef extern void flwm_goNextDesktop()
cdef extern void flwm_goPrevDesktop()

cdef extern void flwm_showMenu()

cdef extern void flwm_test()

cdef extern void flwm_setTimer(double secs, object func)
cdef extern void flwm_repeatTimer(double secs, object func)

#@-body
#@-node:4::flwm c/c++ imports
#@+node:5::Python FLTK classes
#@+node:1::class Fl_Window
#@+body
class Fl_Window:
    """
    This class allows you to create popup windows for user
    interaction.
    
    The windows are created/managed by the FLTK graphical toolkit
    (which the FLWM engine uses), and are not managed as application
    windows (meaning - no border, no window handle etc).
    
    The python interface to Fl_Window is a close subset of the
    FLTK Fl_Window class (refer to your FLTK manual - http://fltk.sf.net
    if you don't have it locally).
    """
    def __init__(self, x, y, w, h, label=""):
        """
        Creates a new window manager popup window.
        
        Arguments:
         - x, y, w, h - x-position, y-position, width and height
         - label - probably no point to this since there's no titlebar
        """
        print "Fl_Window: entered"
        on_enter = self.on_enter
        on_click = self.on_click
        print "about to create fl window"
        self.wid = flwm_.Fl_Window_create(x, y, w, h, label,
                                          self, on_enter, on_click)

    def on_enter(self, x, y):
        """
        Called when the mouse enters this window.
        If you override this, you must accept 2 args, x and y,
        which will be the position of the mouse on entry.
        """
        print "Fl_Window.on_enter: mouse at %d,%d" % (x, y)

    def on_click(self, but, x, y):
        """
        Called when the mouse enters this window.
        If you override this, you must accept three args,
        but, x and y, which will be the button number, and
        the position of the mouse on entry.
        """
        print "Fl_Window.on_click: button %d, mouse at %d,%d" % (but, x, y)

    def end(self, arg=None):
        """
        End a window's definition phase. Call this after all widgets
        have been added to the window
        """
        flwm_.Fl_Window_end(self.wid)

    def show(self, arg=None):
        """
        Make the window visible
        """
        flwm_.Fl_Window_show(self.wid)

    def hide(self, arg=None):
        """
        Hide the window
        """
        
        #print "Fl_Window_hide??"
        flwm_.Fl_Window_hide(self.wid)

    def move(self, x, y):
        """
        Move the window to x, y
        """
        flwm_.Fl_Window_move(self.wid, x, y)

    def stayOnTop(self):
        """
        Bring the window to the top where it will hopefully stay.
        """
        flwm_.Fl_Window_stayOnTop(self.wid)



#@-body
#@-node:1::class Fl_Window
#@+node:2::fltk imports
#@+body
# Import the C shims for FLTK funcs

cdef extern long fltk_window_create(int x, int y, int w, int h, char *label,
                                    object self, object onenter, object onclick)
cdef extern void fltk_window_destroy(long pWin)
cdef extern void fltk_window_end(long pWin)
cdef extern void fltk_window_show(long pWin)
cdef extern void fltk_window_hide(long pWin)
cdef extern void fltk_window_activate(long pWin)
cdef extern void fltk_window_set_modal(long pWin)
cdef extern void fltk_window_move(long pWin, long x, long y)
cdef extern void fltk_window_stayOnTop(long pWin)

cdef extern long fltk_button_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_button_destroy(long pWin)
cdef extern void fltk_button_show(long pWin)
cdef extern void fltk_button_hide(long pWin)
cdef extern void fltk_button_callback(long pWin, object func)
cdef extern void fltk_button_symbollabel(long pWid)

cdef extern long fltk_repeat_button_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_repeat_button_destroy(long pWin)
cdef extern void fltk_repeat_button_show(long pWin)
cdef extern void fltk_repeat_button_hide(long pWin)
cdef extern void fltk_repeat_button_callback(long pWin, object func)
cdef extern void fltk_repeat_button_symbollabel(long pWid)

cdef extern long fltk_select_browser_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_select_browser_destroy(long pWin)
cdef extern void fltk_select_browser_show(long pWin)
cdef extern void fltk_select_browser_hide(long pWin)
cdef extern void fltk_select_browser_callback(long pWin, object func)
cdef extern void fltk_select_browser_add(long pWin, char *item)
cdef extern void fltk_select_browser_remove(long pWin, int item)
cdef extern void fltk_select_browser_clear(long pWin)
cdef extern void fltk_select_browser_color(long pWin, int color)
cdef extern void fltk_select_browser_textcolor(long pWin, int color)
cdef extern void fltk_select_browser_selectioncolor(long pWin, int color)
cdef extern int fltk_select_browser_value(long pWin, int val)

cdef extern long fltk_hold_browser_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_hold_browser_destroy(long pWin)
cdef extern void fltk_hold_browser_show(long pWin)
cdef extern void fltk_hold_browser_hide(long pWin)
cdef extern void fltk_hold_browser_callback(long pWin, object func)
cdef extern void fltk_hold_browser_add(long pWin, char *item)
cdef extern void fltk_hold_browser_remove(long pWin, int item)
cdef extern void fltk_hold_browser_clear(long pWin)
cdef extern void fltk_hold_browser_color(long pWin, int color)
cdef extern void fltk_hold_browser_textcolor(long pWin, int color)
cdef extern void fltk_hold_browser_selectioncolor(long pWin, int color)
cdef extern int fltk_hold_browser_value(long pWin, int val)

cdef extern long fltk_color(int red, int green, int blue)

cdef extern long fltk_input_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_input_callback(long pWin, object func)
cdef extern char *fltk_input_value(long pWin, char *val)
cdef extern void fltk_input_color(long pWin, int color)
cdef extern void fltk_input_textcolor(long pWin, int color)
cdef extern void fltk_input_selectioncolor(long pWin, int color)
cdef extern void fltk_input_take_focus(long pWid)


#@-body
#@-node:2::fltk imports
#@+node:3::Fl_Window_*
#@+body
# funcs for working with Fl_Window

def Fl_Window_create(x, y, w, h, label, self, onenter, onclick):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid

    print "Fl_Window_create: entered"

    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
    cLabel = PyString_AsString(label)

    cWid = fltk_window_create(cX, cY, cW, cH, cLabel, self, onenter, onclick)
    #printf("Fl_Window_create: cWid=0x%lx\n", cWid)
    wid = PyLong_FromLong(cWid)
    #print "Fl_Window_create: self.cWid=0x%lx" % wid
    return wid

def Fl_Window_end(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_end: cWid=0x%lx\n" % cWid
    fltk_window_end(cWid)

def Fl_Window_show(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_show: cWid=0x%lx\n" % cWid
    fltk_window_show(cWid)

def Fl_Window_hide(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_hide cWid=0x%lx\n" % cWid
    fltk_window_hide(cWid)

def Fl_Window_activate(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_hide cWid=0x%lx\n" % cWid
    fltk_window_activate(cWid)

def Fl_Window_set_modal(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_hide cWid=0x%lx\n" % cWid
    fltk_window_set_modal(cWid)

def Fl_Window_move(wid, x, y):
    cdef long cWid
    cdef long cX, cY
    #print "Fl_Window_move"
    cWid = PyLong_AsLong(wid)
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    #print "Fl_Window_hide cWid=0x%lx\n" % cWid
    fltk_window_move(cWid, cX, cY)

def Fl_Window_stayOnTop(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    #print "Fl_Window_stayOnTop cWid=0x%lx\n" % cWid
    fltk_window_stayOnTop(cWid)

#@-body
#@-node:3::Fl_Window_*
#@+node:4::Fl_Button_*
#@+body
def Fl_Button_create(x, y, w, h, label):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid
        
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
       
    cLabel = PyString_AsString(label)
    cWid = fltk_button_create(cX, cY, cW, cH, cLabel)
    wid = PyLong_FromLong(cWid)
    return wid
   
def Fl_Button_show(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_button_show(cWid)

def Fl_Button_hide(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_button_hide(cWid)

def Fl_Button_callback(wid, func):
    cdef long cWid
    cdef long cFunc
    cWid = PyLong_AsLong(wid)
    #cFunc = <long>func
    #print "Fl_Button: callback=0x%lx" % <long>func
    fltk_button_callback(cWid, func)

def Fl_Button_symbollabel(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_button_symbollabel(cWid)

#@-body
#@-node:4::Fl_Button_*
#@+node:5::Fl_Repeat_Button_*
#@+body
def Fl_Repeat_Button_create(x, y, w, h, label):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid
        
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
       
    cLabel = PyString_AsString(label)
    cWid = fltk_repeat_button_create(cX, cY, cW, cH, cLabel)
    wid = PyLong_FromLong(cWid)
    return wid
   
def Fl_Repeat_Button_show(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_repeat_button_show(cWid)

def Fl_Repeat_Button_hide(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_repeat_button_hide(cWid)

def Fl_Repeat_Button_callback(wid, func):
    cdef long cWid
    cdef long cFunc
    cWid = PyLong_AsLong(wid)
    #cFunc = <long>func
    #print "Fl_Button: callback=0x%lx" % <long>func
    fltk_repeat_button_callback(cWid, func)

def Fl_Repeat_Button_symbollabel(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_repeat_button_symbollabel(cWid)

#@-body
#@-node:5::Fl_Repeat_Button_*
#@+node:6::Fl_Select_Browser_*
#@+body
def Fl_Select_Browser_create(x, y, w, h, label):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid
        
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
       
    cLabel = PyString_AsString(label)
    cWid = fltk_select_browser_create(cX, cY, cW, cH, cLabel)
    wid = PyLong_FromLong(cWid)
    return wid
   
def Fl_Select_Browser_show(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_select_browser_show(cWid)

def Fl_Select_Browser_hide(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_select_browser_hide(cWid)

def Fl_Select_Browser_callback(wid, func):
    cdef long cWid
    cdef long cFunc
    cWid = PyLong_AsLong(wid)
    #cFunc = <long>func
    #print "Fl_Select_Browser: callback=0x%lx" % <long>func
    fltk_select_browser_callback(cWid, func)

def Fl_Select_Browser_add(wid, item):
    cdef long cWid
    cdef char *cItem

    cWid = PyLong_AsLong(wid)
    cItem = PyString_AsString(item)
    fltk_select_browser_add(cWid, cItem)

def Fl_Select_Browser_remove(wid, item):
    cdef long cWid
    cdef int cItem

    cWid = PyLong_AsLong(wid)
    cItem = PyInt_AsLong(item)
    fltk_select_browser_remove(cWid, cItem)

def Fl_Select_Browser_clear(wid):
    cdef long cWid

    cWid = PyLong_AsLong(wid)
    fltk_select_browser_clear(cWid)

def Fl_Select_Browser_color(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_select_browser_color(cWid, cColor)

def Fl_Select_Browser_textcolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_select_browser_textcolor(cWid, cColor)

def Fl_Select_Browser_selectioncolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_select_browser_selectioncolor(cWid, cColor)

def Fl_Select_Browser_value(wid, val=-2):
    cdef long cWid
    cdef int cVal

    cWid = PyLong_AsLong(wid)
    cVal = PyInt_AsLong(val)
    cVal = fltk_select_browser_value(cWid, cVal)
    val = PyInt_FromLong(cVal)
    return val

#@-body
#@-node:6::Fl_Select_Browser_*
#@+node:7::Fl_Hold_Browser_*
#@+body
def Fl_Hold_Browser_create(x, y, w, h, label):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid
        
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
       
    cLabel = PyString_AsString(label)
    cWid = fltk_hold_browser_create(cX, cY, cW, cH, cLabel)
    wid = PyLong_FromLong(cWid)
    return wid
   
def Fl_Hold_Browser_show(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_hold_browser_show(cWid)

def Fl_Hold_Browser_hide(wid):
    cdef long cWid
    cWid = PyLong_AsLong(wid)
    fltk_hold_browser_hide(cWid)

def Fl_Hold_Browser_callback(wid, func):
    cdef long cWid
    cdef long cFunc
    cWid = PyLong_AsLong(wid)
    #cFunc = <long>func
    #print "Fl_Hold_Browser: callback=0x%lx" % <long>func
    fltk_hold_browser_callback(cWid, func)

def Fl_Hold_Browser_add(wid, item):
    cdef long cWid
    cdef char *cItem

    cWid = PyLong_AsLong(wid)
    cItem = PyString_AsString(item)
    fltk_hold_browser_add(cWid, cItem)

def Fl_Hold_Browser_remove(wid, item):
    cdef long cWid
    cdef int cItem

    cWid = PyLong_AsLong(wid)
    cItem = PyInt_AsLong(item)
    fltk_hold_browser_remove(cWid, cItem)

def Fl_Hold_Browser_clear(wid):
    cdef long cWid

    print "Fl_Hold_Browser_clear: entered"
    cWid = PyLong_AsLong(wid)
    fltk_hold_browser_clear(cWid)

def Fl_Hold_Browser_color(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_hold_browser_color(cWid, cColor)

def Fl_Hold_Browser_textcolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_hold_browser_textcolor(cWid, cColor)

def Fl_Hold_Browser_selectioncolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_hold_browser_selectioncolor(cWid, cColor)

def Fl_Hold_Browser_value(wid, val=-2):
    cdef long cWid
    cdef int cVal

    cWid = PyLong_AsLong(wid)
    cVal = PyInt_AsLong(val)
    cVal = fltk_hold_browser_value(cWid, cVal)
    val = PyInt_FromLong(cVal)
    return val


#@-body
#@-node:7::Fl_Hold_Browser_*
#@+node:8::Fl_Input_*
#@+body
def Fl_Input_create(x, y, w, h, label):
    cdef int cX, cY, cW, cH
    cdef char *cLabel
    cdef long cWid
        
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)
       
    cLabel = PyString_AsString(label)
    cWid = fltk_input_create(cX, cY, cW, cH, cLabel)
    wid = PyLong_FromLong(cWid)
    return wid

def Fl_Input_callback(wid, func):
    cdef long cWid
    cdef long cFunc
    cWid = PyLong_AsLong(wid)
    #cFunc = <long>func
    #print "Fl_Hold_Browser: callback=0x%lx" % <long>func
    fltk_input_callback(cWid, func)

def Fl_Input_value(wid, val):
    cdef long cWid
    cdef char *cVal

    #print "flwm_.Fl_Input_value: entered"
    cWid = PyLong_AsLong(wid)
    if val == None:
        #print "input: just getting value"
        cVal = <char *>0
    else:
        cVal = PyString_AsString(val)
    cVal = fltk_input_value(cWid, cVal)
    val = PyString_FromString(cVal)
    return val

def Fl_Input_color(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_input_color(cWid, cColor)

def Fl_Input_textcolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_input_textcolor(cWid, cColor)

def Fl_Input_selectioncolor(wid, pColor):
    cdef long cWid
    cdef int cColor

    cWid = PyLong_AsLong(wid)
    cColor = PyInt_AsLong(pColor)
    fltk_input_selectioncolor(cWid, cColor)

def Fl_Input_take_focus(wid):
    cdef long cWid

    cWid = PyLong_AsLong(wid)
    fltk_input_take_focus(cWid)

#@-body
#@-node:8::Fl_Input_*
#@+node:9::color
#@+body
def color(red, green, blue):
    cdef int cRed
    cdef int cGreen
    cdef int cBlue
    cdef long cColor

    cRed = PyInt_AsLong(red)
    cGreen = PyInt_AsLong(green)
    cBlue = PyInt_AsLong(blue)
    
    cColor = fltk_color(cRed, cGreen, cBlue)
    pColor = PyInt_FromLong(cColor)
    #print "flwm_.color: %x %x %x -> %x" % (red, green, blue, pColor)
    return pColor



#@-body
#@-node:9::color
#@-node:5::Python FLTK classes
#@-others


#@-body
#@-node:1::C header imports
#@+node:2::Python Imports
#@+body
import sys
import os
import traceback

#@-body
#@-node:2::Python Imports
#@+node:3::CALLBACKS from flwm
#@+node:1::globals
#@+body
callbackNexting = 0

#@-body
#@-node:1::globals
#@+node:2::py_on_startup
#@+body
cdef public py_on_startup(object wm):
    
    for f in wm.hdlrsStartup:
        try:
            if f():
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_startup: chained handler ok"
        except:
            print "flwm_.py_on_create: a chained handler failed"
            traceback.print_exc()
            pass

    #printf("on_startup: entered\n")
    wm.on_startup()
    #printf("on_startup: done\n")


#@-body
#@-node:2::py_on_startup
#@+node:3::py_on_create
#@+body
cdef public int py_on_create(object wm, unsigned long win):

    cdef int res
    
    hWin = PyLong_FromLong(win)
    #print "py_on_create: win=%lx" % hWin
    pWin = wm._addWin(hWin)

    for f in wm.hdlrsCreate:
        try:
            if f(pWin):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_create: chained handler ok"
        except:
            print "flwm_.py_on_create: a chained handler failed"
            traceback.print_exc()
            pass

    wm.on_create(pWin)
    
    #print "py_on_create: pRes=%d" % pRes
    #res = PyLong_AsLong(pRes)
    #printf("py_on_create: res=%d\n", res)
    #return res



#@-body
#@-node:3::py_on_create
#@+node:4::py_on_destroy
#@+body
cdef public py_on_destroy(object wm, unsigned long win):
    hWin = PyLong_FromLong(win)
    pWin = wm.window(hWin)
    #printf("py_on_destroy: entered - got instance\n", win)

    wm.on_destroy(pWin)
    wm._delWin(hWin)

    for f in wm.hdlrsDestroy:
        try:
            if f(pWin):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_destroy: chained handler ok"
        except:
            print "flwm_.py_on_destroy: a chained handler failed"
            traceback.print_exc()
            pass

    #printf("py_on_destroy: exit\n")



#@-body
#@-node:4::py_on_destroy
#@+node:5::py_on_activate
#@+body
cdef long lastActiveWindow

cdef public py_on_activate(object wm, unsigned long win):
    global lastActiveWindow
    if win != lastActiveWindow:
        lastActiveWindow = win
        hWin = PyLong_FromLong(win)
        pWin = wm.window(hWin)
        #printf("on_activate: entered - got instance\n", win)
        wm.currentWindow = pWin

        for f in wm.hdlrsActivate:
            try:
                if f(pWin):
                    print "flwm_.py: handler break"
                    break
                print "flwm_.py_on_activate: chained handler ok"
            except:
                print "flwm_.py_on_activate: a chained handler failed"
                traceback.print_exc()
                pass

        wm.on_activate(pWin)
#@-body
#@-node:5::py_on_activate
#@+node:6::py_on_deactivate
#@+body
cdef public py_on_deactivate(object wm, unsigned long win):
    hWin = PyLong_FromLong(win)
    pWin = wm.window(hWin)
    #printf("on_deactivate: entered - got instance\n", win)

    for f in wm.hdlrsDeactivate:
        try:
            if f(pWin):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_deactivate: chained handler ok"
        except:
            print "flwm_.py_on_deactivate: a chained handler failed"
            traceback.print_exc()
            pass

    wm.on_deactivate(pWin)

#@-body
#@-node:6::py_on_deactivate
#@+node:7::py_onSize
#@+body
#cdef public int py_onSize(unsigned long hWin, int x, int y, int w, int h, int warp):
#    print "py_onSize: x=%d y=%d w=%d h=%d warp=%d" % (
#        x, y, w, h, warp)


#@-body
#@-node:7::py_onSize
#@+node:8::py_on_resize
#@+body
cdef public int py_on_resize(object wm, unsigned long win,
                             int x, int y, int w, int h, int warp):
    hWin = PyLong_FromLong(win)
    pWin = wm.window(hWin)
    #printf("on_resize: entered - got instance\n")

    for f in wm.hdlrsResize:
        try:
            if f(pWin):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_resize: chained handler ok"
        except:
            print "flwm_.py_on_resize: a chained handler failed"
            traceback.print_exc()
            pass

    wm.on_resize(pWin, x, y, w, h)

#@-body
#@-node:8::py_on_resize
#@+node:9::py_on_buttonPress
#@+body
cdef public int py_on_buttonPress(object wm, long button):
    cdef int cRes
    pButton = PyLong_FromLong(button)
    
    for f in wm.hdlrsButtonPress:
        try:
            if f(pButton):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_buttonPress: chained handler ok"
        except:
            print "flwm_.py_on_buttonPress: a chained handler failed"
            traceback.print_exc()
            pass

    cRes = <int>PyLong_AsLong(wm.on_buttonPress(pButton))
    return cRes



#@-body
#@-node:9::py_on_buttonPress
#@+node:10::py_on_keyEvent
#@+body
cdef public int py_on_keyEvent(object wm, int key):
    cdef long cRes
    pKey = PyLong_FromLong(key)
    #print "py_on_keyEvent: got keystroke"

    for f in wm.hdlrsKeyEvent:
        try:
            if f(pKey):
                print "flwm_.py: handler break"
                break
            print "flwm_.py_on_keyEvent: chained handler ok"
        except:
            print "flwm_.py_on_keyEvent: a chained handler failed"
            traceback.print_exc()
            pass

    res = wm.on_keyEvent(pKey)
    cRes = PyLong_AsLong(res)
    return cRes



#@-body
#@-node:10::py_on_keyEvent
#@+node:11::py_on_enter
#@+body
cdef public py_on_enter(object wm, unsigned long win):
    hWin = PyLong_FromLong(win)
    pWin = wm.window(hWin)
    wm.on_enter(pWin)

#@-body
#@-node:11::py_on_enter
#@+node:12::py_on_leave
#@+body
cdef public py_on_leave(object wm, unsigned long win):
    hWin = PyLong_FromLong(win)
    pWin = wm.window(hWin)
    printf("on_leave: entered\n")
    wm.on_leave(pWin)

#@-body
#@-node:12::py_on_leave
#@-node:3::CALLBACKS from flwm
#@+node:4::ACTIONS to FLWM
#@+body
#
# Define funcs in the API layer, allowing pyrex code to
# make calls into flwm


#@+others
#@+node:1::flwm_.run
#@+body
def run(args, wmobj):
    """
    Runs the window manager.
    
    Argument is a ref to the window manager object (which will
    receive callbacks).
    """
    cdef int i
    cdef int _argc
    cdef char **_argv

    _argc = len(args) + 1
    _argv = <char **>malloc(sizeof(char *) * _argc)
    _argv[0] = PyString_AsString(sys.argv[0])

    i = 1
    for arg in args:
        _argv[i] = PyString_AsString(arg)
        i = i + 1
    flwm_main(_argc, _argv, wmobj)

#@-body
#@-node:1::flwm_.run
#@+node:2::flwm_.getMousePos
#@+body
def getMousePos():
    """
    Get the current position of the mouse and return it as
    an (x, y) tuple
    """
    cdef int x
    cdef int y
    flwm_getMousePos(&x, &y)
    return x, y

#@-body
#@-node:2::flwm_.getMousePos
#@+node:3::flwm_.getScreenSize
#@+body
def getScreenSize():
    """
    Get the size of the screen and returns it as
    an (x, y) tuple
    """
    cdef int x
    cdef int y
    flwm_getScreenSize(&x, &y)
    return x, y

#@-body
#@-node:3::flwm_.getScreenSize
#@+node:4::flwm_.keycode
#@+body
def keyCode(keystr, shift, control, alt, meta):
    """
    Determines the keycode number for a given key sequence
    
    Arguments:
     - as for WM.bindKey, without the first 'func' arg
    """
    cdef char *cKeystr
    cdef int cShift
    cdef int cControl
    cdef int cAlt
    cdef int cMeta
    cdef int cRes

    cKeystr = PyString_AsString(keystr)
    cShift = <int>PyLong_AsLong(shift)
    cControl = <int>PyLong_AsLong(control)
    cAlt = <int>PyLong_AsLong(alt)
    cMeta = <int>PyLong_AsLong(meta)
    cRes = flwm_keyCode(cKeystr, cShift, cControl, cAlt, cMeta)
    res = PyLong_FromLong(<long>cRes)
    return res

#@-body
#@-node:4::flwm_.keycode
#@+node:5::flwm_.bindKey
#@+body
def bindKey(keynum):
    """
    Tells the FLWM engine to watch out for a certain key.combination
    
    When such key combination is pressed, the window manager object's
    on_keyEvent callback will fire
    """
    cdef long cKeyNum

    #print "flwm_.bindKey: entered"
    cKeyNum = PyLong_AsLong(keynum)
    #print "flwm_.bindKey: calling flwm_bind_key key=%x" % keynum
    flwm_bindKey(cKeyNum)
    #print "flwm_.bindKey: returned from flwm_bind_key"


#@-body
#@-node:5::flwm_.bindKey
#@+node:6::flwm_.getLabel
#@+body
def getLabel(hWin):
    """
    Returns the label of the given window.
    Argument is window 'handle'
    """
    cdef long win

    #print "flwm_.getLabel: hwin=0x%lx" % hWin
    win = PyLong_AsLong(hWin)    

    label = flwm_getLabel(win)

    # when I was relying on automatic conversions, the window manager
    # would start up, the on_create callbacks would fire, and this
    # call was succeeding.
    
    # but strangely, when new windows were created *after* launching
    # the window manager, calls to this method would fail, and the
    # print statement below would never get reached.

    #print "flwm_.getLabel done"
    return label

#@-body
#@-node:6::flwm_.getLabel
#@+node:7::flwm_.getSize
#@+body
def getSize(hWin):
    """
    Return the size of the given window (arg window handle)
    as an x, y tuple
    """
    cdef long cWin
    cdef int x
    cdef int y

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.getSize: calling flwm_getSize"
    flwm_getSize(cWin, &x, &y)
    #print "flwm_.getSize: returned from flwm_getSize"

    return x, y

#@-body
#@-node:7::flwm_.getSize
#@+node:8::flwm_.setSize
#@+body
def setSize(hWin, w, h):
    """
    Resize the given window to x y
    """
    # declare explicitly C variables
    cdef long cWin
    cdef long cW
    cdef long cH

    # perform manual conversions
    cWin = PyLong_AsLong(hWin)
    cW = PyLong_AsLong(w)
    cH = PyLong_AsLong(h)

    #print "flwm_.setSize: calling flwm_setSize"
    flwm_setSize(cWin, cW, cH)
    #print "flwm_.setSize: returned from flwm_setSize"

#@-body
#@-node:8::flwm_.setSize
#@+node:9::flwm_.getPos
#@+body
def getPos(hWin):
    """
    Return the position of the given window (handle) as
    an x, y tuple
    """
    cdef long cWin
    cdef int x
    cdef int y

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.getPos: calling flwm_getPos"
    flwm_getPos(cWin, &x, &y)
    #print "flwm_.getPos: returned from flwm_getPos"

    return x, y




#@-body
#@-node:9::flwm_.getPos
#@+node:10::flwm_.setPos
#@+body
def setPos(hWin, x, y):
    """
    Set the position of the given window (handle) to x, y
    """
    cdef long cWin
    cdef long cX
    cdef long cY

    cWin = PyLong_AsLong(hWin)
    cX = PyLong_AsLong(x)
    cY = PyLong_AsLong(y)

    #print "flwm_.setPos: calling flwm_setPos"
    flwm_setPos(cWin, cX, cY)
    #print "flwm_.setPos: returned from flwm_setPos"


#@-body
#@-node:10::flwm_.setPos
#@+node:11::flwm_.setBorder
#@+body
def setBorder(hWin, mode):
    """
    Set the position of the given window (handle) to x, y
    """
    cdef long cWin
    cdef long cMode

    cWin = PyLong_AsLong(hWin)
    cMode = PyLong_AsLong(mode)

    #print "flwm_.setBorder: calling flwm_setBorder"
    flwm_setBorder(cWin, cMode)
    #print "flwm_.setBorder: returned from flwm_setBorder"



#@-body
#@-node:11::flwm_.setBorder
#@+node:12::flwm_.closeWindow
#@+body
def closeWindow(hWin):
    """
    Close the current window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.closeWindow: calling flwm_closeWindow"
    flwm_closeWindow(cWin)
    #print "flwm_.closeWindow: returned from flwm_closeWindow"

#@-body
#@-node:12::flwm_.closeWindow
#@+node:13::flwm_.raiseWindow
#@+body
def raiseWindow(hWin):
    """
    Raise the given window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.raiseWindow: calling flwm_raiseWindow"
    flwm_raiseWindow(cWin)
    #print "flwm_.raiseWindow: returned from flwm_raiseWindow"

#@-body
#@-node:13::flwm_.raiseWindow
#@+node:14::flwm_.lowerWindow
#@+body
def lowerWindow(hWin):
    """
    Lower the given window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.lowerWindow: calling flwm_lowerWindow"
    flwm_lowerWindow(cWin)
    #print "flwm_.lowerWindow: returned from flwm_lowerWindow"

#@-body
#@-node:14::flwm_.lowerWindow
#@+node:15::flwm_.iconiseWindow
#@+body
def iconiseWindow(hWin):
    """
    Iconise the given window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.iconiseWindow: calling flwm_iconiseWindow"
    flwm_iconiseWindow(cWin)
    #print "flwm_.iconiseWindow: returned from flwm_iconiseWindow"

#@-body
#@-node:15::flwm_.iconiseWindow
#@+node:16::flwm_.activateWindow
#@+body
def activateWindow(hWin):
    """
    Activate the given window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.activateWindow: calling flwm_activateWindow"
    flwm_activateWindow(cWin)
    #print "flwm_.activateWindow: returned from flwm_activateWindow"

#@-body
#@-node:16::flwm_.activateWindow
#@+node:17::flwm_.deactivateWindow
#@+body
def deactivateWindow(hWin):
    """
    Activate the given window (handle)
    """
    cdef long cWin

    cWin = PyLong_AsLong(hWin)

    #print "flwm_.activateWindow: calling flwm_activateWindow"
    flwm_deactivateWindow(cWin)
    #print "flwm_.activateWindow: returned from flwm_activateWindow"

#@-body
#@-node:17::flwm_.deactivateWindow
#@+node:18::flwm_.goNextWindow
#@+body
def goNextWindow():
    """
    Raise and activate the next window in the chain
    """
    print "flwm_.goNextWindow: calling flwm_goNextWindow"
    flwm_goNextWindow()
    print "flwm_.goNextWindow: returned from flwm_goNextWindow"


#@-body
#@-node:18::flwm_.goNextWindow
#@+node:19::flwm_.goPrevWindow
#@+body
def goPrevWindow():
    """
    Raise and activate the previous window in the chain
    """
    print "flwm_.goPrevWindow: calling flwm_goPrevWindow"
    flwm_goPrevWindow()
    print "flwm_.goPrevWindow: returned from flwm_goPrevWindow"

#@-body
#@-node:19::flwm_.goPrevWindow
#@+node:20::flwm_.goNextDesk
#@+body
def goNextDesk():
    """
    Go to the next desktop
    """
    #print "flwm_.goNextDesk: calling flwm_goNextDesk"
    flwm_goNextDesk()
    #print "flwm_.goNextDesk: returned from flwm_goNextDesk"

#@-body
#@-node:20::flwm_.goNextDesk
#@+node:21::flwm_.goPrevDesk
#@+body
def goPrevDesk():
    """
    Go to the previous desktop
    """
    #print "flwm_.goPrevDesk: calling flwm_goPrevDesk"
    flwm_goPrevDesk()
    #print "flwm_.goPrevDesk: returned from flwm_goPrevDesk"

#@-body
#@-node:21::flwm_.goPrevDesk
#@+node:22::flwm_.showMenu
#@+body
def showMenu():
    """
    Pop up the FLWM main menu
    """
    print "flwm_.showMenu"
    flwm_showMenu()

#@-body
#@-node:22::flwm_.showMenu
#@+node:23::flwm_.test
#@+body
def test(arg=None):
    print "flwm_.test: running flwm_test()"
    flwm_test()
    print "flwm_.test: done"

#@-body
#@-node:23::flwm_.test
#@+node:24::flwm_.setTimer
#@+body
def setTimer(secs, func):
    cdef double cSecs

    cSecs = PyFloat_AsDouble(secs)

    #print "flwm_.setTimer: calling flwm_setTimer"
    flwm_setTimer(cSecs, func)
    #print "flwm_.setTimer: returned from flwm_setTimer"

#@-body
#@-node:24::flwm_.setTimer
#@+node:25::flwm_.repeatTimer
#@+body
def repeatTimer(secs, func):
    cdef double cSecs

    cSecs = PyFloat_AsDouble(secs)

    #print "flwm_.setTimer: calling flwm_repeatTimer"
    flwm_repeatTimer(cSecs, func)
    #print "flwm_.setTimer: returned from flwm_repeatTimer"


#@-body
#@-node:25::flwm_.repeatTimer
#@-others


#@-body
#@-node:4::ACTIONS to FLWM
#@-others


#@-body
#@-node:0::@file src-pyrex/flwm_.pyx
#@-leo
