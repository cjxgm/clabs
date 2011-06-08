#@+leo
#@+node:0::@file src-python/flwm.py
#@+body
#@@language python

"""
Module flwm

Implements the python-level interface with hooks to and from the
Fast Light Window Manager (FLWM) (http://flwm.sourceforge.net)

Aim is to allow FLWM to be driven under Python control as an
X window manager, for Python callbacks to receive selected
events and to be able to take certain actions.

Written 2003 and Copyright (C) by David McNab <david@freenet.org.nz>
"""


#@+others
#@+node:1::Python imports
#@+body
import sys
import os

import flwm_


#@-body
#@-node:1::Python imports
#@+node:2::class window
#@+body
class window:
    """
    This class wraps an X window in a form where your python
    window manager code can manipulate it.
    
    Sorry, but at present there's no way to subclass this window and
    override methods. Therefore, all events will be targetted at callbacks
    in the WM class (or your subclass).
    """

    #@+others
    #@+node:1::window.__init__
    #@+body
    def __init__(self, hWin):
        """
        You should not instantiate windows yourself.
        This constructor gets called when the WM engine detects that a
        window has been created (or a previously known window has been
        restored into view).
        """
        #print "window.__init__: entered, hWin=0x%lx" % hWin
        self.hWin = hWin        # save the ptr for later use
        #print "window.__init__:2: entered, hWin=0x%lx" % hWin
        self.name = self.getname()           # get titlebar text
        #print "window.__init__:3"
        #print "window.__init__:name=%s" % self.name
    
    
    
    
    
    #@-body
    #@-node:1::window.__init__
    #@+node:2::window.close
    #@+body
    def close(self):
        """
        Closes the window
        """    
        #print "window.close: trying to close window"
        flwm_.closeWindow(self.hWin)
        #print "window.close: returned"
    
    
    #@-body
    #@-node:2::window.close
    #@+node:3::window.getname
    #@+body
    def getname(self):
        """
        Call this to return the current setting of the window's title bar
        """
        #print "window.getname 1"
        name = flwm_.getLabel(self.hWin)
        #print "window.getname 2"
        return name
    
    
    #@-body
    #@-node:3::window.getname
    #@+node:4::window.position
    #@+body
    def position(self, x=None, y=None):
        """
        Gets or sets the window position
        
        Call with no arguments to fetch the position as a tuple
        Call with x,y to set the window position
        """
        #print "window.position: entered"
        #return 0, 0
        if x == None:
            #print "window.size: about to call flwm_.getSize"
            x, y = flwm_.getPos(self.hWin)
            #print "window.size: returned from flwm_.getSize"
            return x, y
        else:
            #print "window.position: trying to reposition"
            if y == None:
                raise Exception("Must call with no args, or x,y")
            #print "window.position: about to call flwm_.setPos"
            flwm_.setPos(self.hWin, x, y)
            #print "window.position: returned from flwm_.setPos"
    
    
    
    
    #@-body
    #@-node:4::window.position
    #@+node:5::window.size
    #@+body
    def size(self, w=None, h=None):
        """
        Gets or sets the window size
        
        Call with no arguments to fetch the position as a tuple
        Call with x,y to set the window position
        """
        #print "window.size: bailing"
        #return 0, 0
        if w == None:
            #print "window.size: about to call flwm_.getSize"
            x, y = flwm_.getSize(self.hWin)
            #print "window.size: returned from flwm_.getSize"
            return x, y
        else:
            #print "window.size: setting size"
            if h == None:
                raise Exception("Must call with no args, or w,h")
            #print "window.size: about to call flwm_.setSize"
            flwm_.setSize(self.hWin, w, h)
            #print "window.size: returned from flwm_.setSize"
    
    
    #@-body
    #@-node:5::window.size
    #@+node:6::window.width
    #@+body
    def width(self):
        return self.size()[0]
    
    #@-body
    #@-node:6::window.width
    #@+node:7::window.height
    #@+body
    def height(self):
        return self.size()[1]
    
    #@-body
    #@-node:7::window.height
    #@+node:8::window.hide
    #@+body

    def hide(self):
        """
        Hides the window off the screen.
        
        Note - this will trigger an 'on_destroy' callback, so watch for this
        in your callback
        """
        # get window as a pointer
        flwm_.hide(self.hWin)
    
    
    #@-body
    #@-node:8::window.hide
    #@+node:9::window.unhide
    #@+body
    def unhide(self):
        """
        UnHides the window back onto the screen.
        
        Note - this will trigger an 'on_create' callback, so watch for this
        in your callback
        """
        flwm_.unhide(self.hWin)
    
    #@-body
    #@-node:9::window.unhide
    #@+node:10::Callbacks
    #@+node:1::window.on_resize
    #@+body
    def on_resize(self, x, y, w, h):
        print "WM.on_resize: please override this method"
    
    #@-body
    #@-node:1::window.on_resize
    #@-node:10::Callbacks
    #@-others


