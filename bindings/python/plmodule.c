/*	plmodule.c : Version 0.3

	Permission granted to copy and distribute this work provided that this 
	notice remains intact.

	Python interface bindings
	Written by: interran@uluru.Stanford.EDU (John Interrante) 
                         et@appl-math.tu-muenchen.de (Thomas Schwaller)

      1995/08/19:
        -  I (Thomas) wrote the plmodule at exactly the same time as John using a script
            and some handwritten procedures. As John used the new naming convention
            I decided to support his version (included in the plplot distribution) and
            to enhance and support it. Since the good data structures for this module
            are matrices I waited for this module to make the big changes here.
        -  Added the Macro PL_ARGS to test for PLFLT numbers at compile time 
            and not at runtime.
	  -  added plcont (contour plots!). If something is wrong with your
            plotter function a Python RuntimeError will be raised, but only
            after plcont is executed. There's no possibility to stop plcont after
            the first error occured, at least I do not know any solution for that problem.
	  -  plstyl arguments can be three integers or two integer lists now.

     1995/11/19 Big Changes (by Thomas Schwaller)
        - Reworked the whole module.
        - All List inputs are replaced by matrix inputs (Not backward compatible !)
           Could have made boths inputs possible, but matrix input is much faster 
           and is the first choice here (MatLab style), so wy people should use the 
           slower lists instead? Changed that, see 1995/11/26)
        - Replaced PyArg_Parse by PyArg_ParseTuple and added document strings.
           (these were the C-comments of John, now they are available in the Python 
            module, eg. print plsfam.__doc__)

      1995/11/26 Some Improvements (by Thomas Schwaller)
        - reworked pl_PyArray_AsFloat{Array, Matrix} .
        - used PyArray_ContiguousFromObject to convert input to matrices
           as suggested by James Hugunin. I'm not sure this works really with
           non-matrices (Python lists and the like..) as input.
        - Made extensive use of the TRY Macro
        - added pl_arrows dor arrow plotting. This function is not documented in
           plplot yet, but it's there! Produces strange results at the moment!

      1995/12/10 Discovered a bug (Thomas Schwaller)
	  - Py_DECREF(mp) was made too early! 
	     For Matrices this has no effect, but for List or Tuple input the data
           was destroyed before being used!

   	Rules for translating C PLplot function calls into Python PLplot:
   	  - All names are the the same (plot3d modified to plplot3d for uniformity), but
           now for uniformity with C API change back to plot3d---AWI.
   	  - C arrays are replaced by Python matrices or objects which can be 
           converted to matrices.
   	  - Array and Matrix lengths are eliminated since matrices know how long they are.
   	  - Output parameters are replaced by function return values or tuples.

      Thanxs to Jim Fulton, James Hugunin, Konrad Hinsen and all the other people in the
      Python Matrix SIG for the stimulating talks and the same spirit of mind!

      Bug Reports please to et@appl-math.tu-muenchen.de (Thomas Schwaller)
*/

#include "plplot/plmodule.h"

/*static*/ int pl_PyArray_AsFloatArray(PyObject **op, PLFLT **pitems, 
				       PLINT *pn)
{
    PyArrayObject *mp;
    mp = (PyArrayObject *) 
     PyArray_ContiguousFromObject (*op, PyArray_PLFLT, 0, 1);
    if (!mp) return 0;
    *pitems =(PLFLT*) (mp->data);
    *pn = mp->dimensions[0];
    *op = (PyObject*) mp;
    return 1;
}

/*static*/ int pl_PyArray_AsIntArray(PyObject **op, PLINT **pitems, PLINT *pn)
{
    PyArrayObject *mp;
    mp = (PyArrayObject *) PyArray_ContiguousFromObject (*op, PyArray_INT, 0, 1);
    if (!mp) return 0;
    *pitems =(PLINT*) (mp->data);
    *pn = mp->dimensions[0];
    *op = (PyObject*) mp;
    return 1;
}

/* Thanxs to James Hugunin for helping me with that! */

/*static*/ int  pl_PyArray_AsFloatMatrix (PyObject **op, PLINT* m, PLINT* n, PLFLT*** pitems)
{
    char *dp, **dpp;
    int j, size;
    PyArrayObject *mp;
    mp = (PyArrayObject *) PyArray_ContiguousFromObject (*op, PyArray_PLFLT, 2, 2);
    if (!mp) return 0;
    *m = mp->dimensions[0];
    *n = mp->dimensions[1];
    dp = mp->data;
    size = *n * sizeof(PLFLT);
    if (!(dpp = PyMem_NEW(char*, *m))) {
        PyErr_NoMemory();
        Py_DECREF(mp);
        return 0;
    }
    for(j=0; j< *m; j++) 
        dpp[j] = dp+size*j;
    *pitems = (PLFLT**) dpp;
    *op = (PyObject*) mp;
    return 1;
}

/* Convert a Python list of strings to a C array.  Code copied from stdwinmodule.c. */

/*static*/ int pl_PyList_AsStringArray(PyObject *list, char ***pitems, int *pn)
{
    char **items;
    int n, i;
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

/* Store a C array back into a Python list of strings.  Code copied from stdwinmodule.c. */

/*static*/ int pl_PyList_SetFromStringArray(PyObject *list, char **items, int n)
{
    int i, oldsize = PyList_Size(list);
    PyObject *newlist;
    if (n == oldsize) return 1;
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
    if (newlist == NULL) return 0;
    PyList_SetSlice(list, 0, oldsize, newlist);
    Py_DECREF(newlist);
    return 1;
}

static char doc_pladv[] = "Advance to subpage \"page\", or to the next one if \"page\" = 0";

static PyObject * pl_adv(PyObject *self, PyObject *args)
{
    PLINT page;
    TRY (PyArg_ParseTuple(args, "i", &page));
    pladv(page);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plarrows[] = "simple arrow plotter";

static PyObject * pl_arrows(PyObject *self, PyObject *args)
{
    PLFLT *u, *v, *x, *y, scale, dx, dy;
    PLINT un, vn, xn, yn;
    PyObject *uop, *vop, *xop, *yop;
    TRY (PyArg_ParseTuple(args, PL_ARGS("OOOOddd", "OOOOfff"), 
                                              &uop, &vop, &xop, &yop, &scale, &dx, &dy));
    TRY (pl_PyArray_AsFloatArray(&uop, &u, &un));
    TRY (pl_PyArray_AsFloatArray(&vop, &v, &vn));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &xn));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &yn));
     if ((un != vn) || (vn != xn) || (xn != yn)) {
	    PyErr_SetString(PyExc_ValueError, "first 4 arguments should have the same length");
          Py_DECREF(uop);
          Py_DECREF(vop);
          Py_DECREF(xop);
          Py_DECREF(yop);
	    return NULL;
    }
    plarrows(u, v, x, y, un, scale, dx, dy);
    Py_DECREF(uop);
    Py_DECREF(vop);
    Py_DECREF(xop);
    Py_DECREF(yop);                                   
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plaxes[]=" This functions similarly to plbox() except that the origin of the axes is placed at the user-specified point (x0, y0)";

static PyObject * pl_axes(PyObject *self, PyObject *args)
{
    PLFLT x0, y0, xtick, ytick;
    PLINT nxsub, nysub;
    const char *xopt, *yopt;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddsdisdi", "ffsfisfi"), &x0, &y0, &xopt, &xtick, 
                                                                                           &nxsub, &yopt, &ytick, &nysub));
    plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plbin[]="Plot a histogram using x to store data values and y to store frequencies";

