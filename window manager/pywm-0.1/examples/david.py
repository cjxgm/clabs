#!/usr/bin/env python
#@+leo-ver=4
#@+node:@file examples/david.py
#@@first
#@@language python

"""
example5.py

Same as example4, but adds left-edge border sensitivity

Also, adds a whole lot more keybindings
"""

#@+others
#@+node:imports
import pywm
import sys
from random import randint
import traceback
import pywm.applets

#@-node:imports
#@+node:class leftEdge
class leftEdge(pywm.Fl_Window):
    """
    This is a pywm window which sits on the left edge of the screen,
    and fires callbacks when the mouse enters or clicks on the left edge.
    """
    #@    @+others
    #@+node:__init__
    def __init__(self, wm):
        self.wm = wm
        print "leftEdge: creating..."
        self.scrW, self.scrH = wm.getScreenSize()
        print "scrW=%d scrH=%d" % (self.scrW, self.scrH)
        pywm.Fl_Window.__init__(self, 0, 0, 3, self.scrH-2, "")
        self.show()
        self.stayOnTop()
        self.move(0, 1)
        self.yMin = int(self.scrH * 0.3)
        self.yMax = int(self.scrH * 0.5)
        print "leftEdge: now active"
    
    #@-node:__init__
    #@+node:on_enter
    def on_enter(self, x, y):
        print "leftEdge: screen size (%d,%d) - mouse at %d,%d" % (
             self.scrW, self.scrH, x, y)
        if y > self.yMin and y < self.yMax:
            try:
                self.wm.showMenu()
            except:
                print "leftEdge: showMenu failed"
            else:
                print "leftEdge: is menu up?"
    #@-node:on_enter
    #@+node:on_click
    def on_click(self, but, x, y):
        print "leftEdge: but=%d, mouse at %d,%d" % (but, x, y)
    
    #@-node:on_click
    #@-others
#@-node:class leftEdge
#@+node:class popupWindow
class popupWindow(pywm.Fl_Window):
    """
    With pywm, you can create dynamic windows for user interaction
    via FLTK classes (a subset of these exposed to Python).
    Vastly easier than hacking raw X stuff.
    
    This is a simple window that just pops up a small window with
    a single button.
    """
    #@    @+others
    #@+node:__init__
    def __init__(self, wm):
        print "popupWindow: trying to make fltk window"
        self.wm = wm
        pywm.Fl_Window.__init__(self, 0, 0, 100, 60, "Hi There!")
        flB = pywm.Fl_Button(5,5,90,50,"A Button")
        flB.callback(self.cb)
        self.end()
        self.hide()
        #print "popupWindow window should be hidden"
    #@-node:__init__
    #@+node:cb
    def cb(self, *args):
        print "cb"
        self.hide()
        print "cb: done"
    #@-node:cb
    #@-others
