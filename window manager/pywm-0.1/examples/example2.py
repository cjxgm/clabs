#!/usr/bin/env python
#@+leo
#@+node:0::@file examples/example2.py
#@+body
#@@first
#@@language python

"""
example2.py

Same as example1, but uses a subclass.
Nothing too exciting so far.
"""

import pywm

class myWindowManagerClass(pywm.WM):
    pass # will define stuff in later examples

def run():
    # create a window manager object using our class
    windowManager = myWindowManagerClass()

    # and launch the window manager
    windowManager.run()

if __name__ == '__main__':
    run()

#@-body
#@-node:0::@file examples/example2.py
#@-leo
