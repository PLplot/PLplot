/* $Id$
 * $Log$
 * Revision 1.1  1995/05/23 22:08:36  mjl
 * Files to handle PLplot Python bindings.
 *
*/

/*	plmodule.c

	Python interface bindings
	Written by: interran@uluru.Stanford.EDU (John Interrante)
*/


/* Rules for translating C PLplot function calls into Python PLplot:
   - All names drop their initial prefix 'pl' or 'PL'
   - C arrays are replaced by Python lists
   - Array lengths are eliminated since lists know how long they are
   - Output parameters are replaced by function return values or tuples
   - Functions taking a pointer to function as an argument are not (yet)
     callable from Python
*/

#include <Python.h>
#include <modsupport.h>
#include "plplot.h"	/* macros and prototypes for the PLplot package */
#include "plplotP.h"	/* private macros and prototypes that we also need */

/* We need to know whether PLFLT is a float or double. */

#if defined(PL_DOUBLE) || defined(DOUBLE)
#undef DOUBLE
#define DOUBLE 1
#else
#define DOUBLE 0
#endif

/* Declare some static functions we will use so ANSI C compilers can
   type-check the arguments. */

static int pl_PyList_AsFloatMatrix Py_PROTO((
    PyObject *list, PLINT expected_nx, PLINT expected_ny, PLFLT ***pitems));
static int pl_PyList_AsFloatArray Py_PROTO((
    PyObject *list, PLFLT **pitems, PLINT *pn));
static int pl_PyList_AsIntArray Py_PROTO((
    PyObject *list, PLINT **pitems, PLINT *pn));
static int pl_PyList_AsStringArray Py_PROTO((
    PyObject *list, char ***pitems, int *pn));
static int pl_PyList_SetFromStringArray Py_PROTO((
    PyObject *list, char **items, int n));

/* Convert a Python list of lists of floating numbers to a C array of
   arrays (matrix). */

static int
pl_PyList_AsFloatMatrix(list, expected_nx, expected_ny, pitems)
    PyObject *list;
    PLINT expected_nx;
    PLINT expected_ny;
    PLFLT ***pitems;
{
    PLFLT **items;
    PLINT nx;
    PLINT i;

    nx = PyList_Size(list);
    if (nx != expected_nx) {
	PyErr_SetString(PyExc_ValueError,
			"z list should have same length as x list");
	return 0;
    }

    items = PyMem_NEW(PLFLT *, nx);
    if (items == NULL) {
	PyErr_NoMemory();
	return 0;
    }

    for (i = 0; i < nx; i++) {
	PyObject *sublist = PyList_GetItem(list, i);
	PLFLT *subitems;
	PLINT ny;

	if (!PyList_Check(sublist)) {
	    PyErr_SetString(PyExc_TypeError,
			    "sublists expected within z list");
	    return 0;
	}

	if (!pl_PyList_AsFloatArray(sublist, &subitems, &ny))
	    return 0;

	if (ny != expected_ny) {
	    PyErr_SetString(PyExc_ValueError,
			    "each sublist should have same length as y list");
	    return 0;
	}

	items[i] = subitems;
    }

    *pitems = items;
    return 1;
}

/* Convert a Python list of floating numbers to a C array. */

static int
pl_PyList_AsFloatArray(list, pitems, pn)
    PyObject *list;
    PLFLT **pitems;
    PLINT *pn;
{
    PLFLT *items;
    PLINT n;
    PLINT i;

    n = PyList_Size(list);
    items = PyMem_NEW(PLFLT, n);
    if (items == NULL) {
	PyErr_NoMemory();
	return 0;
    }

    for (i = 0; i < n; i++) {
	PyObject *item = PyList_GetItem(list, i);
	items[i] = PyFloat_AsDouble(item);
    }

    *pitems = items;
    *pn = n;
    return 1;
}

/* Convert a Python list of integers to a C array. */

static int
pl_PyList_AsIntArray(list, pitems, pn)
    PyObject *list;
    PLINT **pitems;
    PLINT *pn;
{
    PLINT *items;
    PLINT n;
    PLINT i;

    n = PyList_Size(list);
    items = PyMem_NEW(PLINT, n);
    if (items == NULL) {
	PyErr_NoMemory();
	return 0;
    }

    for (i = 0; i < n; i++) {
	PyObject *item = PyList_GetItem(list, i);
	items[i] = PyInt_AsLong(item);
    }

    *pitems = items;
    *pn = n;
    return 1;
}

/* Convert a Python list of strings to a C array.  Code copied from
   stdwinmodule.c. */

static int
pl_PyList_AsStringArray(list, pitems, pn)
    PyObject *list;
    char ***pitems;
    int *pn;
{
    char **items;
    int n;
    int i;

    n = PyList_Size(list);
    items = PyMem_NEW(char*, n + 1);
    if (items == NULL) {
	PyErr_NoMemory();
	return 0;
    }

    for (i = 0; i < n; i++) {
	PyObject *item = PyList_GetItem(list, i);
	if (!PyString_Check(item)) {
	    PyErr_SetString(PyExc_TypeError, "list of strings expected");
	    return 0;
	}
	items[i] = PyString_AsString(item);
    }

    items[n] = NULL;		/* Obey convention of NULL-terminated argv */
    *pitems = items;
    *pn = n;
    return 1;
}

/* Store a C array back into a Python list of strings.  Code copied
   from stdwinmodule.c. */

static int
pl_PyList_SetFromStringArray(list, items, n)
    PyObject *list;
    char **items;
    int n;
{
    int oldsize = PyList_Size(list);
    PyObject *newlist;
    int i;

    if (n == oldsize)
	return 1;

    newlist = PyList_New(n);
    for (i = 0; i < n && newlist != NULL; i++) {
	if (items[i]) {
	    PyObject *item = PyString_FromString(items[i]);
	    if (item == NULL) {
		Py_DECREF(newlist);
		newlist = NULL;
	    } else
		PyList_SetItem(newlist, i, item);
	} else {
	    /* plParseOpts is buggy; it can return NULLs in the
	       middle of the argument list.  We still have n - i args
	       left to store, so skip over the NULL. */
	    items++;
	    i--;
	}
    }
    if (newlist == NULL)
	return 0;

    PyList_SetSlice(list, 0, oldsize, newlist);
    Py_DECREF(newlist);
    return 1;
}

