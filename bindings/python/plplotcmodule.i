%module plplotc

/* 
Copyright 2002 Gary Bishop
This file is part of PLplot.

This file is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; version 2 of the License.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with the file; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/* 
A SWIG interface to PLplot for Python. This wrapper is different from
the one supplied in plmodule.c in that:

   1) it strictly provides the C-API with the usual change of not
      requiring lengths for arrays,

   2) it attempts to provide the entire API including callbacks for
      plcont and plshade.
      
   3) it works both with the single and double-precision versions of the
      PLplot library.

This is known to work with swig-1.3.11 and python-2.2.1 on Windows 2000 and
swig-1.3.11 and python-2.1.3 on Linux/Unix (only tested on Debian woody so
far).

*/

%include typemaps.i

%{
/* Change this to the recommended
#include <Numeric/arrayobject.h> 
 once we no longer support python1.5 */
#include <arrayobject.h>        
#include "plplot.h"      
#include "plplotP.h"

#ifdef PL_DOUBLE
#define  PyArray_PLFLT PyArray_DOUBLE
#else
#define  PyArray_PLFLT PyArray_FLOAT
#endif

#define  PyArray_PLINT PyArray_INT
/* python-1.5 compatibility mode? */
#define PySequence_Fast_GET_ITEM PySequence_GetItem
#define PySequence_Size PySequence_Length
%}

#ifdef PL_DOUBLE
typedef double PLFLT;
#else
typedef float PLFLT;
#endif

typedef int PLINT;

/* We have to get import_array called in our extension before we can use Numeric */
%init %{
  import_array();
  %}

/* I hate global variables but this is the best way I can think of to manage consistency
   checking among function arguments. */
%{
  static PLINT Alen = 0;
  static PLINT Xlen = 0, Ylen = 0;
  %}

/* The following typemaps take care of marshaling values into and out of PLplot functions. The
Array rules are trickly because of the need for length checking. These rules manage
some global variables (above) to handle consistency checking amoung parameters. 

Naming rules:
	Array 		(sets Alen to dim[0])
	ArrayCk 	(tests that dim[0] == Alen)
	ArrayX 		(sets Xlen to dim[0]
	ArrayCkX 	(tests dim[0] == Xlen)
	ArrayY 		(sets Ylen to dim[1])
	ArrayCkY 	(tests dim[1] == Ylen)
	Matrix 		(sets Xlen to dim[0], Ylen to dim[1])
	MatrixCk 	(test Xlen == dim[0] && Ylen == dim[1])
*/

/**********************************************************************************
			 PLINT arrays
**********************************************************************************/

/* With preceding count */
%typemap(in) (PLINT n, PLINT* Array) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)PyArray_ContiguousFromObject($input, PyArray_PLINT, 1, 1);
  if(tmp == NULL) return NULL;
  $1 = Alen = tmp->dimensions[0];
  $2 = (PLINT*)tmp->data;
}
%typemap(freearg) (PLINT n, PLINT* Array) {Py_DECREF(tmp$argnum);}

/* No count but check consistency with previous */
%typemap(in) PLINT* ArrayCk (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)PyArray_ContiguousFromObject($input, PyArray_PLINT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Alen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must be same length.");
    return NULL;
  }
  $1 = (PLINT*)tmp->data;
}
%typemap(freearg) PLINT* ArrayCk { Py_DECREF(tmp$argnum);}

/* Weird case to allow argument to be one shorter than others */
%typemap(in) PLINT* ArrayCkMinus1 (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)PyArray_ContiguousFromObject($input, PyArray_PLINT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] < Alen-1) {
    PyErr_SetString(PyExc_ValueError, "Vector must be at least length of others minus 1.");
    return NULL;
  }
  $1 = (PLINT*)tmp->data;
}
%typemap(freearg) PLINT* ArrayCkMinus1 { Py_DECREF(tmp$argnum);}

/* No length but remember size to check others */
%typemap(in) PLINT *Array (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)PyArray_ContiguousFromObject($input, PyArray_PLINT, 1, 1);
  if(tmp == NULL) return NULL;
  Alen = tmp->dimensions[0];
  $1 = (PLINT*)tmp->data;
}
%typemap(freearg) (PLINT* Array) {Py_DECREF(tmp$argnum);}

/* Trailing count */
%typemap(in) (PLINT *ArrayCk, PLINT n) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)PyArray_ContiguousFromObject($input, PyArray_PLINT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Alen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must be same length.");
    return NULL;
  }
  $2 = tmp->dimensions[0];
  $1 = (PLINT*)tmp->data;
}
%typemap(freearg) (PLINT* ArrayCk, int n) {Py_DECREF(tmp$argnum); }

/******************************************************************************
				 PLFLT Arrays 
******************************************************************************/

#ifndef PL_DOUBLE
%wrapper %{
/* some really twisted stuff to allow calling a single precision library from python */
PyArrayObject* myArray_ContiguousFromObject(PyObject* in, int type, int mindims, int maxdims)
{
  PyArrayObject* tmp = (PyArrayObject*)PyArray_ContiguousFromObject(in, PyArray_FLOAT,
								    mindims, maxdims);
  if (!tmp) {
    /* could be an incoming double array which can't be "safely" converted, do it anyway */
    if(PyArray_Check(in)) {
      PyErr_Clear();
      tmp = (PyArrayObject*)PyArray_Cast((PyArrayObject*)in, PyArray_FLOAT);
    }
  }
  return tmp;
}
 %}
#else
%wrapper %{
#define myArray_ContiguousFromObject PyArray_ContiguousFromObject
  %}
#endif

/* with preceding count */
%typemap(in) (PLINT n, PLFLT* Array) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  $1 = Alen = tmp->dimensions[0];
  $2 = (PLFLT*)tmp->data;
}
%typemap(freearg) (PLINT n, PLFLT* Array) { Py_DECREF(tmp$argnum);}

/* check consistency with previous */
%typemap(in) PLFLT* ArrayCk (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Alen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must be same length.");
    return NULL;
  }
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) PLFLT* ArrayCk { Py_DECREF(tmp$argnum);}

/* check consistency with X dimension of previous */
%typemap(in) PLFLT* ArrayCkX (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Xlen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must be same length.");
    return NULL;
  }
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) PLFLT* ArrayCkX { Py_DECREF(tmp$argnum);}

/* check consistency with Y dimension of previous */
%typemap(in) PLFLT* ArrayCkY (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Ylen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must be same length.");
    return NULL;
  }
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) PLFLT* ArrayCkY { Py_DECREF(tmp$argnum);}