static PyObject * pl_bin(PyObject *self, PyObject *args)
{
    PLINT nbin, center, ylen;
    PLFLT *x, *y;
    PyObject *xop, *yop;
    TRY (PyArg_ParseTuple(args, "OOi", &xop, &yop, &center));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &nbin));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (nbin != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
	  return NULL;
    }
    plbin(nbin, x, y, center);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plbop[]="Start new page.  Should only be used with pleop()";

static PyObject * pl_bop(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plbop();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plbox[]="This draws a box around the current viewport";

static PyObject * pl_box(PyObject *self, PyObject *args)
{
    const char *xopt, *yopt;
    PLFLT xtick, ytick;
    PLINT nxsub, nysub;
    TRY (PyArg_ParseTuple(args, PL_ARGS("sdisdi", "sfisfi"),
                                             &xopt, &xtick, &nxsub, &yopt, &ytick, &nysub));
    plbox(xopt, xtick, nxsub, yopt, ytick, nysub);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plbox3[]="This is the 3-d analogue of plbox()";

static PyObject * pl_box3(PyObject *self, PyObject *args)
{
    const char *xopt, *xlabel, *yopt, *ylabel, *zopt, *zlabel;
    PLFLT xtick, ytick, ztick;
    PLINT nsubx, nsuby, nsubz;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ssdissdissdi", "ssfissfissfi"),
		     &xopt, &xlabel, &xtick, &nsubx, &yopt, &ylabel, &ytick, &nsuby,
		     &zopt, &zlabel, &ztick, &nsubz));
    plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plcol0[]="Set color, map 0.  Argument is integer between 0 and 15";

static PyObject * pl_col0(PyObject *self, PyObject *args)
{
    PLINT icol0;
    TRY (PyArg_ParseTuple(args, "i", &icol0));
    plcol0(icol0);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plcol1[]="Set color, map 1.  Argument is a float between 0. and 1";

static PyObject * pl_col1(PyObject *self, PyObject *args)
{
    PLFLT col1;
    TRY (PyArg_ParseTuple(args, PL_ARGS("d", "f"), &col1));
    plcol1(col1);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plcont_ts[]="Draws a contour plot from data in f(nx,ny).  Is just a front-end to plfcont, with a particular choice for f2eval and f2eval_data";

static void pypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer data)
{
    PyObject *ret, *value;
    value = Py_BuildValue(PL_ARGS("(dd)", "(ff)"), x, y);
    ret = PyEval_CallObject( (PyObject*) data, value); 
    PyArg_ParseTuple(ret, PL_ARGS("dd", "ff"), tx, ty);
    Py_DECREF(value);
    Py_DECREF(ret);
}

static PyObject * pl_cont_ts(PyObject *self, PyObject *args)
{
    PLINT nx, ny, kx, lx, ky, ly, nlevel;
    int i;
    PLFLT **z, *clevel;	
    PyObject *op, *levelop, *f;
    TRY (PyArg_ParseTuple(args, "OiiiiOO!", &op, &kx, &lx, &ky, &ly, &levelop, 
			  &PyFunction_Type, &f));
    TRY (pl_PyArray_AsFloatMatrix(&op, &nx, &ny, &z));
    TRY (pl_PyArray_AsFloatArray(&levelop, &clevel, &nlevel));
    plcont(z, nx, ny, kx, lx, ky, ly, clevel, nlevel, pypltr, f);
    Py_DECREF(op);
    Py_DECREF(levelop);
    PyMem_DEL(z);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_RuntimeError, "Check your plotter function!");
	return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plcont2[]="Draws a contour plot from data in f(nx,ny).  Is just a front-end to plfcont, with a particular choice for f2eval and f2eval_data";

static void
pyt0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data)
{
/*    printf( "evaluating tr0(%lf,%lf)\n", x, y );
 */    *tx = x;
    *ty = y;
}

#include "plplot/plplotP.h"

/* static int pl_pymateval_modx, pl_pymateval_mody; */

/*--------------------------------------------------------------------------*\
 * plf2eval2()
 *
 * Does a lookup from a 2d function array.  Array is of type (PLFLT **),
 * and is column dominant (normal C ordering).
\*--------------------------------------------------------------------------*/

PLFLT
pyf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data)
{
    PLFLT value;
    PLfGrid2 *grid = (PLfGrid2 *) plf2eval_data;

/*    printf( "In pyf2eval2(%d,%d)=%lf\n",
	    ix, iy, grid->f[ix][iy] );
	    */
    ix = ix % grid->nx;
    iy = iy % grid->ny;

    value = grid->f[ix][iy];

    return value;
}

static PyObject * pl_cont2(PyObject *self, PyObject *args)
{
    PLINT nx, ny, kx, lx, ky, ly, nlevel;
    int i;
    PLFLT **z, *clevel;	
    PyObject *op, *levelop;
    PLfGrid2 grid;

/*    printf( "In Geoff's plcont2.\n" );*/

    TRY( PyArg_ParseTuple( args, "OiiiiO",
			   &op, &kx, &lx, &ky, &ly, &levelop ) );
    TRY (pl_PyArray_AsFloatMatrix(&op, &nx, &ny, &z));
    TRY (pl_PyArray_AsFloatArray(&levelop, &clevel, &nlevel));

/*    printf( "nx=%d ny=%d kx=%d lx=%d ky=%d ly=%d\n",
	    nx, ny, kx, lx, ky, ly );
    printf( "nlevel=%d\n", nlevel );
    for( i=0; i < nlevel; i++ )
	printf( "level[%d] = %lf\n", i, clevel[i] );

    for( i=kx-1; i < lx; i++ ) {
	int j;
	for( j=ky-1; j < ly; j++ ) {
	    printf( " data(%d,%d) = %lf\n", i, j, z[i][j] );
	}
    }*/

    grid.f = z;

/*    plcont(z, nx, ny, kx, lx, ky, ly, clevel, nlevel, pyt0, NULL );*/
    plcontf( pyf2eval2, (PLPointer) &grid, 
	    nx, ny, kx, lx, ky, ly, clevel, nlevel,
	    pyt0, NULL );
    Py_DECREF(op);
    Py_DECREF(levelop);
    PyMem_DEL(z);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_RuntimeError, "Check your plotter function!");
	return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}

/*
static char doc_plcont[]="Draws a contour plot using the function evaluator f2eval and data stored by way of the f2eval_data pointer. This allows arbitrary organizations of 2d array data to beused.";

void plfcont(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data);
*/

static char doc_plcpstrm[]="Copies state parameters from the reference stream to the current stream";

static PyObject * pl_cpstrm(PyObject *self, PyObject *args)
{
    PLINT iplsr, flags;
    TRY (PyArg_ParseTuple(args, "ii", &iplsr, &flags));
    plcpstrm(iplsr, flags);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pldid2pc[]="Converts input values from relative device coordinates to relative plot coordinates";

static PyObject * pl_did2pc(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd","ffff"), 
                                              &xmin, &ymin, &xmax, &ymax));
    pldid2pc(&xmin, &ymin, &xmax, &ymax);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), xmin, ymin, xmax, ymax);
}

static char doc_pldip2dc[]="Converts input values from relative plot coordinates to relative device coordinates";

