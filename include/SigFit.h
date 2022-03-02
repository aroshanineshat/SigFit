#ifndef __SIGFIT__
#define __SIGFIT__

#define PY_SSIZE_T_CLEAN
#include "SigFit.h"
#include <Python.h>

static PyObject * FitThreeParam(PyObject* self, PyObject* args);

#endif /* __SIGFIT__ */