/* Advance to subpage "page", or to the next one if "page" = 0. */

static PyObject *
pl_adv(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT page;

    if (!PyArg_Parse(args, "i", &page))
	return NULL;

    pladv(page);

    Py_INCREF(Py_None);
    return Py_None;
}

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */

static PyObject *
pl_axes(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT x0;
    PLFLT y0;
    const char *xopt;
    PLFLT xtick;
    PLINT nxsub;
    const char *yopt;
    PLFLT ytick;
    PLINT nysub;

    if (!PyArg_Parse(args, DOUBLE ? "(ddsdisdi)" : "(ffsfisfi)",
		     &x0, &y0, &xopt, &xtick, &nxsub, &yopt, &ytick, &nysub))
	return NULL;

    plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Plot a histogram using x to store data values and y to store frequencies */

static PyObject *
pl_bin(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT nbin;
    PLFLT *x;
    PLFLT *y;
    PLINT center;
    PyObject *xlist;
    PyObject *ylist;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!i)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist, &center))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &nbin))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (nbin != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plbin(nbin, x, y, center);

    PyMem_DEL(x);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Start new page.  Should only be used with pleop(). */

static PyObject *
pl_bop(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plbop();

    Py_INCREF(Py_None);
    return Py_None;
}

/* This draws a box around the current viewport. */

static PyObject *
pl_box(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *xopt;
    PLFLT xtick;
    PLINT nxsub;
    const char *yopt;
    PLFLT ytick;
    PLINT nysub;

    if (!PyArg_Parse(args, DOUBLE ? "(sdisdi)" : "(sfisfi)",
		     &xopt, &xtick, &nxsub, &yopt, &ytick, &nysub))
	return NULL;

    plbox(xopt, xtick, nxsub, yopt, ytick, nysub);

    Py_INCREF(Py_None);
    return Py_None;
}

/* This is the 3-d analogue of plbox(). */

static PyObject *
pl_box3(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *xopt;
    const char *xlabel;
    PLFLT xtick;
    PLINT nsubx;
    const char *yopt;
    const char *ylabel;
    PLFLT ytick;
    PLINT nsuby;
    const char *zopt;
    const char *zlabel;
    PLFLT ztick;
    PLINT nsubz;

    if (!PyArg_Parse(args, DOUBLE ? "(ssdissdissdi)" : "(ssfissfissfi)",
		     &xopt, &xlabel, &xtick, &nsubx,
		     &yopt, &ylabel, &ytick, &nsuby,
		     &zopt, &zlabel, &ztick, &nsubz))
	return NULL;

    plbox3(xopt, xlabel, xtick, nsubx,
	   yopt, ylabel, ytick, nsuby,
	   zopt, zlabel, ztick, nsubz);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set color, map 0.  Argument is integer between 0 and 15. */

static PyObject *
pl_col0(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT icol0;

    if (!PyArg_Parse(args, "i", &icol0))
	return NULL;

    plcol0(icol0);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set color, map 1.  Argument is a float between 0. and 1. */

static PyObject *
pl_col1(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT col1;

    if (!PyArg_Parse(args, DOUBLE ? "d" : "f", &col1))
	return NULL;

    plcol1(col1);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data. 
 */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
void
c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);
	 */

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.  */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
void
plfcont(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data);
	*/

/* Copies state parameters from the reference stream to the current stream. */

static PyObject *
pl_cpstrm(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT iplsr;
    PLINT flags;

    if (!PyArg_Parse(args, "(ii)", &iplsr, &flags))
	return NULL;

    plcpstrm(iplsr, flags);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

static PyObject *
pl_did2pc(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT ymin;
    PLFLT xmax;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &ymin, &xmax, &ymax))
	return NULL;

    pldid2pc(&xmin, &ymin, &xmax, &ymax);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", xmin, ymin, xmax, ymax);
}

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

static PyObject *
pl_dip2dc(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT ymin;
    PLFLT xmax;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &ymin, &xmax, &ymax))
	return NULL;

    pldip2dc(&xmin, &ymin, &xmax, &ymax);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", xmin, ymin, xmax, ymax);
}

/* End a plotting session for all open streams. */

static PyObject *
pl_end(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plend();

    Py_INCREF(Py_None);
    return Py_None;
}

/* End a plotting session for the current stream only. */

