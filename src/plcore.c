/* $Id$
 * $Log$
 * Revision 1.19  1993/08/31 20:14:30  mjl
 * Fix to plend1() so that prematurely aborted streams do not result in a
 * core dump.  Put font loading code before plsc->level is set to 1, to
 * prevent core dumps when fonts are not found.
 *
 * Revision 1.18  1993/08/18  20:33:22  mjl
 * Many changes to driver interface to properly modify the device window
 * based on orientation and coordinate mapping.  Orientation switches now
 * automatically set the device window so as to preserve the aspect ratio.
 * Ditto for coordinate mapping (only used when copying stream parameters
 * from one to another, such as used when dumping a plot to disk from the TK
 * driver).  Switched over to new page description variables mar, aspect, jx,
 * and jy, and deleted the old ones.  Added a variable widthlock that is set
 * when -width is used to modify the pen width, so that subsequent plwid()
 * calls are ignored.
 *
 * Revision 1.17  1993/08/11  19:19:03  mjl
 * Changed debugging code to print to stderr instead of stdout, fixed
 * up some minor type mismatches.
 *
 * Revision 1.16  1993/08/09  22:20:34  mjl
 * Fixed plcpstrm() so that it no longer sucks.
 *
 * Revision 1.15  1993/08/03  01:46:58  mjl
 * Changes to eliminate warnings when compiling with gcc -Wall.
 *
 * Revision 1.14  1993/07/31  08:17:37  mjl
 * Action for driver interface setup functions now deferred until after
 * plinit() called so that these can be set via command line arguments.
 * Drivers that request it can handle these directly.
 *
 * Revision 1.13  1993/07/28  05:53:23  mjl
 * Put in code to ensure all malloc'ed memory is freed upon exit.
 *
 * Revision 1.12  1993/07/16  22:35:05  mjl
 * Addition of driver interface function for converting between device
 * coordinate systems, functions for retrieving current driver interface
 * parameters.  Also added functions for creating a new stream, copying
 * an existing stream, replaying the current plot, returning a list of
 * file-oriented devices.
 *
 * Revision 1.11  1993/07/01  22:25:16  mjl
 * Changed all plplot source files to include plplotP.h (private) rather
 * than plplot.h.  Rationalized namespace -- all externally-visible
 * internal plplot functions now start with "plP_".  Moved functions
 * plend() and plend1() to here.  Added driver interface -- sits
 * between the plplot library calls and the driver to filter the data in
 * various ways, such as to support zooms, page margins, orientation
 * changes, etc.  Changed line and polyline draw functions to go through
 * this layer.  Changed all references to the current plplot stream to be
 * through "plsc", which is set to the location of the current stream (now
 * dynamically allocated).  A table of stream pointers (max of 100) is
 * kept to allow switching between streams as before.
 *
 * Revision 1.10  1993/04/26  19:57:58  mjl
 * Fixes to allow (once again) output to stdout and plrender to function as
 * a filter.  A type flag was added to handle file vs stream differences.
*/

/*	plcore.c

	Central dispatch facility for plplot.
	Also contains the plplot main data structures, external access
	routines, and initialization calls.

	This stuff used to be in "dispatch.h", "dispatch.c", and "base.c".
*/

#include "plcore.h"

/*----------------------------------------------------------------------*\
* Driver Interface
*
* These routines are the low-level interface to the driver -- all calls
* to driver functions must pass through here.  For implementing driver-
* specific functions, the escape function is provided.  The command stream
* gets duplicated to the plot buffer here.
*
* All functions that result in graphics actually being plotted (rather
* than just a change of state) are filtered as necessary before being
* passed on.  The default settings do not require any filtering, i.e.
* plplot physical coordinates are the same as the device physical
* coordinates (currently this can't be changed anyway), and a global view
* equal to the entire page is used.
*
* The reason one wants to put view-specific filtering here is that if
* enabled, the plot buffer should receive the unfiltered data stream.
* This allows a specific view to be used from an interactive device
* (e.g. TCL/TK driver) but be restored to the full view at any time
* merely by reprocessing the contents of the plot buffer.
*
* The metafile, on the other hand, *should* be affected by changes in the
* view, since this is a crucial editing capability.  It is recommended
* that the initial metafile be created without a restricted global view,
* and modification of the view done on a per-plot basis as desired during
* subsequent processing.
*
\*----------------------------------------------------------------------*/

/* Initialize device. */
/* The plot buffer must be called last */

void
plP_init(void)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_init) (plsc);

    if (plsc->plbuf_write)
	plbuf_init(plsc);
}

/* End of page */
/* The plot buffer must be called first */

void
plP_eop(void)
{
    if (plsc->plbuf_write)
	plbuf_eop(plsc);

    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_eop) (plsc);
}

/* Set up new page. */
/* The plot buffer must be called last */

void
plP_bop(void)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_bop) (plsc);

    if (plsc->plbuf_write)
	plbuf_bop(plsc);
}

/* Tidy up device (flush buffers, close file, etc.) */

void
plP_tidy(void)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_tidy) (plsc);

    if (plsc->plbuf_write)
	plbuf_tidy(plsc);
}

/* Change state. */

void
plP_state(PLINT op)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_state) (plsc, op);

    if (plsc->plbuf_write)
	plbuf_state(plsc, op);
}

/* Escape function, for driver-specific commands. */

void
plP_esc(PLINT op, void *ptr)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_esc) (plsc, op, ptr);

    if (plsc->plbuf_write)
	plbuf_esc(plsc, op, ptr);
}

/*----------------------------------------------------------------------*\
*  Drawing commands.
\*----------------------------------------------------------------------*/

/* Draw line between two points */
/* The plot buffer must be called first so it gets the unfiltered data */

void
plP_line(short *x, short *y)
{
    PLINT i, npts = 2, clpxmi, clpxma, clpymi, clpyma;

    if (plsc->plbuf_write)
	plbuf_line(plsc, x[0], y[0], x[1], y[1]);

    if (plsc->difilt) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = x[i];
	    yscl[i] = y[i];
	}
	difilt(xscl, yscl, npts, &clpxmi, &clpxma, &clpymi, &clpyma);
	plP_pllclp(xscl, yscl, npts, clpxmi, clpxma, clpymi, clpyma, grline);
    }
    else {
	grline(x, y, npts);
    }
}

/* Draw polyline */
/* The plot buffer must be called first */

void
plP_polyline(short *x, short *y, PLINT npts)
{
    PLINT i, clpxmi, clpxma, clpymi, clpyma;

    if (plsc->plbuf_write)
	plbuf_polyline(plsc, x, y, npts);

    if (plsc->difilt) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = x[i];
	    yscl[i] = y[i];
	}
	difilt(xscl, yscl, npts, &clpxmi, &clpxma, &clpymi, &clpyma);
	plP_pllclp(xscl, yscl, npts, clpxmi, clpxma, clpymi, clpyma,
		   grpolyline);
    }
    else {
	grpolyline(x, y, npts);
    }
}

/* Fill polygon */
/* The plot buffer must be called first */

void
plP_fill(short *x, short *y, PLINT npts)
{
    PLINT i, clpxmi, clpxma, clpymi, clpyma;

/*    if (plsc->plbuf_write)
	plbuf_fill(plsc, x, y, npts);
*/

    if (plsc->difilt) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = x[i];
	    yscl[i] = y[i];
	}
	difilt(xscl, yscl, npts, &clpxmi, &clpxma, &clpymi, &clpyma);
	plP_plfclp(xscl, yscl, npts, clpxmi, clpxma, clpymi, clpyma,
		   grfill);
    }
    else {
	grfill(x, y, npts);
    }
}

static void
grline(short *x, short *y, PLINT npts)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_line) (plsc, x[0], y[0], x[1], y[1]);
}

static void
grpolyline(short *x, short *y, PLINT npts)
{
    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_polyline) (plsc, x, y, npts);
}

static void	plfill_pat	(short *, short *, PLINT);

static void
grfill(short *x, short *y, PLINT npts)
{
    if (plsc->dev_fill) {
	plsc->dev_npts = npts;
	plsc->dev_x = x;
	plsc->dev_y = y;

	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_FILL, NULL);
    }
    else
	plfill_pat(x, y, npts);
}

static void
plfill_pat(short *x, short *y, PLINT npts)
{
}

/*----------------------------------------------------------------------*\
* void difilt
*
* Driver interface filter -- passes all coordinates through a variety
* of filters.  These include filters to change :
*
*	- mapping of meta to physical coordinates
*	- plot orientation
*	- window into plot (zooms)
*	- window into device (i.e set margins)
*
* The filters are applied in the order specified above.  Because the
* orientation change comes first, subsequent window specifications affect
* the new coordinates (i.e. after a 90 degree flip, what was x is now y).
* This is the only way that makes sense from a graphical interface
* (e.g. TCL/TK driver).
*
* Where appropriate, the page clip limits are modified.
\*----------------------------------------------------------------------*/

