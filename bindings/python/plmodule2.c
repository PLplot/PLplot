#include "plmodule.h"

#define PyAssert(c,m) if (!(c)) { PyErr_SetString( PyExc_RuntimeError, m ); \
                                  return NULL; }

/*static*/ char doc_plcont[] = "Draws a contour plot from data in z(x,y)";

/*
  Allowable syntaxes:

  plcont( z, kx, lx, ky, ly, clev, pltr, xg, yg, wrap )
  plcont( z, clev, pltr, xg, yg, wrap )

  where in both cases, parameters from pltr to the end are optional.

  First case, arg count = 6 +3? +1?
  Second case, arg count = 2 +3? +1?

  The only way I see to disambiguate this is to either be able to
  nondestructively hand parse the arg list, or to mandate fewer valid forms.
  Since I cannot find info on how to do a nondestructive trial parse, we opt
  for the second.  Thus, by fiat, I declare the following forms to be
  admissible:

  plcont( z, kx, lx, ky, ly, clev, [pltr, xg, yg, wrap] )
  plcont( z, clev, pltr, xg, yg, wrap )
 */

/*static*/ PyObject *pl_cont( PyObject *self, PyObject *args )
{
    PLINT nx, ny, kx=0, lx=0, ky=0, ly=0, nlev;
    int i=0, j;
    PLFLT **z, *clev;	
    PyObject *op, *levelop, *f;
    PyObject *zop, *cop, *xop, *yop;
    PLfGrid2 grid;
    char *pltrname = "pltr0";
    void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer);
    PLPointer pltr_data = NULL;
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;
    int match=0, wrap=0;

    int argc = PyTuple_Size( args );
    PyObject *o;

    PyAssert( i < argc, "Invalid arg list for plcont" );

    zop = PyTuple_GetItem( args, 0 ); i++;
    TRY( pl_PyArray_AsFloatMatrix( &zop, &nx, &ny, &z ) );

    grid.f = z;
    grid.nx = nx;
    grid.ny = ny;

    PyAssert( i < argc, "Invalid arg list for plcont" );

    o = PyTuple_GetItem( args, i );

    if ( o && PyArray_Check(o) ) {
    /* They skipped all the way to clev */
	kx = 1; lx = nx;
	ky = 1; ly = ny;
    }
    else {
	PyAssert( i+3 < argc, "Invalid arg list for plcont" );
/* 	TRY( PyArg_GetLong( args, argc, i++, &kx ) ); */
	TRY( kx = PyLong_AsLong( PyTuple_GetItem( args, i++ ) ) );
/* 	TRY( PyArg_GetLong( args, argc, i++, &lx ) ); */
	TRY( lx = PyLong_AsLong( PyTuple_GetItem( args, i++ ) ) );
/* 	TRY( PyArg_GetLong( args, argc, i++, &ky ) ); */
	TRY( ky = PyLong_AsLong( PyTuple_GetItem( args, i++ ) ) );
/* 	TRY( PyArg_GetLong( args, argc, i++, &ly ) ); */
	TRY( ly = PyLong_AsLong( PyTuple_GetItem( args, i++ ) ) );
	o = PyTuple_GetItem( args, i );
    }

    printf( "plcont, kx=%d lx=%d ky=%d ly=%d\n", kx, lx, ky, ly );

/* Must now be positioned on clev, so pull it out and convert. */

    TRY( pl_PyArray_AsFloatArray( &o, &clev, &nlev ) );

    printf( "nlev=%d\n", nlev );

    i++;

/* Now parse the pltr stuff. */

    if ( i <= argc ) {
/* 	pltrname = PyArg_GetString( PyTuple_GetItem( args, i++ ) ); */
/* 	TRY( PyArg_GetString( args, argc, i++, &pltrname ) ); */
	TRY( pltrname = PyString_AsString( PyTuple_GetItem( args, i++ ) ) );
	printf( "pltr=%s\n", pltrname );
	if ( i < argc-1 ) {
	/* Then there must be at least two args left, which means the xg, yg
           stuff has been specified. */

	    xop = PyTuple_GetItem( args, i++ );
	    yop = PyTuple_GetItem( args, i++ );
	}

	if (i == argc) {
/* 	    TRY( PyArg_GetLong( args, argc, i, &wrap ) ); */
            TRY( wrap = PyLong_AsLong( PyTuple_GetItem( args, i ) ) );
	}
    }

    printf( "wrap=%d\n", wrap );

