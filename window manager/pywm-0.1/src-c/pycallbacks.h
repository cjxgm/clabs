//@+leo
//@+node:0::@file src-c/pycallbacks.h
//@+body
//@@language c

//
// definitions of callbacks sent from the FLWM engine to
// the pywm.WM handlers.
//

#include "Python.h"

//extern "C" void initflwm(void);
extern PyObject *pyClient;

//extern "C" int  py_onSize(void *, int ,int ,int ,int ,int );

extern "C" int py_on_startup(PyObject *wm);
extern "C" int py_on_keyEvent(PyObject *wm, long key);
extern "C" int py_on_buttonPress(PyObject *wm, long button);
extern "C" int py_on_create(PyObject *wm, long win);
extern "C" int py_on_destroy(PyObject *wm, long win);

extern "C" int py_on_resize(PyObject *wm, long win,
                         int x, int y, int w, int h, int warp);

extern "C" int py_on_activate(PyObject *wm, long win);
extern "C" int py_on_deactivate(PyObject *wm, long win);


//@-body
//@-node:0::@file src-c/pycallbacks.h
//@-leo