/* set X length for later consistency checking */
%typemap(in) PLFLT *ArrayX (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  Xlen = tmp->dimensions[0];
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) PLFLT* ArrayX {Py_DECREF(tmp$argnum);}

/* set Y length for later consistency checking */
%typemap(in) PLFLT *ArrayY (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  Ylen = tmp->dimensions[0];
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) (PLFLT* ArrayY) {Py_DECREF(tmp$argnum);}

/* with trailing count */
%typemap(in) (PLFLT *Array, PLINT n) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 1, 1);
  if(tmp == NULL) return NULL;
  $2 = tmp->dimensions[0];
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) (PLFLT* Array, PLINT n) {Py_DECREF(tmp$argnum); }

/* 2D array with trailing dimensions, check consistency with previous */
%typemap(in) (PLFLT **MatrixCk, PLINT nx, PLINT ny) (PyArrayObject* tmp) {
  int i, size;
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  if(Xlen != tmp->dimensions[0] || Ylen != tmp->dimensions[1]) {
    PyErr_SetString(PyExc_ValueError, "Vectors must match matrix.");
    return NULL;
  }
  $2 = tmp->dimensions[0];
  $3 = tmp->dimensions[1];
  size = sizeof(PLFLT)*$3;
  $1 = (PLFLT**)malloc(sizeof(PLFLT*)*$2);
  for(i=0; i<$2; i++)
    $1[i] = (PLFLT*)(tmp->data + i*size);
}
%typemap(freearg) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
  Py_DECREF(tmp$argnum);
  free($1);
}

/* 2D array with trailing dimensions, set the X, Y size for later checking */
%typemap(in) (PLFLT **Matrix, PLINT nx, PLINT ny) (PyArrayObject* tmp) {
  int i, size;
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  Xlen = $2 = tmp->dimensions[0];
  Ylen = $3 = tmp->dimensions[1];
  size = sizeof(PLFLT)*$3;
  $1 = (PLFLT**)malloc(sizeof(PLFLT*)*$2);
  for(i=0; i<$2; i++)
    $1[i] = (PLFLT*)(tmp->data + i*size);
}
%typemap(freearg) (PLFLT **Matrix, PLINT nx, PLINT ny) {
  Py_DECREF(tmp$argnum);
  free($1);
}

/* for plshade1, note the difference in the type for the first arg */
%typemap(in) (PLFLT *Matrix, PLINT nx, PLINT ny) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  Xlen = $2 = tmp->dimensions[0];
  Ylen = $3 = tmp->dimensions[1];
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) (PLFLT *Matrix, PLINT nx, PLINT ny) {
  Py_DECREF(tmp$argnum);
}

/* 2D array, check for consistency */
%typemap(in) PLFLT **MatrixCk (PyArrayObject* tmp) {
  int i, size;
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Xlen || tmp->dimensions[1] != Ylen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must match matrix.");
    return NULL;
  }
  size = sizeof(PLFLT)*Ylen;
  $1 = (PLFLT**)malloc(sizeof(PLFLT*)*Xlen);
  for(i=0; i<Xlen; i++)
    $1[i] = (PLFLT*)(tmp->data + i*size);
}
%typemap(freearg) PLFLT **MatrixCk {
  Py_DECREF(tmp$argnum);
  free($1);
}

/***************************
	String returning functions
****************************/

/* This currently just used for plgdev, plgfnam, and plgver which apparently
 * have a limit of 80 bytes.  But to (hopefully) be safe for any future use
 * have a 1000 byte limit here. */
%typemap(in, numinputs=0) char* OUTPUT ( char buff[1000] ) {
  $1 = buff;
}
%typemap(argout) char* OUTPUT {
  $result = PyString_FromString($1);
}

/***************************
	A trick for docstrings
****************************/

