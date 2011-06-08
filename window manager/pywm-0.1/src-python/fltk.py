#@+leo
#@+node:0::@file src-python/fltk.py
#@+body
#@@language python
"""
fltk.py - a subset of FLTK exposed to Python

"""

#@+others
#@+node:1::imports
#@+body
import fltk_ as flwm_


#@-body
#@-node:1::imports
#@+node:2::class Fl_Window
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
#@-node:2::class Fl_Window
#@+node:3::class Fl_Button
#@+body
class Fl_Button:
    """
    Creates and manages button widgets for use in Fl_Window popup
    windows.
    """
    def __init__(self, x, y, w, h, label):
        """
        Creates a button widget.
        
        Arguments:
         - x, y, w, h - x and y-position within window, width and height
         - label - text to put on the button
        """
        self.wid = flwm_.Fl_Button_create(x, y, w, h, label)
        
    def show(self):
        """probably not much point to this
        """
        flwm_.Fl_Button_show(self.wid)
        
    def hide(self):
        """
        probably not much point to this either
        """
        flwm_.Fl_Button_hide(self.wid)

    def callback(self, func):
        """
        Sets the callback to be triggered when the button gets pushed.
        
        Argument:
         - func - a callable object, eg function, bound method, to be
           invoked when the button gets pushed. Note that at this stage,
           the callable you provide will be called with no arguments.
        """
        flwm_.Fl_Button_callback(self.wid, func)


#@-body
#@-node:3::class Fl_Button
#@+node:4::class Fl_Select_Browser
#@+body
class Fl_Select_Browser:
    """
    Creates and manages listbox widgets for use in Fl_Window popup
    windows.
    """
    def __init__(self, x, y, w, h, label):
        """
        Creates a button widget.
        
        Arguments:
         - x, y, w, h - x and y-position within window, width and height
         - label - text to put on the browser label
        """
        self.wid = flwm_.Fl_Select_Browser_create(x, y, w, h, label)
        
    def show(self):
        """probably not much point to this
        """
        flwm_.Fl_Select_Browser_show(self.wid)
        
    def hide(self):
        """
        probably not much point to this either
        """
        flwm_.Fl_Select_Browser_hide(self.wid)

    def callback(self, func):
        """
        Sets the callback to be triggered when the button gets pushed.
        
        Argument:
         - func - a callable object, eg function, bound method, to be
           invoked when an item in the listbox gets selected.
           Note that at this stage,
           the callable you provide will be called with no arguments.
        """
        flwm_.Fl_Select_Browser_callback(self.wid, func)


#@-body
#@-node:4::class Fl_Select_Browser
#@-others


#@-body
#@-node:0::@file src-python/fltk.py
#@-leo
