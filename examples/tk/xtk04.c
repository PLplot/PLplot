/* $Id$
 * $Log$
 * Revision 1.4  1995/06/09 22:50:19  mjl
 * Eliminated vestigial lookup of tclMatrix variable (no longer works).
 *
 * Revision 1.3  1995/06/01  21:43:25  mjl
 * Change to header file inclusion: to get PLplot/Tk global function
 * prototypes, must now include pltk.h.  Some cleaning up.
 *
 * Revision 1.2  1994/10/10  19:45:04  furnish
 * Imlemented plshade from Tcl.
 *
 * Revision 1.1  1994/10/10  17:22:51  furnish
 * New Tk demo to show off the 2-d api.
 *
 */

#include "pltk.h"
#include <itcl.h>
#include <math.h>

static int
AppInit(Tcl_Interp *interp);

int mat_max( tclMatrix *pm, Tcl_Interp *interp,
	     int argc, char *argv[] )
{
    float max = pm->fdata[0];
    int i;
    for( i=1; i < pm->len; i++ )
	if (pm->fdata[i] > max)
	    max = pm->fdata[i];

    sprintf( interp->result, "%f", max );
    return TCL_OK;
}

int mat_min( tclMatrix *pm, Tcl_Interp *interp,
	     int argc, char *argv[] )
{
    float min = pm->fdata[0];
    int i;
    for( i=1; i < pm->len; i++ )
	if (pm->fdata[i] < min)
	    min = pm->fdata[i];

    sprintf( interp->result, "%f", min );
    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 * main --
 *
 * Just a stub routine to call pltkMain.  The latter is nice to have
 * when building extended wishes, since then you don't have to rely on
 * sucking the Tk main out of libtk (which doesn't work correctly on all
 * systems/compilers/linkers/etc).  Hopefully in the future Tk will
 * supply a sufficiently capable tkMain() type function that can be used
 * instead. 
\*--------------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
    exit(pltkMain(argc, argv, NULL, AppInit));
}

/*
 *--------------------------------------------------------------------------
 *
 * AppInit --
 *
 *	This procedure performs application-specific initialization.
 *	Most applications, especially those that incorporate additional
 *	packages, will have their own version of this procedure.
 *
 * Results:
 *	Returns a standard Tcl completion code, and leaves an error
 *	message in interp->result if an error occurs.
 *
 * Side effects:
 *	Depends on the startup script.
 *
 * Taken from tkAppInit.c --
 *
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *--------------------------------------------------------------------------
 */

int   get_dataCmd        (ClientData, Tcl_Interp *, int, char **);

static int
AppInit(Tcl_Interp *interp)
{
    Tk_Window main;

    main = Tk_MainWindow(interp);

/*
 * Call the init procedures for included packages.  Each call should
 * look like this:
 *
 * if (Mod_Init(interp) == TCL_ERROR) {
 *     return TCL_ERROR;
 * }
 *
 * where "Mod" is the name of the module.
 */

    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Itcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Pltk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

/*
 * Call Tcl_CreateCommand for application-specific commands, if
 * they weren't already created by the init procedures called above.
 */

    Tcl_CreateCommand(interp, "get_data", get_dataCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

    Tcl_MatrixInstallXtnsn( "max", mat_max );
    Tcl_MatrixInstallXtnsn( "min", mat_min );

    return TCL_OK;
}

int   get_dataCmd( ClientData cd, Tcl_Interp *interp, int argc, char **argv )
{
    tclMatrix *pm, *matPtr;
    int nx, ny, i, j;
    float pi = 3.1415927;
    int kx = 3, ky = 2;

    pm = Tcl_GetMatrixPtr( interp, argv[1] );
    matPtr = pm;

    if ( pm->dim != 2 ) {
	interp->result = "must use 2-d matrix.";
	return TCL_ERROR;
    }

    nx = pm->n[0], ny = pm->n[1];

    for( i=0; i < nx; i++ ) {
	for( j=0; j < ny; j++ ) {
	    pm->fdata[I2D(i,j)] = sin(pi*kx*i/64) * sin(pi*ky*j/64);
	}
    }

    return TCL_OK;
}