static PyObject * pl_dip2dc(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"),
		                                  &xmin, &ymin, &xmax, &ymax));
    pldip2dc(&xmin, &ymin, &xmax, &ymax);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), xmin, ymin, xmax, ymax);
}

static char doc_plend[]="End a plotting session for all open streams";

static PyObject * pl_end(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plend();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plend1[]="End a plotting session for the current stream only";

static PyObject * pl_end1(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plend1();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plenv[]="Simple interface for defining viewport and window";

static PyObject * pl_env(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    PLINT just, axis;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddddii", "ffffii"),
		                                 &xmin, &xmax, &ymin, &ymax, &just, &axis));
    plenv(xmin, xmax, ymin, ymax, just, axis);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pleop[]="End current page. Should only be used with plbop()";

static PyObject * pl_eop(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    pleop();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plerrx[]="Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i))";

static PyObject * pl_errx(PyObject *self, PyObject *args)
{
    PLINT n, xmaxlen, ylen;
    PLFLT *xmin, *xmax, *y;
    PyObject *xminop, *xmaxop, *yop;
    TRY (PyArg_ParseTuple(args, "OOO", &xminop, &xmaxop, &yop));
    TRY (pl_PyArray_AsFloatArray(&xminop, &xmin, &n));
    TRY (pl_PyArray_AsFloatArray(&xmaxop, &xmax, &xmaxlen));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (n != xmaxlen || n != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(xminop);
        Py_DECREF(xmaxop);
        Py_DECREF(yop);
	  return NULL;
    }
    plerrx(n, xmin, xmax, y);
    Py_DECREF(xminop);
    Py_DECREF(xmaxop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plerry[]="Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i))";

static PyObject * pl_erry(PyObject *self, PyObject *args)
{
    PLINT n, yminlen, ymaxlen;
    PLFLT *x, *ymin, *ymax;
    PyObject *xop, *yminop, *ymaxop;
    TRY (PyArg_ParseTuple(args, "OOO", &xop, &yminop, &ymaxop));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yminop, &ymin, &yminlen));
    TRY (pl_PyArray_AsFloatArray(&ymaxop, &ymax, &ymaxlen));
    if (n != yminlen || n != ymaxlen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yminop);
        Py_DECREF(ymaxop);
	  return NULL;
    }
    plerry(n, x, ymin, ymax);
    Py_DECREF(xop);
    Py_DECREF(yminop);
    Py_DECREF(ymaxop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plfamadv[]="Advance to the next family file on the next new page";

static PyObject * pl_famadv(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plfamadv();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plfill[]="Pattern fills the polygon bounded by the input points";

static PyObject * pl_fill(PyObject *self, PyObject *args)
{
    PLINT n, ylen;
    PLFLT *x, *y;
    PyObject *xop, *yop;
    TRY (PyArg_ParseTuple(args, "OO", &xop, &yop));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (n != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
	  return NULL;
    }
    plfill(n, x, y);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plflush[]="Flushes the output stream.  Use sparingly, if at all.";

static PyObject * pl_flush(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plflush();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plfont[]="Sets the global font flag to 'ifont'";

static PyObject * pl_font(PyObject *self, PyObject *args)
{
    PLINT ifont;
    TRY (PyArg_ParseTuple(args, "i", &ifont));
    plfont(ifont);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plfontld[]="Load specified font set.";

static PyObject * pl_fontld(PyObject *self, PyObject *args)
{
    PLINT fnt;
    TRY (PyArg_ParseTuple(args, "i", &fnt));
    plfontld(fnt);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plgchr[]="Get character default height and current (scaled) height";

static PyObject * pl_gchr(PyObject *self, PyObject *args)
{
    PLFLT def, ht;
    TRY (PyArg_ParseTuple(args, ""));
    plgchr(&def, &ht);
    return Py_BuildValue(PL_ARGS("(dd)", "(ff)"), def, ht);
}

static char doc_plgcol0[]="Returns 8 bit RGB values for given color from color map 0";

static PyObject * pl_gcol0(PyObject *self, PyObject *args)
{
    PLINT icol0, r, b, g;
    TRY (PyArg_ParseTuple(args, "i", &icol0));
    plgcol0(icol0, &r, &g, &b);
    return Py_BuildValue("(iii)", r, g, b);
}

static char doc_plgcolbg[]="Returns the background color by 8 bit RGB value";

static PyObject * pl_gcolbg(PyObject *self, PyObject *args)
{
    PLINT r, g, b;
    TRY (PyArg_ParseTuple(args, ""));
    plgcolbg(&r, &g, &b);
    return Py_BuildValue("(iii)", r, g, b);
}

static char doc_plgdidev[]="Retrieve current window into device space";

static PyObject * pl_gdidev(PyObject *self, PyObject *args)
{
    PLFLT mar, aspect, jx, jy;
    TRY (PyArg_ParseTuple(args, ""));
    plgdidev(&mar, &aspect, &jx, &jy);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), mar, aspect, jx, jy);
}

static char doc_plgdiori[]="Get plot orientation";

static PyObject * pl_gdiori(PyObject *self, PyObject *args)
{
    PLFLT rot;
    TRY (PyArg_ParseTuple(args, ""));
    plgdiori(&rot);
    return Py_BuildValue(PL_ARGS("d", "f"), rot);
}

static char doc_plgdiplt[]="Retrieve current window into plot space";

static PyObject * pl_gdiplt(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    TRY (PyArg_ParseTuple(args, ""));
    plgdiplt(&xmin, &ymin, &xmax, &ymax);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), xmin, ymin, xmax, ymax);
}

static char doc_plgfam[]="Get family file parameters";

static PyObject * pl_gfam(PyObject *self, PyObject *args)
{
    PLINT fam, num, bmax;
    TRY (PyArg_ParseTuple(args, ""));
    plgfam(&fam, &num, &bmax);
    return Py_BuildValue("(iii)", fam, num, bmax);
}

static char doc_plgfnam[]="Get the (current) output file name.  Must be preallocated to >80 bytes";

static PyObject * pl_gfnam(PyObject *self, PyObject *args)
{
    char fnam[81];
    TRY (PyArg_ParseTuple(args, ""));
    plgfnam(fnam);
    return Py_BuildValue("s", fnam);
}

static char doc_plgpage[]="Get output device parameters";

static PyObject * pl_gpage(PyObject *self, PyObject *args)
{
    PLFLT xp, yp;
    PLINT xleng, yleng, xoff, yoff;
    TRY (PyArg_ParseTuple(args, ""));
    plgpage(&xp, &yp, &xleng, &yleng, &xoff, &yoff);
    return Py_BuildValue(PL_ARGS("(ddiiii)", "(ffiiii)"), xp, yp, xleng, yleng, xoff, yoff);
}

static char doc_plgra[]="Switches to graphics screen.";

static PyObject * pl_gra(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plgra();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plgspa[]="Get subpage boundaries in absolute coordinates";

static PyObject * pl_gspa(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax;
    TRY (PyArg_ParseTuple(args, ""));
    plgspa(&xmin, &xmax, &ymin, &ymax);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), xmin, xmax, ymin, ymax);
}

static char doc_plgstrm[]="Get current stream number.";

static PyObject * pl_gstrm(PyObject *self, PyObject *args)
{
    PLINT strm;
    TRY (PyArg_ParseTuple(args, ""));
    plgstrm(&strm);
    return Py_BuildValue("i", strm);
}

static char doc_plgver[]="Get the current library version number";

static PyObject * pl_gver(PyObject *self, PyObject *args)
{
    char p_ver[80];
    TRY (PyArg_ParseTuple(args, ""));
    plgver(p_ver);
    return Py_BuildValue("s", p_ver);
}

static char doc_plPgvpw[]="Get viewport boundaries in world coordinates";

static PyObject * pl_Pgvpw(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax;
    TRY (PyArg_ParseTuple(args, ""));
    plP_gvpw(&xmin, &xmax, &ymin, &ymax);
    return Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), xmin, xmax, ymin, ymax);
}

static char doc_plgxax[]="Get x axis labeling parameters";

static PyObject * pl_gxax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, ""));
    plgxax(&digmax, &digits);
    return Py_BuildValue("(ii)", digmax, digits);
}

