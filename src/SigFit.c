/***
    Date: March 1st 2022
    Author: Arash Roshanineshat

    This file wraps the C code with Python API
    The compiled module can be imported in Python
    Refer to the documentaion for more information
***/

#include <SineFit.h>
#include <SigFit.h>


static PyObject * FitThreeParam(PyObject* self, PyObject* args){

  PyObject *pList;
  PyObject *pItem;
  int Fs;
  int CW;

  Py_ssize_t n;
  int i;

  if (!PyArg_ParseTuple(args, "Oii", &pList, &CW, &Fs)) {
    /* Converting Python Objects to C Objects Failed */
    return NULL;
  }
  
  n = PyList_Size(pList); // Size of the array

  float *data_array;
  data_array = (float*)malloc(n*sizeof(float)); 

  if (!data_array){
    /* malloc has failed */
    PyErr_SetString(PyExc_MemoryError, "Error allocating memory.");
    return NULL;
  }

  /*
    Extracting the data from list
  */
  for (i=0; i<n; i++) {
    pItem = PyList_GetItem(pList, i);
    float c;
    if(PyFloat_Check(pItem)){
      c = (float)PyFloat_AsDouble(pItem); 
    }
    else if(PyLong_Check(pItem)) {
      c = (float)PyLong_AsDouble(pItem);
    } else {
        free (data_array); // Garbage Collection
        PyErr_SetString(PyExc_TypeError, "list items must be integers or floats.");
        return NULL;
    }
    data_array[i] = c;
  }

  Mat* result;
  result = ThreeFitData((int)n, CW, data_array, Fs);
  PyObject * output;
  output = Py_BuildValue ("(fff)", result->Data[0][0], result->Data[1][0], result->Data[2][0]); 
  
  /* Garbage Collection */
  cleanup(&result);
  free(data_array);

  return output;
};