static void
difilt(PLINT *xscl, PLINT *yscl, PLINT npts,
       PLINT *clpxmi, PLINT *clpxma, PLINT *clpymi, PLINT *clpyma)
{
    PLINT i;

/* Map meta coordinates to physical coordinates */

    if (plsc->difilt & PLDI_MAP) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = plsc->dimxax * xscl[i] + plsc->dimxb;
	    yscl[i] = plsc->dimyay * yscl[i] + plsc->dimyb;
	}
    }

/* Change orientation */

    if (plsc->difilt & PLDI_ORI) {
	for (i = 0; i < npts; i++) {
	    xscl1[i] = plsc->dioxax * xscl[i] + plsc->dioxay * yscl[i] +
		plsc->dioxb;
	    yscl1[i] = plsc->dioyax * xscl[i] + plsc->dioyay * yscl[i] +
		plsc->dioyb;
	}
	for (i = 0; i < npts; i++) {
	    xscl[i] = xscl1[i];
	    yscl[i] = yscl1[i];
	}
    }

/* Change window into plot space */

    if (plsc->difilt & PLDI_PLT) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = plsc->dipxax * xscl[i] + plsc->dipxb;
	    yscl[i] = plsc->dipyay * yscl[i] + plsc->dipyb;
	}
    }

/* Change window into device space and set clip limits */
/* (this is the only filter that modifies them) */

    if (plsc->difilt & PLDI_DEV) {
	for (i = 0; i < npts; i++) {
	    xscl[i] = plsc->didxax * xscl[i] + plsc->didxb;
	    yscl[i] = plsc->didyay * yscl[i] + plsc->didyb;
	}
	*clpxmi = plsc->diclpxmi;
	*clpxma = plsc->diclpxma;
	*clpymi = plsc->diclpymi;
	*clpyma = plsc->diclpyma;
    }
    else {
	*clpxmi = plsc->phyxmi;
	*clpxma = plsc->phyxma;
	*clpymi = plsc->phyymi;
	*clpyma = plsc->phyyma;
    }
}

/*----------------------------------------------------------------------*\
* void pldi_ini
*
* Updates driver interface, making sure everything is in order.
* Even if filter is not being used, the defaults need to be set up.
\*----------------------------------------------------------------------*/

static void
setdef_diplt()
{
    plsc->dipxmin = 0.0;
    plsc->dipxmax = 1.0;
    plsc->dipymin = 0.0;
    plsc->dipymax = 1.0;
}

static void
setdef_didev()
{
    plsc->mar = 0.0;
    plsc->aspect = 0.0;
    plsc->jx = 0.0;
    plsc->jy = 0.0;
}

static void
setdef_diori()
{
    plsc->diorot = 0.;
}

static void
pldi_ini(void)
{
    if (plsc->level >= 1) {
	if (plsc->difilt & PLDI_ORI)	/* Orientation */
	    calc_diori();
	else
	    setdef_diori();

	if (plsc->difilt & PLDI_PLT) 	/* Plot window */
	    calc_diplt();
	else
	    setdef_diplt();

	if (plsc->difilt & PLDI_DEV)	/* Device window */
	    calc_didev();
	else
	    setdef_didev();
    }
}

/*----------------------------------------------------------------------*\
* void pldid2pc
*
* Converts input values from relative device coordinates to relative plot
* coordinates.  This function must be called when selecting a plot window
* from a display driver, since the coordinates chosen by the user are
* necessarily device-specific.
\*----------------------------------------------------------------------*/

void
pldid2pc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax)
{
    PLFLT pxmin, pymin, pxmax, pymax;
    PLFLT sxmin, symin, sxmax, symax;
    PLFLT rxmin, rymin, rxmax, rymax;

#ifdef DEBUG
    fprintf(stderr, "Relative device coordinates (input): %f, %f, %f, %f\n",
	    *xmin, *ymin, *xmax, *ymax);
#endif

    if (plsc->difilt & PLDI_DEV) {

	pxmin = plsc->phyxmi + (plsc->phyxma - plsc->phyxmi) * *xmin;
	pymin = plsc->phyymi + (plsc->phyyma - plsc->phyymi) * *ymin;
	pxmax = plsc->phyxmi + (plsc->phyxma - plsc->phyxmi) * *xmax;
	pymax = plsc->phyymi + (plsc->phyyma - plsc->phyymi) * *ymax;

	sxmin = (pxmin - plsc->didxb) / plsc->didxax;
	symin = (pymin - plsc->didyb) / plsc->didyay;
	sxmax = (pxmax - plsc->didxb) / plsc->didxax;
	symax = (pymax - plsc->didyb) / plsc->didyay;

	rxmin = (sxmin - plsc->phyxmi) / (plsc->phyxma - plsc->phyxmi);
	rymin = (symin - plsc->phyymi) / (plsc->phyyma - plsc->phyymi);
	rxmax = (sxmax - plsc->phyxmi) / (plsc->phyxma - plsc->phyxmi);
	rymax = (symax - plsc->phyymi) / (plsc->phyyma - plsc->phyymi);

	*xmin = (rxmin < 0) ? 0 : rxmin;
	*xmax = (rxmax > 1) ? 1 : rxmax;
	*ymin = (rymin < 0) ? 0 : rymin;
	*ymax = (rymax > 1) ? 1 : rymax;
    }

#ifdef DEBUG
    fprintf(stderr, "Relative plot coordinates (output): %f, %f, %f, %f\n",
	    rxmin, rymin, rxmax, rymax);
#endif
}

/*----------------------------------------------------------------------*\
* void pldip2dc
*
* Converts input values from relative plot coordinates to relative
* device coordinates.
\*----------------------------------------------------------------------*/

void
pldip2dc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax)
{
    PLFLT pxmin, pymin, pxmax, pymax;
    PLFLT sxmin, symin, sxmax, symax;
    PLFLT rxmin, rymin, rxmax, rymax;

#ifdef DEBUG
    fprintf(stderr, "Relative plot coordinates (input): %f, %f, %f, %f\n",
	    *xmin, *ymin, *xmax, *ymax);
#endif

    if (plsc->difilt & PLDI_DEV) {

	pxmin = plsc->phyxmi + (plsc->phyxma - plsc->phyxmi) * *xmin;
	pymin = plsc->phyymi + (plsc->phyyma - plsc->phyymi) * *ymin;
	pxmax = plsc->phyxmi + (plsc->phyxma - plsc->phyxmi) * *xmax;
	pymax = plsc->phyymi + (plsc->phyyma - plsc->phyymi) * *ymax;

	sxmin = pxmin * plsc->didxax + plsc->didxb;
	symin = pymin * plsc->didyay + plsc->didyb;
	sxmax = pxmax * plsc->didxax + plsc->didxb;
	symax = pymax * plsc->didyay + plsc->didyb;

	rxmin = (sxmin - plsc->phyxmi) / (plsc->phyxma - plsc->phyxmi);
	rymin = (symin - plsc->phyymi) / (plsc->phyyma - plsc->phyymi);
	rxmax = (sxmax - plsc->phyxmi) / (plsc->phyxma - plsc->phyxmi);
	rymax = (symax - plsc->phyymi) / (plsc->phyyma - plsc->phyymi);

	*xmin = (rxmin < 0) ? 0 : rxmin;
	*xmax = (rxmax > 1) ? 1 : rxmax;
	*ymin = (rymin < 0) ? 0 : rymin;
	*ymax = (rymax > 1) ? 1 : rymax;
    }

#ifdef DEBUG
    fprintf(stderr, "Relative device coordinates (output): %f, %f, %f, %f\n",
	    rxmin, rymin, rxmax, rymax);
#endif
}

/*----------------------------------------------------------------------*\
* void plsdiplt
*
* Set window into plot space
\*----------------------------------------------------------------------*/

void
c_plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax)
{
    plsc->dipxmin = (xmin < xmax) ? xmin : xmax;
    plsc->dipxmax = (xmin < xmax) ? xmax : xmin;
    plsc->dipymin = (ymin < ymax) ? ymin : ymax;
    plsc->dipymax = (ymin < ymax) ? ymax : ymin;

    if (xmin == 0. && xmax == 1. && ymin == 0. && ymax == 1.)  {
	plsc->difilt &= ~PLDI_PLT;
	return;
    }

    plsc->difilt |= PLDI_PLT;
    pldi_ini();
}

/*----------------------------------------------------------------------*\
* void plsdiplz
*
* Set window into plot space incrementally (zoom)
\*----------------------------------------------------------------------*/

void
c_plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax)
{
    if (plsc->difilt & PLDI_PLT) {
	xmin = plsc->dipxmin + (plsc->dipxmax - plsc->dipxmin) * xmin;
	ymin = plsc->dipymin + (plsc->dipymax - plsc->dipymin) * ymin;
	xmax = plsc->dipxmin + (plsc->dipxmax - plsc->dipxmin) * xmax;
	ymax = plsc->dipymin + (plsc->dipymax - plsc->dipymin) * ymax;
    }

    plsdiplt(xmin, ymin, xmax, ymax);
}

/*----------------------------------------------------------------------*\
* void calc_diplt
*
* Calculate transformation coefficients to set window into plot space.
*
* Note: if driver has requested to handle these commands itself, we must
* send the appropriate escape command.  If the driver succeeds it will
* cancel the filter operation.  The command is deferred until this point
* to ensure that the driver has been initialized.
\*----------------------------------------------------------------------*/