static char doc_plgyax[]="Get y axis labeling parameters";

static PyObject * pl_gyax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, ""));
    plgyax(&digmax, &digits);
    return Py_BuildValue("(ii)", digmax, digits);
}

static char doc_plgzax[]="Get z axis labeling parameters";

static PyObject * pl_gzax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, ""));
    plgzax(&digmax, &digits);
    return Py_BuildValue("(ii)", digmax, digits);
}

static char doc_plhist[]="Draws a histogram of n values of a variable in array data[0..n-1]";

static PyObject * pl_hist(PyObject *self, PyObject *args)
{
    PLINT n, nbin, oldwin;
    PLFLT *data, datmin, datmax;
    PyObject *op;
    TRY (PyArg_ParseTuple(args, PL_ARGS("Oddii", "O!ffii"), 
                                             &op, &datmin, &datmax, &nbin, &oldwin));
    TRY (pl_PyArray_AsFloatArray(&op, &data, &n));
    plhist(n, data, datmin, datmax, nbin, oldwin);
    Py_DECREF(op);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plhls[]="Set current color (map 0) by hue, lightness, and saturation";

static PyObject * pl_hls(PyObject *self, PyObject *args)
{
    PLFLT h, l, s;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddd", "fff"), &h, &l, &s));
    plhls(h, l, s);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plinit[]="Initializes PLplot, using preset or default options";

static PyObject * pl_init(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plinit();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pljoin[]="Draws a line segment from (x1, y1) to (x2, y2)";

static PyObject * pl_join(PyObject *self, PyObject *args)
{
    PLFLT x1, y1, x2, y2;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), &x1, &y1, &x2, &y2));
    pljoin(x1, y1, x2, y2);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pllab[]="Simple routine for labelling graphs";

static PyObject * pl_lab(PyObject *self, PyObject *args)
{
    const char *xlabel, *ylabel, *tlabel;
    TRY (PyArg_ParseTuple(args, "sss", &xlabel, &ylabel, &tlabel));
    pllab(xlabel, ylabel, tlabel);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plline[]="Draws line segments connecting a series of points";

static PyObject * pl_line(PyObject *self, PyObject *args)
{
    PLINT n, ylen;
    PLFLT *x,*y;
    PyObject *xop, *yop;
    TRY (PyArg_ParseTuple(args, "OO", &xop, &yop));	 
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (n != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
	  return NULL;
    }
    plline(n, x, y);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plline3[]="Draws a line in 3 space";

static PyObject * pl_line3(PyObject *self, PyObject *args)
{
    PLINT n, ylen, zlen;
    PLFLT *x, *y, *z;
    PyObject *xop, *yop, *zop;
    TRY (PyArg_ParseTuple(args, "OOO", &xop, &yop, &zop));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    TRY (pl_PyArray_AsFloatArray(&zop, &z, &zlen));
    if (n != ylen || n != zlen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
        Py_DECREF(zop);
  	  return NULL;
    }
    plline3(n, x, y, z);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_DECREF(zop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pllsty[]="Set line style";

static PyObject * pl_lsty(PyObject *self, PyObject *args)
{
    PLINT lin;
    TRY (PyArg_ParseTuple(args, "i", &lin));
    pllsty(lin);
    Py_INCREF(Py_None);
    return Py_None;
}

/*
Not done yet...

static char doc_plmap[]="plot continental outline in world coordinates 

void plmap(void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
      PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
*/

/*
Not done yet... 

static char doc_plmeridians[]="Plot the latitudes and longitudes on the background";

void  plmeridians(void (*mapform)(PLINT, PLFLT *, PLFLT *), 
	    PLFLT dlong, PLFLT dlat,
	    PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);
*/

static char doc_plmesh[]="Plots a mesh representation of the function z[x][y]";

static PyObject * pl_mesh(PyObject *self, PyObject *args)
{
    PLFLT *x, *y, **z;
    PLINT nx, ny, opt;
    PyObject *xop, *yop, *zop;
    int i;
    TRY (PyArg_ParseTuple(args, "OOOi", &xop, &yop, &zop, &opt));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &nx));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ny));
    TRY (pl_PyArray_AsFloatMatrix(&zop, &nx, &ny, &z));
    plmesh(x, y, z, nx, ny, opt);
    PyMem_DEL(z);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_DECREF(zop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plmkstrm[]="Creates a new stream and makes it the default";

static PyObject * pl_mkstrm(PyObject *self, PyObject *args)
{
    PLINT strm;
    TRY (PyArg_ParseTuple(args, ""));
    plmkstrm(&strm);
    return Py_BuildValue("i", strm);
}

static char doc_plmtex[]="Prints out \"text\" at specified position relative to viewport";

static PyObject * pl_mtex(PyObject *self, PyObject *args)
{
    const char *side, *text;
    PLFLT disp, pos, just;
    TRY (PyArg_ParseTuple(args, PL_ARGS("sddds", "sfffs"), 
                                              &side, &disp, &pos, &just, &text));
    plmtex(side, disp, pos, just, text);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plot3d[]="Plots a 3-d representation of the function z[x][y]";

static PyObject * pl_ot3d(PyObject *self, PyObject *args)
{
    PLFLT *x, *y, **z;
    PLINT nx, ny, opt, side;
    PyObject *xop, *yop, *zop;
    TRY (PyArg_ParseTuple(args, "OOOii", &xop, &yop, &zop, &opt, &side));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &nx));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ny));
    TRY (pl_PyArray_AsFloatMatrix(&zop, &nx, &ny, &z));
    plot3d(x, y, z, nx, ny, opt, side);
    PyMem_DEL(z);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_DECREF(zop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plpat[]="Set fill pattern directly";

static PyObject * pl_pat(PyObject *self, PyObject *args)
{
    PLINT nlin, *inc, *del, dellen;
    PyObject *incop, *delop;
    TRY (PyArg_ParseTuple(args, "OO", &incop, &delop));
    TRY (pl_PyArray_AsIntArray(&incop, &inc, &nlin));
    TRY (pl_PyArray_AsIntArray(&delop, &del, &dellen));
    if (nlin != dellen) {
	PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(incop);
        Py_DECREF(delop);
	return NULL;
    }
    plpat(nlin, inc, del);
    Py_DECREF(incop);
    Py_DECREF(delop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plpoin[]="Plots array y against x for n points using ASCII code \"code\"";

static PyObject * pl_poin(PyObject *self, PyObject *args)
{
    PLINT n, code, ylen;
    PLFLT*x, *y;
    PyObject *xop, *yop;
    TRY (PyArg_ParseTuple(args, "OOi", &xop, &yop, &code));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (n != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
	  return NULL;
    }
    plpoin(n, x, y, code);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plpoin3[]="Draws a series of points in 3 space";

static PyObject * pl_poin3(PyObject *self, PyObject *args)
{
    PLINT n, code, ylen, zlen;
    PLFLT *x, *y, *z;
    PyObject *xop, *yop, *zop;
    TRY (PyArg_ParseTuple(args, "OOOi", &xop, &yop, &zop, &code));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    TRY (pl_PyArray_AsFloatArray(&zop, &z, &zlen));
    if (n != ylen || n != zlen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
        Py_DECREF(zop);
	  return NULL;
    }
    plpoin3(n, x, y, z, code);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_DECREF(zop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plpoly3[]="Draws a polygon in 3 space";

static PyObject * pl_poly3(PyObject *self, PyObject *args)
{
    PLINT n, *draw, ylen, zlen, drawlen;
    PLFLT *x, *y, *z;
    PyObject *xop, *yop, *zop, *drawop;
    TRY (PyArg_ParseTuple(args, "OOOO", &xop, &yop, &zop, &drawop));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    TRY (pl_PyArray_AsFloatArray(&zop, &z, &zlen));
    TRY (pl_PyArray_AsIntArray(&drawop, &draw, &drawlen));
    if (n != ylen || n != zlen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
        Py_DECREF(zop);
        Py_DECREF(drawop);
	  return NULL;
    }
    if (drawlen != n - 1) {
	  PyErr_SetString(PyExc_ValueError, 
                                    "arg 4 should have a length one less than the other args");
        Py_DECREF(xop);
        Py_DECREF(yop);
        Py_DECREF(zop);
        Py_DECREF(drawop);
	  return NULL;
    }
    plpoly3(n, x, y, z, draw);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_DECREF(zop);           
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plprec[]="Set the floating point precision (in number of places) in numeric labels";

static PyObject * pl_prec(PyObject *self, PyObject *args)
{
    PLINT setp, prec;
    TRY (PyArg_ParseTuple(args, "ii", &setp, &prec));
    plprec(setp, prec);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plpsty[]="Set fill pattern, using one of the predefined patterns";

static PyObject * pl_psty(PyObject *self, PyObject *args)
{
    PLINT patt;
    TRY (PyArg_ParseTuple(args, "i", &patt));
    plpsty(patt);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plptex[]="Prints out \"text\" at world cooordinate (x,y)";

static PyObject * pl_ptex(PyObject *self, PyObject *args)
{
    PLFLT x, y, dx, dy, just;
    const char *text;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddddds", "fffffs"), 
                                              &x, &y, &dx, &dy, &just, &text));
    plptex(x, y, dx, dy, just, text);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plreplot[]="Replays contents of plot buffer to current device/file";

static PyObject * pl_replot(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plreplot();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plrgb[]="Set line color by red, green, blue from  0.0=0 to 1.0";

static PyObject * pl_rgb(PyObject *self, PyObject *args)
{
    PLFLT r, g, b;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddd", "fff"), &r, &g, &b));
    plrgb(r, g, b);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plrgb1[]="Set line color by 8 bit RGB values";

static PyObject * pl_rgb1(PyObject *self, PyObject *args)
{
    PLINT r, g, b;
    TRY (PyArg_ParseTuple(args, "iii", &r, &g, &b));
    plrgb1(r, g, b);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plschr[]="Set character height";

static PyObject * pl_schr(PyObject *self, PyObject *args)
{
    PLFLT def, scale;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dd", "ff"), &def, &scale));
    plschr(def, scale);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscmap0n[]="Set number of colors in cmap 0";

static PyObject * pl_scmap0n(PyObject *self, PyObject *args)
{
    PLINT ncol0;
    TRY (PyArg_ParseTuple(args, "i", &ncol0));
    plscmap0n(ncol0);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscmap1n[]="Set number of colors in cmap 1";

static PyObject * pl_scmap1n(PyObject *self, PyObject *args)
{
    PLINT ncol1;
    TRY (PyArg_ParseTuple(args, "i", &ncol1));
    plscmap1n(ncol1);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscmap0[]="Set color map 0 colors by 8 bit RGB values";

static PyObject * pl_scmap0(PyObject *self, PyObject *args)
{
    PLINT *r, *g, *b;
    PLINT ncol0, glen, blen;
    PyObject *rop, *gop, *bop;
    TRY (PyArg_ParseTuple(args, "OOO", &rop, &gop, &bop));
    TRY (pl_PyArray_AsIntArray(&rop, &r, &ncol0));
    TRY (pl_PyArray_AsIntArray(&gop, &g, &glen));
    TRY (pl_PyArray_AsIntArray(&bop, &b, &blen));
    if (ncol0 != glen || ncol0 != blen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(rop);
        Py_DECREF(gop);
        Py_DECREF(bop);
	  return NULL;
    }
    plscmap0(r, g, b, ncol0);
    Py_DECREF(rop);
    Py_DECREF(gop);
    Py_DECREF(bop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscmap1[]="Set color map 1 colors by 8 bit RGB values";

static PyObject * pl_scmap1(PyObject *self, PyObject *args)
{
    PLINT *r, *g, *b;
    PLINT ncol1, glen, blen;
    PyObject *rop, *gop, *bop;
    TRY (PyArg_ParseTuple(args, "OOO", &rop, &gop, &bop));
    TRY (pl_PyArray_AsIntArray(&rop, &r, &ncol1));
    TRY (pl_PyArray_AsIntArray(&gop, &g, &glen));
    TRY (pl_PyArray_AsIntArray(&bop, &b, &blen));
    if (ncol1 != glen || ncol1 != blen) {
	  PyErr_SetString(PyExc_ValueError, "args 1, 2, and 3 should have the same length");
        Py_DECREF(rop);
        Py_DECREF(gop);
        Py_DECREF(bop);
	  return NULL;
    }
    plscmap1(r, g, b, ncol1);
    Py_DECREF(rop);
    Py_DECREF(gop);
    Py_DECREF(bop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscmap1l[]="Set color map 1 colors using a piece-wise linear relationship between intensity [0,1] (cmap 1 index) and position in HLS or RGB color space.";

static PyObject * pl_scmap1l(PyObject *self, PyObject *args)
{
    PLINT itype, npts, coord1len, coord2len, coord3len;
    PLFLT *intensity, *coord1, *coord2, *coord3;
    PyObject *intensityop, *coord1op, *coord2op, *coord3op;
    TRY (PyArg_ParseTuple(args, "iOOOO", &itype, &intensityop, 
                                             &coord1op, &coord2op, &coord3op));
    TRY (pl_PyArray_AsFloatArray(&intensityop, &intensity, &npts));
    TRY (pl_PyArray_AsFloatArray(&coord1op, &coord1, &coord1len));
    TRY (pl_PyArray_AsFloatArray(&coord2op, &coord2, &coord2len));
    TRY (pl_PyArray_AsFloatArray(&coord3op, &coord3, &coord3len));
    if (npts != coord1len || npts != coord2len || npts != coord3len) {
	  PyErr_SetString(PyExc_ValueError, "args 2, 3, 4, and 5 should have the same length");
        Py_DECREF(intensityop);
        Py_DECREF(coord1op);
        Py_DECREF(coord2op);
        Py_DECREF(coord3op);
	  return NULL;
    }
    plscmap1l(itype, npts, intensity, coord1, coord2, coord3, NULL);
    Py_DECREF(intensityop);
    Py_DECREF(coord1op);
    Py_DECREF(coord2op);
    Py_DECREF(coord3op);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscol0[]="Set a given color from color map 0 by 8 bit RGB value";

static PyObject * pl_scol0(PyObject *self, PyObject *args)
{
    PLINT icol0, r, g, b;
    TRY (PyArg_ParseTuple(args, "iiii", &icol0, &r, &g, &b));
    plscol0(icol0, r, g, b);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscolbg[]="Set the background color by 8 bit RGB value";

static PyObject * pl_scolbg(PyObject *self, PyObject *args)
{
    PLINT r, g, b;
    TRY (PyArg_ParseTuple(args, "iii", &r, &g, &b));
    plscolbg(r, g, b);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plscolor[]="Used to globally turn color output on/off";

static PyObject * pl_scolor(PyObject *self, PyObject *args)
{
    PLINT color;
    TRY (PyArg_ParseTuple(args, "i", &color));
    plscolor(color);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdev[]="Set the device (keyword) name";

static PyObject * pl_sdev(PyObject *self, PyObject *args)
{
    const char *devname;
    TRY (PyArg_ParseTuple(args, "s", &devname));
    plsdev(devname);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdidev[]="Set window into device space using margin, aspect ratio, and justification";

static PyObject * pl_sdidev(PyObject *self, PyObject *args)
{
    PLFLT mar, aspect, jx, jy;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), &mar, &aspect, &jx, &jy));
    plsdidev(mar, aspect, jx, jy);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdimap[]="Set up transformation from metafile coordinates";

static PyObject * pl_sdimap(PyObject *self, PyObject *args)
{
    PLINT dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm;
    TRY (PyArg_ParseTuple(args, PL_ARGS("iiiidd", "iiiiff"),
		                                  &dimxmin, &dimxmax, &dimymin, 
                                              &dimymax, &dimxpmm, &dimypmm));
    plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdiori[]="Set plot orientation, specifying rotation in units of pi/2";

static PyObject * pl_sdiori(PyObject *self, PyObject *args)
{
    PLFLT rot;
    TRY (PyArg_ParseTuple(args, PL_ARGS("d", "f"), &rot));
    plsdiori(rot);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdiplt[]="Set window into plot space";

static PyObject * pl_sdiplt(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), 
                                              &xmin, &ymin, &xmax, &ymax));
    plsdiplt(xmin, ymin, xmax, ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsdiplz[]="Set window into plot space incrementally (zoom)";

static PyObject * pl_sdiplz(PyObject *self, PyObject *args)
{
    PLFLT xmin, ymin, xmax, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), 
                                              &xmin, &ymin, &xmax, &ymax));
    plsdiplz(xmin, ymin, xmax, ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsesc[]="Set the escape character for text strings";

static PyObject * pl_sesc(PyObject *self, PyObject *args)
{
    char esc;
    TRY (PyArg_ParseTuple(args, "c", &esc));
    plsesc(esc);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsfam[]="Set family file parameters";

static PyObject * pl_sfam(PyObject *self, PyObject *args)
{
    PLINT fam, num, bmax;
    TRY (PyArg_ParseTuple(args, "iii", &fam, &num, &bmax));
    plsfam(fam, num, bmax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsfnam[]="Set the output file name";

static PyObject * pl_sfnam(PyObject *self, PyObject *args)
{
    const char *fnam;
    TRY (PyArg_ParseTuple(args, "s", &fnam));
    plsfnam(fnam);
    Py_INCREF(Py_None);
    return Py_None;
}
/*
#include "plmodule2.c"
*/
/* 
Not done yet...

static char doc_plshade[]="Shade region";

void  c_plshade(PLFLT **a, PLINT nx, PLINT ny, const char **defined,
	                     PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	                     PLFLT shade_min, PLFLT shade_max,
	                     PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	                     PLINT min_color, PLINT min_width,
	                     PLINT max_color, PLINT max_width,
	                     void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	                     void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	                     PLPointer pltr_data);

void plshade1(PLFLT *a, PLINT nx, PLINT ny, const char *defined,
	                  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	                  PLFLT shade_min, PLFLT shade_max,
	                  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	                  PLINT min_color, PLINT min_width,
	                  PLINT max_color, PLINT max_width,
	                  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	                  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	                  PLPointer pltr_data);

void  plfshade(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
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

static char doc_plsmaj[]="Set up lengths of major tick marks";

static PyObject * pl_smaj(PyObject *self, PyObject *args)
{
    PLFLT def, scale;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dd", "ff"), &def, &scale));
    plsmaj(def, scale);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsmin[]="Set up lengths of minor tick marks";

static PyObject * pl_smin(PyObject *self, PyObject *args)
{
    PLFLT def, scale;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dd", "ff"), &def, &scale));
    plsmin(def, scale);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsori[]="Set orientation.  Must be done before calling plinit.";

static PyObject *pl_sori(PyObject *self, PyObject *args)
{
    PLINT ori;
    TRY (PyArg_ParseTuple(args, "i", &ori));
    plsori(ori);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plspage[]="Set output device parameters.  Usually ignored by the driver.";

static PyObject * pl_spage(PyObject *self, PyObject *args)
{
    PLFLT xp, yp;
    PLINT xleng, yleng, xoff, yoff;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddiiii", "ffiiii"), 
                                              &xp, &yp, &xleng, &yleng, &xoff, &yoff));
    plspage(xp, yp, xleng, yleng, xoff, yoff);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plspause[]="Set the pause (on end-of-page) status";

static PyObject * pl_spause(PyObject *self, PyObject *args)
{
    PLINT pause;
    TRY (PyArg_ParseTuple(args, "i", &pause));
    plspause(pause);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsstrm[]="Set stream number";

static PyObject * pl_sstrm(PyObject *self, PyObject *args)
{
    PLINT strm;
    TRY (PyArg_ParseTuple(args, "i", &strm));
    plsstrm(strm);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plssub[]="Set the number of subwindows in x and y";

static PyObject * pl_ssub(PyObject *self, PyObject *args)
{
    PLINT nx, ny;
    TRY (PyArg_ParseTuple(args, "ii", &nx, &ny));
    plssub(nx, ny);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plssym[]="Set symbol height.";

static PyObject * pl_ssym(PyObject *self, PyObject *args)
{
    PLFLT def, scale;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dd", "ff"), &def, &scale));
    plssym(def, scale);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plstar[]="Initialize PLplot, passing in the windows/page settings";

static PyObject * pl_star(PyObject *self, PyObject *args)
{
    PLINT nx, ny;
    TRY (PyArg_ParseTuple(args, "ii", &nx, &ny));
    plstar(nx, ny);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plstart[]="Initialize PLplot, passing the device name and windows/page settings";

static PyObject *pl_start(PyObject *self, PyObject *args)
{
    const char *devname;
    PLINT nx, ny;
    TRY (PyArg_ParseTuple(args, "sii", &devname, &nx, &ny));
    plstart(devname, nx, ny);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plstyl[]="Set up a new line style";

static PyObject * pl_styl(PyObject *self, PyObject *args)
{
    PLINT n, a, b, nms, *mark, *space, spacelen;
    PyObject *markop, *spaceop;
    if (PyArg_ParseTuple(args, "iii", &n, &a, &b))
        plstyl(n, &a, &b);
    else {
        PyErr_Clear();
        TRY (PyArg_ParseTuple(args, "OO", &markop, &spaceop));
        TRY (pl_PyArray_AsIntArray(&markop, &mark, &nms));
        TRY (pl_PyArray_AsIntArray(&spaceop, &space, &spacelen));
        if (nms != spacelen) {
	      PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
            Py_DECREF(markop);
            Py_DECREF(spaceop);
	      return NULL;
        }
        plstyl(nms, mark, space);
        Py_DECREF(markop);
        Py_DECREF(spaceop);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsvpa[]="Sets the edges of the viewport to the specified absolute coordinates";

static PyObject * pl_svpa(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), 
                                              &xmin, &xmax, &ymin, &ymax));
    plsvpa(xmin, xmax, ymin, ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsxax[]="Set x axis labeling parameters";

static PyObject * pl_sxax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, "ii", &digmax, &digits));
    plsxax(digmax, digits);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsxwin[]="Set inferior X window";

static PyObject * pl_sxwin(PyObject *self, PyObject *args)
{
    PLINT window_id;
    TRY (PyArg_ParseTuple(args, "i", &window_id));
    plsxwin(window_id);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsyax[]="Set y axis labeling parameters";

static PyObject * pl_syax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, "ii", &digmax, &digits));
    plsyax(digmax, digits);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plsym[]="Plots array y against x for n points using Hershey symbol \"code\"";

static PyObject * pl_sym(PyObject *self, PyObject *args)
{
    PLINT n, code, ylen;
    PLFLT *x, *y;
    PyObject *xop, *yop;
    TRY (PyArg_ParseTuple(args, "OOi", &xop, &yop, &code));
    TRY (pl_PyArray_AsFloatArray(&xop, &x, &n));
    TRY (pl_PyArray_AsFloatArray(&yop, &y, &ylen));
    if (n != ylen) {
	  PyErr_SetString(PyExc_ValueError, "args 1 and 2 should have the same length");
        Py_DECREF(xop);
        Py_DECREF(yop);
	  return NULL;
    }
    plsym(n, x, y, code);
    Py_DECREF(xop);
    Py_DECREF(yop);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plszax[]="Set z axis labeling parameters";

static PyObject * pl_szax(PyObject *self, PyObject *args)
{
    PLINT digmax, digits;
    TRY (PyArg_ParseTuple(args, "ii", &digmax, &digits));
    plszax(digmax, digits);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_pltext[]="Switches to text screen";

static PyObject * pl_text(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    pltext();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plvasp[]="Sets the edges of the viewport with the given aspect ratio, leaving room for labels";

static PyObject * pl_vasp(PyObject *self, PyObject *args)
{
    PLFLT aspect;
    TRY (PyArg_ParseTuple(args, PL_ARGS("d", "f"), &aspect));
    plvasp(aspect);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plvpas[]="Creates the largest viewport of the specified aspect ratio that fits within the specified normalized subpage coordinates";

static PyObject * pl_vpas(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax, aspect;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddddd", "fffff"), 
                                              &xmin, &xmax, &ymin, &ymax, &aspect));
    plvpas(xmin, xmax, ymin, ymax, aspect);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plvpor[]="Creates a viewport with the specified normalized subpage coordinates";

static PyObject * pl_vpor(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), 
                                              &xmin, &xmax, &ymin, &ymax));
    plvpor(xmin, xmax, ymin, ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plvsta[]="Defines a \"standard\" viewport with seven character heights for the left margin and four character heights everywhere else";

static PyObject * pl_vsta(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ""));
    plvsta();
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plw3d[]="Set up a window for three-dimensional plotting";

static PyObject * pl_w3d(PyObject *self, PyObject *args)
{
    PLFLT basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az;
    TRY (PyArg_ParseTuple(args, PL_ARGS("ddddddddddd", "fffffffffff"),
		                             &basex, &basey, &height, &xmin0, &xmax0, &ymin0, &ymax0,
		                             &zmin0, &zmax0, &alt, &az));
    plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plwid[]="Set pen width";

static PyObject * pl_wid(PyObject *self, PyObject *args)
{
    PLINT width;
    TRY (PyArg_ParseTuple(args, "i", &width));
    plwid(width);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plwind[]="Set up world coordinates of the viewport boundaries (2d plots)";

static PyObject * pl_wind(PyObject *self, PyObject *args)
{
    PLFLT xmin, xmax, ymin, ymax;
    TRY (PyArg_ParseTuple(args, PL_ARGS("dddd", "ffff"), 
                                              &xmin, &xmax, &ymin, &ymax));
    plwind(xmin, xmax, ymin, ymax);
    Py_INCREF(Py_None);
    return Py_None;
}

static char doc_plParseOpts[]="Process PLplot internal options list";

static PyObject * pl_ParseOpts(PyObject *self, PyObject *args)
{
    int argc, status;
    char **argv;
    PLINT mode;
    PyObject *argvlist;
    TRY (PyArg_ParseTuple(args, "O!i", &PyList_Type, &argvlist, &mode));
    TRY (pl_PyList_AsStringArray(argvlist, &argv, &argc));
    status = plParseOpts(&argc, argv, mode);
    TRY (pl_PyList_SetFromStringArray(argvlist, argv, argc));
    PyMem_DEL(argv);
    return Py_BuildValue("i", status);
}

/* 
Not done yet...

static char doc_plParseOpts[]="Process options list

int plParseOpts(int *p_argc, char **argv, PLINT mode, PLOptionTable *option_table,
	    void (*usage_handler) (char *));
*/

static char doc_plSetOpt[]="Process input strings, treating them as an option and argument pair";

static PyObject * pl_SetOpt(PyObject *self, PyObject *args)
{
    char *opt, *optarg;
    int status;
    TRY (PyArg_ParseTuple(args, "ss", &opt, &optarg));
    status = plSetOpt(opt, optarg);
    return Py_BuildValue("i", status);
}

static char doc_plGetCursor[]="Wait for right button mouse event and translate to world coordinates";

static PyObject * pl_GetCursor(PyObject *self, PyObject *args)
{
    PLGraphicsIn gin;
    int translated;
    PyObject *result;
    TRY (PyArg_ParseTuple(args, ""));
    translated = plGetCursor(&gin);
    if (translated)
	  result = Py_BuildValue(PL_ARGS("(dddd)", "(ffff)"), gin.dX, gin.dY, gin.wX, gin.wY);
    else
	  result = Py_BuildValue(PL_ARGS("(dd)", "(ff)"), gin.dX, gin.dY);
    return result;
}

static PyMethodDef pl_methods[] = {
    {"pladv",			pl_adv, 1, doc_pladv},
    {"plarrows",		pl_arrows, 1, doc_plarrows},
    {"plaxes",		pl_axes, 1, doc_plaxes},
    {"plbin",			pl_bin, 1, doc_plbin},
    {"plbop",			pl_bop, 1, doc_plbop},
    {"plbox",			pl_box, 1, doc_plbox},
    {"plbox3",		pl_box3, 1, doc_plbox3},
    {"plclr",			pl_eop, 1, doc_pleop},		/* old name for backward compatibility */
    {"plcol",			pl_col0, 1, doc_plcol0}, 		/* old name for backward compatibility */
    {"plcol0",		pl_col0, 1, doc_plcol0},
    {"plcol1",		pl_col1, 1, doc_plcol1},
    {"plcont",		pl_cont, 1, doc_plcont},
    {"plcont2",		pl_cont2, 1, doc_plcont2},
    {"plcpstrm",		pl_cpstrm, 1, doc_plcpstrm},
    {"pldid2pc",		pl_did2pc, 1, doc_pldid2pc},
    {"pldip2dc",		pl_dip2dc, 1, doc_pldip2dc},
    {"plend",			pl_end, 1, doc_plend},
    {"plend1",		pl_end1, 1, doc_plend1},
    {"plenv",			pl_env, 1, doc_plenv},
    {"pleop",			pl_eop, 1, doc_pleop},
    {"plerrx",			pl_errx, 1, doc_plerrx},
    {"plerry",			pl_erry, 1, doc_plerry},
    {"plfamadv",		pl_famadv, 1, doc_plfamadv},
    {"plfill",			pl_fill, 1, doc_plfill},
    {"plflush",		pl_flush, 1, doc_plflush},
    {"plfont",			pl_font, 1, doc_plfont},
    {"plfontld",		pl_fontld, 1, doc_plfontld},
    {"plgchr",		pl_gchr, 1, doc_plgchr},
    {"plgcol0",		pl_gcol0, 1, doc_plgcol0},
    {"plgcolbg",		pl_gcolbg, 1, doc_plgcolbg},
    {"plgdidev",		pl_gdidev, 1, doc_plgdidev},
    {"plgdiori",		pl_gdiori, 1, doc_plgdiori},
    {"plgdiplt",		pl_gdiplt, 1, doc_plgdiplt},
    {"plgfam",		pl_gfam, 1, doc_plgfam},
    {"plgfnam",		pl_gfnam, 1, doc_plgfnam},
    {"plgpage",		pl_gpage, 1, doc_plgpage},
    {"plgra",			pl_gra, 1, doc_plgra},
    {"plgspa",		pl_gspa, 1, doc_plgspa},
    {"plgstrm",		pl_gstrm, 1, doc_plgstrm},
    {"plgver",		pl_gver, 1, doc_plgver},
    {"plgxax",		pl_gxax, 1, doc_plgxax},
    {"plgyax",		pl_gyax, 1, doc_plgyax},
    {"plgzax",		pl_gzax, 1, doc_plgzax},
    {"plhist",			pl_hist, 1, doc_plhist},
    {"plhls",			pl_hls, 1, doc_plhls},
    {"plinit",			pl_init, 1, doc_plinit},
    {"pljoin",			pl_join, 1, doc_pljoin},
    {"pllab",			pl_lab, 1, doc_pllab},
    {"plline",			pl_line, 1, doc_plline},
    {"plline3",		pl_line3, 1, doc_plline3},
    {"pllsty",			pl_lsty, 1, doc_pllsty},
    {"plmesh",		pl_mesh, 1, doc_plmesh},
    {"plmkstrm",		pl_mkstrm, 1, doc_plmkstrm},
    {"plmtex",		pl_mtex, 1, doc_plmtex},
    {"plot3d",		pl_ot3d, 1, doc_plot3d},
    {"plpage",		pl_bop, 1, doc_plbop},		/* old name for backward compatibility */
    {"plpat",			pl_pat, 1, doc_plpat},
    {"plpoin",		pl_poin, 1, doc_plpoin},
    {"plpoin3",		pl_poin3, 1, doc_plpoin3},
    {"plpoly3",		pl_poly3, 1, doc_plpoly3},
    {"plprec",		pl_prec, 1, doc_plprec},
    {"plpsty",		pl_psty, 1, doc_plpsty},
    {"plptex",		pl_ptex, 1, doc_plptex},
    {"plreplot",		pl_replot, 1, doc_plreplot},
    {"plrgb",			pl_rgb, 1, doc_plrgb},
    {"plrgb1",		pl_rgb1, 1, doc_plrgb1},
    {"plschr",		pl_schr, 1, doc_plschr},
    {"plscmap0n",	pl_scmap0n, 1, doc_plscmap0n},
    {"plscmap1n",	pl_scmap1n, 1, doc_plscmap1n},
    {"plscmap0",		pl_scmap0, 1, doc_plscmap0},
    {"plscmap1",		pl_scmap1, 1, doc_plscmap1},
    {"plscmap1l",	pl_scmap1l, 1, doc_plscmap1l},
    {"plscol0",		pl_scol0, 1, doc_plscol0},
    {"plscolbg",		pl_scolbg, 1, doc_plscolbg},
    {"plscolor",		pl_scolor, 1, doc_plscolor},
    {"plsdev",		pl_sdev, 1, doc_plsdev},
    {"plsdidev",		pl_sdidev, 1, doc_plsdidev},
    {"plsdimap",		pl_sdimap, 1, doc_plsdimap},
    {"plsdiori",		pl_sdiori, 1, doc_plsdiori},
    {"plsdiplt",		pl_sdiplt, 1, doc_plsdiplt},
    {"plsdiplz",		pl_sdiplz, 1, doc_plsdiplz},
    {"plsesc",		pl_sesc, 1, doc_plsesc},
    {"plsfam",		pl_sfam, 1, doc_plsfam},
    {"plsfnam",		pl_sfnam, 1, doc_plsfnam},
    {"plshade",		pl_shade, 1, doc_plshade},
    {"plsmaj",		pl_smaj, 1, doc_plsmaj},
    {"plsmin",		pl_smin, 1, doc_plsmin},
    {"plsori",			pl_sori, 1, doc_plsori},
    {"plspage",		pl_spage, 1, doc_plspage},
    {"plspause",		pl_spause, 1, doc_plspause},
    {"plsstrm",		pl_sstrm, 1, doc_plsstrm},
    {"plssub",		pl_ssub, 1, doc_plssub},
    {"plssym",		pl_ssym, 1, doc_plssym},
    {"plstar",			pl_star, 1, doc_plstar},
    {"plstart",		pl_start, 1, doc_plstart},
    {"plstyl",			pl_styl, 1, doc_plstyl},
    {"plsvpa",		pl_svpa, 1, doc_plsvpa},
    {"plsxax",		pl_sxax, 1, doc_plsxax},
    {"plsxwin",		pl_sxwin, 1, doc_plsxwin},
    {"plsyax",		pl_syax, 1, doc_plsyax},
    {"plsym",			pl_sym, 1, doc_plsym},
    {"plszax",		pl_szax, 1, doc_plszax},
    {"pltext",			pl_text, 1, doc_pltext},
    {"plvasp",		pl_vasp, 1, doc_plvasp},
    {"plvpas",		pl_vpas, 1, doc_plvpas},
    {"plvpor",		pl_vpor, 1, doc_plvpor},
    {"plvsta",		pl_vsta, 1, doc_plvsta},
    {"plw3d",		pl_w3d, 1, doc_plw3d},
    {"plwid",			pl_wid, 1, doc_plwid},
    {"plwind",		pl_wind, 1, doc_plwind},
    {"plParseOpts",	pl_ParseOpts, 1, doc_plParseOpts},
    {"plSetOpt",		pl_SetOpt, 1, doc_plSetOpt},
    {"plGetCursor",	pl_GetCursor, 1, doc_plGetCursor},

    /* These are a few functions from plplotP.h that I found I needed
       to call in order to get information about the viewport, etc. */

    {"plPgvpw",		pl_Pgvpw, 1, doc_plPgvpw},
    {NULL,			NULL}
};

void initpl()
{
    PyObject *m;
    PyObject *d;
    PyObject *v;

    import_array();

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