#@-node:class popupWindow
#@+node:class myWindowClass
class myWindowClass(pywm.window):
    """
    These window classes get instantiated by the PYWM engine.
    You should never instantiate this class yourself.
    
    If you create a subclass of pywm.window, and pass them in
    when constructing your window manager, then your subclass will be
    used every time a window is created.
    """
    #@    @+others
    #@+node:__init__
    def __init__(self, hWin, wm, **kwds):
        print "myWindowClass.__init__: creating a window"
        pywm.window.__init__(self, hWin, wm, **kwds)
        print "myWindowClass.__init__: window %s created" % self.name
    
        # if window is gnome-terminal, move it to bottom left
        try:
            if self.name == 'Terminal':
                self.setborder('thin')
                self.goSouthWest()
                self.xGranularity = 10
                self.yGranularity = 18
                
            elif self.name[0:9] == "ProcMeter":
                self.setborder('thin')
                self.lock()
                self.size(100, 160)
                #self.position(0, 100)
                self.goNorthWest()
                #pywm.setTimer(3, self.moveDown)
            elif self.name in ['xterm', 'pywm']:
                self.setborder('thin')
                self.lock()
                self.size(360, 120)
                self.position(100, 0)
            elif self.name == 'xclock':
                self.setborder('thin')
                self.lock()
                self.size(100, 100)
                self.position(0,0)
                              
        except:
            pass
    
        #print "window init ok1"
        self.x0, self.y0 = self.position()
        self.w0, self.h0 = self.size()
        self.maxdelta = 20
        self.dead = False
    
        # uncomment this line if you feel like warping your mind
        #pywm.setTimer(10, self.hallucinate)
    
    #@-node:__init__
    #@+node:_moveDown
    def _moveDown(self):
        if self.dead:
            return
        x, y = self.position()
        y = y + 1
        if y > 600:
            y = 0
        self.position(x, y)
        #pywm.repeatTimer(1, self.moveDown)
    
    #@-node:_moveDown
    #@+node:hallucinate
    def hallucinate(self):
        """
        Fun routine which creates slight repeating random changes in size and position
        """
        #print "hallucinate 1"
        if self.dead:
            return
        #print "hallucinate 1a"
    
        x, y = self.position()
        w, h = self.size()
    
        #print "hallucinate 2"
        x = x + randint(-10, 10)
        if x < 0 or x < self.x0 - self.maxdelta:
            x = self.x0 - self.maxdelta
        elif x > self.x0 + self.maxdelta:
            x = self.x0 + self.maxdelta
    
        #print "hallucinate 3"
        y = y + randint(-10, 10)
        if y < 0 or y < self.y0 - self.maxdelta:
            y = self.y0 - self.maxdelta
        elif y > self.y0 + self.maxdelta:
            y = self.y0 + self.maxdelta
    
        w = w + randint(-1, 1)
        if w < self.w0 - self.maxdelta:
            w = self.w0 - self.maxdelta
        elif w > self.w0 + self.maxdelta:
            w = self.w0 + self.maxdelta
    
        h = h + randint(-1, 1)
        if h < self.h0 - self.maxdelta:
            h = self.h0 - self.maxdelta
        elif h > self.h0 + self.maxdelta:
            h = self.h0 + self.maxdelta
    
        #print "hallucinate 4"
        self.position(x, y)
        self.size(w, h)
        pywm.setTimer(0.5, self.hallucinate)
        #print "hallucinate 5"
    #@-node:hallucinate
    #@-others
#@-node:class myWindowClass
#@+node:function cbTimer
def cbTimer():
    print "global cbTimer fired"