%define DOC(func, string) 
%wrapper %{#define _doc_ ## func string %}
%enddef

/******************************************************************************
			Mostly plplot.h from here on down
******************************************************************************/

/* For passing user data, as with X's XtPointer */

typedef void* PLPointer;

/*--------------------------------------------------------------------------*\
 * Complex data types and other good stuff
\*--------------------------------------------------------------------------*/

/* Switches for escape function call. */
/* Some of these are obsolete but are retained in order to process
   old metafiles */

#define PLESC_SET_RGB		1	/* obsolete */
#define PLESC_ALLOC_NCOL	2	/* obsolete */
#define PLESC_SET_LPB		3	/* obsolete */
#define PLESC_EXPOSE		4	/* handle window expose */
#define PLESC_RESIZE		5	/* handle window resize */
#define PLESC_REDRAW		6	/* handle window redraw */
#define PLESC_TEXT		7	/* switch to text screen */
#define PLESC_GRAPH		8	/* switch to graphics screen */
#define PLESC_FILL		9	/* fill polygon */
#define PLESC_DI		10	/* handle DI command */
#define PLESC_FLUSH		11	/* flush output */
#define PLESC_EH		12      /* handle Window events */
#define PLESC_GETC		13	/* get cursor position */
#define PLESC_SWIN		14	/* set window parameters */
#define PLESC_PLFLTBUFFERING	15	/* configure PLFLT buffering */
#define PLESC_XORMOD		16	/* set xor mode */
#define PLESC_SET_COMPRESSION	17	/* AFR: set compression */
#define PLESC_CLEAR		18      /* RL: clear graphics region */
#define PLESC_DASH		19	/* RL: draw dashed line */
#define PLESC_HAS_TEXT		20	/* driver draws text */
#define PLESC_IMAGE		21	/* handle image */
#define PLESC_IMAGEOPS          22      /* plimage related operations */

/* image operations */
#if 0
#define ZEROW2B   1
#define ZEROW2D   2
#define ONEW2B    3
#define ONEW2D    4
#endif

/* definitions for the opt argument in plsurf3d() */

#define SURF_CONT 0x10 /* draw contour plot at surface */
#define BASE_CONT 0x20 /* draw contour plot at xy plane */
#define DRAW_SIDES 0x40 /* draw sides */
#define FACETED   0x80 /* draw outline for each square that makes up the surface */
#define MAG_COLOR 0x04 /* draw the mesh with a color dependent of the magnitude */

/* Window parameter tags */

#define PLSWIN_DEVICE		1	/* device coordinates */
#define PLSWIN_WORLD		2	/* world coordinates */

/* PLplot Option table & support constants */

/* Option-specific settings */

#define PL_OPT_ENABLED		0x0001	/* Obsolete */
#define PL_OPT_ARG		0x0002	/* Option has an argment */
#define PL_OPT_NODELETE		0x0004	/* Don't delete after processing */
#define PL_OPT_INVISIBLE	0x0008	/* Make invisible */
#define PL_OPT_DISABLED		0x0010	/* Processing is disabled */

/* Option-processing settings -- mutually exclusive */

#define PL_OPT_FUNC		0x0100	/* Call handler function */
#define PL_OPT_BOOL		0x0200	/* Set *var = 1 */
#define PL_OPT_INT		0x0400	/* Set *var = atoi(optarg) */
#define PL_OPT_FLOAT		0x0800	/* Set *var = atof(optarg) */
#define PL_OPT_STRING		0x1000	/* Set var = optarg */

/* Global mode settings */
/* These override per-option settings */

#define PL_PARSE_PARTIAL	0x0000	/* For backward compatibility */
#define PL_PARSE_FULL		0x0001	/* Process fully & exit if error */
#define PL_PARSE_QUIET		0x0002	/* Don't issue messages */
#define PL_PARSE_NODELETE	0x0004	/* Don't delete options after */
					/* processing */
#define PL_PARSE_SHOWALL	0x0008	/* Show invisible options */
#define PL_PARSE_OVERRIDE	0x0010	/* Obsolete */
#define PL_PARSE_NOPROGRAM	0x0020	/* Program name NOT in *argv[0].. */
#define PL_PARSE_NODASH		0x0040	/* Set if leading dash NOT required */
#define PL_PARSE_SKIP		0x0080	/* Skip over unrecognized args */

#define PL_MAXKEY 16

/* Structure for describing the plot window */

#define PL_MAXWINDOWS	64	/* Max number of windows/page tracked */

/* Macro used (in some cases) to ignore value of argument */
/* I don't plan on changing the value so you can hard-code it */

#define PL_NOTSET (-42)


#define PLESPLFLTBUFFERING_ENABLE     1
#define PLESPLFLTBUFFERING_DISABLE    2
#define PLESPLFLTBUFFERING_QUERY      3

/* All code associated with special call-back functions. */
/* Identity transformation. */

%typemap(in, numinputs=0) PLPointer IGNORE {
  $1 = NULL;
}
void
pltr0(PLFLT x, PLFLT y, PLFLT *OUTPUT, PLFLT *OUTPUT, PLPointer IGNORE);

/* This typemap takes care of converting a Python 2-tuple of 1D Arrays to the
   PLcGrid structure that pltr1 expects */
%wrapper %{
  PyArrayObject *pltr_xg, *pltr_yg;
  static PLcGrid tmpGrid1;
  static PLcGrid2 tmpGrid2;

  PLcGrid* marshal_PLcGrid1(PyObject* input) {
    /* fprintf(stderr, "marshal PLcGrid1\n"); */
    if(!PySequence_Check(input) || PySequence_Size(input) != 2) {
      PyErr_SetString(PyExc_ValueError, "Expected a sequence of two arrays.");
      return NULL;
    }
    pltr_xg = (PyArrayObject*)myArray_ContiguousFromObject(PySequence_Fast_GET_ITEM(input, 0),
							   PyArray_PLFLT, 1, 1);
    pltr_yg = (PyArrayObject*)myArray_ContiguousFromObject(PySequence_Fast_GET_ITEM(input, 1), 
							   PyArray_PLFLT, 1, 1);
    if(pltr_xg == 0 || pltr_yg == 0) {
      PyErr_SetString(PyExc_ValueError, "Expected a sequence to two 1D arrays.");
      return NULL;
    }
    tmpGrid1.nx = pltr_xg->dimensions[0];
    tmpGrid1.ny = pltr_yg->dimensions[0];
    if(Xlen != tmpGrid1.nx || Ylen != tmpGrid1.ny) {
      PyErr_SetString(PyExc_ValueError, "pltr arguments must have X and Y dimensions of first arg.");
      return NULL;
    }
    tmpGrid1.xg = (PLFLT*)pltr_xg->data;
    tmpGrid1.yg = (PLFLT*)pltr_yg->data;
    return &tmpGrid1;
  }

  void cleanup_PLcGrid1(void) {
    /* fprintf(stderr, "cleanup PLcGrid1\n"); */
    Py_DECREF(pltr_xg);
    Py_DECREF(pltr_yg);
  }

  PLcGrid2* marshal_PLcGrid2(PyObject* input) {
    int i, size;
    /* fprintf(stderr, "marshal PLcGrid2\n"); */
    if(!PySequence_Check(input) || PySequence_Size(input) != 2) {
      PyErr_SetString(PyExc_ValueError, "Expected a sequence of two arrays.");
      return NULL;
    }
    pltr_xg = (PyArrayObject*)myArray_ContiguousFromObject(PySequence_Fast_GET_ITEM(input, 0),
							   PyArray_PLFLT, 2, 2);
    pltr_yg = (PyArrayObject*)myArray_ContiguousFromObject(PySequence_Fast_GET_ITEM(input, 1), 
							   PyArray_PLFLT, 2, 2);
    if(pltr_xg == 0 || pltr_yg == 0) {
      PyErr_SetString(PyExc_ValueError, "Expected a sequence of two 2D arrays.");
      return NULL;
    }
    if(pltr_xg->dimensions[0] != pltr_yg->dimensions[0] ||
       pltr_xg->dimensions[1] != pltr_yg->dimensions[1]) {
      PyErr_SetString(PyExc_ValueError, "Arrays must be same size.");
      return NULL;
    }
    tmpGrid2.nx = pltr_xg->dimensions[0];
    tmpGrid2.ny = pltr_xg->dimensions[1];
    if(Xlen != tmpGrid2.nx || Ylen != tmpGrid2.ny) {
      PyErr_SetString(PyExc_ValueError, "pltr arguments must have X and Y dimensions of first arg.");
      return NULL;
    }
    size = sizeof(PLFLT)*tmpGrid2.ny;
    tmpGrid2.xg = (PLFLT**)malloc(sizeof(PLFLT*)*tmpGrid2.nx);
    for(i=0; i<tmpGrid2.nx; i++)
      tmpGrid2.xg[i] = (PLFLT*)(pltr_xg->data + i*size);
    tmpGrid2.yg = (PLFLT**)malloc(sizeof(PLFLT*)*tmpGrid2.nx);
    for(i=0; i<tmpGrid2.nx; i++)
      tmpGrid2.yg[i] = (PLFLT*)(pltr_yg->data + i*size);
    return &tmpGrid2;
  }

  void cleanup_PLcGrid2(void) {
    /* fprintf(stderr, "cleanup PLcGrid2\n"); */
    free(tmpGrid2.xg);
    free(tmpGrid2.yg);
    Py_DECREF(pltr_xg);
    Py_DECREF(pltr_yg);
  }
  %}

%typemap(in) PLcGrid* cgrid {
  $1 = marshal_PLcGrid1($input);
  if(!$1)
    return NULL;
}
%typemap(freearg) PLcGrid* cgrid {
  cleanup_PLcGrid1();
}

/* Does linear interpolation from singly dimensioned coord arrays. */
void
pltr1(PLFLT x, PLFLT y, PLFLT *OUTPUT, PLFLT *OUTPUT, PLcGrid* cgrid);

/* This typemap marshals a Python 2-tuple of 2D arrays into the PLcGrid2
   structure that pltr2 expects */

%typemap(in) PLcGrid2* cgrid {
  $1 = marshal_PLcGrid2($input);
  if(!$1)
    return NULL;
}
%typemap(freearg) PLcGrid2* cgrid {
  cleanup_PLcGrid2();
}

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (column dominant, as per normal C 2d arrays). */
void
pltr2(PLFLT x, PLFLT y, PLFLT *OUTPUT, PLFLT *OUTPUT, PLcGrid2* cgrid);

typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);