static void
calc_diplt(void)
{
    PLINT pxmin, pxmax, pymin, pymax, pxlen, pylen;

    if (plsc->dev_di) {
	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_DI, NULL);
    }

    if ( ! (plsc->difilt & PLDI_PLT))
	return;

    pxmin = plsc->dipxmin * (plsc->phyxma - plsc->phyxmi) + plsc->phyxmi;
    pxmax = plsc->dipxmax * (plsc->phyxma - plsc->phyxmi) + plsc->phyxmi;
    pymin = plsc->dipymin * (plsc->phyyma - plsc->phyymi) + plsc->phyymi;
    pymax = plsc->dipymax * (plsc->phyyma - plsc->phyymi) + plsc->phyymi;

    pxlen = pxmax - pxmin;
    pylen = pymax - pymin;
    pxlen = MAX(1, pxlen);
    pylen = MAX(1, pylen);

    plsc->dipxax = (float) (plsc->phyxma - plsc->phyxmi) / (float) pxlen;
    plsc->dipyay = (float) (plsc->phyyma - plsc->phyymi) / (float) pylen;
    plsc->dipxb = plsc->phyxmi - plsc->dipxax * pxmin;
    plsc->dipyb = plsc->phyymi - plsc->dipyay * pymin;
}

/*----------------------------------------------------------------------*\
* void plgdiplt
*
* Retrieve current window into plot space
\*----------------------------------------------------------------------*/

void
c_plgdiplt(PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax)
{
    *p_xmin = plsc->dipxmin;
    *p_xmax = plsc->dipxmax;
    *p_ymin = plsc->dipymin;
    *p_ymax = plsc->dipymax;
}

/*----------------------------------------------------------------------*\
* void plsdidev
*
* Set window into device space using margin, aspect ratio, and
* justification.  If you want to just use the previous value for any of
* these, just pass in the magic value PL_NOTSET.
*
* It is unlikely that one should ever need to change the aspect ratio
* but it's in there for completeness.
\*----------------------------------------------------------------------*/

void
c_plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy)
{
    plsetvar(plsc->mar, mar);
    plsetvar(plsc->aspect, aspect);
    plsetvar(plsc->jx, jx);
    plsetvar(plsc->jy, jy);

    if (mar == 0. && aspect == 0. && jx == 0. && jy == 0. && 
	! (plsc->difilt & PLDI_ORI)) {
	plsc->difilt &= ~PLDI_DEV;
	return;
    }

    plsc->difilt |= PLDI_DEV;
    pldi_ini();
}

/*----------------------------------------------------------------------*\
* void calc_didev
*
* Calculate transformation coefficients to set window into device space.
* Calculates relative window bounds and calls plsdidxy to finish the job.
\*----------------------------------------------------------------------*/

static void
calc_didev(void)
{
    PLFLT lx, ly, aspect, aspdev;
    PLFLT xmin, xmax, xlen, ymin, ymax, ylen;
    PLINT pxmin, pxmax, pymin, pymax, pxlen, pylen;

    if (plsc->dev_di) {
	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_DI, NULL);
    }

    if ( ! (plsc->difilt & PLDI_DEV))
	return;

/* Calculate aspect ratio of physical device */

    lx = (plsc->phyxma - plsc->phyxmi + 1) / plsc->xpmm;
    ly = (plsc->phyyma - plsc->phyymi + 1) / plsc->ypmm;
    aspdev = lx / ly;

    if (plsc->difilt & PLDI_ORI)
	aspect = plsc->aspori;
    else
	aspect = plsc->aspect;

    if (aspect <= 0.)
	aspect = plsc->aspdev;
/*
#ifdef DEBUG
    fprintf(stderr, "Device aspect ratio: %f\n", aspdev);
    fprintf(stderr, "Specified aspect ratio: %f\n", aspect);
#endif
*/
/* Failsafe */

    plsc->mar = (plsc->mar > 0.5) ? 0.5 : plsc->mar;
    plsc->mar = (plsc->mar < 0.0) ? 0.0 : plsc->mar;
    plsc->jx = (plsc->jx >  0.5) ?  0.5 : plsc->jx;
    plsc->jx = (plsc->jx < -0.5) ? -0.5 : plsc->jx;
    plsc->jy = (plsc->jy >  0.5) ?  0.5 : plsc->jy;
    plsc->jy = (plsc->jy < -0.5) ? -0.5 : plsc->jy;

/* Relative device coordinates that neutralize aspect ratio difference */

    xlen = (aspect < aspdev) ? (aspect / aspdev) : 1.0;
    ylen = (aspect < aspdev) ? 1.0 : (aspdev / aspect);

    xlen *= (1.0 - 2.*plsc->mar);
    ylen *= (1.0 - 2.*plsc->mar);

    xmin = (1. - xlen) * (0.5 + plsc->jx);
    xmax = xmin + xlen;

    ymin = (1. - ylen) * (0.5 + plsc->jy);
    ymax = ymin + ylen;

/* Calculate transformation coefficients */

    pxmin = xmin * (plsc->phyxma - plsc->phyxmi) + plsc->phyxmi;
    pxmax = xmax * (plsc->phyxma - plsc->phyxmi) + plsc->phyxmi;
    pymin = ymin * (plsc->phyyma - plsc->phyymi) + plsc->phyymi;
    pymax = ymax * (plsc->phyyma - plsc->phyymi) + plsc->phyymi;

    pxlen = pxmax - pxmin;
    pylen = pymax - pymin;
    pxlen = MAX(1, pxlen);
    pylen = MAX(1, pylen);

    plsc->didxax = (float) pxlen / (float) (plsc->phyxma - plsc->phyxmi);
    plsc->didyay = (float) pylen / (float) (plsc->phyyma - plsc->phyymi);
    plsc->didxb = pxmin - plsc->didxax * plsc->phyxmi;
    plsc->didyb = pymin - plsc->didyay * plsc->phyymi;

/* Set clip limits to conform to new page size */

    plsc->diclpxmi = plsc->didxax * plsc->phyxmi + plsc->didxb;
    plsc->diclpxma = plsc->didxax * plsc->phyxma + plsc->didxb;
    plsc->diclpymi = plsc->didyay * plsc->phyymi + plsc->didyb;
    plsc->diclpyma = plsc->didyay * plsc->phyyma + plsc->didyb;

/* Transformation test */
/*
#ifdef DEBUG
    pxmin = plsc->didxax * plsc->phyxmi + plsc->didxb;
    pxmax = plsc->didxax * plsc->phyxma + plsc->didxb;
    pymin = plsc->didyay * plsc->phyymi + plsc->didyb;
    pymax = plsc->didyay * plsc->phyyma + plsc->didyb;
    fprintf(stderr, "pxmin, pymin, pxmax, pymax: %d, %d, %d, %d\n",
	    pxmin, pymin, pxmax, pymax);
#endif
*/
}

/*----------------------------------------------------------------------*\
* void plgdidev
*
* Retrieve current window into device space
\*----------------------------------------------------------------------*/

void
c_plgdidev(PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy)
{
    *p_mar = plsc->mar;
    *p_aspect = plsc->aspect;
    *p_jx = plsc->jx;
    *p_jy = plsc->jy;
}

/*----------------------------------------------------------------------*\
* void plsdiori
*
* Set plot orientation
* Input is the rotation in units of pi/2.
\*----------------------------------------------------------------------*/

void
c_plsdiori(PLFLT rot)
{
    plsc->diorot = rot;
    if (rot == 0.) {
	plsc->difilt &= ~PLDI_ORI;
	pldi_ini();
	return;
    }

    plsc->difilt |= PLDI_ORI;
    pldi_ini();
}

/*----------------------------------------------------------------------*\
* void calc_diori
*
* Calculate transformation coefficients to arbitrarily orient plot.
* Preserve aspect ratios so the output doesn't suck.
\*----------------------------------------------------------------------*/

static void
calc_diori(void)
{
    PLFLT r11, r21, r12, r22, cost, sint;
    PLFLT x0, y0, lx, ly, aspect;

    if (plsc->dev_di) {
	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_DI, NULL);
    }

    if ( ! (plsc->difilt & PLDI_ORI))
	return;

/* Center point of rotation */

    x0 = (plsc->phyxma + plsc->phyxmi) / 2.;
    y0 = (plsc->phyyma + plsc->phyymi) / 2.;

/* Rotation matrix */

    r11 = cos(plsc->diorot * PI / 2.);
    r21 = sin(plsc->diorot * PI / 2.);
    r12 = -r21;
    r22 = r11;

    cost = ABS(r11);
    sint = ABS(r21);

/* Flip aspect ratio as necessary.  Grungy but I don't see a better way */

    aspect = plsc->aspect;
    if (aspect == 0.)
	aspect = plsc->aspdev;

    plsc->aspori = (aspect * cost + sint) / (aspect * sint + cost);

    if ( ! (plsc->difilt & PLDI_DEV)) {
	plsc->difilt |= PLDI_DEV;
	setdef_didev();
    }
    calc_didev();