/* Figure out which coordinate transformation model is being used, and setup
   accordingly. */

    if (!pltrname || !strcmp( pltrname, "pltr0")) {
	pltr = pltr0;

	if (wrap) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Must not specify wrapping with pltr0." );
	    return NULL;
	}
    }
    else if ( !strcmp( pltrname, "pltr1" ) ) {
	pltr = pltr1;
	pltr_data = &cgrid1;

    /* Check wrap. */
	if (wrap) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Must not specify wrapping with pltr0." );
	    return NULL;
	}

    /* Check dimensionality of xg1 and xg2 */
	TRY( pl_PyArray_AsFloatArray( &xop, &cgrid1.xg, &cgrid1.nx ) );
	TRY( pl_PyArray_AsFloatArray( &yop, &cgrid1.yg, &cgrid1.ny ) );
	if ( (cgrid1.nx != nx) || (cgrid1.ny != ny) ) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Incompatible coord specifiers." );
	    return NULL;
	}
    }
    else if ( !strcmp( pltrname, "pltr2" ) ) {
    /* printf( "plshade, setting up for pltr2\n" ); */
	if (!wrap) {
 	/* printf( "plshade, no wrapping is needed.\n" ); */
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx, ny );
	    plAlloc2dGrid( &cgrid2.yg, nx, ny );
	    cgrid2.nx = nx;
	    cgrid2.ny = ny;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}
	}
	else if (wrap == 1) {
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx+1, ny );
	    plAlloc2dGrid( &cgrid2.yg, nx+1, ny );
	    cgrid2.nx = nx+1;
	    cgrid2.ny = ny;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}

	    for( j=0; j < ny; j++ ) {
		cgrid2.xg[nx][j] = cgrid2.xg[0][j];
		cgrid2.yg[nx][j] = cgrid2.yg[0][j];
	    }

	    nx++;
	}
	else if (wrap == 2) {
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx, ny+1 );
	    plAlloc2dGrid( &cgrid2.yg, nx, ny+1 );
	    cgrid2.nx = nx;
	    cgrid2.ny = ny+1;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}

	    for( i=0; i < nx; i++ ) {
		cgrid2.xg[i][ny] = cgrid2.xg[i][0];
		cgrid2.yg[i][ny] = cgrid2.yg[i][0];
	    }

	    ny++;
	}
	else {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Invalid wrap specifier, must be 0, 1 or 2." );
	    return NULL;
	}

	pltr = pltr2;
	pltr_data = &cgrid2;
    }
    else {
	PyErr_SetString( PyExc_RuntimeError,
			 "Unrecognized coordinate transformation spec.  Must be pltr0, pltr1 or pltr2." );
	return NULL;
    }

/* Now go make the plot. */

/*     plcont( z, nx, ny, kx, lx, ky, ly, clev, nlev, pltr, pltr_data ); */
    plfcont( pyf2eval2, &grid, nx, ny, kx, lx, ky, ly,
	     clev, nlev, pltr, pltr_data );

/*
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
    */
/* Now free up any space which got allocated for our coordinate trickery. */

    if (pltr == pltr1) {
    /* Hmm, actually, nothing to do here currently, since we just used the
        Python Matrix data directly, rather than allocating private space. */
    }
    else if (pltr == pltr2) {
    /* printf( "plshade, freeing space for grids used in pltr2\n" ); */
	plFree2dGrid( cgrid2.xg, nx, ny );
	plFree2dGrid( cgrid2.yg, nx, ny );
    }

    plflush();
    Py_INCREF(Py_None);
    return Py_None;
}

/* GMF 8/12/96, this implementation cobbled together from tclAPI.c */
/*--------------------------------------------------------------------------*\
 * plshadeCmd
 *
 * Processes plshade Tcl command.
 * C version takes:
 *    data, nx, ny, defined,
 *    xmin, xmax, ymin, ymax,
 *    sh_min, sh_max, sh_cmap, sh_color, sh_width,
 *    min_col, min_wid, max_col, max_wid,
 *    plfill, rect, pltr, pltr_data
 *
 * We will be getting data through a 2-d Matrix, which carries along
 * nx and ny, so no need for those.  Toss defined since it's not supported
 * anyway.  Toss plfill since it is the only valid choice.  Take an optional 
 * pltr spec just as for plcont, and add a wrapping specifier, also just as
 * in plcont.  So the new command looks like:
 * 
 * 	plshade z xmin xmax ymin ymax \
 * 	    sh_min sh_max sh_cmap sh_color sh_width \
 * 	    min_col min_wid max_col max_wid \
 * 	    rect [pltr x y] [wrap]
\*--------------------------------------------------------------------------*/

/*static*/ char doc_plshade[] = "Color fill plot";

/*static*/ PyObject *pl_shade( PyObject *self, PyObject *args )
{
    PyObject *zop, *xop=NULL, *yop=NULL;
    PLfGrid2 grid;
    PLFLT **z;
    PLINT nx, ny;
    PLFLT xmin, xmax, ymin, ymax, sh_min, sh_max, sh_col;

    PLINT sh_cmap =1, sh_wid =2;
    PLINT min_col =1, min_wid =0, max_col =0, max_wid =0;
    PLINT rect =1;
    char *pltrname = NULL;
    void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer);
    PLPointer pltr_data = NULL;
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;
    PLINT wrap = 0;
    int i, j;

    TRY( PyArg_ParseTuple( args, PL_ARGS( "Oddddddidiiiiii|sOOi",
					  "Offffffifiiiiii|sOOi" ),
			   &zop, &xmin, &xmax, &ymin, &ymax, &sh_min,
			   &sh_max, &sh_cmap, &sh_col, &sh_wid,
			   &min_col, &min_wid, &max_col, &max_wid, &rect,
			   &pltrname, &xop, &yop, &wrap ) );

    TRY( pl_PyArray_AsFloatMatrix( &zop, &nx, &ny, &z ) );

    grid.f = z;
    grid.nx = nx;
    grid.ny = ny;

