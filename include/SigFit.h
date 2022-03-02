#ifndef __SIGFIT__
#define __SIGFIT__

#define PY_SSIZE_T_CLEAN
#include "SineFit.h"
#include <python3.8/Python.h>

static PyObject * FitThreeParam(PyObject* self, PyObject* args);

static PyMethodDef SigFitMethods[] = {
    {"FitThreeParam",  FitThreeParam, METH_VARARGS,
     "Fits a sine wave to data with known freq."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef sigfitmodule = {
    PyModuleDef_HEAD_INIT,
    "SigFit",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    SigFitMethods
};

PyMODINIT_FUNC
PyInit_SigFit(void)
{
    return PyModule_Create(&sigfitmodule);
}

#endif /* __SIGFIT__ */
