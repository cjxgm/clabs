#@+leo
#@+node:0::@file src-python/applets.py
#@+body
#@@language python
"""
pywm.applet

A set of applets
"""

import sys, os, traceback, re

sys.path.append('..')
import pywm
sys.path.pop()


#@+others
#@+node:1::class tasklist
#@+body
class tasklist(pywm.appletBase):
    """
    Creates and operates a tasklist applet.
    """
    def __init__(self, wm, x, y, w, h):
        """
        Creates the tasklist applet
        
        Args:
         - wm - ref to window manager
         - x, y - position of tasklist
         - w, h - dimensions of tasklist
        """
        #print "tasklist.__init__: entered"
        pywm.appletBase.__init__(self, wm, x, y, w, h)

        #print "tasklist.__init__: started"
        if getattr(self, 'win', None) == None:
            lst = self.lst = pywm.Fl_Hold_Browser(5, 5, w-10, h-10, "")
            lst.color("#000000")
            lst.textcolor("#ffcc00")
            lst.selectioncolor("#ffcc00")
            for win in self.wm.windows.values():
                lst.add(win.name)

            self.end()
            self.show()
            lst.callback(self.onCallback)
        else:
            self.taskbarRefresh()
            self.show()
        #print "tasklist.__init__: finished"

        self.addHandlers(startup=self.onStartup,
                         create=self.taskbarRefresh,
                         destroy=self.taskbarRefresh,
                         activate=self.taskbarRefresh,
                         deactivate=self.taskbarRefresh,
                         )

    def onStartup(self):
        """
        """
        #print "tasklist.onStartup: started"
        self.show()

    def taskbarRefresh(self, win=None):
        #print "tasklistRefresh: entered"
        if win:
            print win
        try:
            self.lst.clear()
            for w in self.wm.windowlist:
                if not w.dead:
                    self.lst.add(w.name)
            if win:
                #print "tasklistRefresh: marking window active"
                self.lst.value(self.wm.indexFromWindow(win)+1)
        except:
            pass
        return 0 # pass this to other handlers in the chain
        #print "taskbarRefresh: done"
       
    def onCallback(self, arg=None):
        sel = self.lst.value()
        if sel >= 1:
            try:
                win = self.wm.windowFromIndex(sel-1)
                if win:
                    win.raise_()
                    win.activate()
            except:
                print "window select failed"


#@-body
#@-node:1::class tasklist
#@+node:2::class navigator
#@+body
class navigator(pywm.appletBase):
    """
    Creates and operates a navigator applet
    """
    def __init__(self, wm, x, y, w, h, factor=10):
        """
        Creates a navigator applet, with pan/zoom buttons
        
        Args:
         - wm - ref to window manager
         - x, y - position of navigator
         - w, h - dimensions of navigator
        """
        print "navigator.__init__: entered"
        pywm.appletBase.__init__(self, wm, x, y, w, h)
        
        self.factor = factor

        butw = (w - 10)/5
        buth = (h - 10)/2

        # add buttons to zoom and pan the desktop
        butPL = self.butPL = pywm.Fl_Repeat_Button(5, 5, butw, buth, "@4>")
        butPL.callback(self.onButPL)
        butPL.symbollabel()

        butPR = self.butPR = pywm.Fl_Repeat_Button(5+butw, 5, butw, buth, "@>")
        butPR.callback(self.onButPR)
        butPR.symbollabel()

        butPU = self.butPU = pywm.Fl_Repeat_Button(5+2*butw, 5, butw, buth, "@8>")
        butPU.callback(self.onButPU)
        butPU.symbollabel()

        butPD = self.butPD = pywm.Fl_Repeat_Button(5+3*butw, 5, butw, buth, "@2>")
        butPD.callback(self.onButPD)
        butPD.symbollabel()

        # faster ones
        butPL = self.butPL = pywm.Fl_Repeat_Button(5, 5+buth, butw, buth, "@4>>")
        butPL.callback(self.onButPLf)
        butPL.symbollabel()

        butPR = self.butPR = pywm.Fl_Repeat_Button(5+butw, 5+buth, butw, buth, "@>>")
        butPR.callback(self.onButPRf)
        butPR.symbollabel()

        butPU = self.butPU = pywm.Fl_Repeat_Button(5+2*butw, 5+buth, butw, buth, "@8>>")
        butPU.callback(self.onButPUf)
        butPU.symbollabel()

        butPD = self.butPD = pywm.Fl_Repeat_Button(5+3*butw, 5+buth, butw, buth, "@2>>")
        butPD.callback(self.onButPDf)
        butPD.symbollabel()

        # zoom buttons
        butZO = self.butZO = pywm.Fl_Repeat_Button(5+4*butw, 5, butw, buth, "@8->")
        butZO.callback(self.onButZO)
        butZO.symbollabel()

        butZI = self.butZI = pywm.Fl_Repeat_Button(5+4*butw, 5+buth, butw, buth, "@2->")
        butZI.callback(self.onButZI)
        butZI.symbollabel()

        self.end()
        self.show()

    def onButPL(self):
        self.wm.panLeft(self.factor)

    def onButPU(self):
        self.wm.panUp(self.factor)

    def onButPD(self):
        self.wm.panDown(self.factor)

    def onButPR(self):
        self.wm.panRight(self.factor)

    def onButPLf(self):
        self.wm.panLeft(self.factor * 12)

    def onButPUf(self):
        self.wm.panUp(self.factor * 12)

    def onButPDf(self):
        self.wm.panDown(self.factor * 12)

    def onButPRf(self):
        self.wm.panRight(self.factor * 12)

    def onButZI(self):
        self.wm.zoomIn(self.factor)

    def onButZO(self):
        self.wm.zoomOut(self.factor)

    def onStartup(self):
        """
        """
        self.show()



