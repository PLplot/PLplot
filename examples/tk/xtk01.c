/* $Id$
 * $Log$
 * Revision 1.10  1996/06/26 21:35:21  furnish
 * Various hacks to support Tcl 7.5 and Tk 4.1.
 *
 * Revision 1.9  1995/06/01  21:43:48  mjl
 * Change to header file inclusion: to get PLplot/Tk global function
 * prototypes, must now include pltk.h.  Some cleaning up and resource leaks
 * plugged.
 *
 * Revision 1.8  1995/03/16  23:23:14  mjl
 * Changed one of the plots to a plshade demo, in order to make it easier to
 * investigate cmap1-related behavior when the widget is being driven directly.
 *
 * Revision 1.7  1994/09/23  07:47:29  mjl
 * Modified to use the new syntax for pltkMain().
 */

#include "pltk.h"
#include <math.h>

static int
AppInit(Tcl_Interp *interp);

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

int   myplotCmd        (ClientData, Tcl_Interp *, int, char **);

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
/*
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    */
    if (Pltk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

/*
 * Call Tcl_CreateCommand for application-specific commands, if
 * they weren't already created by the init procedures called above.
 */

    Tcl_CreateCommand(interp, "myplot", myplotCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

    return TCL_OK;
}

 /* =============================================================== */

/*	Taken from:
	x01c.c

	Simple line plot and multiple windows demo.
*/

static PLFLT xs[6] =
{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
static PLFLT ys[6] =
{1.0, 4.0, 9.0, 16.0, 25.0, 36.0};
static PLFLT x[101], y[101];
static PLFLT xscale, yscale, xoff, yoff, xs1[6], ys1[6];
static PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;

void plot1(void);
void plot2(void);
void plot3(void);

void myplot1()
{
/* Set up the data */
/* Original case */

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;

/* Do a plot */

    plot1();
}

void myplot2()
{
    PLINT digmax;

/* Set up the data */

    xscale = 1.;
    yscale = 0.0014;
    yoff = 0.0185;

/* Do a plot */

    digmax = 5;
    plsyax(digmax, 0);
    plot1();
}

void myplot3()
{
    plot2();
}

void myplot4()
{
    plot3();
}

 /* =============================================================== */

void
plot1(void)
{
    int i;
    PLFLT xmin, xmax, ymin, ymax;

    for (i = 0; i < 60; i++) {
	x[i] = xoff + xscale * (i + 1) / 60.0;
	y[i] = yoff + yscale * pow(x[i], 2.);
    }

    xmin = x[0];
    xmax = x[59];
    ymin = y[0];
    ymax = y[59];

    for (i = 0; i < 6; i++) {
	xs1[i] = x[i * 10 + 3];
	ys1[i] = y[i * 10 + 3];
    }

/* Set up the viewport and window using PLENV. The range in X is */
/* 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are */
/* scaled separately (just = 0), and we just draw a labelled */
/* box (axis = 0). */

    plcol(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol(6);
    pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

/* Plot the data points */

    plcol(9);
    plpoin(6, xs1, ys1, 9);

/* Draw the line through the data */

    plcol(4);
    plline(60, x, y);
}

 /* =============================================================== */

void
plot2(void)
{
    int i;

/* Set up the viewport and window using PLENV. The range in X is -2.0 to
   10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
   (just = 0), and we draw a box with axes (axis = 1). */

    plcol(1);
    plenv((PLFLT) -2.0, (PLFLT) 10.0, (PLFLT) -0.4, (PLFLT) 1.2, 0, 1);
    plcol(2);
    pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

/* Fill up the arrays */

    for (i = 0; i < 100; i++) {
	x[i] = (i - 19.0) / 6.0;
	y[i] = 1.0;
	if (x[i] != 0.0)
	    y[i] = sin(x[i]) / x[i];
    }

/* Draw the line */

    plcol(3);
    plline(100, x, y);

}

 /* =============================================================== */

void
plot3(void)
{
    int i;

/* For the final graph we wish to override the default tick intervals, and
   so do not use PLENV */

    pladv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
       from -1.2 to 1.2. */

    plvsta();
    plwind((PLFLT) 0.0, (PLFLT) 360.0, (PLFLT) -1.2, (PLFLT) 1.2);

/* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

    plcol(1);
    plbox("bcnst", (PLFLT) 60.0, 2, "bcnstv", (PLFLT) 0.2, 2);

/* Superimpose a dashed line grid, with 1.5 mm marks and spaces. plstyl
   expects a pointer!! */

    plstyl(1, &mark1, &space1);
    plcol(2);
    plbox("g", (PLFLT) 30.0, 0, "g", (PLFLT) 0.2, 0);
    plstyl(0, &mark0, &space0);

    plcol(3);
    pllab("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
	x[i] = 3.6 * i;
	y[i] = sin(x[i] * 3.141592654 / 180.0);
    }

    plcol(4);
    plline(101, x, y);
}

 /* =============================================================== */

/*	Taken from:
	x16c.c

	plshade demo, using color fill.

	Maurice LeBrun
	IFS, University of Texas at Austin
	20 Mar 1994
*/

#define NCONTR	30		/* Number of contours */
#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Datat points in y */

#define XSPA    2./(XPTS-1)
#define YSPA    2./(YPTS-1)

static PLFLT clevel[NCONTR];

/* Utility macros */

#ifndef PI
#define PI	3.1415926535897932384
#endif
#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/* Transformation function */

PLFLT tr[6] =
{XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

static void
mypltr(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data)
{
    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

/* Function prototypes */

static void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax);

/*--------------------------------------------------------------------------*\
 * shade
 *
 * Does several shade plots using different coordinate mappings.
\*--------------------------------------------------------------------------*/

int
shade(void)
{
    int i, j;
    PLFLT x, y, argx, argy, distort;

    PLFLT **z, **w, zmin, zmax;
    PLFLT xg1[XPTS], yg1[YPTS];
    PLcGrid  cgrid1;
    PLcGrid2 cgrid2;

    PLFLT shade_min, shade_max, sh_color;
    PLINT sh_cmap = 1, sh_width;
    PLINT min_color = 1, min_width = 0, max_color = 0, max_width = 0;

/* Set up function arrays */

    plAlloc2dGrid(&z, XPTS, YPTS);
    plAlloc2dGrid(&w, XPTS, YPTS);

/* Set up data array */

    for (i = 0; i < XPTS; i++) {
	x = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	for (j = 0; j < YPTS; j++) {
	    y = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;

	    z[i][j] = - sin(7.*x) * cos(7.*y) + x*x - y*y;
	    w[i][j] = - cos(7.*x) * sin(7.*y) + 2 * x * y;
	}
    }
    f2mnmx(z, XPTS, YPTS, &zmin, &zmax);
    for (i = 0; i < NCONTR; i++)
	clevel[i] = zmin + (zmax - zmin) * (i + 0.5) / (float) NCONTR;

/* Set up coordinate grids */

    cgrid1.xg = xg1;
    cgrid1.yg = yg1;
    cgrid1.nx = XPTS;
    cgrid1.ny = YPTS;

    plAlloc2dGrid(&cgrid2.xg, XPTS, YPTS);
    plAlloc2dGrid(&cgrid2.yg, XPTS, YPTS);
    cgrid2.nx = XPTS;
    cgrid2.ny = YPTS;

    for (i = 0; i < XPTS; i++) {
	for (j = 0; j < YPTS; j++) {
	    mypltr((PLFLT) i, (PLFLT) j, &x, &y, NULL);

	    argx = x * PI/2;
	    argy = y * PI/2;
	    distort = 0.4;

	    cgrid1.xg[i] = x + distort * cos(argx);
	    cgrid1.yg[j] = y - distort * cos(argy);

	    cgrid2.xg[i][j] = x + distort * cos(argx) * cos(argy);
	    cgrid2.yg[i][j] = y - distort * cos(argx) * cos(argy);
	}
    }

/* Plot using identity transform */

    pladv(0);
    plvpor(0.1, 0.9, 0.1, 0.9);
    plwind(-1.0, 1.0, -1.0, 1.0);

    for (i = 0; i < NCONTR; i++) {
	shade_min = zmin + (zmax - zmin) * i / (float) NCONTR;
	shade_max = zmin + (zmax - zmin) * (i +1) / (float) NCONTR;
	sh_color = i / (float) (NCONTR-1);
	sh_width = 2;
	plpsty(0);

	plshade(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
		shade_min, shade_max, 
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		plfill, 1, NULL, NULL);
    }

    plcol(1);
    plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
    plcol(2);
/*
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, NCONTR, mypltr, NULL);
    */
    pllab("distance", "altitude", "Bogon density");

/* Clean up */

    plFree2dGrid(z, XPTS, YPTS);
    plFree2dGrid(w, XPTS, YPTS);
    plFree2dGrid(cgrid2.xg, XPTS, YPTS);
    plFree2dGrid(cgrid2.yg, XPTS, YPTS);
}

/*--------------------------------------------------------------------------*\
 * f2mnmx
 *
 * Returns min & max of input 2d array.
\*--------------------------------------------------------------------------*/

static void
f2mnmx(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = f[0][0];
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
            *fmax = MAX(*fmax, f[i][j]);
            *fmin = MIN(*fmin, f[i][j]);
	}
    }
}
 /* =============================================================== */

int   myplotCmd( ClientData cd, Tcl_Interp *interp, int argc, char **argv )
{
    if (!strcmp(argv[1],"1"))
      myplot1();

    if (!strcmp(argv[1],"2"))
      myplot2();

    if (!strcmp(argv[1],"3"))
      myplot3();

    if (!strcmp(argv[1],"4"))
      shade();

    plflush();
    return TCL_OK;
}