#@-body
#@-node:2::class window
#@+node:3::class WM
#@+body
class WM:
    """
    WM class.

    Runs the Pythonised 'FLWM' window manager for X.
    
    Subclass this and override methods according to your tastes

    Python components written by David McNab <david@freenet.org.au>
    """


    #@+others
    #@+node:1::WM attribs
    #@+body
    _optionKeywords = {'display': '-d',
                       'visual': '-v',
                       'geometry': '-g',
                       'maximum':'-m',
                       'exit': '-x',
                       'bg': '-bg',
                       'fg': '-fg',
                       'bg2': '-bg2',
                       'cursor': '-c',
                       'cfg': '-cfg',
                       'cbg': '-cbg'
                       }
    
    _options = {}
    
    _keyBindings = {}
    
    windows = {}
    
    
    #@-body
    #@-node:1::WM attribs
    #@+node:2::WM.__init__
    #@+body
    def __init__(self, **kwds):
        """
        Constructor.
    
        Creates an FLWM object.
    
        Available keywords are:
    
          display (host:#.#) X display & screen to use
          visual  (#) visual to use
          geometry (WxH+X+Y) limits windows to this area
          maximum (WxH) size of maximized windows
          exit - menu says Exit instead of logout
          bg - (color) Frame color
          fg - (color) Label color
          bg2 - (color) Text field color
          cursor - (#) Cursor number for root
          cfg - (color) Cursor color
          cbg - (color) Cursor outline color
          winclass - python class ref for windows - should be a subclass of flwm.window
        """
    
        optionKeywords = {'display': '-d',
                           'visual': '-v',
                           'geometry': '-g',
                           'maximum':'-m',
                           'exit': '-x',
                           'bg': '-bg',
                           'fg': '-fg',
                           'bg2': '-bg2',
                           'cursor': '-c',
                           'cfg': '-cfg',
                           'cbg': '-cbg'
                           }
    
        self.options = []
        for arg in optionKeywords:
            if kwds.has_key(arg):
                self.options.append(optionKeywords[arg])
                if arg != 'exit':
                    self.options.append(kwds[arg])
    
        badargs = []
        for arg in kwds:
            if not optionKeywords.has_key(arg):
                badargs.append("'"+arg+"'")
        if len(badargs) > 0:
            raise Exception("Illegal arguments: %s" % ", ".join(badargs))
    
        self.windows = {}
        self._keyBindings = {}
    
    
    #@-body
    #@-node:2::WM.__init__
    #@+node:3::WM.run
    #@+body
    def run(self):
        """
        Launch the window manager, and receive events via
        callbacks.
            
        If overriding this in a subclass, don't forget to call this
        parent method.
            
        Note that you can pass in keywords, just like with the constructor.
        See __init__ for details
        """
    
        flwm_.run(self.options, self)
    
    #@-body
    #@-node:3::WM.run
    #@+node:4::WM._addWin
    #@+body
    def _addWin(self, hWin):
        """
        Makes a window known to this wm object
        """
        #print "_add 2: hWin=0x%lx" % hWin
        win = window(hWin)
        #print "_add 3"
        self.windows[hWin] = win
        return win
    
    
    #@-body
    #@-node:4::WM._addWin
    #@+node:5::WM._delWin
    #@+body
    def _delWin(self, hWin):
        if wmobj.windows.has_key(win):
            #print "wm.on_destroy: destroyed '%s'" % self.windows[win].name
            del self.windows[win]
        else:
            print "WM._delWin destroying unknown window??!?"
    
    #@-body
    #@-node:5::WM._delWin
    #@+node:6::WM.window
    #@+body
    def window(self, hWin):
        return self.windows[hWin]
    
    #@-body
    #@-node:6::WM.window
    #@+node:7::WM.launch
    #@+body
    def launch(self, cmd):
        """
        Launch a new window in a separate process
        
        Arguments:
         - cmd - a command to run in the shell to create the window
        
        If you want that command to run inside a terminal window, then
        modify your command to invoke your favourite X terminal prog
        """
        print "WM.launch - wnating to run '%s'" % cmd
        #print "WM.launch: bailing"
        #return
        return os.system(cmd + " &")
    
    #@-body
    #@-node:7::WM.launch
    #@+node:8::WM.getMousePos
    #@+body
    def getMousePos(self):
        """
        Returns the current x, y coordinates of the mouse as a tuple
        """
        return flwm_.getMousePos()
    
    
    #@-body
    #@-node:8::WM.getMousePos
    #@+node:9::WM.bindKey
    #@+body
    def bindKey(self, key, func):
        """
        Allows you to bind a Ctrl-Alt key combo to a function
    
        Argument:
         - key - key to bind
         - func - function to call when key gets pressed
        """
        keynum = flwm_._keymap[key]
        self._keyBindings[keynum] = func
        #bind_key(keynum)
        print "WM.bindKey: *** NOT IMPLEMENTED"
    
    
    #@-body
    #@-node:9::WM.bindKey
    #@+node:10::Callbacks
    #@+node:1::working
    #@+node:1::WM.on_resize
    #@+body
    def on_resize(self, win, x, y, w, h):
        """
        Called when the size of the window changes.
            
        If overriding, you need to accept the arguments:
          - hWin, x, y, w, h
        where hWin is the python handle for the window
        and x, y, w, h are the new position and size for the window
        """
        label = win.name
        #print "WM.on_destroy: entered"
        print "flwm.WM.on_resize: x=%d y=%d w=%d h=%d name=%s" % (x, y, w, h, label)
        #print "label = '%s'" % label
    
    
    #@-body
    #@-node:1::WM.on_resize
    #@+node:2::WM.on_create
    #@+body
    def on_create(self, win):
        """
        Called when a new window is created (or an existing one un-hidden).
        
        Single argument is 'win', a ref to a window object
        """
        print "WM.on_create: created window '%s'" % win.name
    
    #@-body
    #@-node:2::WM.on_create
    #@+node:3::WM.on_destroy
    #@+body
    def on_destroy(self, win):
        """
        Called when an existing window is being destroyed (or hidden)
        
        If overriding in a subclass, call this parent method just before you exit
        """
        print "WM.on_destroy: destroying '%s'" % win.name
    
    #@-body
    #@-node:3::WM.on_destroy
    #@-node:1::working
    #@+node:2::WM.on_enter
    #@+body
    def on_enter(self, win):
        """
        Called when the mouse pointer enters a window
        
        If overriding, just accept a single argument, hWin
        """
        print "wm.on_enterWindow: entered window %s" % win.name
    
    
    #@-body
    #@-node:2::WM.on_enter
    #@+node:3::WM.on_leave
    #@+body
    #def on_leave(self, win):
    #    """
    #    Called when the mouse pointer leaves a window
    #    
    #    If overriding, just accept a single argument, hWin
    #    """
    #    print "wm.on_leaveWindow: left window %s" % self.windows[win].name
    
    
    #@-body
    #@-node:3::WM.on_leave
    #@+node:4::WM.on_buttonPress
    #@+body
    def on_buttonPress(self, button):
        """
        Called when a button click is received.
        Buttons 1, 2, 3, 4, 5 are left-click, right-click, middle-click,
        wheelscroll up and wheelscroll down, respectively.
        """
        print "wm.on_buttonPress callback: button %d" % button
    
    
    #@-body
    #@-node:4::WM.on_buttonPress
    #@+node:5::WM.on_keyEvent
    #@+body
    def on_keyEvent(self, key):
        """
        Called when a Ctrl-Alt key sequence is received.
        """
        print "wm.on_keyEvent callback: key=%d" % key
    
    
    #@-body
    #@-node:5::WM.on_keyEvent
    #@-node:10::Callbacks
    #@-others


#@-body
#@-node:3::class WM
#@-others



#@-body
#@-node:0::@file src-python/flwm.py
#@-leo