/* Figure out which coordinate transformation model is being used, and setup
   accordingly. */

    if (!pltrname || !strcmp( pltrname, "pltr0")) {
	pltr = pltr0;

	if (wrap) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Must not specify wrapping with pltr0." );
	    return NULL;
	}
    }
    else if (!strcmp( pltrname, "pltr1" ) ) {
	pltr = pltr1;
	pltr_data = &cgrid1;

    /* Check wrap. */
	if (wrap) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Must not specify wrapping with pltr0." );
	    return NULL;
	}

    /* Check dimensionality of xg1 and xg2 */
	TRY( pl_PyArray_AsFloatArray( &xop, &cgrid1.xg, &cgrid1.nx ) );
	TRY( pl_PyArray_AsFloatArray( &yop, &cgrid1.yg, &cgrid1.ny ) );
	if ( (cgrid1.nx != nx) || (cgrid1.ny != ny) ) {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Incompatible coord specifiers." );
	    return NULL;
	}
    }
    else if ( !strcmp( pltrname, "pltr2" ) ) {
    /* printf( "plshade, setting up for pltr2\n" ); */
	if (!wrap) {
 	/* printf( "plshade, no wrapping is needed.\n" ); */
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx, ny );
	    plAlloc2dGrid( &cgrid2.yg, nx, ny );
	    cgrid2.nx = nx;
	    cgrid2.ny = ny;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}
	}
	else if (wrap == 1) {
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx+1, ny );
	    plAlloc2dGrid( &cgrid2.yg, nx+1, ny );
	    cgrid2.nx = nx+1;
	    cgrid2.ny = ny;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}

	    for( j=0; j < ny; j++ ) {
		cgrid2.xg[nx][j] = cgrid2.xg[0][j];
		cgrid2.yg[nx][j] = cgrid2.yg[0][j];
	    }

	    nx++;
	}
	else if (wrap == 2) {
	    PLINT xnx, xny, ynx, yny;
	    PLFLT **xg, **yg;
	    TRY( pl_PyArray_AsFloatMatrix( &xop, &xnx, &xny, &xg ) );
	    TRY( pl_PyArray_AsFloatMatrix( &yop, &ynx, &yny, &yg ) );

	    if ( (xnx != nx) || (ynx != nx) || (xny != ny) || (yny != ny) ) {
		PyErr_SetString( PyExc_RuntimeError,
				 "Bogus transformation arrays." );
		return NULL;
	    }

	    plAlloc2dGrid( &cgrid2.xg, nx, ny+1 );
	    plAlloc2dGrid( &cgrid2.yg, nx, ny+1 );
	    cgrid2.nx = nx;
	    cgrid2.ny = ny+1;

	    for( i=0; i < nx; i++ )
		for( j=0; j < ny; j++ ) {
		    cgrid2.xg[i][j] = xg[i][j];
		    cgrid2.yg[i][j] = yg[i][j];
		}

	    for( i=0; i < nx; i++ ) {
		cgrid2.xg[i][ny] = cgrid2.xg[i][0];
		cgrid2.yg[i][ny] = cgrid2.yg[i][0];
	    }

	    ny++;
	}
	else {
	    PyErr_SetString( PyExc_RuntimeError,
			     "Invalid wrap specifier, must be 0, 1 or 2." );
	    return NULL;
	}

	pltr = pltr2;
	pltr_data = &cgrid2;
    }
    else {
	PyErr_SetString( PyExc_RuntimeError,
			 "Unrecognized coordinate transformation spec.  Must be pltr0, pltr1 or pltr2." );
	return NULL;
    }

/* Now go make the plot. */

    plfshade( pyf2eval2, &grid, NULL, NULL, nx, ny,
	      xmin, xmax, ymin, ymax,
	      sh_min, sh_max, sh_cmap, sh_col, sh_wid,
	      min_col, min_wid, max_col, max_wid,
	      plfill, rect, pltr, pltr_data );

/* Now free up any space which got allocated for our coordinate trickery. */

    if (pltr == pltr1) {
    /* Hmm, actually, nothing to do here currently, since we just used the
        Python Matrix data directly, rather than allocating private space. */
    }
    else if (pltr == pltr2) {
    /* printf( "plshade, freeing space for grids used in pltr2\n" ); */
	plFree2dGrid( cgrid2.xg, nx, ny );
	plFree2dGrid( cgrid2.yg, nx, ny );
    }

    plflush();
    Py_INCREF( Py_None );
    return Py_None;
}