%{
typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);
  %}

#if 0
/* We need to get this PyThreadState structure initialized to use it for controlling
   threads on the callback. Probably not *really* necessary since I'm not allowing
   threads through the call */

%init %{
  save_interp = PyThreadState_Get()->interp;
  %}

#endif

%wrapper %{

  /* helper code for handling the callback */
#if 0
 static PyInterpreterState *save_interp = NULL;
#endif
 enum callback_type { CB_0, CB_1, CB_2, CB_Python } pltr_type;
 PyObject* python_pltr = NULL;
 PyObject* python_f2eval = NULL;

#if 0
#define MY_BLOCK_THREADS { \
	PyThreadState *prev_state, *new_state; \
	/* need to have started a thread at some stage */ \
	/* for the following to work */ \
	PyEval_AcquireLock(); \
	new_state = PyThreadState_New(save_interp); \
	prev_state = PyThreadState_Swap(new_state);
#define MY_UNBLOCK_THREADS \
	new_state = PyThreadState_Swap(prev_state); \
	PyThreadState_Clear(new_state); \
	PyEval_ReleaseLock(); \
	PyThreadState_Delete(new_state); \
}
#else
#define MY_BLOCK_THREADS
#define MY_UNBLOCK_THREADS
#endif

/* This is the callback that gets handed to the C code. It, in turn, calls the Python callback */

  void do_pltr_callback(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer data)
    {
      PyObject *pdata, *arglist, *result;
      PyArrayObject *tmp;

      /* the data argument is acutally a pointer to a python object */
      pdata = (PyObject*)data;
      if(data == NULL) {
	pdata = Py_None;
      }
      if(python_pltr) { /* if not something is terribly wrong */
	/* hold a reference to the data object */
	Py_XINCREF(pdata);
	/* grab the Global Interpreter Lock to be sure threads don't mess us up */
	MY_BLOCK_THREADS
	/* build the argument list */
#ifdef PL_DOUBLE
	arglist = Py_BuildValue("(ddO)", x, y, pdata);
#else
	arglist = Py_BuildValue("(ffO)", x, y, pdata);
#endif
	if(arglist == NULL) {
	  fprintf(stderr, "Py_BuildValue failed to make argument list.\n");
	  *tx = *ty = 0;
	  return;
	}
	/* call the python function */
	result = PyEval_CallObject(python_pltr, arglist);
	/* release the argument list */
	Py_XDECREF(arglist);
	/* check and unpack the result */
	if(result == NULL) {
	  fprintf(stderr, "call to python pltr function with 3 arguments failed\n");
	  PyErr_SetString(PyExc_RuntimeError, "pltr callback must take 3 argments.");
	  *tx = *ty = 0;
	} else {
	  tmp = (PyArrayObject *)myArray_ContiguousFromObject(result, PyArray_PLFLT, 1, 1);
	  if(tmp == 0 || tmp->dimensions[0] != 2) {
	    fprintf(stderr, "pltr callback must return a 2 element array or sequence\n");
	    PyErr_SetString(PyExc_RuntimeError, "pltr callback must return a 2-sequence.");
	    *tx = *ty = 0;
	  } else {
	    PLFLT* t = (PLFLT*)tmp->data;
	    *tx = t[0];
	    *ty = t[1];
	    Py_XDECREF(tmp);
	  }
	}
	/* release the result */
	Py_XDECREF(result);
	/* release the global interpreter lock */
	MY_UNBLOCK_THREADS
      }	
    }

  PLFLT do_f2eval_callback(PLINT x, PLINT y, PLPointer data)
    {
      PyObject *pdata, *arglist, *result;
      PLFLT fresult = 0.0;

      /* the data argument is acutally a pointer to a python object */
      pdata = (PyObject*)data;
      if(python_f2eval) { /* if not something is terribly wrong */
	/* hold a reference to the data object */
	Py_XINCREF(pdata);
	/* grab the Global Interpreter Lock to be sure threads don't mess us up */
	MY_BLOCK_THREADS
	/* build the argument list */
	arglist = Py_BuildValue("(iiO)", x, y, pdata);
	/* call the python function */
	result = PyEval_CallObject(python_pltr, arglist);
	/* release the argument list */
	Py_DECREF(arglist);
	/* check and unpack the result */
	if(!PyFloat_Check(result)) {
	  fprintf(stderr, "f2eval callback must return a float\n");
	  PyErr_SetString(PyExc_RuntimeError, "f2eval callback must return a float.");
	} else {
	  /* should I test the type here? */
	  fresult = (PLFLT)PyFloat_AsDouble(result);
	}
	/* release the result */
	Py_XDECREF(result);
	/* release the global interpreter lock */
	MY_UNBLOCK_THREADS
      }	
      return fresult;
    }