#@-node:function cbTimer
#@+node:class myWindowManagerClass
class myWindowManagerClass(pywm.WM):
    """
    Our custom window manager class
    """
    #@    @+others
    #@+node:__init__
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
        self.gotTerminal = False
        self.gotProcmeter = False
        self.gotClock = False
        self.win_xemacs = None
        self.win_conspyre = None
    #@-node:__init__
    #@+node:on_startup
    def on_startup(self):
        """
        If you define an 'on_startup' method in your class, this
        method will get invoked just before the eindow manager
        engine enters its event loop. Note, however, that it
        gets called *after* the existing windows are discovered
        and the corresponding on_create callbacks fired.
        """
        print "myWindowManager.on_startup"
    
        self.leftbar = pywm.appletBar(self, 0, 0, 100, 'down')
        #self.leftbar.add(pywm.applets.xapp, 100,
        #                 "xclock -bg \\#000000 -fg \\#ffcc00 -hd \\#ffcc00 -update 1",
        #                 "xclock")
    
        #self.leftbar.add(pywm.applets.xapp, 170, "procmeter3", "ProcMeter3.*")
    
        self.leftbar.add(pywm.applets.tasklist, 150)
        if 1:
            try:
                self.leftbar.add(pywm.applets.quicklaunch, 240,
                                 ('Terminal', 'gnome-terminal'),
                                 ('Browser', 'galeon'),
                                 ('Email', 'evolution'),
                                 ('X-Emacs', 'xemacs'),
                                 ('X-term', 'xterm'),
                                 ('Pan News', 'pan'),
                                 ('Moz Editor', 'mozilla -edit'),
                                 ('XMMS', 'xmms'),
                                 ("X-Chat", 'xchat'),
                                 ("gtop", "gtop"),
                                 ("ConsPyre", self.hackconspyre),
                                 ("CONFIG", "pywm-hack"),
                                 )
            except:
                print "myWindowManager.on_startup: quicklaunch create failed"
                traceback.print_exc()
        if 1:
            self.leftbar.add(pywm.applets.navigator, 40)
            self.leftbar.add(pywm.applets.shellbox, 40)
            #self.leftbar.add(pywm.applets.xapp, 120, "xbiff", "xbiff")
    
        if 0:
            try:
                print "trying to create xbiff ***********************"
                self.xbiff = pywm.applets.xapp(self, 130, 160, 150, 150,
                                               "xbiff -bg \\#000000 -fg \\#ffcc00", "xbiff")
                print "created xbiff ********************************"
            except:
                print "myWindowManager.on_startup: xbiff create failed"
                traceback.print_exc()
            else:
                print "on_startup: xbiff created"
    
        # bind Shift-Alt-A to run self.launch with arg 'xterm'
        self.bindKey(self.launch, "xterm",
                     'a', 1, 0, 1, 0)
    
        # bind Shift-Alt-Meta-A to run vi
        self.bindKey(self.launch, "xterm -e vi",
                     'a', 1, 0, 1, 1)
    
        # bind Shift-Alt-Meta-Insert to run emacs in terminal
        self.bindKey(self.launch, "xterm -e emacs -nw",
                     'insert', 1, 0, 1, 1)
    
        # general bindings
        self.bindKey(self.launch, "galeon", 'g', 0, 0, 1, 1)
        self.bindKey(self.launch, "gnome-terminal", "t", 0, 0, 1, 1)
        self.bindKey(self.launch, "mozilla-thunderbird", "m", 0, 0, 1, 1)
        self.bindKey(self.launch, "xemacs", "e", 0, 0, 1, 1)
    
        self.bindKey(self.goPrevWindow, 0, '[', 0,0,0,1)
        self.bindKey(self.goNextWindow, 0, ']', 0,0,0,1)
    
        # a few assored launcher bindings
        #self.bindKey(self.launch, "sylpheed-claws", 's', 0, 0, 1, 1)
        #self.bindKey(self.launch, "firebird", "f", 0, 0, 1, 1)
        #self.bindKey(self.launch, "knode", "d", 0, 0, 1, 1)
    
        # add a few bindings to allow moving windows via keyboard
        self.bindKey(self.moveCurrent, 'up', 'up', 0, 0, 0, 1)
        self.bindKey(self.moveCurrent, 'down', 'down', 0, 0, 0, 1)
        self.bindKey(self.moveCurrent, 'left', 'left', 0, 0, 0, 1)
        self.bindKey(self.moveCurrent, 'right', 'right', 0, 0, 0, 1)
        self.bindKey(self.moveCurrent, 'north', 'up', 0, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'south', 'down', 0, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'west', 'left', 0, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'east', 'right', 0, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'centre', 'home', 0, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'northwest', 'up', 1, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'northeast', 'right', 1, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'southwest', 'left', 1, 1, 0, 1)
        self.bindKey(self.moveCurrent, 'southeast', 'down', 1, 1, 0, 1)
    
        self.bindKey(self.resizeCurrent, 'wider', 'right', 1, 0, 0, 1)
        self.bindKey(self.resizeCurrent, 'taller', 'down', 1, 0, 0, 1)
        self.bindKey(self.resizeCurrent, 'narrower', 'left', 1, 0, 0, 1)
        self.bindKey(self.resizeCurrent, 'shorter', 'up', 1, 0, 0, 1)
    
        self.bindKey(self.toggleMaxCurrent, None, '.', 0, 0, 1, 1)
        self.bindKey(self.toggleMaxWidthCurrent, None, ',', 0, 0, 1, 1)
        self.bindKey(self.toggleMaxHeightCurrent, None, '/', 0, 0, 1, 1)
    
        self.bindKey(self.cascadeWindows, 40, "q", 1, 0, 1, 1)
    
        self.bindKey(self.closeCurrent, None, 'x', 1, 0, 1, 1)
    
        self.bindKey(self.zoomIn, 1.1, 'down', 1, 0, 1, 1)
        self.bindKey(self.zoomOut, 0.88, 'up', 1, 0, 1, 1)
        self.bindKey(self.zoomInMouse, 1.1, 'down', 1, 1, 1, 1)
        self.bindKey(self.zoomOutMouse, 0.88, 'up', 1, 1, 1, 1)
    
        self.bindKey(self.panLeft, 30, 'left', 0, 0, 1, 1)
        self.bindKey(self.panRight, 30, 'right', 0, 0, 1, 1)
        self.bindKey(self.panUp, 30, 'up', 0, 0, 1, 1)
        self.bindKey(self.panDown, 30, 'down', 0, 0, 1, 1)
    
        print "myWindowManager: key binding done"
    
        self.flWin = None
    
        # create key binding so Meta-Alt-f pops up a window
        self.bindKey(self.flShow, None, "f", 1,0, 1, 1)
    
        #if not self.gotProcmeter:
        #    self.launch("procmeter3")
    
        #if not self.gotTerminal:
        #    pywm.setTimer(1, self.cbTimer)
    
        pywm.setTimer(1, self.cbShowEdges)
    
    
    
    #@-node:on_startup
    #@+node:on_resize
    def on_resize(self, win, x, y, w, h):
        """
        """
        win.x0, win.y0 = win.position()
        #print "myWindowManager: resized %s to (%d,%d %d,%d)" % (
        #         win.name, x, y, w, h)
    
        # create a 'docking' effect - if moving pyweb leo window, move emacs as well
        if win == self.win_conspyre:
            if self.win_xemacs:
                w0, h0 = self.win_xemacs.size()
                self.win_xemacs.position(x+w+4, y)
                self.win_xemacs.size(w0, h)
    #@-node:on_resize
    #@+node:on_create
    def on_create(self, win):
        print "myWindowManager: created window %s" % win.name
        if win.name == 'Terminal':
            self.gotTerminal = True
        elif win.name[0:9] == 'ProcMeter':
            self.gotProcmeter = True
        elif win.name == 'xclock':
            self.gotClock = True
        elif win.name.find("emacs:") >=0:
            print "Got XEmacs"
            self.win_xemacs = win
        elif win.name.startswith("/main/pywindows/myprogs/conspyre/code.leo"):
            print "Got conspyre"
            self.win_conspyre = win
    #@-node:on_create
    #@+node:on_destroy
    def on_destroy(self, win):
        if win.name.find("emacs:") >=0:
            print "Killing XEmacs"
            self.win_xemacs = None
        elif win.name.startswith("/main/pywindows/myprogs/conspyre/code.leo"):
            print "Killing conspyre"
            self.win_conspyre = None
    #@-node:on_destroy
    #@+node:on_keyEvent
    def _on_keyEvent(self, key):
        print "myWM.on_keyEvent callback: key=%d" % key
    
    #@-node:on_keyEvent
    #@+node:cbTimer
    def cbTimer(self):
        print "self.cbTimer: got callback"
        self.launch("gnome-terminal")
        #pywm.repeatTimer(5, self.cbTimer)
    
    #@-node:cbTimer
    #@+node:cbShowEdges
    def cbShowEdges(self):
        # put up the left-edge window
        self.leftEdge = leftEdge(self)
        self.leftEdge.show()
        self.leftEdge.stayOnTop()
        print "try left edge now"
    
    #@-node:cbShowEdges
    #@+node:flShow
    def flShow(self, *args):
        # put up the left-edge window
        #self.leftEdge = leftEdge(self)
    
        if self.flWin == None:
            self.flWin = popupWindow(self)
        x, y = self.getMousePos()
        self.flWin.move(x, y)
        self.flWin.show()
    #@-node:flShow
    #@+node:hackconspyre
    def hackconspyre(self):
        print "hackconspyre: entered"
        if not self.win_xemacs:
            print "launching xemacs"
            self.launch("xemacs")
        if not self.win_conspyre:
            print "launching leo conspyre"
            self.launch("leo-conspyre")
        pywm.setTimer(1, self.position_conspyre)
    
    #@-node:hackconspyre
    #@+node:position_conspyre
    def position_conspyre(self):
        if not (self.win_xemacs and self.win_conspyre):
            print "waiting for xemacs and/or leo conspyre"
            pywm.setTimer(1, self.position_conspyre)
        else:
            print "positioning leo and xemacs for conspyre"
            self.win_conspyre.position(120, 40)
            self.win_conspyre.size(400, 1000)
            self.win_xemacs.position(520, 40)
            self.win_xemacs.size(1000, 1000)
    #@-node:position_conspyre
    #@-others
#@-node:class myWindowManagerClass
#@+node:function run
def run():         
    # create a window manager object using our class
    windowManager = myWindowManagerClass()

    # and launch the window manager
    windowManager.run()
#@-node:function run
#@+node:mainline
if __name__ == '__main__':
    run()
#@-node:mainline
#@-others
#@-node:@file examples/david.py
#@-leo