/* Compute scale factors */

    lx = plsc->phyxma - plsc->phyxmi;
    ly = plsc->phyyma - plsc->phyymi;

/* Transformation coefficients */

    plsc->dioxax = r11;
    plsc->dioxay = r21 * (lx / ly);
    plsc->dioxb = (1. - r11) * x0 - r21 * y0 * (lx / ly);

    plsc->dioyax = r12 * (ly / lx);
    plsc->dioyay = r22;
    plsc->dioyb = (1. - r22) * y0 - r12 * x0 * (ly / lx);

/* Transformation test */

#ifdef DEBUG
{
    PLINT x1, y1, px1, py1;
    fprintf(stderr, "plsc->phyxmi: %d,  plsc->phyxma: %d\n",
	    plsc->phyxmi, plsc->phyxma);

    fprintf(stderr, "plsc->phyymi: %d,  plsc->phyyma: %d\n",
	    plsc->phyymi, plsc->phyyma);

    x1 = plsc->phyxmi; y1 = plsc->phyymi;
    px1 = plsc->dioxax * x1 + plsc->dioxay * y1 + plsc->dioxb;
    py1 = plsc->dioyax * x1 + plsc->dioyay * y1 + plsc->dioyb;
    fprintf(stderr, "Point %d %d is transformed to %d %d\n", x1, y1, px1, py1);

    x1 = plsc->phyxmi; y1 = plsc->phyyma;
    px1 = plsc->dioxax * x1 + plsc->dioxay * y1 + plsc->dioxb;
    py1 = plsc->dioyax * x1 + plsc->dioyay * y1 + plsc->dioyb;
    fprintf(stderr, "Point %d %d is transformed to %d %d\n", x1, y1, px1, py1);

    x1 = plsc->phyxma; y1 = plsc->phyymi;
    px1 = plsc->dioxax * x1 + plsc->dioxay * y1 + plsc->dioxb;
    py1 = plsc->dioyax * x1 + plsc->dioyay * y1 + plsc->dioyb;
    fprintf(stderr, "Point %d %d is transformed to %d %d\n", x1, y1, px1, py1);

    x1 = plsc->phyxma; y1 = plsc->phyyma;
    px1 = plsc->dioxax * x1 + plsc->dioxay * y1 + plsc->dioxb;
    py1 = plsc->dioyax * x1 + plsc->dioyay * y1 + plsc->dioyb;
    fprintf(stderr, "Point %d %d is transformed to %d %d\n", x1, y1, px1, py1);
}
#endif
}

/*----------------------------------------------------------------------*\
* void plgdiori
*
* Get plot orientation
\*----------------------------------------------------------------------*/

void
c_plgdiori(PLFLT *p_rot)
{
    *p_rot = plsc->diorot;
}

/*----------------------------------------------------------------------*\
* void plsdimap
*
* Set up transformation from metafile coordinates.
* The size of the plot is scaled so as to preserve aspect ratio.
* This isn't intended to be a general-purpose facility just yet
* (not sure why the user would need it, for one).
\*----------------------------------------------------------------------*/

void
c_plsdimap(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax,
	   PLFLT ixpmm, PLFLT iypmm)
{
    PLFLT lx, ly;
    PLINT pxmin, pxmax, pymin, pymax;
    PLFLT ixlen, iylen, pxlen, pylen;

    if (plsc->level < 1)
	plexit("plsdimap: Please call plinit first.");

    if ((ixmin == plsc->phyxmi) && (ixmax == plsc->phyxma) &&
	(iymin == plsc->phyymi) && (iymax == plsc->phyyma) &&
	(ixpmm == plsc->xpmm) && (iypmm == plsc->ypmm)) {
	plsc->difilt &= ~PLDI_MAP;
	return;
    }

    plsc->difilt |= PLDI_MAP;

#ifdef DEBUG
    fprintf(stderr, "plsc->phyxmi: %d,  plsc->phyxma: %d\n",
	    plsc->phyxmi, plsc->phyxma);

    fprintf(stderr, "plsc->phyymi: %d,  plsc->phyyma: %d\n",
	    plsc->phyymi, plsc->phyyma);

    fprintf(stderr, "plsc->xpmm: %f,  plsc->ypmm: %f\n",
	    plsc->xpmm, plsc->ypmm);

    fprintf(stderr, "ixmin: %d,  ixmax: %d\n",
	    ixmin, ixmax);

    fprintf(stderr, "iymin: %d,  iymax: %d\n",
	    iymin, iymax);

    fprintf(stderr, "ixpmm: %f,  iypmm: %f\n",
	    ixpmm, iypmm);
#endif

/* Set default aspect ratio */

    lx = (ixmax - ixmin + 1) / ixpmm;
    ly = (iymax - iymin + 1) / iypmm;

    plsc->aspdev = lx / ly;
    pldi_ini();

/* Build transformation to correct physical coordinates */

    ixlen = ixmax - ixmin;
    iylen = iymax - iymin;

    pxmin = plsc->phyxmi;
    pxmax = plsc->phyxma;
    pymin = plsc->phyymi;
    pymax = plsc->phyyma;
    pxlen = pxmax - pxmin;
    pylen = pymax - pymin;

    plsc->dimxax = pxlen / ixlen;
    plsc->dimyay = pylen / iylen;
    plsc->dimxb = pxmin - pxlen * ixmin / ixlen;
    plsc->dimyb = pymin - pylen * iymin / iylen;

#ifdef DEBUG
    fprintf(stderr, "pxmin: %d,  pxmax: %d,  pymin: %d,  pymax: %d\n",
	    pxmin, pxmax, pymin, pymax);

    fprintf(stderr, "dimxax: %f,  dimxb: %f,  dimyay: %f,  dimyb: %f\n",
	    plsc->dimxax, plsc->dimxb, plsc->dimyay, plsc->dimyb);

    fprintf(stderr, "pxlen: %f,  pylen: %f\n", pxlen, pylen);

    pxmin = ixmin * plsc->dimxax + plsc->dimxb;
    pxmax = ixmax * plsc->dimxax + plsc->dimxb;
    pymin = iymin * plsc->dimyay + plsc->dimyb;
    pymax = iymax * plsc->dimyay + plsc->dimyb;

    fprintf(stderr, "Transformation test:\n\
pxmin: %d,  pxmax: %d,  pymin: %d,  pymax: %d\n",
	    pxmin, pxmax, pymin, pymax);
#endif
}

/*----------------------------------------------------------------------*\
* Startup routines.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* void plstar(nx, ny)
*
* Here we are passing the windows/page in x and y.
\*----------------------------------------------------------------------*/

void
c_plstar(PLINT nx, PLINT ny)
{
    if (plsc->level != 0)
	plend1();

    plssub(nx, ny);

    c_plinit();
}

/*----------------------------------------------------------------------*\
* void plstart(devname, nx, ny)
*
* Here we are passing the device name, and windows/page in x and y.
\*----------------------------------------------------------------------*/

void
c_plstart(char *devname, PLINT nx, PLINT ny)
{
    if (plsc->level != 0)
	plend1();

    plssub(nx, ny);
    plsdev(devname);

    c_plinit();
}

/*----------------------------------------------------------------------*\
* void plinit()
*
* Checks certain startup parameters for validity, then proceeds with
* initialization.  Accepts no arguments.
\*----------------------------------------------------------------------*/

void
c_plinit()
{
    PLFLT gscale, hscale;
    PLFLT size_chr, size_sym, size_maj, size_min, lx, ly;
    PLINT mk = 0, sp = 0, inc = 0, del = 2000;

    if (plsc->level != 0)
	plend1();

/* Set device number */

    plGetDev();

/* Subpage checks */

    if (plsc->nsubx <= 0)
	plsc->nsubx = 1;
    if (plsc->nsuby <= 0)
	plsc->nsuby = 1;
    plsc->cursub = 0;

/* Stream number */

    plsc->ipls = ipls;

/* Initialize color maps */

    plCmaps_init(plsc);

/* Load fonts */

    font = 1;
    if (fontset) {
	plfntld(initfont);
	fontset = 0;
    }
    else
	plfntld(0);

/* Initialize device & first page */

    plP_init();
    plsc->level = 1;

    plP_bop();

/*
* Set default sizes
* Global scaling:
*	Normalize to the page length for more uniform results.
* 	A virtual page length of 200 mm is assumed.
* Subpage scaling:
*	Reduce sizes with plot area (non-proportional, so that character
*	size doesn't get too small).
*/
    gscale = 0.5 *
	((plsc->phyxma - plsc->phyxmi) / plsc->xpmm +
	 (plsc->phyyma - plsc->phyymi) / plsc->ypmm) / 200.0;

    hscale = gscale / sqrt((double) plsc->nsuby);

    size_chr = 4.0;
    size_sym = 4.0;		/* All these in virtual plot units */
    size_maj = 3.0;
    size_min = 1.5;

    plP_schr((PLFLT) (size_chr * hscale), (PLFLT) (size_chr * hscale));
    plP_ssym((PLFLT) (size_sym * hscale), (PLFLT) (size_sym * hscale));
    plP_smaj((PLFLT) (size_maj * hscale), (PLFLT) (size_maj * hscale));
    plP_smin((PLFLT) (size_min * hscale), (PLFLT) (size_min * hscale));

/* Switch to graphics mode and set color */

    plgra();
    plcol(1);

    plstyl(0, &mk, &sp);
    plpat(1, &inc, &del);

/* Set clip limits. */

    plsc->clpxmi = plsc->phyxmi;
    plsc->clpxma = plsc->phyxma;
    plsc->clpymi = plsc->phyymi;
    plsc->clpyma = plsc->phyyma;

/* Page aspect ratio. */

    lx = (plsc->phyxma - plsc->phyxmi) / plsc->xpmm;
    ly = (plsc->phyyma - plsc->phyymi) / plsc->ypmm;
    plsc->aspdev = lx / ly;

/* Initialize driver interface */

    pldi_ini();
}

