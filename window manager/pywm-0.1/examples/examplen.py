#!/usr/bin/env python
#@+leo
#@+node:0::@file examples/examplen.py
#@+body
#@@first
#@@language python
#@<< test declarations >>
#@+node:1::<< test declarations >>
#@+body
#!/usr/bin/env python

import random
import pywm


#@-body
#@-node:1::<< test declarations >>


#@+others
#@+node:2::class myWindow
#@+body
class myWindow(pywm.window):

    #@+others
    #@+node:1::myWindow.on_resize
    #@+body
    def on_resize(self, x, y, w, h):
        print "myWindow: %d %d %d %d %s" % (x, y, w, h, self.name)
    
    
    #@-body
    #@-node:1::myWindow.on_resize
    #@-others


#@-body
#@-node:2::class myWindow
#@+node:3::class myWM
#@+body
class myWM(pywm.WM):

    #@+others
    #@+node:1::myWM.__init__
    #@+body
    def __init__(self, **kwds):
    	pywm.WM.__init__(self, **kwds)
        #self.bindKey("z", self.hello)
        self.vi = None
    
    def hello(self):
        print "hello"
    
    
    #@-body
    #@-node:1::myWM.__init__
    #@+node:2::myWM.on_create
    #@+body
    def on_create(self, win):
    
        print "myWM.on_create: created window '%s'" % win.name
        #return
    
        x, y = win.position()
        #print "myWM:on_create: win.position succeeded"
        w, h = win.size()
        #print "myWM:on_create: win.size succeeded"
        #print "myWM:on_create: x=%d y=%d w=%d h=%d" % (x, y, w, h)
        #return
     
        # move the window
        if win.name == "xterm":
            #print "got xterm,, but bailing for now"
            #return
            self.xterm = win
            #print "myWM.on_create: trying to reposition and resize xterm"
            win.position(0, 0)
            win.size(600, 600)
            #print "myWM.on_create: repos/resize seems ok"
            return
            if self.vi:
                print "trying to close vi window"
                self.vi.close()
    
        if win.name == "vi":
            #print "myWM.on_create: got vi started"
            self.vi = win
            #return 
            self.launch("xterm")
            #print "myWM.on_create: tried to launch xterm"
    
    #@-body
    #@-node:2::myWM.on_create
    #@+node:3::myWM.on_destroy
    #@+body
    def on_destroy(self, win):
        print "myWM:on_destroy: destroyed window '%s'" % win.name
    
    #@-body
    #@-node:3::myWM.on_destroy
    #@+node:4::myWM.on_resize
    #@+body
    def on_resize(self, win, x, y, w, h):
        #print "on_resize"
        #print "myWM.on_resize: x=%d y=%d w=%d h=%d name=%s" % (x, y, w, h, win.name)
        return
    
        if win.name == 'Terminal':
            self.xterm.position(x, y-self.xterm.height())
        return 0
    
    
    #@-body
    #@-node:4::myWM.on_resize
    #@+node:5::myWM.on_buttonPress
    #@+body
    def on_buttonPress(self, button):
        print "myWM.on_buttonPress callback: button %d" % button
    
    #@-body
    #@-node:5::myWM.on_buttonPress
    #@+node:6::myWM.on_keyEvent
    #@+body
    def on_keyEvent(self, key):
        print "myWM.on_keyEvent callback: key=%d" % key
    
    
    #@-body
    #@-node:6::myWM.on_keyEvent
    #@+node:7::myWM.on_enter
    #@+body
    def on_enter(self, win):
        
        name = win.name
    
        x, y = self.getMousePosition()
        print "myWM.on_enter: x=%d y=%d" % (x, y)
        if name == 'vi':
            print "mouse at %d,%d" % (x, y)
            win.position(random.randint(0, 400), random.randint(0, 400))
    
    
    #@-body
    #@-node:7::myWM.on_enter
    #@+node:8::myWM.on_leave
    #@+body
    #def on_leave(self, hWin):
    #    
    #    win = self.windows[hWin]
    #    name = win.name
    #
    #    x, y = self.getMousePosition()
    #    print "on_leave: mouse at %d,%d" % (x, y)
    
    
    #@-body
    #@-node:8::myWM.on_leave
    #@-others


#@-body
#@-node:3::class myWM
#@+node:4::run
#@+body
def run():
    #wm = flwm.WM(fg="#ffcc00", bg="#000000", exit=1)
    wm = myWM(windowclass=myWindow,
              fg="#ffcc00",
              abg="#800080",
              bg="#000000",
              exit=1)
    wm.run()


#@-body
#@-node:4::run
#@+node:5::MAINLINE
#@+body
if __name__ == '__main__':
    run()

#@-body
#@-node:5::MAINLINE
#@-others


#@-body
#@-node:0::@file examples/examplen.py
#@-leo