static PyObject *
pl_end1(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plend1();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Simple interface for defining viewport and window. */

static PyObject *
pl_env(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;
    PLINT just;
    PLINT axis;

    if (!PyArg_Parse(args, DOUBLE ? "(ddddii)" : "(ffffii)",
		     &xmin, &xmax, &ymin, &ymax, &just, &axis))
	return NULL;

    plenv(xmin, xmax, ymin, ymax, just, axis);

    Py_INCREF(Py_None);
    return Py_None;
}

/* End current page.  Should only be used with plbop(). */

static PyObject *
pl_eop(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    pleop();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

static PyObject *
pl_errx(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *xmin;
    PLFLT *xmax;
    PLFLT *y;
    PyObject *xminlist;
    PyObject *xmaxlist;
    PyObject *ylist;
    PLINT xmaxlen;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!O!)",
		     &PyList_Type, &xminlist, &PyList_Type, &xmaxlist,
		     &PyList_Type, &ylist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xminlist, &xmin, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(xmaxlist, &xmax, &xmaxlen))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (n != xmaxlen || n != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plerrx(n, xmin, xmax, y);

    PyMem_DEL(xmin);
    PyMem_DEL(xmax);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

static PyObject *
pl_erry(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *ymin;
    PLFLT *ymax;
    PyObject *xlist;
    PyObject *yminlist;
    PyObject *ymaxlist;
    PLINT yminlen;
    PLINT ymaxlen;

    if (!PyArg_Parse(args, "(O!O!O!)",
		     &PyList_Type, &xlist, &PyList_Type, &yminlist,
		     &PyList_Type, &ymaxlist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(yminlist, &ymin, &yminlen))
	return NULL;

    if (!pl_PyList_AsFloatArray(ymaxlist, &ymax, &ymaxlen))
	return NULL;

    if (n != yminlen || n != ymaxlen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plerry(n, x, ymin, ymax);

    PyMem_DEL(x);
    PyMem_DEL(ymin);
    PyMem_DEL(ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Advance to the next family file on the next new page */

static PyObject *
pl_famadv(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plfamadv();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Pattern fills the polygon bounded by the input points. */

static PyObject *
pl_fill(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PyObject *xlist;
    PyObject *ylist;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (n != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plfill(n, x, y);

    PyMem_DEL(x);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Flushes the output stream.  Use sparingly, if at all. */

static PyObject *
pl_flush(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plflush();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Sets the global font flag to 'ifont'. */

static PyObject *
pl_font(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT ifont;

    if (!PyArg_Parse(args, "i", &ifont))
	return NULL;

    plfont(ifont);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Load specified font set. */

static PyObject *
pl_fontld(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT fnt;

    if (!PyArg_Parse(args, "i", &fnt))
	return NULL;

    plfontld(fnt);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Get character default height and current (scaled) height */

static PyObject *
pl_gchr(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT def;
    PLFLT ht;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgchr(&def, &ht);

    return Py_BuildValue(DOUBLE ? "(dd)" : "(ff)", def, ht);
}

/* Returns 8 bit RGB values for given color from color map 0 */

static PyObject *
pl_gcol0(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT icol0;
    PLINT r;
    PLINT g;
    PLINT b;

    if (!PyArg_Parse(args, "i", &icol0))
	return NULL;

    plgcol0(icol0, &r, &g, &b);

    return Py_BuildValue("(iii)", r, g, b);
}

/* Returns the background color by 8 bit RGB value */

static PyObject *
pl_gcolbg(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT r;
    PLINT g;
    PLINT b;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgcolbg(&r, &g, &b);

    return Py_BuildValue("(iii)", r, g, b);
}

/* Retrieve current window into device space */

static PyObject *
pl_gdidev(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT mar;
    PLFLT aspect;
    PLFLT jx;
    PLFLT jy;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgdidev(&mar, &aspect, &jx, &jy);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", mar, aspect, jx, jy);
}

/* Get plot orientation */

static PyObject *
pl_gdiori(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT rot;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgdiori(&rot);

    return Py_BuildValue(DOUBLE ? "d" : "f", rot);
}

/* Retrieve current window into plot space */

static PyObject *
pl_gdiplt(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT ymin;
    PLFLT xmax;
    PLFLT ymax;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgdiplt(&xmin, &ymin, &xmax, &ymax);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", xmin, ymin, xmax, ymax);
}

/* Get family file parameters */

static PyObject *
pl_gfam(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT fam;
    PLINT num;
    PLINT bmax;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgfam(&fam, &num, &bmax);

    return Py_BuildValue("(iii)", fam, num, bmax);
}

/* Get the (current) output file name.  Must be preallocated to >80 bytes */

static PyObject *
pl_gfnam(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    char fnam[81];

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgfnam(fnam);

    return Py_BuildValue("s", fnam);
}

/* Get output device parameters. */

static PyObject *
pl_gpage(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xp;
    PLFLT yp;
    PLINT xleng;
    PLINT yleng;
    PLINT xoff;
    PLINT yoff;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgpage(&xp, &yp, &xleng, &yleng, &xoff, &yoff);

    return Py_BuildValue(DOUBLE ? "(ddiiii)" : "(ffiiii)",
			 xp, yp, xleng, yleng, xoff, yoff);
}

/* Switches to graphics screen. */

static PyObject *
pl_gra(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plgra();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Get subpage boundaries in absolute coordinates */

static PyObject *
pl_gspa(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgspa(&xmin, &xmax, &ymin, &ymax);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", xmin, xmax, ymin, ymax);
}

/* Get current stream number. */

static PyObject *
pl_gstrm(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT strm;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgstrm(&strm);

    return Py_BuildValue("i", strm);
}

/* Get the current library version number */

static PyObject *
pl_gver(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    char p_ver[80];

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgver(p_ver);

    return Py_BuildValue("s", p_ver);
}

/* Get viewport boundaries in world coordinates */

static PyObject *
pl_Pgvpw(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plP_gvpw(&xmin, &xmax, &ymin, &ymax);

    return Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)", xmin, xmax, ymin, ymax);
}

/* Get x axis labeling parameters */

static PyObject *
pl_gxax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgxax(&digmax, &digits);

    return Py_BuildValue("(ii)", digmax, digits);
}

/* Get y axis labeling parameters */

static PyObject *
pl_gyax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgyax(&digmax, &digits);

    return Py_BuildValue("(ii)", digmax, digits);
}

/* Get z axis labeling parameters */

static PyObject *
pl_gzax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plgzax(&digmax, &digits);

    return Py_BuildValue("(ii)", digmax, digits);
}

/* Draws a histogram of n values of a variable in array data[0..n-1] */

static PyObject *
pl_hist(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *data;
    PLFLT datmin;
    PLFLT datmax;
    PLINT nbin;
    PLINT oldwin;
    PyObject *datalist;

    if (!PyArg_Parse(args, DOUBLE ? "(O!ddii)" : "(O!ffii)",
		     &PyList_Type, &datalist, &datmin, &datmax,
		     &nbin, &oldwin))
	return NULL;

    if (!pl_PyList_AsFloatArray(datalist, &data, &n))
	return NULL;

    plhist(n, data, datmin, datmax, nbin, oldwin);

    PyMem_DEL(data);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set current color (map 0) by hue, lightness, and saturation. */

static PyObject *
pl_hls(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT h;
    PLFLT l;
    PLFLT s;

    if (!PyArg_Parse(args, DOUBLE ? "(ddd)" : "(fff)", &h, &l, &s))
	return NULL;

    plhls(h, l, s);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Initializes PLplot, using preset or default options */

static PyObject *
pl_init(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plinit();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws a line segment from (x1, y1) to (x2, y2). */

static PyObject *
pl_join(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT x1;
    PLFLT y1;
    PLFLT x2;
    PLFLT y2;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)", &x1, &y1, &x2, &y2))
	return NULL;

    pljoin(x1, y1, x2, y2);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Simple routine for labelling graphs. */

static PyObject *
pl_lab(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *xlabel;
    const char *ylabel;
    const char *tlabel;

    if (!PyArg_Parse(args, "(sss)", &xlabel, &ylabel, &tlabel))
	return NULL;

    pllab(xlabel, ylabel, tlabel);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws line segments connecting a series of points. */

static PyObject *
pl_line(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PyObject *xlist;
    PyObject *ylist;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (n != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plline(n, x, y);

    PyMem_DEL(x);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws a line in 3 space.  */

static PyObject *
pl_line3(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PLFLT *z;
    PyObject *xlist;
    PyObject *ylist;
    PyObject *zlist;
    PLINT ylen;
    PLINT zlen;

    if (!PyArg_Parse(args, "(O!O!O!)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist,
		     &PyList_Type, &zlist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (!pl_PyList_AsFloatArray(zlist, &z, &zlen))
	return NULL;

    if (n != ylen || n != zlen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plline3(n, x, y, z);

    PyMem_DEL(x);
    PyMem_DEL(y);
    PyMem_DEL(z);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set line style. */

static PyObject *
pl_lsty(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT lin;

    if (!PyArg_Parse(args, "i", &lin))
	return NULL;

    pllsty(lin);

    Py_INCREF(Py_None);
    return Py_None;
}

/* plot continental outline in world coordinates */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
void
plmap(void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
      PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
      */

/* Plot the latitudes and longitudes on the background. */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
void 
plmeridians(void (*mapform)(PLINT, PLFLT *, PLFLT *), 
	    PLFLT dlong, PLFLT dlat,
	    PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
	    */

/* Plots a mesh representation of the function z[x][y]. */

static PyObject *
pl_mesh(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT *x;
    PLFLT *y;
    PLFLT **z;
    PLINT nx;
    PLINT ny;
    PLINT opt;
    PyObject *xlist;
    PyObject *ylist;
    PyObject *zlist;
    int i;

    if (!PyArg_Parse(args, "(O!O!O!i)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist,
		     &PyList_Type, &zlist, &opt))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &nx))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ny))
	return NULL;

    if (!pl_PyList_AsFloatMatrix(zlist, nx, ny, &z))
	return NULL;

    plmesh(x, y, z, nx, ny, opt);

    PyMem_DEL(x);
    PyMem_DEL(y);
    for (i = 0; i < nx; i++)
	PyMem_DEL(z[i]);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Creates a new stream and makes it the default.  */

static PyObject *
pl_mkstrm(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT strm;

    if (!PyArg_Parse(args, ""))
	return NULL;

    plmkstrm(&strm);

    return Py_BuildValue("i", strm);
}

/* Prints out "text" at specified position relative to viewport */

static PyObject *
pl_mtex(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *side;
    PLFLT disp;
    PLFLT pos;
    PLFLT just;
    const char *text;

    if (!PyArg_Parse(args, DOUBLE ? "(sddds)" : "(sfffs)",
		     &side, &disp, &pos, &just, &text))
	return NULL;

    plmtex(side, disp, pos, just, text);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Plots a 3-d representation of the function z[x][y]. */

static PyObject *
pl_plot3d(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT *x;
    PLFLT *y;
    PLFLT **z;
    PLINT nx;
    PLINT ny;
    PLINT opt;
    PLINT side;
    PyObject *xlist;
    PyObject *ylist;
    PyObject *zlist;
    int i;

    if (!PyArg_Parse(args, "(O!O!O!ii)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist,
		     &PyList_Type, &zlist, &opt, &side))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &nx))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ny))
	return NULL;

    if (!pl_PyList_AsFloatMatrix(zlist, nx, ny, &z))
	return NULL;

    plot3d(x, y, z, nx, ny, opt, side);

    PyMem_DEL(x);
    PyMem_DEL(y);
    for (i = 0; i < nx; i++)
	PyMem_DEL(z[i]);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set fill pattern directly. */

static PyObject *
pl_pat(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT nlin;
    PLINT *inc;
    PLINT *del;
    PyObject *inclist;
    PyObject *dellist;
    PLINT dellen;

    if (!PyArg_Parse(args, "(O!O!)",
		     &PyList_Type, &inclist, &PyList_Type, &dellist))
	return NULL;

    if (!pl_PyList_AsIntArray(inclist, &inc, &nlin))
	return NULL;

    if (!pl_PyList_AsIntArray(dellist, &del, &dellen))
	return NULL;

    if (nlin != dellen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plpat(nlin, inc, del);

    PyMem_DEL(inc);
    PyMem_DEL(del);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Plots array y against x for n points using ASCII code "code".*/

static PyObject *
pl_poin(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PLINT code;
    PyObject *xlist;
    PyObject *ylist;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!i)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist, &code))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (n != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plpoin(n, x, y, code);

    PyMem_DEL(x);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws a series of points in 3 space. */

static PyObject *
pl_poin3(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PLFLT *z;
    PLINT code;
    PyObject *xlist;
    PyObject *ylist;
    PyObject *zlist;
    PLINT ylen;
    PLINT zlen;

    if (!PyArg_Parse(args, "(O!O!O!i)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist,
		     &PyList_Type, &zlist, &code))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (!pl_PyList_AsFloatArray(zlist, &z, &zlen))
	return NULL;

    if (n != ylen || n != zlen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plpoin3(n, x, y, z, code);

    PyMem_DEL(x);
    PyMem_DEL(y);
    PyMem_DEL(z);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Draws a polygon in 3 space.  */

static PyObject *
pl_poly3(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PLFLT *z;
    PLINT *draw;
    PyObject *xlist;
    PyObject *ylist;
    PyObject *zlist;
    PyObject *drawlist;
    PLINT ylen;
    PLINT zlen;
    PLINT drawlen;

    if (!PyArg_Parse(args, "(O!O!O!O!)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist,
		     &PyList_Type, &zlist, &PyList_Type, &drawlist))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (!pl_PyList_AsFloatArray(zlist, &z, &zlen))
	return NULL;

    if (!pl_PyList_AsIntArray(drawlist, &draw, &drawlen))
	return NULL;

    if (n != ylen || n != zlen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    if (drawlen != n - 1) {
	PyErr_SetString(PyExc_ValueError,
			"arg 4 should have a length one less than the other args");
	return NULL;
    }

    plpoly3(n, x, y, z, draw);

    PyMem_DEL(x);
    PyMem_DEL(y);
    PyMem_DEL(z);
    PyMem_DEL(draw);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the floating point precision (in number of places) in numeric labels. */

static PyObject *
pl_prec(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT setp;
    PLINT prec;

    if (!PyArg_Parse(args, "(ii)", &setp, &prec))
	return NULL;

    plprec(setp, prec);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set fill pattern, using one of the predefined patterns.*/

static PyObject *
pl_psty(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT patt;

    if (!PyArg_Parse(args, "i", &patt))
	return NULL;

    plpsty(patt);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Prints out "text" at world cooordinate (x,y). */

static PyObject *
pl_ptex(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT x;
    PLFLT y;
    PLFLT dx;
    PLFLT dy;
    PLFLT just;
    const char *text;

    if (!PyArg_Parse(args, DOUBLE ? "(ddddds)" : "(fffffs)",
		     &x, &y, &dx, &dy, &just, &text))
	return NULL;

    plptex(x, y, dx, dy, just, text);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Replays contents of plot buffer to current device/file. */

static PyObject *
pl_replot(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plreplot();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set line color by red, green, blue from  0. to 1. */

static PyObject *
pl_rgb(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT r;
    PLFLT g;
    PLFLT b;

    if (!PyArg_Parse(args, DOUBLE ? "(ddd)" : "(fff)", &r, &g, &b))
	return NULL;

    plrgb(r, g, b);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set line color by 8 bit RGB values. */

static PyObject *
pl_rgb1(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT r;
    PLINT g;
    PLINT b;

    if (!PyArg_Parse(args, "(iii)", &r, &g, &b))
	return NULL;

    plrgb1(r, g, b);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Obsolete.  Use page driver interface instead. */

/* If it's obsolete, might as well omit it.
void
c_plsasp(PLFLT asp);
*/

/* Set character height. */

static PyObject *
pl_schr(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT def;
    PLFLT scale;

    if (!PyArg_Parse(args, DOUBLE ? "(dd)" : "(ff)", &def, &scale))
	return NULL;

    plschr(def, scale);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set number of colors in cmap 0 */

static PyObject *
pl_scmap0n(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT ncol0;

    if (!PyArg_Parse(args, "i", &ncol0))
	return NULL;

    plscmap0n(ncol0);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set number of colors in cmap 1 */

static PyObject *
pl_scmap1n(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT ncol1;

    if (!PyArg_Parse(args, "i", &ncol1))
	return NULL;

    plscmap1n(ncol1);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set color map 0 colors by 8 bit RGB values */

static PyObject *
pl_scmap0(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT *r;
    PLINT *g;
    PLINT *b;
    PLINT ncol0;
    PyObject *rlist;
    PyObject *glist;
    PyObject *blist;
    PLINT glen;
    PLINT blen;

    if (!PyArg_Parse(args, "(O!O!O!)",
		     &PyList_Type, &rlist, &PyList_Type, &glist,
		     &PyList_Type, &blist))
	return NULL;

    if (!pl_PyList_AsIntArray(rlist, &r, &ncol0))
	return NULL;

    if (!pl_PyList_AsIntArray(glist, &g, &glen))
	return NULL;

    if (!pl_PyList_AsIntArray(blist, &b, &blen))
	return NULL;

    if (ncol0 != glen || ncol0 != blen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plscmap0(r, g, b, ncol0);

    PyMem_DEL(r);
    PyMem_DEL(g);
    PyMem_DEL(b);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set color map 1 colors by 8 bit RGB values */

static PyObject *
pl_scmap1(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT *r;
    PLINT *g;
    PLINT *b;
    PLINT ncol1;
    PyObject *rlist;
    PyObject *glist;
    PyObject *blist;
    PLINT glen;
    PLINT blen;

    if (!PyArg_Parse(args, "(O!O!O!)",
		     &PyList_Type, &rlist, &PyList_Type, &glist,
		     &PyList_Type, &blist))
	return NULL;

    if (!pl_PyList_AsIntArray(rlist, &r, &ncol1))
	return NULL;

    if (!pl_PyList_AsIntArray(glist, &g, &glen))
	return NULL;

    if (!pl_PyList_AsIntArray(blist, &b, &blen))
	return NULL;

    if (ncol1 != glen || ncol1 != blen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1, 2, and 3 should have the same length");
	return NULL;
    }

    plscmap1(r, g, b, ncol1);

    PyMem_DEL(r);
    PyMem_DEL(g);
    PyMem_DEL(b);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

static PyObject *
pl_scmap1l(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT itype;
    PLINT npts;
    PLFLT *intensity;
    PLFLT *coord1;
    PLFLT *coord2;
    PLFLT *coord3;
    PyObject *intensitylist;
    PyObject *coord1list;
    PyObject *coord2list;
    PyObject *coord3list;
    PLINT coord1len;
    PLINT coord2len;
    PLINT coord3len;

    if (!PyArg_Parse(args, "(iO!O!O!O!)",
		     &itype,
		     &PyList_Type, &intensitylist,
		     &PyList_Type, &coord1list,
		     &PyList_Type, &coord2list,
		     &PyList_Type, &coord3list))
	return NULL;

    if (!pl_PyList_AsFloatArray(intensitylist, &intensity, &npts))
	return NULL;

    if (!pl_PyList_AsFloatArray(coord1list, &coord1, &coord1len))
	return NULL;

    if (!pl_PyList_AsFloatArray(coord2list, &coord2, &coord2len))
	return NULL;

    if (!pl_PyList_AsFloatArray(coord3list, &coord3, &coord3len))
	return NULL;

    if (npts != coord1len || npts != coord2len || npts != coord3len) {
	PyErr_SetString(PyExc_ValueError,
			"args 2, 3, 4, and 5 should have the same length");
	return NULL;
    }

    plscmap1l(itype, npts, intensity, coord1, coord2, coord3, NULL);

    PyMem_DEL(intensity);
    PyMem_DEL(coord1);
    PyMem_DEL(coord2);
    PyMem_DEL(coord3);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set a given color from color map 0 by 8 bit RGB value */

static PyObject *
pl_scol0(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT icol0;
    PLINT r;
    PLINT g;
    PLINT b;

    if (!PyArg_Parse(args, "(iiii)", &icol0, &r, &g, &b))
	return NULL;

    plscol0(icol0, r, g, b);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the background color by 8 bit RGB value */

static PyObject *
pl_scolbg(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT r;
    PLINT g;
    PLINT b;

    if (!PyArg_Parse(args, "(iii)", &r, &g, &b))
	return NULL;

    plscolbg(r, g, b);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Used to globally turn color output on/off */

static PyObject *
pl_scolor(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT color;

    if (!PyArg_Parse(args, "i", &color))
	return NULL;

    plscolor(color);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the device (keyword) name */

static PyObject *
pl_sdev(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *devname;

    if (!PyArg_Parse(args, "s", &devname))
	return NULL;

    plsdev(devname);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set window into device space using margin, aspect ratio, and */
/* justification */

static PyObject *
pl_sdidev(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT mar;
    PLFLT aspect;
    PLFLT jx;
    PLFLT jy;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &mar, &aspect, &jx, &jy))
	return NULL;

    plsdidev(mar, aspect, jx, jy);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set up transformation from metafile coordinates. */

static PyObject *
pl_sdimap(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT dimxmin;
    PLINT dimxmax;
    PLINT dimymin;
    PLINT dimymax;
    PLFLT dimxpmm;
    PLFLT dimypmm;

    if (!PyArg_Parse(args, DOUBLE ? "(iiiidd)" : "(iiiiff)",
		     &dimxmin, &dimxmax, &dimymin, &dimymax,
		     &dimxpmm, &dimypmm))
	return NULL;

    plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set plot orientation, specifying rotation in units of pi/2. */

static PyObject *
pl_sdiori(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT rot;

    if (!PyArg_Parse(args, DOUBLE ? "d" : "f", &rot))
	return NULL;

    plsdiori(rot);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set window into plot space */

static PyObject *
pl_sdiplt(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT ymin;
    PLFLT xmax;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &ymin, &xmax, &ymax))
	return NULL;

    plsdiplt(xmin, ymin, xmax, ymax);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set window into plot space incrementally (zoom) */

static PyObject *
pl_sdiplz(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT ymin;
    PLFLT xmax;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &ymin, &xmax, &ymax))
	return NULL;

    plsdiplz(xmin, ymin, xmax, ymax);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the escape character for text strings. */

static PyObject *
pl_sesc(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    char esc;

    if (!PyArg_Parse(args, "c", &esc))
	return NULL;

    plsesc(esc);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set family file parameters */

static PyObject *
pl_sfam(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT fam;
    PLINT num;
    PLINT bmax;

    if (!PyArg_Parse(args, "(iii)", &fam, &num, &bmax))
	return NULL;

    plsfam(fam, num, bmax);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the output file name. */

static PyObject *
pl_sfnam(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *fnam;

    if (!PyArg_Parse(args, "s", &fnam))
	return NULL;

    plsfnam(fnam);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Shade region. */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
void 
c_plshade(PLFLT **a, PLINT nx, PLINT ny, const char **defined,
	  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	  PLFLT shade_min, PLFLT shade_max,
	  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	  PLINT min_color, PLINT min_width,
	  PLINT max_color, PLINT max_width,
	  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	  PLPointer pltr_data);

void 
plshade1(PLFLT *a, PLINT nx, PLINT ny, const char *defined,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

void 
plfshade(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	 PLPointer f2eval_data,
	 PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny, 
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);
	 */

/* I've canned this for now */

/*
void
c_plslpb(PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma);
*/

/* Set up lengths of major tick marks. */

static PyObject *
pl_smaj(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT def;
    PLFLT scale;

    if (!PyArg_Parse(args, DOUBLE ? "(dd)" : "(ff)", &def, &scale))
	return NULL;

    plsmaj(def, scale);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set up lengths of minor tick marks. */

static PyObject *
pl_smin(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT def;
    PLFLT scale;

    if (!PyArg_Parse(args, DOUBLE ? "(dd)" : "(ff)", &def, &scale))
	return NULL;

    plsmin(def, scale);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set orientation.  Must be done before calling plinit. */

static PyObject *
pl_sori(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT ori;

    if (!PyArg_Parse(args, "i", &ori))
	return NULL;

    plsori(ori);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set output device parameters.  Usually ignored by the driver. */

static PyObject *
pl_spage(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xp;
    PLFLT yp;
    PLINT xleng;
    PLINT yleng;
    PLINT xoff;
    PLINT yoff;

    if (!PyArg_Parse(args, DOUBLE ? "(ddiiii)" : "(ffiiii)",
		     &xp, &yp, &xleng, &yleng, &xoff, &yoff))
	return NULL;

    plspage(xp, yp, xleng, yleng, xoff, yoff);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the pause (on end-of-page) status */

static PyObject *
pl_spause(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT pause;

    if (!PyArg_Parse(args, "i", &pause))
	return NULL;

    plspause(pause);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set stream number.  */

static PyObject *
pl_sstrm(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT strm;

    if (!PyArg_Parse(args, "i", &strm))
	return NULL;

    plsstrm(strm);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set the number of subwindows in x and y */

static PyObject *
pl_ssub(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT nx;
    PLINT ny;

    if (!PyArg_Parse(args, "(ii)", &nx, &ny))
	return NULL;

    plssub(nx, ny);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set symbol height. */

static PyObject *
pl_ssym(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT def;
    PLFLT scale;

    if (!PyArg_Parse(args, DOUBLE ? "(dd)" : "(ff)", &def, &scale))
	return NULL;

    plssym(def, scale);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Initialize PLplot, passing in the windows/page settings. */

static PyObject *
pl_star(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT nx;
    PLINT ny;

    if (!PyArg_Parse(args, "(ii)", &nx, &ny))
	return NULL;

    plstar(nx, ny);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Initialize PLplot, passing the device name and windows/page settings. */

static PyObject *
pl_start(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    const char *devname;
    PLINT nx;
    PLINT ny;

    if (!PyArg_Parse(args, "(sii)", &devname, &nx, &ny))
	return NULL;

    plstart(devname, nx, ny);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set up a new line style */

static PyObject *
pl_styl(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT nms;
    PLINT *mark;
    PLINT *space;
    PyObject *marklist;
    PyObject *spacelist;
    PLINT spacelen;

    if (!PyArg_Parse(args, "(O!O!)",
		     &PyList_Type, &marklist, &PyList_Type, &spacelist))
	return NULL;

    if (!pl_PyList_AsIntArray(marklist, &mark, &nms))
	return NULL;

    if (!pl_PyList_AsIntArray(spacelist, &space, &spacelen))
	return NULL;

    if (nms != spacelen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plstyl(nms, mark, space);

    PyMem_DEL(mark);
    PyMem_DEL(space);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Sets the edges of the viewport to the specified absolute coordinates */

static PyObject *
pl_svpa(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &xmax, &ymin, &ymax))
	return NULL;

    plsvpa(xmin, xmax, ymin, ymax);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set x axis labeling parameters */

static PyObject *
pl_sxax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, "(ii)", &digmax, &digits))
	return NULL;

    plsxax(digmax, digits);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set inferior X window */

static PyObject *
pl_sxwin(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT window_id;

    if (!PyArg_Parse(args, "i", &window_id))
	return NULL;

    plsxwin(window_id);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set y axis labeling parameters */

static PyObject *
pl_syax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, "(ii)", &digmax, &digits))
	return NULL;

    plsyax(digmax, digits);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Plots array y against x for n points using Hershey symbol "code" */

static PyObject *
pl_sym(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT n;
    PLFLT *x;
    PLFLT *y;
    PLINT code;
    PyObject *xlist;
    PyObject *ylist;
    PLINT ylen;

    if (!PyArg_Parse(args, "(O!O!i)",
		     &PyList_Type, &xlist, &PyList_Type, &ylist, &code))
	return NULL;

    if (!pl_PyList_AsFloatArray(xlist, &x, &n))
	return NULL;

    if (!pl_PyList_AsFloatArray(ylist, &y, &ylen))
	return NULL;

    if (n != ylen) {
	PyErr_SetString(PyExc_ValueError,
			"args 1 and 2 should have the same length");
	return NULL;
    }

    plsym(n, x, y, code);

    PyMem_DEL(x);
    PyMem_DEL(y);
    Py_INCREF(Py_None);
    return Py_None;
}

/* Set z axis labeling parameters */

static PyObject *
pl_szax(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT digmax;
    PLINT digits;

    if (!PyArg_Parse(args, "(ii)", &digmax, &digits))
	return NULL;

    plszax(digmax, digits);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Switches to text screen. */

static PyObject *
pl_text(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    pltext();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

static PyObject *
pl_vasp(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT aspect;

    if (!PyArg_Parse(args, DOUBLE ? "d" : "f", &aspect))
	return NULL;

    plvasp(aspect);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

static PyObject *
pl_vpas(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;
    PLFLT aspect;

    if (!PyArg_Parse(args, DOUBLE ? "(ddddd)" : "(fffff)",
		     &xmin, &xmax, &ymin, &ymax, &aspect))
	return NULL;

    plvpas(xmin, xmax, ymin, ymax, aspect);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Creates a viewport with the specified normalized subpage coordinates. */

static PyObject *
pl_vpor(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &xmax, &ymin, &ymax))
	return NULL;

    plvpor(xmin, xmax, ymin, ymax);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

static PyObject *
pl_vsta(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    if (!PyArg_Parse(args, ""))
	return NULL;

    plvsta();

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set up a window for three-dimensional plotting. */

static PyObject *
pl_w3d(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT basex;
    PLFLT basey;
    PLFLT height;
    PLFLT xmin0;
    PLFLT xmax0;
    PLFLT ymin0;
    PLFLT ymax0;
    PLFLT zmin0;
    PLFLT zmax0;
    PLFLT alt;
    PLFLT az;

    if (!PyArg_Parse(args, DOUBLE ? "(ddddddddddd)" : "(fffffffffff)",
		     &basex, &basey, &height, &xmin0, &xmax0, &ymin0, &ymax0,
		     &zmin0, &zmax0, &alt, &az))
	return NULL;

    plw3d(basex, basey, height,
	  xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set pen width. */

static PyObject *
pl_wid(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLINT width;

    if (!PyArg_Parse(args, "i", &width))
	return NULL;

    plwid(width);

    Py_INCREF(Py_None);
    return Py_None;
}

/* Set up world coordinates of the viewport boundaries (2d plots). */

static PyObject *
pl_wind(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLFLT xmin;
    PLFLT xmax;
    PLFLT ymin;
    PLFLT ymax;

    if (!PyArg_Parse(args, DOUBLE ? "(dddd)" : "(ffff)",
		     &xmin, &xmax, &ymin, &ymax))
	return NULL;

    plwind(xmin, xmax, ymin, ymax);

    Py_INCREF(Py_None);
    return Py_None;
}

	/* Command line parsing utilities */

/* Process PLplot internal options list */

static PyObject *
pl_ParseOpts(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    int argc;
    char **argv;
    PLINT mode;
    PyObject *argvlist;
    int status;

    if (!PyArg_Parse(args, "(O!i)", &PyList_Type, &argvlist, &mode))
	return NULL;

    if (!pl_PyList_AsStringArray(argvlist, &argv, &argc))
	return NULL;

    status = plParseOpts(&argc, argv, mode);

    if (!pl_PyList_SetFromStringArray(argvlist, argv, argc))
	return NULL;

    PyMem_DEL(argv);
    return Py_BuildValue("i", status);
}

/* Process options list */

/* Not sure how to interface Python to a function taking a pointer to
   function, so omitted for now.
int
plParseOpts(int *p_argc, char **argv, PLINT mode, PLOptionTable *option_table,
	    void (*usage_handler) (char *));
	    */

/* Process input strings, treating them as an option and argument pair. */

static PyObject *
pl_SetOpt(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    char *opt;
    char *optarg;
    int status;

    if (!PyArg_Parse(args, "(ss)", &opt, &optarg))
	return NULL;

    status = plSetOpt(opt, optarg);

    return Py_BuildValue("i", status);
}

/* Wait for right button mouse event and translate to world coordinates */

static PyObject *
pl_GetCursor(self, args)
    PyObject *self; /* Not used */
    PyObject *args;
{
    PLGraphicsIn gin;
    int translated;
    PyObject *result;

    if (!PyArg_Parse(args, ""))
	return NULL;

    translated = plGetCursor(&gin);

    if (translated)
	result = Py_BuildValue(DOUBLE ? "(dddd)" : "(ffff)",
			       gin.dX, gin.dY, gin.wX, gin.wY);
    else
	result = Py_BuildValue(DOUBLE ? "(dd)" : "(ff)",
			       gin.dX, gin.dY);

    return result;
}

/* The rest of plplot.h didn't seem worth converting to Python. */

/* List of functions defined in the module */

static PyMethodDef pl_methods[] = {
    {"adv",		pl_adv},
    {"axes",		pl_axes},
    {"bin",		pl_bin},
    {"bop",		pl_bop},
    {"box",		pl_box},
    {"box3",		pl_box3},
    {"col",		pl_col0}, /* old name for backward compatibility */
    {"col0",		pl_col0},
    {"col1",		pl_col1},
    {"cpstrm",		pl_cpstrm},
    {"did2pc",		pl_did2pc},
    {"dip2dc",		pl_dip2dc},
    {"end",		pl_end},
    {"end1",		pl_end1},
    {"env",		pl_env},
    {"eop",		pl_eop},
    {"errx",		pl_errx},
    {"erry",		pl_erry},
    {"famadv",		pl_famadv},
    {"fill",		pl_fill},
    {"flush",		pl_flush},
    {"font",		pl_font},
    {"fontld",		pl_fontld},
    {"gchr",		pl_gchr},
    {"gcol0",		pl_gcol0},
    {"gcolbg",		pl_gcolbg},
    {"gdidev",		pl_gdidev},
    {"gdiori",		pl_gdiori},
    {"gdiplt",		pl_gdiplt},
    {"gfam",		pl_gfam},
    {"gfnam",		pl_gfnam},
    {"gpage",		pl_gpage},
    {"gra",		pl_gra},
    {"gspa",		pl_gspa},
    {"gstrm",		pl_gstrm},
    {"gver",		pl_gver},
    {"gxax",		pl_gxax},
    {"gyax",		pl_gyax},
    {"gzax",		pl_gzax},
    {"hist",		pl_hist},
    {"hls",		pl_hls},
    {"init",		pl_init},
    {"join",		pl_join},
    {"lab",		pl_lab},
    {"line",		pl_line},
    {"line3",		pl_line3},
    {"lsty",		pl_lsty},
    {"mesh",		pl_mesh},
    {"mkstrm",		pl_mkstrm},
    {"mtex",		pl_mtex},
    {"plot3d",		pl_plot3d},
    {"pat",		pl_pat},
    {"poin",		pl_poin},
    {"poin3",		pl_poin3},
    {"poly3",		pl_poly3},
    {"prec",		pl_prec},
    {"psty",		pl_psty},
    {"ptex",		pl_ptex},
    {"replot",		pl_replot},
    {"rgb",		pl_rgb},
    {"rgb1",		pl_rgb1},
    {"schr",		pl_schr},
    {"scmap0n",		pl_scmap0n},
    {"scmap1n",		pl_scmap1n},
    {"scmap0",		pl_scmap0},
    {"scmap1",		pl_scmap1},
    {"scmap1l",		pl_scmap1l},
    {"scol0",		pl_scol0},
    {"scolbg",		pl_scolbg},
    {"scolor",		pl_scolor},
    {"sdev",		pl_sdev},
    {"sdidev",		pl_sdidev},
    {"sdimap",		pl_sdimap},
    {"sdiori",		pl_sdiori},
    {"sdiplt",		pl_sdiplt},
    {"sdiplz",		pl_sdiplz},
    {"sesc",		pl_sesc},
    {"sfam",		pl_sfam},
    {"sfnam",		pl_sfnam},
    {"smaj",		pl_smaj},
    {"smin",		pl_smin},
    {"sori",		pl_sori},
    {"spage",		pl_spage},
    {"spause",		pl_spause},
    {"sstrm",		pl_sstrm},
    {"ssub",		pl_ssub},
    {"ssym",		pl_ssym},
    {"star",		pl_star},
    {"start",		pl_start},
    {"styl",		pl_styl},
    {"svpa",		pl_svpa},
    {"sxax",		pl_sxax},
    {"sxwin",		pl_sxwin},
    {"syax",		pl_syax},
    {"sym",		pl_sym},
    {"szax",		pl_szax},
    {"text",		pl_text},
    {"vasp",		pl_vasp},
    {"vpas",		pl_vpas},
    {"vpor",		pl_vpor},
    {"vsta",		pl_vsta},
    {"w3d",		pl_w3d},
    {"wid",		pl_wid},
    {"wind",		pl_wind},
    {"ParseOpts",	pl_ParseOpts},
    {"SetOpt",		pl_SetOpt},
    {"GetCursor",	pl_GetCursor},

    /* These are a few functions from plplotP.h that I found I needed
       to call in order to get information about the viewport, etc. */

    {"Pgvpw",		pl_Pgvpw},
    {NULL,		NULL}	/* sentinel */
};

/* Initialization function for the module (*must* be called initpl) */

void
initpl()
{
    PyObject *m;
    PyObject *d;
    PyObject *v;

    /* Create the module and add the functions */
    m = Py_InitModule("pl", pl_methods);
    d = PyModule_GetDict(m);

    /* Add some symbolic constants to the module */
    v = PyInt_FromLong(PL_PARSE_PARTIAL);
    PyDict_SetItemString(d, "PARSE_PARTIAL", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_FULL);
    PyDict_SetItemString(d, "PARSE_FULL", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_QUIET);
    PyDict_SetItemString(d, "PARSE_QUIET", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_NODELETE);
    PyDict_SetItemString(d, "PARSE_NODELETE", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_SHOWALL);
    PyDict_SetItemString(d, "PARSE_SHOWALL", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_OVERRIDE);
    PyDict_SetItemString(d, "PARSE_OVERRIDE", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_NOPROGRAM);
    PyDict_SetItemString(d, "PARSE_NOPROGRAM", v);
    Py_XDECREF(v);

    v = PyInt_FromLong(PL_PARSE_NODASH);
    PyDict_SetItemString(d, "PARSE_NODASH", v);
    Py_XDECREF(v);

    /* Check for errors */
    if (PyErr_Occurred())
	Py_FatalError("pl module initialization failed");
}