/*----------------------------------------------------------------------*\
* void plend()
*
* End a plotting session for all open streams.
\*----------------------------------------------------------------------*/

void
c_plend()
{
    PLINT i;

    for (i = 0; i < PL_NSTREAMS; i++) {
	if (pls[i] != NULL) {
	    plsstrm(i);
	    c_plend1();
	}
    }
    plsstrm(0);
    plfontrel();
}

/*----------------------------------------------------------------------*\
* void plend1()
*
* End a plotting session for the current stream only.
* After the stream is ended the memory associated with the stream's
* PLStream data structure is freed (for stream > 0), and the stream
* counter is set to 0 (the default).
\*----------------------------------------------------------------------*/

void
c_plend1()
{
    if (plsc->level > 0) {
	plP_eop();
	plP_tidy();
	plP_slev(0);
    }

/* Free all malloc'ed stream memory */

    free_mem(plsc->plwindow);
    free_mem(plsc->geometry);
    free_mem(plsc->dev);
    free_mem(plsc->FileName);
    free_mem(plsc->FamilyName);

/* Free malloc'ed stream if not in initial stream */

    if (ipls > 0) {
	free_mem(plsc);
	pls[ipls] = NULL;
	plsstrm(0);
    }
}

/*----------------------------------------------------------------------*\
* void plsstrm
*
* Set stream number.  If the data structure for a new stream is
* unallocated, we allocate it here.
\*----------------------------------------------------------------------*/

void
c_plsstrm(PLINT strm)
{
    if (strm < 0 || strm >= PL_NSTREAMS) {
	fprintf(stderr,
		"plsstrm: Illegal stream number %d, must be in [0, %d]\n",
		strm, PL_NSTREAMS);
    }
    else {
	ipls = strm;
	if (pls[ipls] == NULL) {
	    pls[ipls] = (PLStream *) malloc((size_t) sizeof(PLStream));
	    if (pls[ipls] == NULL)
		plexit("plsstrm: Out of memory.");

	    memset((char *) pls[ipls], 0, sizeof(PLStream));
	}
	plsc = pls[ipls];
    }
}

/*----------------------------------------------------------------------*\
* void plgstrm
*
* Get current stream number.
\*----------------------------------------------------------------------*/

void
c_plgstrm(PLINT *p_strm)
{
    *p_strm = ipls;
}

/*----------------------------------------------------------------------*\
* void plmkstrm
*
* Creates a new stream and makes it the default.  Differs from using
* plsstrm(), in that a free stream number is found, and returned.
\*----------------------------------------------------------------------*/

void
c_plmkstrm(PLINT *p_strm)
{
    int i;

    for (i = PL_NSTREAMS - 1; i > 0; i--) {
	if (pls[i] == NULL)
	    break;
    }

    if (i == 0) {
	fprintf(stderr, "plmkstrm: Cannot create new stream\n");
	*p_strm = -1;
    }
    else {
	*p_strm = i;
	plsstrm(i);
    }
}

/*----------------------------------------------------------------------*\
* void plcpstrm
*
* Copies state parameters from the reference stream to the current stream.
* Tell driver interface to map device coordinates unless flags == 1.
*
* This function is used for making save files of selected plots (e.g.
* from the TK driver).  After initializing, you can get a copy of the
* current plot to the specified device by switching to this stream and
* issuing a plcpstrm() and a plreplot(), with calls to plbop() and
* pleop() as appropriate.  The plot buffer must have previously been
* enabled (done automatically by some display drivers, such as X).
\*----------------------------------------------------------------------*/

void
c_plcpstrm(PLINT iplsr, PLINT flags)
{
    int i;
    PLStream *plsr;

    plsr = pls[iplsr];
    if (plsr == NULL) {
	fprintf(stderr, "plcpstrm: stream %d not in use\n", iplsr);
	return;
    }

/* Copy state parameters from the reference stream */
/* Can't just copy entire stream unfortunately */

/* Palettes */

    plsc->icol0 = plsr->icol0;
    plsc->ncol0 = plsr->ncol0;
    plsc->bgcolorset = plsr->bgcolorset;

    plsc->cmap1set = plsr->cmap1set;
    plsc->htlvl = plsr->htlvl;
    for (i = 0; i < 16; i++)
	plsc->cmap0setcol[i] = plsr->cmap0setcol[i];

#define CP_COLOR(color) \
    plsc->color.r = plsr->color.r; \
    plsc->color.g = plsr->color.g; \
    plsc->color.b = plsr->color.b;

    CP_COLOR(fgcolor);
    CP_COLOR(bgcolor);
    CP_COLOR(curcolor);
    for (i = 0; i < 16; i++)
	CP_COLOR(cmap0[i]);
    for (i = 0; i < 256; i++)
	CP_COLOR(cmap1[i]);

/* Plot buffer -- need to copy file pointer so that plreplot() works */

    plsc->plbufFile = plsr->plbufFile;
    plsc->plbufOwner = 0;
    plsc->plbuf_write = plsr->plbuf_write;

/* Driver interface */
/* Transformation must be recalculated in current driver coordinates */

    if (plsr->difilt & PLDI_PLT) 
	plsdiplt(plsr->dipxmin, plsr->dipymin, plsr->dipxmax, plsr->dipymax);

    if (plsr->difilt & PLDI_DEV)
	plsdidev(plsr->mar, plsr->aspect, plsr->jx, plsr->jy);

    if (plsr->difilt & PLDI_ORI)
	plsdiori(plsr->diorot);

/* Map device coordinates */

    if ( ! (flags & 0x01))
	plsdimap(plsr->phyxmi, plsr->phyxma, plsr->phyymi, plsr->phyyma,
		 plsr->xpmm, plsr->ypmm);
}

/*----------------------------------------------------------------------*\
* void plGetDev()
*
* If the the user has not already specified the output device, or the
* one specified is either: (a) not available, (b) "?", or (c) NULL, the
* user is prompted for it.
*
* Prompting quits after 10 unsuccessful tries in case the user has
* run the program in the background with insufficient input.
\*----------------------------------------------------------------------*/

static void
plGetDev()
{
    PLINT dev, i, count, length;
    char response[80];

/* Device name already specified.  See if it is valid. */

    if (*(plsc->DevName) != '\0' && *(plsc->DevName) != '?') {
	length = strlen(plsc->DevName);
	for (i = 0; i < npldrivers; i++) {
	    if ((*plsc->DevName == *dispatch_table[i].pl_DevName) &&
		(strncmp(plsc->DevName,
			 dispatch_table[i].pl_DevName, length) == 0))
		break;
	}
	if (i < npldrivers) {
	    plsc->device = i + 1;
	    return;
	}
	else {
	    printf("Requested device %s not available\n", plsc->DevName);
	}
    }

    dev = 0;
    count = 0;

    if (npldrivers == 1)
	dev = 1;

/* User hasn't specified it correctly yet, so we prompt */

    while (dev < 1 || dev > npldrivers) {
	printf("\nPlotting Options:\n");
	for (i = 0; i < npldrivers; i++) {
	    printf(" <%2ld> (%s)\t %s\n", i + 1, dispatch_table[i].pl_DevName,
		   dispatch_table[i].pl_MenuStr);
	}
	if (ipls == 0)
	    printf("\nEnter device number or keyword: ");
	else
	    printf("\nEnter device number or keyword (stream %d): ", ipls);

	fgets(response, sizeof(response), stdin);

/* First check to see if device keyword was entered. */
/* Final "\n" in response messes things up, so ignore it.  */

	length = strlen(response);
	if (*(response - 1 + length) == '\n')
	    length--;

	for (i = 0; i < npldrivers; i++) {
	    if (!strncmp(response, dispatch_table[i].pl_DevName,
			 (unsigned int) length))
		break;
	}
	if (i < npldrivers) {
	    dev = i + 1;
	}
	else {
	    if ((dev = atoi(response)) < 1) {
		printf("Invalid device.");
		dev = 0;
	    }
	}
	if (count++ > 10)
	    plexit("plGetDev: Too many tries.");
    }
    plsc->device = dev;
}

/*----------------------------------------------------------------------*\
* void plwarn()
*
* A handy way to issue warnings, if need be.
\*----------------------------------------------------------------------*/

