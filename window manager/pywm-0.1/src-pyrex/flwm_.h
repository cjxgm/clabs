extern DL_IMPORT(PyObject) *(py_on_startup(PyObject *));
extern DL_IMPORT(int) (py_on_create(PyObject *,unsigned long ));
extern DL_IMPORT(PyObject) *(py_on_destroy(PyObject *,unsigned long ));
extern DL_IMPORT(PyObject) *(py_on_activate(PyObject *,unsigned long ));
extern DL_IMPORT(PyObject) *(py_on_deactivate(PyObject *,unsigned long ));
extern DL_IMPORT(int) (py_on_resize(PyObject *,unsigned long ,int ,int ,int ,int ,int ));
extern DL_IMPORT(int) (py_on_buttonPress(PyObject *,long ));
extern DL_IMPORT(int) (py_on_keyEvent(PyObject *,int ));
extern DL_IMPORT(PyObject) *(py_on_enter(PyObject *,unsigned long ));
extern DL_IMPORT(PyObject) *(py_on_leave(PyObject *,unsigned long ));
extern DL_IMPORT(void) initflwm_(void);