/* marshal the pltr function pointer argument */
  pltr_func marshal_pltr(PyObject* input) {
    pltr_func result = do_pltr_callback;
    PyObject* rep = PyObject_Repr(input);
    if(rep) {
      char* str = PyString_AsString(rep);
      if(strcmp(str, "<built-in function pltr0>") == 0) {
	result = pltr0;
	pltr_type = CB_0;
        python_pltr = NULL;
      } else if(strcmp(str, "<built-in function pltr1>") == 0) {
	result = pltr1;
	pltr_type = CB_1;
        python_pltr = NULL;
      } else if(strcmp(str, "<built-in function pltr2>") == 0) {
	result = pltr2;
	pltr_type = CB_2;
        python_pltr = NULL;
      } else {
	python_pltr = input;
	pltr_type = CB_Python;
	Py_XINCREF(input);
      }
      Py_DECREF(rep);
    } else {
      python_pltr = input;
      pltr_type = CB_Python;
      Py_XINCREF(input);
    }
    return result;
  }

  void cleanup_pltr(void) {
    Py_XDECREF(python_pltr);
    python_pltr = 0;
  }

  PLPointer marshal_PLPointer(PyObject* input) {
    PLPointer result = NULL;
    switch(pltr_type) {
    case CB_0:
      break;
    case CB_1:
      if(input != Py_None)
	result = marshal_PLcGrid1(input);
      break;
    case CB_2:
      if(input != Py_None)
	result = marshal_PLcGrid2(input);
      break;
    case CB_Python:
      Py_XINCREF(input);
      result = (PLPointer*)input;
      break;
    default:
      fprintf(stderr, "pltr_type is invalid\n");
    }
    return result;
  }

  void cleanup_PLPointer(void) {
    switch(pltr_type) {
    case CB_0:
      break;
    case CB_1:
      cleanup_PLcGrid1();
      break;
    case CB_2:
      cleanup_PLcGrid2();
      break;
    case CB_Python:
      Py_XDECREF(python_pltr);
      break;
    default:
      fprintf(stderr, "pltr_type is invalid\n");
    }
    python_pltr = 0;
    pltr_type = CB_0;
  }
      
      
  %}
     
%typemap(in) pltr_func pltr {
  /* it must be a callable */
  if(!PyCallable_Check((PyObject*)$input)) {
    PyErr_SetString(PyExc_ValueError, "pltr argument must be callable");
    return NULL;
  }
  $1 = marshal_pltr($input);
}
%typemap(freearg) pltr_func pltr {
  cleanup_pltr();
}

/* you can omit the pltr func */
%typemap(default) pltr_func pltr {
  python_pltr = 0;
  $1 = NULL;
}

/* convert an arbitrary Python object into the void* pointer they want */
%typemap(in) PLPointer PYOBJECT_DATA {
  if($input == Py_None)
    $1 = NULL;
  else {
    $1 = marshal_PLPointer($input);
  }
}
%typemap(freearg) PLPointer PYOBJECT_DATA {
  cleanup_PLPointer();
}

/* you can omit the data too */
%typemap(default) PLPointer PYOBJECT_DATA {
  $1 = NULL;
}

/* marshall the f2eval function pointer argument */
%typemap(in) f2eval_func f2eval {
  /* it must be a callable */
  if(!PyCallable_Check((PyObject*)$input)) {
    PyErr_SetString(PyExc_ValueError, "pltr argument must be callable");
    return NULL;
  }
  /* hold a reference to it */
  Py_XINCREF((PyObject*)$input);
  python_f2eval = (PyObject*)$input;
  /* this function handles calling the python function */
  $1 = do_f2eval_callback;
}
%typemap(freearg) f2eval_func f2eval {
  Py_XDECREF(python_f2eval);
  python_f2eval = 0;
}
%typemap(in, numinputs=0) defined_func df {
  $1 = NULL;
}
%typemap(in, numinputs=0) fill_func ff {
  $1 = plfill;
}
/* this typemap takes a sequence of strings and converts them for plstripc 
   also checks that previous Arrays were of length 4 
*/
%typemap(in) char *legline[4] {
  int i;
  if(!PySequence_Check($input) || PySequence_Size($input) != 4) {
    PyErr_SetString(PyExc_ValueError, "Requires a sequence of 4 strings.");
    return NULL;
  }
  if(Alen != 4) {
    PyErr_SetString(PyExc_ValueError, "colline and styline args must be length 4.");
    return NULL;
  }
  $1 = malloc(sizeof(char*)*4);
  for(i=0; i<4; i++) {
    $1[i] = PyString_AsString(PySequence_Fast_GET_ITEM($input, i));
    if($1[i] == NULL) {
      free($1);
      return NULL;
    }
  }
}
%typemap(freearg) char *legline[4] {
  free($1);
}

/* End of all code associated with special call-back functions.*/


/* Non-common API that are included here because they traditionally
 * were part of plmodule.c. */

DOC(plarrows, "Plot an arrow.")
void
plarrows(PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
         PLFLT scale, PLFLT dx, PLFLT dy) ;

DOC(plsxwin, "Set inferior X window.")
void
plsxwin(PLINT window_id);

/* Complete list of common API (has "c_" suffix version defined in plplot.h) */

DOC(pl_setcontlabelformat, "Set the format of the contour labels.")
void
pl_setcontlabelformat(PLINT lexp, PLINT sigdig);

DOC(pl_setcontlabelparam, "Set offset and spacing of contour labels.")
void
pl_setcontlabelparam(PLFLT offset, PLFLT size, PLFLT spacing, PLINT active);

DOC(pladv, "Advance to subpage \"page\", or to the next one if \"page\" = 0.")
void
pladv(PLINT page);

DOC(plaxes,"This functions similarly to plbox() except that the origin of the axes is placed at the user-specified point (x0, y0).")
void
plaxes(PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub);

DOC(plbin,"Plot a histogram using x to store data values and y to store frequencies.")
void
plbin(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT center);

DOC(plbop, "Start new page.  Should only be used with pleop().")
void
plbop(void);

DOC(plbox, "Draw a box around the current viewport.")
void
plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub);

DOC(plbox3, "This is the 3-d analogue of plbox().")
void
plbox3(const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
	 const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
	 const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz);

DOC(plcalc_world, "Calculate world coordinates and subpage from relative device coordinates.")
void
plcalc_world(PLFLT rx, PLFLT ry, PLFLT *OUTPUT, PLFLT *OUTPUT, PLINT *OUTPUT);

DOC(plclear, "Clear current subpage.")
void
plclear(void);

DOC(plcol0, "Set color, map 0.  Argument is integer between 0 and 15.")
void
plcol0(PLINT icol0);

DOC(plcol1, "Set color, map 1.  Argument is a float between 0. and 1.")
void
plcol1(PLFLT col1);

DOC(plcont, "Draw a contour plot.")
void
plcont(PLFLT **Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *Array, PLINT n,
	 pltr_func pltr,
	 PLPointer PYOBJECT_DATA);

DOC(plcpstrm, "Copy state parameters from the reference stream to the current stream.")
void
plcpstrm(PLINT iplsr, PLINT flags);

DOC(plend, "End a plotting session for all open streams.")
void
plend(void);

