#@+leo
#@+node:0::@file src-pyrex/fltk_.pyx
#@+body
#@@language python
"""
Pyrex interface layer to an FLTK subset
"""

#@+others
#@+node:1::fltk imports
#@+body
# Import the C shims for FLTK funcs

cdef extern long fltk_window_create(int x, int y, int w, int h, char *label,
                                    object self, object onenter, object onclick)
cdef extern void fltk_window_destroy(long pWin)
cdef extern void fltk_window_end(long pWin)
cdef extern void fltk_window_show(long pWin)
cdef extern void fltk_window_hide(long pWin)
cdef extern void fltk_window_move(long pWin, long x, long y)
cdef extern void fltk_window_stayOnTop(long pWin)

cdef extern long fltk_button_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_button_destroy(long pWin)
cdef extern void fltk_button_show(long pWin)
cdef extern void fltk_button_hide(long pWin)
cdef extern void fltk_button_callback(long pWin, object func)

cdef extern long fltk_select_browser_create(int x, int y, int w, int h, char *label)
cdef extern void fltk_select_browser_destroy(long pWin)
cdef extern void fltk_select_browser_show(long pWin)
cdef extern void fltk_select_browser_hide(long pWin)
cdef extern void fltk_select_browser_callback(long pWin, object func)


#@-body
#@-node:1::fltk imports
#@+node:2::Fl_Window_*
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
#@-node:2::Fl_Window_*
#@+node:3::Fl_Button_*
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


#@-body
#@-node:3::Fl_Button_*
#@+node:4::Fl_Select_Browser_*
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


#@-body
#@-node:4::Fl_Select_Browser_*
#@-others


#@-body
#@-node:0::@file src-pyrex/fltk_.pyx
#@-leo
