#!/usr/bin/env python
#@+leo
#@+node:0::@file examples/example1.py
#@+body
#@@first
#@@language python

"""
example1.y

Demonstrates an absolutely minimal window manager setup.
"""

import pywm

def run():
    windowManager = pywm.WM()
    windowManager.run()
    
if __name__ == '__main__':
    run()

#@-body
#@-node:0::@file examples/example1.py
#@-leo
