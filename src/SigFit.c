#include "SigFit.h"

static PyObject * FitThreeParam(PyObject* self, PyObject* args){

  PyObject *pList;
  PyObject *pItem;
  PyObject *pFs;
  PyObject *pCW;

  Py_ssize_t n;
  int i;

  if (!PyArg_ParseTuple(args, "O!ii", &PyList_Type, &pList, &pCW, &pFs)) {
    return NULL;
  }
  
};