void
plwarn(char *errormsg)
{
    int was_gfx = 0;

    if (plsc->level > 0) {
	if (plsc->graphx == 1) {
	    was_gfx = 1;
	    pltext();
	}
    }

    fprintf(stderr, "\n*** PLPLOT WARNING ***\n");
    if (*errormsg != '\0')
	fprintf(stderr, "%s\n", errormsg);

    if (was_gfx == 1) {
	plgra();
    }
}

/*----------------------------------------------------------------------*\
* void plfontld()
*
* Load specified font set.
\*----------------------------------------------------------------------*/

void
c_plfontld(PLINT fnt)
{
    if (fnt != 0)
	fnt = 1;

    if (plsc->level > 0)
	plfntld(fnt);
    else {
	initfont = fnt;
	fontset = 1;
    }
}

/*----------------------------------------------------------------------*\
* void plreplot()
*
* Replays contents of plot buffer to current device/file.
\*----------------------------------------------------------------------*/

void
c_plreplot()
{
    if (plsc->plbufFile != NULL) {
	plRemakePlot(plsc);
    }
    else {
	plwarn("plreplot: plot buffer not available");
    }
}

/*----------------------------------------------------------------------*\
* void plgFileDevs()
*
* Returns a list of file-oriented device names and their menu strings,
* for use in a graphical interface.  The caller must allocate enough
* space for (*p_menustr) and (*p_devname) to hold a pointer for each
* device -- 20 or so is plenty.  E.g. char *menustr[20].
\*----------------------------------------------------------------------*/

void
plgFileDevs(char ***p_menustr, char ***p_devname, int *p_ndev)
{
    int i, j;

    for (i = j = 0; i < npldrivers; i++) {
	if (dispatch_table[i].pl_type == 0) {
	    (*p_menustr)[j] = dispatch_table[i].pl_MenuStr;
	    (*p_devname)[j] = dispatch_table[i].pl_DevName;
	    j++;
	}
    }
    (*p_menustr)[j] = NULL;
    (*p_devname)[j] = NULL;
    *p_ndev = j;
}

/*----------------------------------------------------------------------*\
*  Various external access routines.
\*----------------------------------------------------------------------*/

void
c_plgpage(PLFLT *p_xp, PLFLT *p_yp,
	  PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff)
{
    *p_xp = plsc->xdpi;
    *p_yp = plsc->ydpi;
    *p_xleng = plsc->xlength;
    *p_yleng = plsc->ylength;
    *p_xoff = plsc->xoffset;
    *p_yoff = plsc->yoffset;
}

void
c_plssub(PLINT nx, PLINT ny)
{
    if (plsc->level > 0) {
	plwarn("plssub: Must be called before plinit.");
	return;
    }
    if (nx > 0)
	plsc->nsubx = nx;
    if (ny > 0)
	plsc->nsuby = ny;
}

void
c_plsdev(char *devname)
{
    if (plsc->level > 0) {
	plwarn("plsdev: Must be called before plinit.");
	return;
    }
    if (devname != NULL) {
	strncpy(plsc->DevName, devname, sizeof(plsc->DevName) - 1);
	plsc->DevName[sizeof(plsc->DevName) - 1] = '\0';
    }
}

void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff)
{
    if (xp)
	plsc->xdpi = xp;
    if (yp)
	plsc->ydpi = yp;

    if (xleng)
	plsc->xlength = xleng;
    if (yleng)
	plsc->ylength = yleng;

    if (xoff)
	plsc->xoffset = xoff;
    if (yoff)
	plsc->yoffset = yoff;

    plsc->pageset = 1;
}

void
plgpls(PLStream **p_pls)
{
    *p_pls = plsc;
}

void
plsKeyEH(void (*KeyEH) (PLKey *, void *, int *), void *KeyEH_data)
{
    plsc->KeyEH = KeyEH;
    plsc->KeyEH_data = KeyEH_data;
}

/* Set orientation.  Must be done before calling plinit. */

void
c_plsori(PLINT ori)
{
    plsdiori((PLFLT) ori);
}

/* Set pen width.  Can be done any time, but before calling plinit is best
   since otherwise it may be volatile (i.e. reset on next page advance). */

void
c_plwid(PLINT width)
{
    plsc->width = width;

    if (plsc->level > 0) {
	if ( ! plsc->widthlock) 
	    plP_state(PLSTATE_WIDTH);
    }
    else
	plsc->widthset = 1;
}

/* Obsolete.  Use page driver interface instead. */

void
c_plsasp(PLFLT asp)
{
}

/* I've canned this for now */

void
c_plslpb(PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma)
{
}

/* Note these two are only useable from C.. */

void
plgfile(FILE **p_file)
{
    *p_file = plsc->OutFile;
}

void
plsfile(FILE *file)
{
    plsc->OutFile = file;
    plsc->fileset = 1;
}

/* Flush current output file.  Use sparingly, if at all. */

void
plflush(void)
{
    fflush(plsc->OutFile);
}

/*
*  The user MUST allocate at least 80 characters to hold the filename.
*  Beyond that, I truncate it.  You have been warned.
*/

void
c_plgfnam(char *fnam)
{
    strncpy(fnam, plsc->FileName, 79);
    fnam[79] = '\0';
}

void
c_plsfnam(char *fnam)
{
    plP_sfnam(plsc, fnam);
}

void
c_plspause(PLINT pause)
{
    plsc->nopause = !pause;
}

/* Set/get the number of points written after the decimal point in labels. */

void
c_plprec(PLINT setp, PLINT prec)
{
    plsc->setpre = setp;
    plsc->precis = prec;
}

void
plP_gprec(PLINT *p_setp, PLINT *p_prec)
{
    *p_setp = plsc->setpre;
    *p_prec = plsc->precis;
}

/*
* Set/get the escape character for text strings.
* From C you can pass as a character, from Fortran it needs to be the decimal
* ASCII value.  Only selected characters are allowed to prevent the user from
* shooting himself in the foot (a '\' isn't allowed since it conflicts with
* C's use of backslash as a character escape).
*/

void
c_plsesc(char esc)
{
    switch (esc) {
	case '!':		/* ASCII 33 */
	case '#':		/* ASCII 35 */
	case '$':		/* ASCII 36 */
	case '%':		/* ASCII 37 */
	case '&':		/* ASCII 38 */
	case '*':		/* ASCII 42 */
	case '@':		/* ASCII 64 */
	case '^':		/* ASCII 94 */
	case '~':		/* ASCII 126 */
	plsc->esc = esc;
	break;

      default:
	plwarn("plsesc: Invalid escape character, ignoring.");
    }
}

void
plgesc(char *p_esc)
{
    if (plsc->esc == '\0')
	plsc->esc = '#';

    *p_esc = plsc->esc;
}

/* Note: you MUST have allocated space for this (80 characters is safe) */

void
c_plgver(char *p_ver)
{
    strcpy(p_ver, PLPLOT_VERSION);
}

/* For plotting into an inferior X window */

void
plsxwin(PLINT window_id)
{
    plsc->window_id = window_id;
}

/*----------------------------------------------------------------------*\
*  Routines that deal with colors & color maps.
\*----------------------------------------------------------------------*/

/* Set color, map 0 */

void
c_plcol(PLINT icol0)
{
    if (plsc->level < 1)
	plexit("plcol: Please call plinit first.");

    if (icol0 < 0 || icol0 > 15) {
	plwarn("plcol: Invalid color.");
	return;
    }

    if (plsc->cmap0setcol[icol0] == 0) {
	plwarn("plcol: Requested color not allocated.");
	return;
    }

    plsc->icol0 = icol0;
    plsc->curcolor.r = plsc->cmap0[icol0].r;
    plsc->curcolor.g = plsc->cmap0[icol0].g;
    plsc->curcolor.b = plsc->cmap0[icol0].b;

    plP_state(PLSTATE_COLOR0);
}

/* Set line color by red, green, blue from  0. to 1. */

void
c_plrgb(PLFLT r, PLFLT g, PLFLT b)
{
    if (plsc->level < 1)
	plexit("plrgb: Please call plinit first.");

    if ((r < 0. || r > 1.) || (g < 0. || g > 1.) || (b < 0. || b > 1.)) {
	plwarn("plrgb: Invalid RGB color value");
	return;
    }

    plsc->icol0 = PL_RGB_COLOR;
    plsc->curcolor.r = MIN(255, (int) (256. * r));
    plsc->curcolor.g = MIN(255, (int) (256. * g));
    plsc->curcolor.b = MIN(255, (int) (256. * b));

    plP_state(PLSTATE_COLOR0);
}

/* Set line color by 8 bit RGB values. */

void
c_plrgb1(PLINT r, PLINT g, PLINT b)
{
    if (plsc->level < 1)
	plexit("plrgb1: Please call plinit first.");

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plwarn("plrgb1: Invalid color");
	return;
    }

    plsc->icol0 = PL_RGB_COLOR;
    plsc->curcolor.r = r;
    plsc->curcolor.g = g;
    plsc->curcolor.b = b;

    plP_state(PLSTATE_COLOR0);
}

