/* 
/* $Id$
 * $Log$
 * Revision 1.3  1994/05/26 22:38:07  mjl
 * Added missing CVS Id and Log fields.
 *
 *
 * tkAppInit.c --
 *
 *	Provides a default version of the Tcl_AppInit procedure for
 *	use in wish and similar Tk-based applications.
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
 */

#ifndef lint
static char rcsid[] = "$Header$ SPRITE (Berkeley)";
#endif /* not lint */

#include "tk.h"

/*
 * The following variable is a special hack that allows applications
 * to be linked using the procedure "main" from the Tk library.  The
 * variable generates a reference to "main", which causes main to
 * be brought in from the library (and all of Tk and Tcl with it).
 */

extern int main();
int *tclDummyMainPtr = (int *) main;

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppInit --
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
 *----------------------------------------------------------------------
 */

int   myplotCmd        (ClientData, Tcl_Interp *, int, char **);

int
Tcl_AppInit(interp)
    Tcl_Interp *interp;		/* Interpreter for application. */
{
int   plFrameCmd        (ClientData, Tcl_Interp *, int, char **);

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
    if (Pltk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    Tcl_CreateCommand(interp, "myplot", myplotCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);


    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    tcl_RcFileName = "~/.wishrc";
    return TCL_OK;
}

void myplot1();
void myplot2();
void myplot3();
void myplot4();

/* Plots several simple functions */
/* Note the compiler should automatically convert all non-pointer arguments
   to satisfy the prototype, but some have problems with constants. */

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int
xxx_main(int argc, char *argv[])
{
    PLINT digmax;
    char ver[80];

/* plplot initialization */
/* Divide page into 2x2 plots unless user overrides */

    plssub(2, 2);

/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

/* Get version number, just for kicks */

    plgver(ver);
    printf("Plplot library version: %s\n", ver);

/* Initialize plplot */

    plinit();

/* Set up the data */
/* Original case */

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;

/* Do a plot */

    plot1();

/* Set up the data */

    xscale = 1.;
    yscale = 0.0014;
    yoff = 0.0185;

/* Do a plot */

    digmax = 5;
    plsyax(digmax, 0);
    plot1();

    plot2();

    plot3();

/* Don't forget to call PLEND to finish off! */

    plend();
    exit(0);
}

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
    pllab("(x)", "(y)", "#frPLPLOT Example 1 - y=x#u2");

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
    pllab("(x)", "sin(x)/x", "#frPLPLOT Example 1 - Sinc Function");

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
    pllab("Angle (degrees)", "sine", "#frPLPLOT Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
	x[i] = 3.6 * i;
	y[i] = sin(x[i] * 3.141592654 / 180.0);
    }

    plcol(4);
    plline(101, x, y);
}

int   myplotCmd( ClientData cd, Tcl_Interp *interp, int argc, char **argv )
{
/*
    int i;
    printf( "argc = %d\n", argc );

    for( i=0; i < argc; i++ )
      printf( "argv[%d] = %s\n", i, argv[i] );
      */

    if (!strcmp(argv[1],"1"))
      myplot1();

    if (!strcmp(argv[1],"2"))
      myplot2();

    if (!strcmp(argv[1],"3"))
      myplot3();

    if (!strcmp(argv[1],"4"))
      myplot4();

    plflush();
    return TCL_OK;
}
