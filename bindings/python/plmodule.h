#include <Python.h>
#include "plplot/plplot.h"	
#include "plplot/plplotP.h"
#include "Numeric/arrayobject.h"	

#if defined(PL_DOUBLE) || defined(DOUBLE)
#define PL_ARGS(a, b) (a)
#define PyArray_PLFLT PyArray_DOUBLE
#else
#define PL_ARGS(a, b) (b)
#define PyArray_PLFLT PyArray_FLOAT
#endif

#define TRY(E) if(! (E)) return NULL

int pl_PyArray_AsFloatArray (PyObject **, PLFLT **, PLINT *);
int pl_PyArray_AsIntArray (PyObject **, PLINT **, PLINT *);
int pl_PyArray_AsFloatMatrix (PyObject **, PLINT*, PLINT*, PLFLT ***);
int pl_PyList_AsStringArray (PyObject *, char ***, int *);
int pl_PyList_SetFromStringArray (PyObject *, char **, int);

extern char doc_plcont[];
PyObject *pl_cont( PyObject *self, PyObject *args );
extern char doc_plshade[];
PyObject *pl_shade( PyObject *self, PyObject *args );

PLFLT pyf2eval2( PLINT ix, PLINT iy, PLPointer plf2eval_data );