/* Set number of colors in color map 0 */

void
c_plscm0n(PLINT ncol0)
{
    if (plsc->level > 0) {
	plwarn("plscm0: Must be called before plinit.");
	return;
    }

    if (ncol0 < 2 || ncol0 > 16)
	plexit("plscm0: Number of colors out of range");

    plsc->ncol0 = ncol0;
}

/* Set color map 0 colors by 8 bit RGB values */
/* WARNING -- This sets ncol0 as well. */

void
c_plscm0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0)
{
    int i;

    if (plsc->level > 0) {
	plwarn("plscm0: Must be called before plinit.");
	return;
    }

    if (ncol0 > 16)
	plexit("plscm0: Maximum of 16 colors in color map 0.");

    plsc->ncol0 = ncol0;

    for (i = 0; i < ncol0; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    plwarn("plscm0: Invalid color");
	    continue;
	}

	plsc->cmap0[i].r = r[i];
	plsc->cmap0[i].g = g[i];
	plsc->cmap0[i].b = b[i];
	plsc->cmap0setcol[i] = 1;
    }
}

/* Set a given color from color map 0 by 8 bit RGB value */

void
c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b)
{
    if (plsc->level > 0) {
	plwarn("plscol0: Must be called before plinit.");
	return;
    }

    if (icol0 < 0 || icol0 > 15)
	plexit("plscol0: Illegal color table value");

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plwarn("plscol0: Invalid color");
	return;
    }

    plsc->cmap0[icol0].r = r;
    plsc->cmap0[icol0].g = g;
    plsc->cmap0[icol0].b = b;
    plsc->cmap0setcol[icol0] = 1;
}

/* Set the background color by 8 bit RGB value */

void
c_plscolbg(PLINT r, PLINT g, PLINT b)
{
    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plwarn("plscolbg: Invalid color");
	return;
    }

    plsc->bgcolor.r = r;
    plsc->bgcolor.g = g;
    plsc->bgcolor.b = b;
    plsc->bgcolorset = 1;
}

/* Set color map 1 colors by 8 bit RGB values */
/* You MUST set all 256 colors if you use this */

void
c_plscm1(PLINT *r, PLINT *g, PLINT *b)
{
    int i;

    if (plsc->level > 0) {
	plwarn("plscm1: Must be called before plinit.");
	return;
    }

    for (i = 0; i < 256; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    fprintf(stderr, "plscm1: Invalid RGB color: %d, %d, %d\n",
		    r[i], g[i], b[i]);
	    plexit("");
	}
	plsc->cmap1[i].r = r[i];
	plsc->cmap1[i].g = g[i];
	plsc->cmap1[i].b = b[i];
    }
    plsc->cmap1set = 1;
}

/* Set color map 1 colors using a linear relationship between function
*  height and position in HLS or RGB color space.
*  There are 6 parameters pointed to by "param"; these correspond to:
*
*   itype=0 (HLS)    itype=1 (RGB)
*   -------------    -------------
*	H-min		R-min
*	H-max		R-max
*	L-min		G-min
*	L-max		G-max
*	S-min		B-min
*	S-max		B-max
*
*  Bounds on RGB coordinates:
*	R,G,B		[0., 1.]	magnitude
*
*  Bounds on HLS coordinates:
*	hue		[0., 360.]	degrees
*	lightness	[0., 1.]	magnitude
*	saturation	[0., 1.]	magnitude
*/

void
c_plscm1f1(PLINT itype, PLFLT *param)
{
    int i;
    PLFLT h, l, s, r, g, b;

    if (plsc->level > 0) {
	plwarn("plscm1f1: Must be called before plinit.");
	return;
    }
    for (i = 0; i < 256; i++) {
	r = param[0] + (param[1] - param[0]) * i / 255.;
	g = param[2] + (param[3] - param[2]) * i / 255.;
	b = param[4] + (param[5] - param[4]) * i / 255.;

	if (itype == 0) {
	    h = r;
	    l = g;
	    s = b;
	    plHLS_RGB(h, l, s, &r, &g, &b);
	}

	if ((r < 0. || r > 1.) || (g < 0. || g > 1.) || (b < 0. || b > 1.)) {
	    fprintf(stderr, "plscm1f1: Invalid RGB color: %f, %f, %f\n",
		    r, g, b);
	    plexit("");
	}
	plsc->cmap1[i].r = MIN(255, (int) (256. * r));
	plsc->cmap1[i].g = MIN(255, (int) (256. * g));
	plsc->cmap1[i].b = MIN(255, (int) (256. * b));
    }
    plsc->cmap1set = 1;
}

/* Used to globally turn color output on/off */

void
c_plscolor(PLINT color)
{
    plsc->colorset = 1;
    plsc->color = color;
}

/*----------------------------------------------------------------------*\
*  These set/get information for family files, and may be called prior
*  to plinit to set up the necessary parameters.  Arguments:
*
*	fam	familying flag (boolean)
*	num	member number
*	bmax	maximum member size
\*----------------------------------------------------------------------*/

void
c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax)
{
    *p_fam = plsc->family;
    *p_num = plsc->member;
    *p_bmax = plsc->bytemax;
}

void
c_plsfam(PLINT fam, PLINT num, PLINT bmax)
{
    if (plsc->level > 0)
	plwarn("plsfam: Must be called before plinit.");

    if (fam >= 0)
	plsc->family = fam;
    if (num >= 0)
	plsc->member = num;
    if (bmax >= 0)
	plsc->bytemax = bmax;
}

void
c_plfamadv(void)
{
    plsc->famadv = 1;
}

/*----------------------------------------------------------------------*\
*  Interface routines for axis labling parameters.
*  See pldtik.c for more info.
\*----------------------------------------------------------------------*/

void
c_plgxax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->xdigmax;
    *p_digits = plsc->xdigits;
}

void
c_plsxax(PLINT digmax, PLINT digits)
{
    plsc->xdigmax = digmax;
    plsc->xdigits = digits;
}

void
c_plgyax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->ydigmax;
    *p_digits = plsc->ydigits;
}

void
c_plsyax(PLINT digmax, PLINT digits)
{
    plsc->ydigmax = digmax;
    plsc->ydigits = digits;
}

void
c_plgzax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->zdigmax;
    *p_digits = plsc->zdigits;
}

void
c_plszax(PLINT digmax, PLINT digits)
{
    plsc->zdigmax = digmax;
    plsc->zdigits = digits;
}

/*----------------------------------------------------------------------*\
*  These should not be called by the user.
\*----------------------------------------------------------------------*/

void
plP_glev(PLINT *p_n)
{
    *p_n = plsc->level;
}

void
plP_slev(PLINT n)
{
    plsc->level = n;
}

void
plP_gbase(PLFLT *p_x, PLFLT *p_y, PLFLT *p_xc, PLFLT *p_yc)
{
    *p_x = plsc->base3x;
    *p_y = plsc->base3y;
    *p_xc = plsc->basecx;
    *p_yc = plsc->basecy;
}

void
plP_sbase(PLFLT x, PLFLT y, PLFLT xc, PLFLT yc)
{
    plsc->base3x = x;
    plsc->base3y = y;
    plsc->basecx = xc;
    plsc->basecy = yc;
}

void
plP_gnms(PLINT *p_n)
{
    *p_n = plsc->nms;
}

void
plP_snms(PLINT n)
{
    plsc->nms = n;
}

void
plP_gcurr(PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = plsc->currx;
    *p_iy = plsc->curry;
}

void
plP_scurr(PLINT ix, PLINT iy)
{
    plsc->currx = ix;
    plsc->curry = iy;
}

void
plP_gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->domxmi;
    *p_xmax = plsc->domxma;
    *p_ymin = plsc->domymi;
    *p_ymax = plsc->domyma;
}

void
plP_sdom(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->domxmi = xmin;
    plsc->domxma = xmax;
    plsc->domymi = ymin;
    plsc->domyma = ymax;
}

void
plP_grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax)
{
    *p_zscl = plsc->zzscl;
    *p_zmin = plsc->ranmi;
    *p_zmax = plsc->ranma;
}

void
plP_srange(PLFLT zscl, PLFLT zmin, PLFLT zmax)
{
    plsc->zzscl = zscl;
    plsc->ranmi = zmin;
    plsc->ranma = zmax;
}

void
plP_gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy, PLFLT *p_dyz)
{
    *p_dxx = plsc->cxx;
    *p_dxy = plsc->cxy;
    *p_dyx = plsc->cyx;
    *p_dyy = plsc->cyy;
    *p_dyz = plsc->cyz;
}

void
plP_sw3wc(PLFLT dxx, PLFLT dxy, PLFLT dyx, PLFLT dyy, PLFLT dyz)
{
    plsc->cxx = dxx;
    plsc->cxy = dxy;
    plsc->cyx = dyx;
    plsc->cyy = dyy;
    plsc->cyz = dyz;
}

void
plP_gvpp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->vppxmi;
    *p_ixmax = plsc->vppxma;
    *p_iymin = plsc->vppymi;
    *p_iymax = plsc->vppyma;
}

