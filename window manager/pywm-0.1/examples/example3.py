#!/usr/bin/env python
#@+leo
#@+node:0::@file examples/example3.py
#@+body
#@@first
#@@language python

"""
example3.py

Same as example2, but demonstrates creation keywords
"""

import pywm

class myWindowClass(pywm.window):
    """
    These window classes get instantiated by the PYWM engine.
    You should never instantiate this class yourself.
    """
    def __init__(self, hWin, wm, **kwds):
        #print "myWindowClass.__init__: creating a window"
        pywm.window.__init__(self, hWin, wm, **kwds)
        print "myWindowClass.__init__: window %s created" % self.name


class myWindowManagerClass(pywm.WM):
    """
    Our custom window manager class
    """
    def __init__(self):
        pywm.WM.__init__(
            self,
            windowclass=myWindowClass,
            # display='localhost:0.0',
            # visual=0 # try different numbers here - ref FLWM doco
            # geometry=WxH+X+Y, # constrain windows to this area
            # maximum=WxH # max size of maximised windows
            fg="#ffcc00", # window decoration foreground 
            bg="#000000", # window decoration background
            bg2="#00FF00", # text colour
            abg="#900000", # colour of active window bar
            cfg="#0000FF", # Cursor foreground color
            cbg="#FFFFFF", # Cursor outline color,
            # cursor=0, # Cursor number for root window
            exit=1, # change 'logout' menu prompt to 'exit'
            )

    def on_startup(self):
        """
        If you define an 'on_startup' method in your class, this
        method will get invoked just before the eindow manager
        engine enters its event loop. Note, however, that it
        gets called *after* the existing windows are discovered
        and the corresponding on_create callbacks fired.
        """
        print "myWindowManager: on_startup"

        # bind Shift-Alt-A to run self.launch with arg 'xterm'
        self.bindKey(self.launch, "xterm",
                     'a', 1, 0, 1, 0)

        # bind Shift-Alt-Meta-A to run vi
        self.bindKey(self.launch, "xterm -e vi",
                     'a', 1, 0, 1, 1)

        # bind Shift-Alt-Meta-Insert to run emacs in terminal
        self.bindKey(self.launch, "xterm -e emacs -nw",
                     'insert', 1, 0, 1, 1)
        print "myWindowManager: key binding done"
    

def run():
    # create a window manager object using our class
    windowManager = myWindowManagerClass()

    # and launch the window manager
    windowManager.run()

if __name__ == '__main__':
    run()


#@-body
#@-node:0::@file examples/example3.py
#@-leo