DOC(plend1, "End a plotting session for the current stream only.")
void
plend1(void);

DOC(plenv, "Simple interface for defining viewport and window.")
void
plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis);

DOC(pleop, "End current page.  Should only be used with plbop().")
void
pleop(void);

DOC(plerrx, "Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)).")
void
plerrx(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

DOC(plerry, "Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)).")
void
plerry(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

DOC(plfamadv, "Advance to the next family file on the next new page.")
void
plfamadv(void);

DOC(plfill, "Pattern fills the polygon bounded by the input points.")
void
plfill(PLINT n, PLFLT *Array, PLFLT *ArrayCk);

DOC(plfill3, "Pattern fills the 3d polygon bounded by the input points.")
void
plfill3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

DOC(plflush, "Flush the output stream.  Use sparingly, if at all.")
void
plflush(void);

DOC(plfont, "Set the global font flag to 'ifont'.")
void
plfont(PLINT ifont);

DOC(plfontld, "Load specified font set.")
void
plfontld(PLINT fnt);

DOC(plgchr, "Get character default height and current (scaled) height.")
void 
plgchr(PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgcol0, "Get 8 bit RGB values for given color from color map 0.")
void
plgcol0(PLINT icol0, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgcolbg, "Get 8-bit RGB background color.")
void
plgcolbg(PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgcompression, "Get the current compression setting.")
void
plgcompression(PLINT *OUTPUT);

DOC(plgdev, "Get the current device (keyword) name.")
void
plgdev(char *OUTPUT);

DOC(plgdidev, "Retrieve current window into device space.")
void
plgdidev(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgdiori, "Get plot orientation.")
void
plgdiori(PLFLT *OUTPUT);

DOC(plgdiplt, "Retrieve current window into plot space.")
void
plgdiplt(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgfam, "Get family file parameters.")
void
plgfam(PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgfnam, "Get the (current) output file name.")
void
plgfnam(char *OUTPUT);

DOC(plglevel, "Get the (current) run level.")
void
plglevel(PLINT *OUTPUT);

DOC(plgpage, "Get output device parameters.")
void
plgpage(PLFLT *OUTPUT, PLFLT *OUTPUT,
	  PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgra, "Switch to graphics screen.")
void
plgra(void);

DOC(plgspa, "Get subpage boundaries in absolute coordinates.")
void
plgspa(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgstrm, "Get current stream number.")
void
plgstrm(PLINT *OUTPUT);

DOC(plgver, "Get current library version number.")
void
plgver(char *OUTPUT);

DOC(plgvpd, "Get viewport boundaries in normalized device coordinates.")
void
plgvpd(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgvpw, "Get viewport boundaries in world coordinates.")
void
plgvpw(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

DOC(plgxax, "Get x axis labeling parameters.")
void
plgxax(PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgyax, "Get y axis labeling parameters.")
void
plgyax(PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plgzax, "Get z axis labeling parameters.")
void
plgzax(PLINT *OUTPUT, PLINT *OUTPUT);

DOC(plhist, "Draw histogram.")
void
plhist(PLINT n, PLFLT *Array, PLFLT datmin, PLFLT datmax,
	 PLINT nbin, PLINT oldwin);

DOC(plhls, "Set current color (map 0) by hue, lightness, and saturation.")
void
plhls(PLFLT h, PLFLT l, PLFLT s);

DOC(plinit, "Initialize PLplot, using preset or default options.")
void
plinit(void);

DOC(pljoin, "Draw a line segment from (x1, y1) to (x2, y2).")
void
pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);

DOC(pllab, "Label graphs.")
void
pllab(const char *xlabel, const char *ylabel, const char *tlabel);

DOC(pllightsource, "Set position of the light source.")
void
pllightsource(PLFLT x, PLFLT y, PLFLT z);

DOC(plline, "Draw line segments connecting a series of points.")
void
plline(PLINT n, PLFLT *Array, PLFLT *ArrayCk);

DOC(plline3, "Draw a line in 3 space.")
void
plline3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

DOC(pllsty, "Set line style.")
void
pllsty(PLINT lin);

DOC(plmesh, "Plot a 3D mesh representation of z[x][y].")
void
plmesh(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk, PLINT nx, PLINT ny, PLINT opt);

DOC(plmkstrm, "Create a new stream and makes it the default.")
void
plmkstrm(PLINT *OUTPUT);

DOC(plmtex, "Print \"text\" at specified position relative to viewport.")
void
plmtex(const char *side, PLFLT disp, PLFLT pos, PLFLT just,
	 const char *text);

DOC(plot3d, "Plot a 3-d representation of the function z[x][y].")
void
plot3d(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side);

DOC(plsurf3d, "Plot a 3-d surface representation of the function z[x][y].")
void
plsurf3d(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n);

DOC(plpat, "Set fill pattern directly.")
void
plpat(PLINT n, PLINT *Array, PLINT *ArrayCk);

DOC(plpoin, "Plot array y against x for n points using ASCII code \"code\".")
void
plpoin(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code);

DOC(plpoin3, "Draw a series of points in 3 space.")
void
plpoin3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT code);

DOC(plpoly3, "Draw a polygon in 3 space. ")
void
plpoly3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT *ArrayCkMinus1,
	    PLINT flag);

DOC(plprec, "Set the floating point precision (in number of places) in numeric labels.")
void
plprec(PLINT setp, PLINT prec);

DOC(plpsty, "Set fill pattern, using one of the predefined patterns.")
void
plpsty(PLINT patt);

DOC(plptex, "Print \"text\" at world cooordinate (x,y).")
void
plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text);

DOC(plreplot, "Replay contents of plot buffer to current device/file.")
void
plreplot(void);

DOC(plschr, "Set character height.")
void
plschr(PLFLT def, PLFLT scale);

DOC(plscmap0, "Set color map 0 colors by 8 bit RGB values.")
void
plscmap0(PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n);

DOC(plscmap0n, "Set number of colors in cmap 0.")
void
plscmap0n(PLINT ncol0);

DOC(plscmap1, "Set color map 1 colors by 8 bit RGB values.")
void
plscmap1(PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n);

DOC(plscmap1l, "Set color map 1 colors using a piece-wise linear relationship between intensity [0,1] (cmap 1 index) and position in HLS or RGB color space.")
void
plscmap1l(PLINT itype, PLINT n, PLFLT *Array,
	    PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT *ArrayCkMinus1);

DOC(plscmap1n, "Set number of colors in cmap 1.")
void
plscmap1n(PLINT ncol1);

