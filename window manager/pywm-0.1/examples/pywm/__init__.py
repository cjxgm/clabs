#@+leo-ver=4
#@+node:@file src-python/__init__.py
#@@language python

"""
Main module for the PYWM window manager.

PYWM is a window manager based on the FLWM (http://flwm.sf.net)
window manager, but wrapped into an easy to use python class.

Aim of PYWM is to create a python-drivable window manager that's
actually useful and approachable. (I had a look at PLWM, but
found it too arcane and unapproachable. In contrast, PYWM is
actually easy to use and customise, and respects those who like
mouse interaction (while not discriminating against rodent-haters).

Written 2003 and Copyright (C) by David McNab <david@freenet.org.nz>

Underlying FLWM engine written and copyright by Bill Spizak
(http://www.cinenet.net/~spitzak/)
"""

__all__ = ['examples']

#@+others
#@+node:Python imports
import sys
import os
import traceback

import flwm_

#@-node:Python imports
#@+node:__all__
__all__ = [
           'WM',
           'window',
           'Fl_Window',
           'Fl_Button',
           'Fl_Select_Browser',
           'Fl_Hold_Browser',
           'applet',
           ]
#@-node:__all__
#@+node:class window
class window:
    """
    This class wraps an X window in a form where your python
    window manager code can manipulate it.

    You can subclass this, and pass your subclass into your window manager
    constructor. This will cause all created windows to be built with
    your custom window class.    
    """
    #@    @+others
    #@+node:Attributes
    
    moveIncrement = 40 # default for the various move actions
    sizeIncrement = 40 # default for the various size actions
    #@-node:Attributes
    #@+node:window.close
    def close(self):
        """
        Closes the window
        """    
        #print "window.close: trying to close window"
        flwm_.closeWindow(self.hWin)
        #print "window.close: returned"
    
    #@-node:window.close
    #@+node:window.raise_
    def raise_(self):
        """
        Raises this window
        """
        #print "window.raise: trying to raise window"
        flwm_.raiseWindow(self.hWin)
        #print "window.raise: returned"
    
    #@-node:window.raise_
    #@+node:window.lower
    def lower(self):
        """
        Lowers this window
        """
        #print "window.lower: trying to lower window"
        flwm_.lowerWindow(self.hWin)
        #print "window.lower: returned"
    
    #@-node:window.lower
    #@+node:window.iconise
    def iconise(self):
        """
        Iconises this window
        """
        #print "window.iconise: trying to iconise window"
        flwm_.iconiseWindow(self.hWin)
        #print "window.iconise: returned"
    #@-node:window.iconise
    #@+node:window.getname
    def getname(self):
        """
        Call this to return the current setting of the window's title bar
        """
        #print "window.getname 1"
        name = flwm_.getLabel(self.hWin)
        #print "window.getname 2"
        return name
    
    
    
    #@-node:window.getname
    #@+node:window.position
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
    
    
    
    #@-node:window.position
    #@+node:window.size
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
    
    #@-node:window.size
    #@+node:window.setborder
    def setborder(self, mode='normal'):
        """
        Sets the window border mode.
        
        Argument - mode:
         - if 'normal', sets normal border with titlebar
         - if 'thin', sets thin border - no titlebar
         - if 'none', sets no border or titlebar
        """
        #print "window.setborder: mode=%s" % mode
        if mode == 'none':
            modenum = 2
        elif mode == 'thin':
            modenum = 1
        else:
            modenum = 0
        flwm_.setBorder(self.hWin, modenum)
    
    #@-node:window.setborder
    #@+node:window.hide
    
    def hide(self):
        """
        Hides the window off the screen.
        
        Note - this will trigger an 'on_destroy' callback, so watch for this
        in your callback
        """
        # get window as a pointer
        flwm_.hide(self.hWin)
    
    #@-node:window.hide
    #@+node:window.unhide
    def unhide(self):
        """
        UnHides the window back onto the screen.
        
        Note - this will trigger an 'on_create' callback, so watch for this
        in your callback
        """
        flwm_.unhide(self.hWin)
    #@-node:window.unhide
    #@+node:window.activate
    def activate(self):
        """
        Brings this window to the top, and makes it the active window
        """
        flwm_.activateWindow(self.hWin)
    
    
    #@-node:window.activate
    #@+node:window.deactivate
    def deactivate(self):
        """
        Brings this window to the top, and makes it the active window
        """
        flwm_.deactivateWindow(self.hWin)
    
    
    #@-node:window.deactivate
    #@+node:window.lock
    def lock(self):
        """
        Calling lock on a window prevents that window from being
        affected by zooming and panning functions
        """
        self.isLocked = True
    #@nonl
    #@-node:window.lock
    #@+node:window.unlock
    def unlock(self):
        """
        Calling unlock on a window makes that window susceptible
        to moving and resizing via zooming and panning functions.
        
        This is the default state when the window gets created.
        """
        self.isLocked = False
    #@-node:window.unlock
    #@+node:window.__init__
    def __init__(self, hWin, wm):
        """
        You should not instantiate windows yourself.
        This constructor gets called when the WM engine detects that a
        window has been created (or a previously known window has been
        restored into view).
        """
        #print "window.__init__: entered, hWin=0x%lx" % hWin
        self.hWin = hWin        # save the ptr for later use
        self.wm = wm            # save back-link to WM object
        #print "window.__init__:2: entered, hWin=0x%lx" % hWin
        self.name = self.getname()           # get titlebar text
    
        #print "getting screen size"
        self.screenWidth, self.screenHeight = self.getScreenSize()
        #print "got screen size"
        
        self.isMax = False
        self.isMaxWidth = False
        self.isMaxHeight = False
    
        self.isLocked = False # set to true to exclude from zooming/panning
        self.dead = False
        self.xGranularity = 1
        self.yGranularity = 1
    
        #print "window.__init__:3"
        #print "window.__init__:name=%s" % self.name
    
    
    #@-node:window.__init__
    #@+node:window.__repr__
    def __repr__(self):
        return "<window hWin:0x%lx '%s'>" % (self.hWin, self.name)
    
    #@-node:window.__repr__
    #@+node:window.getScreenSize
    def getScreenSize(self):
        """
        Returns x, y size of the screen as a tuple
        """
        return flwm_.getScreenSize()
    #@-node:window.getScreenSize
    #@+node:window.width
    def width(self):
        return self.size()[0]
    #@-node:window.width
    #@+node:window.height
    def height(self):
        return self.size()[1]
    #@-node:window.height
    #@+node:window.moveRel
    def moveRel(self, dx=0, dy=0):
        """
        Move the window relative to its present position
        
        Args: dx, dy - if either of these are None, don't move on that axis.
        """
        x, y = self.position()
        self.position(x+dx, y+dy)
    #@-node:window.moveRel
    #@+node:window.moveUp
    def moveUp(self, dy=None):
        """
        Moves window up.
        Arg - dy - is amount to move by, defaults to self.moveIncrement
        """
        if not dy:
            dy = self.moveIncrement
        self.moveRel(0, -dy)
    #@-node:window.moveUp
    #@+node:window.moveDown
    def moveDown(self, dy=None):
        """
        Moves window down
        Arg - dy - is amount to move by, defaults to self.moveIncrement
        """
        if not dy:
            dy = self.moveIncrement
        self.moveRel(0, dy)
    #@-node:window.moveDown
    #@+node:window.moveLeft
    def moveLeft(self, dx=None):
        """
        Moves window left.
        Arg - dx - is amount to move by, defaults to self.moveIncrement
        """
        if not dx:
            dx = self.moveIncrement
        self.moveRel(-dx, 0)
    #@-node:window.moveLeft
    #@+node:window.moveRight
    def moveRight(self, dx=None):
        """
        Moves window right.
        Arg - dx - is amount to move by, defaults to self.moveIncrement
        """
        if not dx:
            dx = self.moveIncrement
        self.moveRel(dx, 0)
    #@-node:window.moveRight
    #@+node:window.goNorth
    def goNorth(self):
        """
        Moves the window to the top (north) position
        """
        #print "pywm.window.goNorth: entered"
        w = self.width()
        scrW, scrH = self.getScreenSize()
        self.position((scrW-w)/2, 0)
    
    
    
    #@-node:window.goNorth
    #@+node:window.goNorthEast
    def goNorthEast(self):
        """
        Moves the window to the top right (northeast) position
        """
        #print "pywm.window.goNorthEast: entered"
        w = self.width()
        scrW, scrH = self.getScreenSize()
        self.position(scrW-w, 0)
    
    
    #@-node:window.goNorthEast
    #@+node:window.goEast
    def goEast(self):
        """
        Moves the window to the right (east) position
        """
        #print "pywm.window.goEast: entered"
        w, h = self.width(), self.height()
        scrW, scrH = self.getScreenSize()
        self.position(scrW-w, (scrH-h)/2)
    
    #@-node:window.goEast
    #@+node:window.goSouthEast
    def goSouthEast(self):
        """
        Moves the window to the bottom right (southeast) position
        """
        #print "pywm.window.goSouthEast: entered"
        w, h = self.width(), self.height()
        scrW, scrH = self.getScreenSize()
        self.position(scrW-w, scrH-h)
    
    #@-node:window.goSouthEast
    #@+node:window.goSouth
    def goSouth(self):
        """
        Moves the window to the top (north) position
        """
        #print "pywm.window.goSouth: entered"
        w, h = self.width(), self.height()
        scrW, scrH = self.getScreenSize()
        self.position((scrW-w)/2, scrH-h)
    
    #@-node:window.goSouth
    #@+node:window.goSouthWest
    def goSouthWest(self):
        """
        Moves the window to the top (north) position
        """
        #print "pywm.window.goSouthWest: entered"
        h = self.height()
        scrW, scrH = self.getScreenSize()
        self.position(0, scrH-h)
    
    #@-node:window.goSouthWest
    #@+node:window.goWest
    def goWest(self):
        """
        Moves the window to the top (north) position
        """
        #print "pywm.window.goWest: entered"
        h = self.height()
        scrW, scrH = self.getScreenSize()
        self.position(0, (scrH-h)/2)
    
                    
    
    #@-node:window.goWest
    #@+node:window.goNorthWest
    def goNorthWest(self):
        """
        Moves the window to the top (north) position
        """
        #print "pywm.window.goNorthWest: entered"
        self.position(0, 0)
    
    #@-node:window.goNorthWest
    #@+node:window.goCentre
    def goCentre(self):
        """
        Centre the window on the screen
        """
        #print "pywm.window.goCentre: entered"
        w, h = self.width(), self.height()
        scrW, scrH = self.getScreenSize()
        self.position((scrW-w)/2, (scrH-h)/2)
    
    
    #@-node:window.goCentre
    #@+node:window.go
    def go(self, direction):
        """
         Arguments:
         - direction - the direction to move. One of:
            - n or north - move to top centre of screen
            - ne or northeast - move to top right of screen
            - e or east - move to centre right of screen
            - se or southeast - move to bottom right of screen
            - s or south - move to bottom centre of screen
            - sw or southwest - move to bottom left of screen
            - w or west - move to centre left of screen
            - nw or northwest - move to top oeft of screen
            - c or centre/center - move to centre of screen
            - up - move up a bit
            - down - move down a bit
            - left - move left a bit
            - right - move right a bit
        """
        direction = direction.lower()
        if direction in ['n', 'north']:
            self.goNorth()
        elif direction in ['ne', 'northeast']:
            self.goNorthEast()
        elif direction in ['e', 'east']:
            self.goEast()
        elif direction in ['se', 'southeast']:
            self.goSouthEast()
        elif direction in ['s', 'south']:
            self.goSouth()
        elif direction in ['sw', 'southwest']:
            self.goSouthWest()
        elif direction in ['w', 'west']:
            self.goWest()
        elif direction in ['nw', 'northwest']:
            self.goNorthWest()
        elif direction in ['c', 'center', 'centre']:
            self.goCentre()
        elif direction in ['u', 'up']:
            self.moveUp()
        elif direction in ['d', 'down']:
            self.moveDown()
        elif direction in ['l', 'left']:
            self.moveLeft()
        elif direction in ['r', 'right']:
            self.moveRight()
        else:
            print "WM.moveCurrent: unknown direction '%s'" % direction
    
    #@-node:window.go
    #@+node:toggleMax
    def toggleMax(self):
        """
        Toggles between existing and maximum size
        """
        #print "pywm.window.toggleMax"
        if self.isMax:
            self.size(self.oldWidth, self.oldHeight)
            self.position(self.oldX, self.oldY)
            self.isMax = False
        else:
            self.oldWidth, self.oldHeight = self.size()
            self.oldX, self.oldY = self.position()
            self.position(0,0)
            self.size(self.screenWidth, self.screenHeight)
            self.isMax = True
    
    
    
    #@-node:toggleMax
    #@+node:toggleMaxWidth
    def toggleMaxWidth(self):
        """
        Toggles the height between current and maximum
        """
        w, h = self.size()
        if self.isMaxWidth:
            self.size(self.oldWidthW, h)
            self.position(self.oldX, self.oldY)
            self.isMaxWidth = False
        else:
            self.oldWidthW, self.oldHeightW = w, h
            self.oldX, self.oldY = self.position()
            self.position(0, self.oldY)
            self.size(self.screenWidth, h)
            self.isMaxWidth = True
    #@-node:toggleMaxWidth
    #@+node:toggleMaxHeight
    def toggleMaxHeight(self):
        """
        Toggles the height between current and maximum
        """
        w, h = self.size()
        if self.isMaxHeight:
            self.size(w, self.oldHeightH)
            self.position(self.oldX, self.oldY)
            self.isMaxHeight = False
            
        else:
            self.oldWidthH, self.oldHeightH = w, h
            self.oldX, self.oldY = self.position()
            self.position(self.oldX, 0)
            self.size(w, self.screenHeight)
            self.isMaxHeight = True
    
    
    #@-node:toggleMaxHeight
    #@+node:window.resizeRel
    def resizeRel(self, dw=0, dh=0):
        """
        Resizes window relative to its present size.
        
        Arguments:
         - dw - amount to grow by horizontally - default 0
         - dh - amount to grow by vertically - default 0
        """
        w, h = self.size()
        self.size(w+dw, h+dh)
    #@-node:window.resizeRel
    #@+node:window.resize
    def resize(self, how):
        """
        Resizes the window according to how.
        
        Argument:
         - how - the resize mode:
             - 'taller' - grow vertically
             - 'shorter' - shrink vertically
             - 'wider - grow horizontally
             - 'narrower' - shrink horizontally
             - 'larger' - grow vertically and horizontally
             - 'smaller' - shrink vertically and horizontally
        """
    
        how = how.lower()
        
        if how in ['taller']:
            self.growBottom()
        elif how in ['shorter']:
            self.shrinkBottom()
        elif how in ['wider']:
            self.growRight()
        elif how in ['narrower']:
            self.shrinkRight()
        elif how in 'larger':
            self.grow()
        elif how in 'smaller':
            self.shrink()
    
    #@-node:window.resize
    #@+node:window.grow
    def grow(self, dw=None, dh=None):
        """
        Grow from the center by dw, dh
        
        Arguments:
         - dw - amount to grow by horizontally, default self.sizeIncrement
         - dh - amount to grow by vertically, default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        if dh == None:
            dh = self.sizeIncrement
        x, y = self.position()
        self.resizeRel(dw, dh)
        self.moveRel(x-dw/2, y-dh/2)
    #@-node:window.grow
    #@+node:window.growLeft
    def growLeft(self, dw=None):
        """
        Grow to the left by 'dw' pizels. Default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        self.resizeRel(dw, 0)
        self.moveRel(dw, 0)
    #@-node:window.growLeft
    #@+node:window.growRight
    def growRight(self, dw=None):
        """
        Grow to the right by 'dw' pizels. Default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        self.resizeRel(dw, 0)
    #@-node:window.growRight
    #@+node:window.growTop
    def growTop(self, dh=None):
        """
        Grow from the top by 'dh' pizels. Default self.sizeIncrement
        """
        if dh == None:
            dh = self.sizeIncrement
        self.resizeRel(0, dh)
        self.moveRel(0, dh)
    #@-node:window.growTop
    #@+node:window.growBottom
    def growBottom(self, dh=None):
        """
        Grow from the bottom by 'dh' pizels. Default self.sizeIncrement
        """
        if dh == None:
            dh = self.sizeIncrement
        self.resizeRel(0, dh)
    #@-node:window.growBottom
    #@+node:window.shrink
    def shrink(self, dw=None, dh=None):
        """
        Shrink from the center by dw, dh
        
        Arguments:
         - dw - amount to shrink by horizontally, default self.sizeIncrement
         - dh - amount to shrink by vertically, default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        if dh == None:
            dh = self.sizeIncrement
        x, y = self.position()
        self.resizeRel(-dw, -dh)
        self.moveRel(x+dw/2, y+dh/2)
    
    #@-node:window.shrink
    #@+node:window.shrinkLeft
    def shrinkLeft(self, dw=None):
        """
        Shrink from the left by 'dw' pizels. Default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        self.resizeRel(-dw, 0)
        self.moveRel(-dw, 0)
    #@-node:window.shrinkLeft
    #@+node:window.shrinkRight
    def shrinkRight(self, dw=None):
        """
        Shrink from the right by 'dw' pizels. Default self.sizeIncrement
        """
        if dw == None:
            dw = self.sizeIncrement
        self.resizeRel(-dw, 0)
    #@-node:window.shrinkRight
    #@+node:window.shrinkTop
    def shrinkTop(self, dh=None):
        """
        Shrink from the top by 'dh' pizels. Default self.sizeIncrement
        """
        if dh == None:
            dh = self.sizeIncrement
        self.resizeRel(0, -dh)
        self.moveRel(0, -dh)
    #@-node:window.shrinkTop
    #@+node:window.shrinkBottom
    def shrinkBottom(self, dh=None):
        """
        Shrink from the bottom by 'dh' pizels. Default self.sizeIncrement
        """
        if dh == None:
            dh = self.sizeIncrement
        self.resizeRel(0, -dh)
    #@-node:window.shrinkBottom
    #@+node:window.on_resize
    def on_resize(self, x, y, w, h):
        print "WM.on_resize: please override this method"
    #@-node:window.on_resize
    #@-others
#@-node:class window
#@+node:class WM
class WM:
    """
    pywm.WM is the main PYWM class that you should subclass when building
    your own custom window manager.

    Encapsulates the Pythonised 'FLWM' window manager for X, into
    a python class.
    
    Subclass this and override methods according to your tastes

    Python components written by David McNab <david@freenet.org.au>
    """

    #@    @+others
    #@+node:WM attribs
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
    _keyBindingsArg = {}
    
    windows = {}
    
    #@-node:WM attribs
    #@+node:WM.__init__
    def __init__(self, **kwds):
        """
        Constructor.
    
        Creates an FLWM object.
    
        Available keywords are:
    
          - display (host:#.#) X display & screen to use
          - visual  (#) visual to use
          - geometry (WxH+X+Y) limits windows to this area
          - maximum (WxH) size of maximized windows
          - exit - menu says Exit instead of logout
          - bg - (color) Frame color
          - abg - (color) Frame color for active windows
          - fg - (color) Label color
          - bg2 - (color) Text field color
          - cursor - (#) Cursor number for root
          - cfg - (color) Cursor color
          - cbg - (color) Cursor outline color
          - windowclass - class to use when windows get created.
            should be a subclass of pywm.window
        """
        optionKeywords = {'display': '-d',
                          'visual': '-v',
                          'geometry': '-g',
                          'maximum':'-m',
                          'exit': '-x',
                          'bg': '-bg',
                          'abg': '-abg',
                          'fg': '-fg',
                          'bg2': '-bg2',
                          'cursor': '-c',
                          'cfg': '-cfg',
                          'cbg': '-cbg'
                          }
    
        # Create an empty chain of handlers for various events
        self.hdlrsStartup = []
        self.hdlrsCreate = []
        self.hdlrsDestroy = []
        self.hdlrsActivate = []
        self.hdlrsDeactivate = []
        self.hdlrsKeyEvent = []
        self.hdlrsButtonPress = []
        self.hdlrsResize = []
        self.handlernames = {'startup':self.hdlrsStartup,
                             'create':self.hdlrsCreate,
                             'destroy':self.hdlrsDestroy,
                             'activate':self.hdlrsActivate,
                             'deactivate':self.hdlrsDeactivate,
                             'keyevent':self.hdlrsKeyEvent,
                             'buttonpress':self.hdlrsButtonPress,
                             'resize':self.hdlrsResize,
                             }
        
        # allow caller to substitute a different window class
        if kwds.has_key('windowclass'):
            #print "WM.__init__: kwds has a windowclass"
            windowclass = kwds['windowclass']
            #print "WM.__init__: 1"
            if not issubclass(windowclass, window):
                #print "WM.__init__: 2"
                #print "bad window class"
                raise Exception(
                   "windowclass arg must be a subclass of pywm.window")
            #print "WM.__init__: 3"
            self.windowclass = windowclass
            #print "WM.__init__: 4"
            del kwds['windowclass']
            #print "WM.__init__: 5"
        else:
            #print "WM.__init__: using default window class"
            self.windowclass = window
    
        #self.windowclass = window
    
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
        self.winindexes = {}
        self.windowlist = []
        self._keyBindings = {}
    
        self.screenWidth, self.screenHeight = self.getScreenSize()
    #@-node:WM.__init__
    #@+node:WM.addHandlers
    def addHandlers(self, **kwds):
        """
        Add a set of handlers for various events.
    
        These are given as 'eventname=func' keywords.
        
        Available eventname keywords are:
         - startup - when WM is started:
             - handler receives no arguments
         - create - when a window is created (or unhidden):
             - handler is called with a single window arg
         - destroy - when a window is destroyed (or hidden)
             - handler is called with a single window arg
         - activate - when a window gets activated
             - handler is called with a single window arg
         - deactivate - when a window is deactivated
             - handler is called with a single window arg
         - keyevent - when a keystroke is received:
             - handler is called with keystroke
         - buttonpress - when a mouse button gets clicked:
             - handler is called with button number
         - resize - when a window gets resized:
             - handler is called with window as arg
        """
        for k in kwds.keys():
            if k in self.handlernames:
                if not callable(kwds[k]):
                    print "ERROR - %s handler is not callable" % k
                    continue
                self.handlernames[k].append(kwds[k])
    #@-node:WM.addHandlers
    #@+node:WM.on_startup
    def on_startup(self):
        """
        Callback which is invoked when WM starts up.
        
        Note that this callback fires *after* all the
        existing windows have been re-framed (and the on_create
        callbacks fired), and *before* the main WM event
        loop starts up.
        
        Override this if you want to do your own processing
        during startup.
        """
        print "WM.on_startup: entered"
    #@-node:WM.on_startup
    #@+node:WM.on_create
    def on_create(self, win):
        """
        Called when a new window is created (or an existing one un-hidden)
        """
        print "WM.on_create: created window '%s'" % win.name
    #@-node:WM.on_create
    #@+node:WM.on_destroy
    def on_destroy(self, win):
        """
        Called when an existing window is being destroyed (or hidden)
        
        If overriding in a subclass, call this parent method just before you exit
        """
        print "WM.on_destroy: destroying '%s'" % win.name
    #@-node:WM.on_destroy
    #@+node:WM.on_activate
    def on_activate(self, win):
        """
        Called when an existing window is activated
        """
        print "WM.on_activate: activated '%s'" % win.name
    #@-node:WM.on_activate
    #@+node:WM.on_deactivate
    def on_deactivate(self, win):
        """
        Called when an existing window is deactivated
        """
        print "WM.on_deactivate: deactivated '%s'" % win.name
    #@-node:WM.on_deactivate
    #@+node:WM.on_keyEvent
    def on_keyEvent(self, key):
        """
        Called when a Ctrl-Alt key sequence is received.
        """
        print "wm.on_keyEvent callback: key=%x" % key
        #print self._keyBindings
        if self._keyBindings.has_key(key):
            print "wm.on_keyEvent: known key, launching func"
            func = self._keyBindings[key]
            arg = self._keyBindingsArg[key]
            func(arg)
            #print "wm.on_keyEvent: back from func"
            return 1
        else:
            #print "wm.on_keyEvent: unknown key"
            #print self._keyBindings.keys()
            return 0
    
    
    
    #@-node:WM.on_keyEvent
    #@+node:WM.on_buttonPress
    def on_buttonPress(self, button):
        """
        Called when a button click is received.
        Buttons 1, 2, 3, 4, 5 are left-click, right-click, middle-click,
        wheelscroll up and wheelscroll down, respectively.
    
        If you override this, you should accept a single argument,
        'button_num', and return 1 if you've handled it locally,
        or 0 if you want the WM to handle it instead.
    
        NOT YET WORKING
        """
        print "wm.on_buttonPress callback: button %d" % button
        if button == 1:
            print self.getMousePos()
        elif button == 2:
            flwm_.test()
        elif button == 3:
            return 0
        elif button == 4:
            self.goNextWindow()
        elif button == 5:
            self.goPrevWindow()
        else:
            # disable menu except for right-click
            return 1
        return 1
    #@-node:WM.on_buttonPress
    #@+node:WM.on_resize
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
    
    #@-node:WM.on_resize
    #@+node:WM.on_enter
    def on_enter(self, win):
        """
        Called when the mouse pointer enters a window
        
        If overriding, just accept a single argument, hWin
        
        NOT YET WORKING
        """
        print "wm.on_enterWindow: entered window %s" % win.name
    
    #@-node:WM.on_enter
    #@+node:WM.on_leave
    #def on_leave(self, win):
    #    """
    #    Called when the mouse pointer leaves a window
    #    
    #    If overriding, just accept a single argument, hWin
    #    """
    #    print "wm.on_leaveWindow: left window %s" % self.windows[win].name
    
    #@-node:WM.on_leave
    #@+node:WM.run
    def run(self):
        """
        Launch the window manager, and receive events via
        callbacks.
            
        If overriding this in a subclass, don't forget to call this
        parent method.
        """
    
        flwm_.run(self.options, self)
    #@-node:WM.run
    #@+node:WM._addWin
    def _addWin(self, hWin):
        """
        Makes a window known to this wm object
        
        Used by the lower level window create event handler.
        Don't call this yourself.
        """
        #print "WM._addWin 1: hWin=0x%lx" % hWin
        #print dir(self)
        #print type(self.windowclass)
        #print "WM._addWin 2"
        #print self.windowclass.__class__
    
        win = self.windowclass(hWin, self)
        #win = window(hWin)
    
        #print "WM._addWin 3"
        self.windows[hWin] = win
        self.windowlist.append(win)
        self.currentWindow = win
        return win
    
    
    #@-node:WM._addWin
    #@+node:WM._delWin
    def _delWin(self, hWin):
        """
        Deletes a window's mapping from our dict.
        
        Called by underlying handler of window delete events.
        
        Don't call this yourself
        """
        #print "WM._delWin: entered"
        if self.windows.has_key(hWin):
            win = self.windows[hWin]
            print "wm._delWin: destroyed '%s'" % win.name
            win.dead = True
            #del self.windows[win]
            try:
                i = self.indexFromWindow(win)
                if i >= 0:
                    del self.windowlist[i]
            except:
                print "oops!"
        else:
            print "WM._delWin destroying unknown window??!?"
        print "WM._delWin: finished"
    #@-node:WM._delWin
    #@+node:WM.window
    def window(self, hWin):
        """
        Maps a window handle to a window object.
        
        You should never need to call this yourself
        """
        return self.windows[hWin]
    #@-node:WM.window
    #@+node:WM.launch
    def launch(self, cmd):
        """
        Launch a new window in a separate process
        
        Arguments:
         - cmd - a command to run in the shell to create the window.
           Syntax of this is anything acceptable to os.system().
        
        If you want that command to run inside a terminal window, then
        modify your command to invoke your favourite X terminal prog
        """
        #print "WM.launch - wnating to run '%s'" % cmd
        #print "WM.launch: bailing"
        #return
        return os.system(cmd + " &")
    #@-node:WM.launch
    #@+node:WM.getMousePos
    def getMousePos(self, arg=None):
        """
        Returns the current x, y coordinates of the mouse as a tuple
        """
        return flwm_.getMousePos()
    
    #@-node:WM.getMousePos
    #@+node:WM.getScreenSize
    def getScreenSize(self, arg=None):
        """
        Returns x, y size of the screen as a tuple
        """
        return flwm_.getScreenSize()
    #@-node:WM.getScreenSize
    #@+node:WM.bindKey
    def bindKey(self, func, arg, key, shift, control, alt, meta):
        """
        Allows you to bind a Ctrl-Alt key combo to a function
    
        Arguments:
         - func - function to call when key gets pressed
         - arg - an argument to pass to the function. Can be a number,
           string, list, tuple, dict or anything you like.
         - key - key to bind - either:
             - a single character
             - one of 'f1'..'f12'
             - one of 'kp0'..'kp9', or 'kpenter'
             - one of 'enter', 'tab', 'backspace', 'intert', 'delete', 'home',
               'end', 'pageup', 'pagedown', 'up', 'down', 'left', 'right'
         - shift - whether shift key is added - 0 or 1
         - control - whether control key is added - 0 or 1
         - alt - whether alt key is added - 0 or 1
         - meta - whether meta key is added - 0 or 1
        
        When the key combination gets pressed, the function you give
        will get called with the argument you give.
    
        If you find it hard to remember the 'shift'/'control'/'alt'/'meta' order,
        just think 'history' - the Shift key was the first to be added (in
        typewriters), then Ctrl (with ASCII), then Alt (with the IBM PC), then
        Meta (contributed by Microsoft with Windows).
        """
        #print "WM.bindKey: entered"
        keycode = flwm_.keyCode(key, shift, control, alt, meta)
        if keycode == 0:
            print "WM.bindKey: invalid key name '%s'" % key
            return
        print "WM.bindKey: numeric key code for %s(%s,%s,%s,%s) is 0x%lx" % (
                key, shift, control, alt, meta, keycode)
        self._keyBindings[keycode] = func
        self._keyBindingsArg[keycode] = arg
        #print "WM.bindKey: added binding"
        flwm_.bindKey(keycode)
        #print "WM.bindKey: returned from flwm_bind_key"
        #print self._keyBindings.keys()
    
    
    #@-node:WM.bindKey
    #@+node:WM.goNextWindow
    def goNextWindow(self, arg=None):
        """
        Switches to next window
        """
        flwm_.goNextWindow()
    #@-node:WM.goNextWindow
    #@+node:WM.goPrevWindow
    def goPrevWindow(self, arg=None):
        """
        Switches to previous window
        """
        flwm_.goPrevWindow()
    #@-node:WM.goPrevWindow
    #@+node:WM.goNextDesk
    def goNextDesk(self, arg=None):
        """
        Switches to next desk
        """
        flwm_.goNextDesk()
    #@-node:WM.goNextDesk
    #@+node:WM.goPrevDesk
    def goPrevDesk(self, arg=None):
        """
        Switches to previous desk
        """
        flwm_.goPrevDesk()
    #@-node:WM.goPrevDesk
    #@+node:WM.showMenu
    def showMenu(self, arg=None):
        """
        Pops up the main window manager menu
        """
        print "WM.showMenu: trying to launch menu"
        flwm_.showMenu()
    #@-node:WM.showMenu
    #@+node:WM.moveCurrent
    def moveCurrent(self, direction):
        """
        Moves the current window in the specified direction.
        
        Arguments:
         - direction - the direction to move. One of:
            - n or north - move to top centre of screen
            - ne or northeast - move to top right of screen
            - e or east - move to centre right of screen
            - se or southeast - move to bottom right of screen
            - s or south - move to bottom centre of screen
            - sw or southwest - move to bottom left of screen
            - w or west - move to centre left of screen
            - nw or northwest - move to top oeft of screen
            - c or centre/center - move to centre of screen
            - up - move up a bit
            - down - move down a bit
            - left - move left a bit
            - right - move right a bit
        """
    
        if self.currentWindow:
            self.currentWindow.go(direction)
    #@-node:WM.moveCurrent
    #@+node:WM.resizeCurrent
    def resizeCurrent(self, how):
        """
        Moves the current window in the specified direction.
        
        Arguments:
         - how - the resize mode:
             - 'taller' - grow vertically
             - 'shorter' - shrink vertically
             - 'wider - grow horizontally
             - 'narrower' - shrink horizontally
             - 'larger' - grow vertically and horizontally
             - 'smaller' - shrink vertically and horizontally
        """
    
        if self.currentWindow:
            self.currentWindow.resize(how)
    #@-node:WM.resizeCurrent
    #@+node:WM.closeCurrent
    def closeCurrent(self, arg=None):
        """
        Closes current window
        """
        if self.currentWindow:
            self.currentWindow.close()
    #@-node:WM.closeCurrent
    #@+node:WM.toggleMaxCurrent
    def toggleMaxCurrent(self, arg=None):
        """
        Toggles maximise on the current window
        """
        print "WM.toggleMaxCurrent"
        if self.currentWindow:
            self.currentWindow.toggleMax()
    
    #@-node:WM.toggleMaxCurrent
    #@+node:WM.toggleMaxWidthCurrent
    def toggleMaxWidthCurrent(self, arg=None):
        """
        Toggles maximise on the current window
        """
        print "WM.toggleMaxCurrent"
        if self.currentWindow:
            self.currentWindow.toggleMaxWidth()
    
    #@-node:WM.toggleMaxWidthCurrent
    #@+node:WM.toggleMaxHeightCurrent
    def toggleMaxHeightCurrent(self, arg=None):
        """
        Toggles maximise on the current window
        """
        print "WM.toggleMaxCurrent"
        if self.currentWindow:
            self.currentWindow.toggleMaxHeight()
    
    #@-node:WM.toggleMaxHeightCurrent
    #@+node:WM.cascadeWindows
    def cascadeWindows(self, spacing=30):
        """
        Arranges all the windows cascaded from top left.
        """
        offset = 0
        try:
            for win in self.windows.values():
                if win.isLocked:
                    continue
                win.position(offset, offset)
                win.raise_()
                win.activate()
                offset += spacing
            for win in self.windows.values():
                if win.isLocked:
                    win.raise_()
        except:
            print "WM.cascadeWindows: got an exception"
    #@-node:WM.cascadeWindows
    #@+node:WM.zoom
    def zoom(self, factor=1, xView=None, yView=None):
        """
        Zooms in or out by a given factor.
        
        Arguments:
         - factor = float - >1 to zoom in, <1 to zoom out
         - xView, yView - x,y origin to use for the zooming, defaults to centre of screen
        """
        #print "WM.zoom: entered"
        if xView == None:
            xView = self.screenWidth / 2
        if yView == None:
            yView = self.screenHeight / 2
        #print "WM.zoom: xView,yView = (%d, %d), factor=%s" % (xView, yView, factor)
        #print self.windows
        try:
            for win in self.windows.values():
                if win.dead or win.isLocked:
                    continue
                #print "window '%s'" % win.name
                x0, y0 = win.position()
                w, h = win.size()
                #print "WM.zoom: 2"
                txt = "(%d,%d)(%d x %d)" % (x0, y0, w, h)
                #print "WM.zoom:", txt
                x1, y1 = x0+w, y0+h
                #print "WM.zoom: 4"
                x0 = int(factor * float(x0 - xView) + xView)
                x1 = int(factor * float(x1 - xView) + xView)
                y0 = int(factor * float(y0 - yView) + yView)
                y1 = int(factor * float(y1 - yView) + yView)
                #print "x0=%d, y0=%d, x1=%d, y1=%d" % (x0,y0,x1,y1)
                #print txt, ("=> (%d, %d)(%d x %d)" % (x0, y0, x1-x0, y1-y0))
                w, h = x1 - x0, y1 - y0
                if win.xGranularity > 1:
                    gran = win.xGranularity
                    if w % gran >= gran / 2:
                        w += gran
                if win.yGranularity > 1:
                    gran = win.yGranularity
                    if h % gran >= gran / 2:
                        h += gran
                win.size(w, h)
                win.position(x0, y0)
        except:
            print "WM.pan: got an exception"
        try:
            for win in self.windows.values():
                if win.isLocked and not win.dead:
                    win.raise_()
        except:
            print "WM.zoom: got an exception"
        #print "WM.zoom: finished"
    
    
    #@-node:WM.zoom
    #@+node:WM.zoomIn
    def zoomIn(self, factor=1.1, fromMouse=False):
        """
        Zooms in to the current view.
        
        Arguments:
         - factor - amount to zoom by, default 1.1
         - fromMouse - whether to use themouse position as the viewpoint - if
           False (default), uses centre of screen as view point
        """
        #print "WM.zoomIn: entered"
        if fromMouse:
            self.zoom(factor, *self.getMousePos())
        else:
            self.zoom(factor)
    #@-node:WM.zoomIn
    #@+node:WM.zoomInMouse
    def zoomInMouse(self, factor=1.1):
        """
        Zooms in to the current view, relative to mouse position
        
        Arguments:
         - factor - amount to zoom by, default 1.1
        """
        print "WM.zoomInMouse: entered"
        self.zoom(factor, *self.getMousePos())
    
    
    #@-node:WM.zoomInMouse
    #@+node:WM.zoomOut
    def zoomOut(self, factor=0.88, fromMouse=False):
        """
        Zooms out from the current view.
        
        Arguments:
         - factor - amount to zoom by, default 0.88
         - fromMouse - whether to use themouse position as the viewpoint - if
           False (default), uses centre of screen as view point
        """
        #print "WM.zoomOut: entered"
        if fromMouse:
            self.zoom(factor, *self.getMousePos())
        else:
            self.zoom(factor)
    
    #@-node:WM.zoomOut
    #@+node:WM.zoomOutMouse
    def zoomOutMouse(self, factor=0.88):
        """
        Zooms out from the current view, relative to mouse position
        
        Arguments:
         - factor - amount to zoom by, default 0.88
        """
        #print "WM.zoomIn: entered"
        self.zoom(factor, *self.getMousePos())
    
    #@-node:WM.zoomOutMouse
    #@+node:WM.pan
    def pan(self, dx=None, dy=None):
        """
        Pans all the windows on the screen
        
        Arguments:
         - dx, dy - x,y offset for panning.
           default is to pan the windows such that the mouse position is used
           as the screen's new central point
        """
        #print "WM.pan: entered"
        xMouse, yMouse = self.getMousePos()
        if dx == None:
            dx = self.screenWidth / 2 - xMouse
        if dy == None:
            dy = self.screenHeight / 2 - yMouse
    
    	try:
            for win in self.windows.values():
                if win.dead or win.isLocked:
                    continue
                x, y = win.position()
                win.position(x+dx, y+dy)
        except:
            print "WM.pan: got an exception"
        try:
            for win in self.windows.values():
                if win.isLocked and not win.dead:
                    win.raise_()
        except:
            print "WM.pan: got an exception"
        #print "WM.pan: finished"
    
    
    
    #@-node:WM.pan
    #@+node:WM.panLeft
    def panLeft(self, amount=30):
        """
        Pans all the windows on the screen to the left
        
        Arguments:
         - amount - number of pixels to pan, default 30
        """
        #print "WM.panLeft: entered"
        self.pan(amount, 0)
        #print "WM.panLeft: finished"
    #@-node:WM.panLeft
    #@+node:WM.panRight
    def panRight(self, amount=30):
        """
        Pans all the windows on the screen to the right
        
        Arguments:
         - amount - number of pixels to pan, default 30
        """
        #print "WM.panRight: entered"
        self.pan(-amount, 0)
        #print "WM.panRight: finished"
    
    
    
    #@-node:WM.panRight
    #@+node:WM.panUp
    def panUp(self, amount=30):
        """
        Pans all the windows on the screen upwards
        
        Arguments:
         - amount - number of pixels to pan, default 30
        """
        #print "WM.panUp: entered"
        self.pan(0, amount)
        #print "WM.panUp: finished"
    
    
    
    #@-node:WM.panUp
    #@+node:WM.panDown
    def panDown(self, amount=30):
        """
        Pans all the windows on the screen downwards
        
        Arguments:
         - amount - number of pixels to pan, default 30
        """
        #print "WM.panDown: entered"
        self.pan(0, -amount)
        #print "WM.panDown: finished"
    
    #@-node:WM.panDown
    #@+node:WM.windowFromIndex
    def windowFromIndex(self, idx):
        """
        Returns a ref to the window with index idx in the creation order
        """
        return self.windowlist[idx]
    #@-node:WM.windowFromIndex
    #@+node:WM.indexFromWindow
    def indexFromWindow(self, win):
        """
        Returns the index of win in the window creation order,
        or -1 if there (oops) is no such window.
        """
        i = 0
        while i < len(self.windowlist):
            if win.hWin == self.windowlist[i].hWin:
                return i
            i += 1
        return -1
    #@nonl
    #@-node:WM.indexFromWindow
    #@-others
#@-node:class WM
#@+node:function setTimer
def setTimer(secs, func):
    """
    Sets a timer to call function 'func' after 'secs' seconds
    """
    #print "flwm.setTimer: setting timer"
    flwm_.setTimer(secs, func)
    #print "flwm.setTimer: done"

#@-node:function setTimer
#@+node:function repeatTimer
def repeatTimer(secs, func):
    """
    Call this within a timer callback, to schedule another timer event
    """
    #print "flwm.repeatTimer: setting timer"
    flwm_.repeatTimer(secs, func)
    #print "flwm.repeatTimer: done"

#@-node:function repeatTimer
#@+node:class Fl_Window
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

    def activate(self):
        """
        Tries to activate this window and make it receive events
        """
        flwm_.Fl_Window_activate(self.wid)

    def set_modal(self):
        """
        Tries to activate this window and make it receive events
        """
        flwm_.Fl_Window_set_modal(self.wid)

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


#@-node:class Fl_Window
#@+node:class Fl_Button
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

    def symbollabel(self):
        """
        Sets the label to diaplay as an FLTK symbol
        """
        flwm_.Fl_Button_symbollabel(self.wid)
#@-node:class Fl_Button
#@+node:class Fl_Repeat_Button
class Fl_Repeat_Button:
    """
    Creates and manages button widgets for use in Fl_Window popup
    windows.
    """
    def __init__(self, x, y, w, h, label):
        """
        Creates a repeat button widget.
        
        Arguments:
         - x, y, w, h - x and y-position within window, width and height
         - label - text to put on the button
        """
        self.wid = flwm_.Fl_Repeat_Button_create(x, y, w, h, label)
        
    def show(self):
        """probably not much point to this
        """
        flwm_.Fl_Repeat_Button_show(self.wid)
        
    def hide(self):
        """
        probably not much point to this either
        """
        flwm_.Fl_Repeat_Button_hide(self.wid)

    def callback(self, func):
        """
        Sets the callback to be triggered when the button gets pushed
        or while it's held
        
        Argument:
         - func - a callable object, eg function, bound method, to be
           invoked when the button gets pushed. Note that at this stage,
           the callable you provide will be called with no arguments.
        """
        flwm_.Fl_Repeat_Button_callback(self.wid, func)

    def symbollabel(self):
        """
        Sets the label to diaplay as an FLTK symbol
        """
        flwm_.Fl_Repeat_Button_symbollabel(self.wid)

#@-node:class Fl_Repeat_Button
#@+node:class Fl_Select_Browser
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
        self.items = []
        
    def show(self):
        """probably not much point to this
        """
        flwm_.Fl_Select_Browser_show(self.wid)
        
    def hide(self):
        """
        probably not much point to this either
        """
        flwm_.Fl_Select_Browser_hide(self.wid)

    def add(self, item):
        """
        Adds a string to the end of the list
        """
        flwm_.Fl_Select_Browser_add(self.wid, item)
        self.items.append(item)

    def remove(self, item):
        """
        Removes an item (index) from the end of the list
        """
        flwm_.Fl_Select_Browser_remove(self.wid, itemindex)
        del self.items[itemindex]

    def clear(self):
        """
        Removes all items from list
        """
        flwm_.Fl_Select_Browser_clear(self.wid)
        self.items = []

    def find(self, itemstr):
        i = 0
        while i < len(self.items):
            if self.items[i] == itemstr:
                return i + 1
            i += 1
        return -1
            
    def itemtext(self, itemidx):
        """
        Returns the text of the given item index
        """
        return self.items[itemidx - 1]

    def color(self, colornum):
        """
        Sets the background colour in the select browser widget
        """
        flwm_.Fl_Select_Browser_color(self.wid, color(colornum))

    def textcolor(self, colornum):
        """
        Sets the colour of text in the select browser widget
        """
        flwm_.Fl_Select_Browser_textcolor(self.wid, color(colornum))

    def selectioncolor(self, colornum):
        """
        Sets the colour of selections in the select browser widget
        """
        fcolor = color(colornum)

        #print "Fl_Select_Browser.selectioncolor: %s -> %s" % (colornum, fcolor)
        
        flwm_.Fl_Select_Browser_selectioncolor(self.wid, color(colornum))

    def value(self, val=None):
        """
        Gets or sets the value selected. Value is an index into the
        list of displayed values
        """
        #print "Fl_Select_Browser: entered"
        if val == None:
            #print "fetching value"
            return flwm_.Fl_Select_Browser_value(self.wid)
        else:
            #print "setting value to %s" % val
            return flwm_.Fl_Select_Browser_value(self.wid, val)

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



#@-node:class Fl_Select_Browser
#@+node:class Fl_Hold_Browser
class Fl_Hold_Browser:
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
        self.wid = flwm_.Fl_Hold_Browser_create(x, y, w, h, label)
        self.items = []
        
    def show(self):
        """probably not much point to this
        """
        flwm_.Fl_Hold_Browser_show(self.wid)
        
    def hide(self):
        """
        probably not much point to this either
        """
        flwm_.Fl_Hold_Browser_hide(self.wid)

    def add(self, item):
        """
        Adds a string to the end of the list
        """
        flwm_.Fl_Hold_Browser_add(self.wid, item)
        self.items.append(item)

    def remove(self, item):
        """
        Removes an item (index) from the end of the list
        """
        flwm_.Fl_Hold_Browser_remove(self.wid, item)
        del self.items[itemindex]

    def clear(self):
        """
        Removes all items from list
        """
        print "Fl_Hold_Browser.clear: entered"
        flwm_.Fl_Hold_Browser_clear(self.wid)
        self.items = []

    def find(self, itemstr):
        i = 0
        while i < len(self.items):
            if self.items[i] == itemstr:
                return i + 1
            i += 1
        return -1

    def itemtext(self, itemidx):
        """
        Returns the text of the given item index
        """
        return self.items[itemidx - 1]

    def color(self, colornum):
        """
        Sets the background colour in the Hold browser widget
        """
        flwm_.Fl_Hold_Browser_color(self.wid, color(colornum))

    def textcolor(self, colornum):
        """
        Sets the colour of text in the Hold browser widget
        """
        flwm_.Fl_Hold_Browser_textcolor(self.wid, color(colornum))

    def selectioncolor(self, colornum):
        """
        Sets the colour of selections in the Hold browser widget
        """
        fcolor = color(colornum)

        #print "Fl_Hold_Browser.selectioncolor: %s -> %s" % (colornum, fcolor)
        
        flwm_.Fl_Hold_Browser_selectioncolor(self.wid, color(colornum))

    def value(self, val=None):
        """
        Gets or sets the value selected. Value is an index into the
        list of displayed values
        """
        #print "Fl_Hold_Browser: entered"
        if val == None:
            #print "fetching value"
            return flwm_.Fl_Hold_Browser_value(self.wid)
        else:
            #print "setting value to %s" % val
            return flwm_.Fl_Hold_Browser_value(self.wid, val)

    def callback(self, func):
        """
        Sets the callback to be triggered when the button gets pushed.
        
        Argument:
         - func - a callable object, eg function, bound method, to be
           invoked when an item in the listbox gets selected.
           Note that at this stage,
           the callable you provide will be called with no arguments.
        """
        flwm_.Fl_Hold_Browser_callback(self.wid, func)


#@-node:class Fl_Hold_Browser
#@+node:class Fl_Input
class Fl_Input:
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
        self.wid = flwm_.Fl_Input_create(x, y, w, h, label)

    def color(self, colornum):
        """
        Sets the background colour in the input widget
        """
        flwm_.Fl_Input_color(self.wid, color(colornum))

    def textcolor(self, colornum):
        """
        Sets the colour of text in the input widget
        """
        flwm_.Fl_Input_textcolor(self.wid, color(colornum))

    def selectioncolor(self, colornum):
        """
        Sets the colour of selections in the input widget
        """
        fcolor = color(colornum)

        #print "Fl_Input.selectioncolor: %s -> %s" % (colornum, fcolor)
        
        flwm_.Fl_Input_selectioncolor(self.wid, color(colornum))

    def callback(self, func):
        """
        Sets the callback to be triggered when user presses ENTER.
        
        Argument:
         - func - a callable object, eg function, bound method, to be
           invoked when the user presses ENTER.
           Note that at this stage,
           the callable you provide will be called with no arguments.
        """
        flwm_.Fl_Input_callback(self.wid, func)

    def value(self, txt=None):
        """
        Gets or sets the displayed text
        
        Arguments:
         - txt - if provided, sets the field to display this.
           If not provided, does not change the field
        
        Returns:
         - Always returns the text in the field
        """
        #print "pywm.Fl_Input.value: entered"
        return flwm_.Fl_Input_value(self.wid, txt)

    def take_focus(self):
        """
        Causes the input field to take focus
        """
        flwm_.Fl_Input_take_focus(self.wid)
#@-node:class Fl_Input
#@+node:class appletBase
class appletBase(Fl_Window):
    """
    Base class for applet windows
    
    The applet should register its callbacks as handlers
    by calling self.addHandlers()
    """
    
    def __init__(self, wm, x, y, w, h):
        """
        Creates an applet window.
        
        Arguments:
          - wm - ref to the window manager
          - x, y - location of applet on screen
          - w, h - size of applet window
        """
        print "pywm.applet.__init__: entered: x=%d y=%d w=%d h=%d" % (
            x, y, w, h)
        self.wm = wm
        print "pywm.applet.__init__: got wm"
        Fl_Window.__init__(self, x, y, w, h, "<applet>")
        self.hide()

    def addHandlers(self, **handlers):
        """
        Adds a set of handlers for various events
        
        handlers are declared as keywords.
        See doc for WM.addHandlers for details
        """
        self.wm.addHandlers(**handlers)


#@-node:class appletBase
#@+node:class appletBar
class appletBar:
    """
    A virtual container for applets.
    
    Alloes applets to be added sequentially, and manages
    the sizing and placement of these applets
    """
    
    def __init__(self, wm, x, y, thickness, direction):
        """
        Creates an applet bar, to which applets can be added
        
        Arguments:
         - wm - ref to window manager
         - x, y - origin of applet bar on screen
         - thickness - thickness of applet bar in pixels
         - direction - 'up', 'down', 'left', 'right' - direction in which
           applets get added
        """
        self.wm = wm
        self.x = x
        self.y = y
        self.xNext = x
        self.yNext = y
        self.thickness = thickness
        self.direction = direction
        self.applets = []

    def add(self, appletclass, length, *args, **kw):
        """
        Adds an applet to the virtual applet bar
        
        Arguments:
         - appletclass - one of the classes in pywm.applets
         - thickness - if applet bar is vertical, this is height. Otherwise,
           this is width.
         - any other applet-dependent args, also keywords
        """
        print "appletBar.add: args=", args
        try:
            if self.direction == 'down':
                x = self.x
                y = self.yNext
                w = self.thickness
                h = length
                self.yNext = self.yNext + length
            elif self.direction == 'up':
                x = self.x
                y = self.yNext - length
                w = self.thickness
                h = length
                self.yNext = y
            elif self.direction == 'right':
                x = self.xNext
                y = self.y
                w = length
                h = self.thickness
                self.xNext = self.xNext + length
            elif self.direction == 'left':
                x = self.xNext - self.length
                y = self.y
                w = length
                h = self.thickness
                self.xNext = self.xNext - length
            appletInst = appletclass(self.wm, x, y, w, h, *args, **kw)
            if appletInst:
                self.applets.append(appletInst)
        except:
            print "pywm.appletBar.add: error adding applet"
            traceback.print_exc()

#@-node:class appletBar
#@+node:function color
colornames = {'red':'#ff0000',
               'green':'#00ff00',
               'blue':'#0000ff',
               'yellow':'#ffff00',
               'purple':'#ff00ff',
               'white':'#ffffff',
               'black':'000000',
               }

def color(spec):
    if colornames.has_key(spec):
        spec = colornames[spec]
    
    if spec[0] != '#' or len(spec) != 7:
        #print "illegal color spec: '%s'" % spec
        return 0
    red = int(spec[1:3], 16)
    green = int(spec[3:5], 16)
    blue = int(spec[5:7], 16)

    col = flwm_.color(red, green, blue)
    #print "pywm.color: %s => %x" % (spec, col)
    return col

#@-node:function color
#@-others

#@-node:@file src-python/__init__.py
#@-leo