void
plP_svpp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->vppxmi = ixmin;
    plsc->vppxma = ixmax;
    plsc->vppymi = iymin;
    plsc->vppyma = iymax;
}

void
plP_gspp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->sppxmi;
    *p_ixmax = plsc->sppxma;
    *p_iymin = plsc->sppymi;
    *p_iymax = plsc->sppyma;
}

void
plP_sspp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->sppxmi = ixmin;
    plsc->sppxma = ixmax;
    plsc->sppymi = iymin;
    plsc->sppyma = iymax;
}

void
plP_gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->clpxmi;
    *p_ixmax = plsc->clpxma;
    *p_iymin = plsc->clpymi;
    *p_iymax = plsc->clpyma;
}

void
plP_sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->clpxmi = ixmin;
    plsc->clpxma = ixmax;
    plsc->clpymi = iymin;
    plsc->clpyma = iymax;
}

void
plP_gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->phyxmi;
    *p_ixmax = plsc->phyxma;
    *p_iymin = plsc->phyymi;
    *p_iymax = plsc->phyyma;
}

void
plP_sphy(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->phyxmi = ixmin;
    plsc->phyxma = ixmax;
    plsc->phyymi = iymin;
    plsc->phyyma = iymax;
}

void
plP_gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs)
{
    *p_nx = plsc->nsubx;
    *p_ny = plsc->nsuby;
    *p_cs = plsc->cursub;
}

void
plP_ssub(PLINT nx, PLINT ny, PLINT cs)
{
    plsc->nsubx = nx;
    plsc->nsuby = ny;
    plsc->cursub = cs;
}

void
plP_gumpix(PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = plsc->umx;
    *p_iy = plsc->umy;
}

void
plP_sumpix(PLINT ix, PLINT iy)
{
    plsc->umx = ix;
    plsc->umy = iy;
}

void
plP_gatt(PLINT *p_ifnt, PLINT *p_icol0)
{
    *p_ifnt = font;
    *p_icol0 = plsc->icol0;
}

void
plP_satt(PLINT ifnt, PLINT icol0)
{
    font = ifnt;
    plsc->icol0 = icol0;
}

void
plP_gcol(PLINT *p_icol0)
{
    *p_icol0 = plsc->icol0;
}

void
plP_scol(PLINT icol0)
{
    plsc->icol0 = icol0;
}

void
plP_gwid(PLINT *p_pwid)
{
    *p_pwid = plsc->width;
}

void
plP_swid(PLINT pwid)
{
    plsc->width = pwid;
}

void
plP_gspd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->spdxmi;
    *p_xmax = plsc->spdxma;
    *p_ymin = plsc->spdymi;
    *p_ymax = plsc->spdyma;
}

void
plP_sspd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->spdxmi = xmin;
    plsc->spdxma = xmax;
    plsc->spdymi = ymin;
    plsc->spdyma = ymax;
}

void
plP_gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->vpdxmi;
    *p_xmax = plsc->vpdxma;
    *p_ymin = plsc->vpdymi;
    *p_ymax = plsc->vpdyma;
}

void
plP_svpd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->vpdxmi = xmin;
    plsc->vpdxma = xmax;
    plsc->vpdymi = ymin;
    plsc->vpdyma = ymax;
}

void
plP_gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->vpwxmi;
    *p_xmax = plsc->vpwxma;
    *p_ymin = plsc->vpwymi;
    *p_ymax = plsc->vpwyma;
}

void
plP_svpw(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->vpwxmi = xmin;
    plsc->vpwxma = xmax;
    plsc->vpwymi = ymin;
    plsc->vpwyma = ymax;
}

void
plP_gpixmm(PLFLT *p_x, PLFLT *p_y)
{
    *p_x = plsc->xpmm;
    *p_y = plsc->ypmm;
}

void
plP_spixmm(PLFLT x, PLFLT y)
{
    plsc->xpmm = x;
    plsc->ypmm = y;
    plsc->umx = 1000.0 / plsc->xpmm;
    plsc->umy = 1000.0 / plsc->ypmm;
}

/* All the drivers call this to set physical pixels/mm. */

void
plP_setpxl(PLFLT xpmm0, PLFLT ypmm0)
{
    plsc->xpmm = xpmm0;
    plsc->ypmm = ypmm0;
    plsc->umx = 1000.0 / plsc->xpmm;
    plsc->umy = 1000.0 / plsc->ypmm;
}

/* Sets up physical limits of plotting device and the mapping between
   normalized device coordinates and physical coordinates. */

void
plP_setphy(PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax)
{
    PLFLT xpmm, ypmm, mpxscl, mpyscl;

    plP_sphy(xmin, xmax, ymin, ymax);
    plP_sdp((PLFLT) (xmax - xmin), (PLFLT) (xmin), (PLFLT) (ymax - ymin),
	    (PLFLT) (ymin));

    plP_gpixmm(&xpmm, &ypmm);
    mpxscl = xpmm;
    if (xmax <= xmin)
	mpxscl = -xpmm;
    mpyscl = ypmm;
    if (ymax <= ymin)
	mpyscl = -ypmm;
    plP_smp(mpxscl, (PLFLT) (xmin), mpyscl, (PLFLT) (ymin));
}

void
plP_gwp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->wpxscl;
    *p_xoff = plsc->wpxoff;
    *p_yscl = plsc->wpyscl;
    *p_yoff = plsc->wpyoff;
}

void
plP_swp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->wpxscl = xscl;
    plsc->wpxoff = xoff;
    plsc->wpyscl = yscl;
    plsc->wpyoff = yoff;
}

void
plP_gwm(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->wmxscl;
    *p_xoff = plsc->wmxoff;
    *p_yscl = plsc->wmyscl;
    *p_yoff = plsc->wmyoff;
}

void
plP_swm(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->wmxscl = xscl;
    plsc->wmxoff = xoff;
    plsc->wmyscl = yscl;
    plsc->wmyoff = yoff;
}

void
plP_gdp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->dpxscl;
    *p_xoff = plsc->dpxoff;
    *p_yscl = plsc->dpyscl;
    *p_yoff = plsc->dpyoff;
}

void
plP_sdp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->dpxscl = xscl;
    plsc->dpxoff = xoff;
    plsc->dpyscl = yscl;
    plsc->dpyoff = yoff;
}

void
plP_gmp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->mpxscl;
    *p_xoff = plsc->mpxoff;
    *p_yscl = plsc->mpyscl;
    *p_yoff = plsc->mpyoff;
}

void
plP_smp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->mpxscl = xscl;
    plsc->mpxoff = xoff;
    plsc->mpyscl = yscl;
    plsc->mpyoff = yoff;
}

void
c_plgchr(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = plsc->chrdef;
    *p_ht = plsc->chrht;
}

void
plP_schr(PLFLT def, PLFLT ht)
{
    plsc->chrdef = def;
    plsc->chrht = ht;
}

void
plP_gsym(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = plsc->symdef;
    *p_ht = plsc->symht;
}

void
plP_ssym(PLFLT def, PLFLT ht)
{
    plsc->symdef = def;
    plsc->symht = ht;
}

void
plP_gmaj(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = plsc->majdef;
    *p_ht = plsc->majht;
}

void
plP_smaj(PLFLT def, PLFLT ht)
{
    plsc->majdef = def;
    plsc->majht = ht;
}

void
plP_gmin(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = plsc->mindef;
    *p_ht = plsc->minht;
}

void
plP_smin(PLFLT def, PLFLT ht)
{
    plsc->mindef = def;
    plsc->minht = ht;
}

void
plP_gpat(PLINT *p_inc[], PLINT *p_del[], PLINT *p_nlin)
{
    *p_inc = plsc->inclin;
    *p_del = plsc->delta;
    *p_nlin = plsc->nps;
}

void
plP_spat(PLINT inc[], PLINT del[], PLINT nlin)
{
    PLINT i;

    plsc->nps = nlin;
    for (i = 0; i < nlin; i++) {
	plsc->inclin[i] = inc[i];
	plsc->delta[i] = del[i];
    }
}

void
plP_gmark(PLINT *p_mar[], PLINT *p_spa[], PLINT *p_nms)
{
    *p_mar = plsc->mark;
    *p_spa = plsc->space;
    *p_nms = plsc->nms;
}

void
plP_smark(PLINT mar[], PLINT spa[], PLINT nms)
{
    PLINT i;

    plsc->nms = nms;
    for (i = 0; i < nms; i++) {
	plsc->mark[i] = mar[i];
	plsc->space[i] = spa[i];
    }
}

void
plP_gcure(PLINT **p_cur, PLINT **p_pen, PLINT **p_tim, PLINT **p_ala)
{
    *p_cur = &(plsc->curel);
    *p_pen = &(plsc->pendn);
    *p_tim = &(plsc->timecnt);
    *p_ala = &(plsc->alarm);
}

void
plP_scure(PLINT cur, PLINT pen, PLINT tim, PLINT ala)
{
    plsc->curel = cur;
    plsc->pendn = pen;
    plsc->timecnt = tim;
    plsc->alarm = ala;
}