DOC(plscol0, "Set 8-bit RGB value in cmap 0.")
void
plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b);

DOC(plscolbg, "Set the background color using 8-bit RGB value.")
void
plscolbg(PLINT r, PLINT g, PLINT b);

DOC(plscolor, "Globally turn color output on/off.")
void
plscolor(PLINT color);

DOC(plscompression, "Set the compression level.")
void
plscompression(PLINT compression);

DOC(plsdev, "Set the device (keyword) name.")
void
plsdev(const char *devname);

DOC(plsdidev, "Set window into device space using margin, aspect ratio, and justification.")
void
plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

DOC(plsdimap, "Set up transformation from metafile coordinates.")
void
plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
	   PLFLT dimxpmm, PLFLT dimypmm);

DOC(plsdiori, "Set plot orientation, specifying rotation in units of pi/2.")
void
plsdiori(PLFLT rot);

DOC(plsdiplt, "Set window into plot space.")
void
plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

DOC(plsdiplz, "Set window into plot space incrementally (zoom).")
void
plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

DOC(plsesc, "Set the escape character for text strings.")
void
plsesc(char esc);

DOC(plsetopt, "Process input strings, treating them as an option and argument pair. The first is for the external API, the second the work routine declared here for backward compatibilty.")
PLINT
plsetopt(char *opt, char *optarg);

DOC(plsfam, "Set family file parameters.")
void
plsfam(PLINT fam, PLINT num, PLINT bmax);

DOC(plsfnam, "Set the output file name.")
void
plsfnam(const char *fnam);

DOC(plshades, "Shade regions with continuous range of colours.")
void 
plshades( PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
	  PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	  PLFLT *Array, PLINT n, PLINT fill_width,
	  PLINT cont_color, PLINT cont_width,
	  fill_func ff, PLINT rectangular,
	  pltr_func pltr,
	  PLPointer PYOBJECT_DATA);

DOC(plshade, "Shade region with discrete colour, pattern fill.")
void 
plshade(PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
	  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	  PLFLT shade_min, PLFLT shade_max,
	  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	  PLINT min_color, PLINT min_width,
	  PLINT max_color, PLINT max_width,
	  fill_func ff, PLINT rectangular,
	  pltr_func pltr,
	  PLPointer PYOBJECT_DATA);

DOC(plsmaj, "Set up lengths of major tick marks.")
void
plsmaj(PLFLT def, PLFLT scale);

DOC(plsmin, "Set up lengths of minor tick marks.")
void
plsmin(PLFLT def, PLFLT scale);

DOC(plsori, "Set orientation.  Must be done before calling plinit.")
void
plsori(PLINT ori);

DOC(plspage, "Set output device parameters.  Usually ignored by the driver.")
void
plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
	  PLINT xoff, PLINT yoff);

DOC(plspause, "Set the pause (on end-of-page) status.")
void
plspause(PLINT pause);

DOC(plsstrm, "Set stream number.")
void
plsstrm(PLINT strm);

DOC(plssub, "Set the number of subwindows in x and y.")
void
plssub(PLINT nx, PLINT ny);

DOC(plssym, "Set symbol height.")
void
plssym(PLFLT def, PLFLT scale);

DOC(plstar, "Initialize PLplot, passing in the windows/page settings.")
void
plstar(PLINT nx, PLINT ny);

DOC(plstart, "Initialize PLplot, passing the device name and windows/page settings.")
void
plstart(const char *devname, PLINT nx, PLINT ny);

DOC(plstripa, "Add a point to a stripchart. ")
void
plstripa(PLINT id, PLINT pen, PLFLT x, PLFLT y);

DOC(plstripc, "Create 1d stripchart.")
void
plstripc(PLINT *OUTPUT, char *xspec, char *yspec,
	PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
	PLFLT xlpos, PLFLT ylpos,
	PLINT y_ascl, PLINT acc,
	PLINT colbox, PLINT collab,
	PLINT *Array, PLINT *ArrayCk, char *legline[4],
	char *labx, char *laby, char *labtop);

DOC(plstripd, "Deletes and releases memory used by a stripchart. ")
void
plstripd(PLINT id);

DOC(plstyl, "Set up a new line style.")
void
plstyl(PLINT n, PLINT *Array, PLINT *ArrayCk);

DOC(plsvpa, "Set the edges of the viewport to the specified absolute coordinates.")
void
plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

DOC(plsxax, "Set x axis labeling parameters.")
void
plsxax(PLINT digmax, PLINT digits);

DOC(plsyax, "Set y axis labeling parameters.")
void
plsyax(PLINT digmax, PLINT digits);

DOC(plsym, "Plot array y against x for n points using Hershey symbol \"code\"")
void
plsym(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code);

DOC(plszax, "Set z axis labeling parameters")
void
plszax(PLINT digmax, PLINT digits);

DOC(pltext, "Switch to text screen.")
void
pltext(void);

DOC(plvasp, "Sets the edges of the viewport with the given aspect ratio, leaving room for labels.")
void
plvasp(PLFLT aspect);

DOC(plvpas, "Create the largest viewport of the specified aspect ratio that fits within the specified normalized subpage coordinates.")
void
plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);

DOC(plvpor, "Create a viewport with the specified normalized subpage coordinates.")
void
plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

DOC(plvsta, "Define a \"standard\" viewport with seven character heights for the left margin and four character heights everywhere else.")
void
plvsta(void);

DOC(plw3d, "Set up a window for three-dimensional plotting.")
void
plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
	PLFLT zmax0, PLFLT alt, PLFLT az);

DOC(plwid, "Set pen width.")
void
plwid(PLINT width);

DOC(plwind, "Set up world coordinates of the viewport boundaries (2d plots).")
void
plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

DOC(plxormod, "Set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device.")
void
plxormod(PLINT mode, PLINT *OUTPUT);

# if 0

/* Deprecated functions that are in common API, but we don't want to
 * propagate them to the python API. */

DOC(plrgb, "Set line color by red, green, blue values in range from  0. to 1.")
void
plrgb(PLFLT r, PLFLT g, PLFLT b);

DOC(plrgb1, "Set line color by 8 bit RGB values.")
void
plrgb1(PLINT r, PLINT g, PLINT b);

void 
plshade1(PLFLT *Matrix, PLINT nx, PLINT ny, defined_func df,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 fill_func ff, PLINT rectangular,
	 pltr_func pltr,
	 PLPointer PYOBJECT_DATA);

#endif