#@-body
#@-node:2::class navigator
#@+node:3::class quicklaunch
#@+body
class quicklaunch(pywm.appletBase):
    """
    Creates a quick launcher applet
    """
    
    def __init__(self, wm, x, y, w, h, *programs):
        """
        Creates the launcher applet
        
        Args:
         - wm - ref to window manager
         - x, y - position of tasklist
         - w, h - dimensions of tasklist
         - programs - a list of ('name', 'command') tuples to put on launcher
        """
        print "quicklaunch.__init__: entered: (%d,%d,%d,%d)" % (x,y,w,h)
        print "quicklaunch.__init__: programs", programs
        pywm.appletBase.__init__(self, wm, x, y, w, h)

        print "quicklaunch.__init__: created fltk window"

        lst = self.lst = pywm.Fl_Select_Browser(5, 5, w-10, h-10, "")
        self.end()
        self.show()
        lst.color("#000000")
        lst.textcolor("#ffcc00")
        lst.selectioncolor("#ffcc00")
        
        print "quicklaunch.__init__: ok1"

        self.progNames = []
        self.progCmds = []
        for prog in programs:
            self.progNames.append(prog[0])
            self.progCmds.append(prog[1])
            lst.add(prog[0])
        lst.callback(self.onSelect)
        lst.value(0)

        print "quicklaunch.__init__: ok2"

    
    def onSelect(self):
        choice = self.lst.value()
        if choice >= 1:
            cmd = self.progCmds[choice-1]
            if type(cmd) is type(""):
                print "Launching: '%s'" % cmd
                os.system(cmd + " &")
            elif callable(cmd):
                print "Invloking quicklaunch callable"
                cmd()


#@-body
#@-node:3::class quicklaunch
#@+node:4::class shellbox
#@+body
class shellbox(pywm.appletBase):
    """
    Displays a small input field for entering shell commands
    """
    def __init__(self, wm, x, y, w=100, h=40):
        """
        Creates the shellbox applet
        
        Args:
         - wm - ref to window manager
         - x, y - position of shellbox
         - w, h - dimensions of shellbox (default 120x40)
        """
        print "shellbox.__init__: entered"
        pywm.appletBase.__init__(self, wm, x, y, w, h)

        fld = self.fld = pywm.Fl_Input(5, 5, w-10, h-10, "<FLWM>")
        fld.color("#000000")
        fld.textcolor("#ffcc00")
        fld.selectioncolor("#ffcc00")
        self.end()

        fld.callback(self.onEnter)
        self.show()
        print "shellbox.__init__: done"

    def onEnter(self):
        #print "shellbox.onEnter: callback fired"
        try:
            cmd = self.fld.value()
        except:
            print "shellbox.onEnter: callback failed"
            traceback.print_exc()
        else:
            #print "got command '%s'" % cmd
            os.system(cmd + " &")

    def on_enter(self, x, y):
        """
        Called when the mouse enters this window.
        If you override this, you must accept 2 args, x and y,
        which will be the position of the mouse on entry.
        """
        self.activate()
        #self.set_modal()
        #try:
        #    self.wm.currentWindow.deactivate()
        #except:
        #    traceback.print_exc()
        #    pass
        self.fld.take_focus()


#@-body
#@-node:4::class shellbox
#@+node:5::class xapp
#@+body
class xapp(pywm.appletBase):
    """
    Generic applet type for generic X applications (eg xbiff, xclock etc)
    """
    def __init__(self, wm, x, y, w, h, cmd, titlematch):
        """
        Creates an applet space for an X application
        
        Args:
         - wm - ref to window manager
         - x, y - position of shellbox
         - w, h - dimensions of shellbox (default 120x40)
         - cmd - shell command line to launch this app
         - titlematch - a regular expression to match for the title bar
        """
        print "shellbox.__init__: entered"
        
        self.wm = wm
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.cmd = cmd
        self.titlematch = re.compile(titlematch)
        self.exists = 0

        # add an onCreate handler to intercept this window's creation
        self.addHandlers(create=self.onCreate)

        # now launch the app
        self.wm.launch(self.cmd)

    def onCreate(self, win):
        """
        Called when x app gets created, and moves/resizes it to the needed size
        and position, strips off the titlebar, and locks it against desktop 
        zoom/pan
        """
        if not self.exists and self.titlematch.match(win.name):
            print "x applet: got window"
            win.setborder('thin')
            win.size(self.w, self.h)
            win.position(self.x, self.y)
            win.lock()
            self.win = win
            self.exists = 1
            return 1 # don't add to task list etc
        return 0

#@-body
#@-node:5::class xapp
#@-others




#@-body
#@-node:0::@file src-python/applets.py
#@-leo