/*--------------------------------------------------------------------------*\
 *		Functions for use from C or C++ only
 *  N.B. If you want these in python, they should be officially put in 
 *  the common API for all front-ends to the PLplot library with "c_" suffix,
 *  DocBook xml documentation in the api.xml chapter, etc. 
\*--------------------------------------------------------------------------*/

#if 0

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used. 
 */
void
plfcont(f2eval_func f2eval,
	PLPointer PYOBJECT_DATA,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	pltr_func pltr,
	PLPointer PYOBJECT_DATA);
/* plot continental outline in world coordinates */

void
plmap( void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
         PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );

/* Plot the latitudes and longitudes on the background. */

void 
plmeridians( void (*mapform)(PLINT, PLFLT *, PLFLT *), 
               PLFLT dlong, PLFLT dlat,
               PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );

void 
plfshade(f2eval_func,
	 PLPointer PYOBJECT_DATA,
	 c2eval_func,
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny, 
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 fill_func, PLINT rectangular,
	 pltr_func,
	 PLPointer PYOBJECT_DATA);

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

void
pldid2pc(PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT);

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

void
pldip2dc(PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT);

/* plots a 2d image (or a matrix too large for plshade() ). */

void
plimage( PLFLT **Matrix, PLINT nx, PLINT ny, 
	 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
	 PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

/* Returns a list of file-oriented device names and their menu strings */
void
plgFileDevs(char ***p_menustr, char ***p_devname, PLINT *p_ndev);

/* Returns a list of all device names and their menu strings */

void
plgDevs(char ***p_menustr, char ***p_devname, PLINT *p_ndev);

/* Set the function pointer for the keyboard event handler */

void
plsKeyEH(void (*KeyEH) (PLGraphicsIn *, void *, PLINT *), void *KeyEH_data);

/* Set the function pointer for the (mouse) button event handler */

void
plsButtonEH(void (*ButtonEH) (PLGraphicsIn *, void *, PLINT *),
	    void *ButtonEH_data);
#endif
/* Set the variables to be used for storing error info */

#if 0
void
plsError(PLINT *errcode, char *errmsg);
#endif

/* Sets an optional user exit handler. */
#if 0
void
plsexit(PLINT (*handler) (char *));
	/* Transformation routines */
#endif

#if 0
/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */

void
pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);
/* Identity transformation for plots from Fortran. */

void
pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (row dominant, i.e. Fortran ordering). */

void
pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Example linear transformation function for contour plotter. */

void 
xform(PLFLT x, PLFLT y, PLFLT * OUTPUT, PLFLT * OUTPUT);
	/* Function evaluators */
/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */

PLFLT
plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);
#endif
	/* Command line parsing utilities */

/* Clear internal option table info structure. */

void
plClearOpts(void);

/* Reset internal option table info structure. */

void
plResetOpts(void);

/* Merge user option table into internal info structure. */
#if 0
PLINT
plMergeOpts(PLOptionTable *options, char *name, char **notes);
#endif
/* Set the strings used in usage and syntax messages. */

void
plSetUsage(char *program_string, char *usage_string);

#if 0
/* This is wrapped by common API plsetopt so ignore. */
PLINT
plSetOpt(char *opt, char *optarg);
#endif

/* Process options list using current options info. */
%typemap(in) (PLINT *p_argc, char **argv) (PLINT tmp) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  tmp = PyList_Size($input);
  $1 = &tmp;
  $2 = (char **) malloc((tmp+1)*sizeof(char *));
  for (i = 0; i < tmp; i++) {
    PyObject *s = PyList_GetItem($input,i);
    if (!PyString_Check(s)) {
        free($2);
        PyErr_SetString(PyExc_ValueError, "List items must be strings");
        return NULL;
    }
    $2[i] = PyString_AsString(s);
  }
  $2[i] = 0;
}

%typemap(freearg) (PLINT *p_argc, char **argv) {
   if ($2) free($2);
}


PLINT
plParseOpts(PLINT *p_argc, char **argv, PLINT mode);

/* Print usage & syntax message. */

void
plOptUsage(void);

	/* Miscellaneous */
#if 0
/* Set the output file pointer */

void
plgfile(FILE **p_file);

/* Get the output file pointer */

void
plsfile(FILE *file);

/* Get the escape character for text strings. */

void
plgesc(char *p_esc);

/* Front-end to driver escape function. */

void
pl_cmd(PLINT op, void *ptr);

/* Return full pathname for given file if executable */

PLINT 
plFindName(char *p);

/* Looks for the specified executable file according to usual search path. */

char *
plFindCommand(char *fn);

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */

void
plGetName(char *dir, char *subdir, char *filename, char **filespec);

/* Prompts human to input an integer in response to given message. */

PLINT
plGetInt(char *s);

/* Prompts human to input a float in response to given message. */

PLFLT
plGetFlt(char *s);

	/* Nice way to allocate space for a vectored 2d grid */

/* Allocates a block of memory for use as a 2-d grid of PLFLT's.  */

void
plAlloc2dGrid(PLFLT ***f, PLINT nx, PLINT ny);

/* Frees a block of memory allocated with plAlloc2dGrid(). */

void
plFree2dGrid(PLFLT **f, PLINT nx, PLINT ny);

/* Find the maximum and minimum of a 2d matrix allocated with plAllc2dGrid(). */

void
plMinMax2dGrid(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmax, PLFLT *fmin);

#endif

/* Functions for converting between HLS and RGB color space */

void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

void
plRGB_HLS(PLFLT r, PLFLT g, PLFLT b, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

/* Wait for graphics input event and translate to world coordinates */

#if 0
PLINT
plGetCursor(PLGraphicsIn *gin);

/* Translates relative device coordinates to world coordinates.  */
#endif
/* Use plcalc_world instead of plTranslateCursor. */
#if 0
%typemap(in) PLGraphicsIn *gin (PLGraphicsIn tmp) {
  if(!PySequence_Check($input) || PySequence_Size($input) != 2) {
    PyErr_SetString(PyExc_ValueError, "Expecting a sequence of 2 numbers.");
    return NULL;
  }
  $1 = &tmp;
  $1->dX = PyFloat_AsDouble(PySequence_Fast_GET_ITEM($input, 0));
  $1->dY = PyFloat_AsDouble(PySequence_Fast_GET_ITEM($input, 1));
}
%typemap(argout) PLGraphicsIn *gin {
  PyObject *o;
  o = PyFloat_FromDouble($1->wX);
  resultobj = t_output_helper(resultobj, o);
  o = PyFloat_FromDouble($1->wY);
  resultobj = t_output_helper(resultobj, o);
}
int
plTranslateCursor(PLGraphicsIn *gin);
#endif
