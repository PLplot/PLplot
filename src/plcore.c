/* $Id$
 * $Log$
 * Revision 1.35  1994/07/03 05:50:22  mjl
 * Added fix to prevent an infinite loop while exiting under certain error
 * conditions, submitted by Radey Shouman.
 *
 * Revision 1.34  1994/07/02  21:32:11  mjl
 * Ensure that fflush() is never called with a null pointer, since some
 * systems don't like it (submitted by Neal Holtz).
 *
 * Revision 1.33  1994/06/30  18:26:16  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.  Driver interface changed to keep track of current
 * status; plsc->status is set to one of AT_BOP, DRAWING, or AT_EOP.  This
 * makes it easy to catch missing plbop/pleop's, and to collapse multiple
 * plbop's or pleop's into a single one.  Subpage initialization code moved
 * to plpage.c.
 *
 * Revision 1.32  1994/06/09  20:31:30  mjl
 * Small changes to the way plmkstrm() works.
 *
 * Revision 1.31  1994/05/07  03:23:07  mjl
 * Eliminated some obsolete operations involving fgcolor and bgcolor.
 *
 * Revision 1.30  1994/04/30  16:15:06  mjl
 * Fixed format field (%ld instead of %d) or introduced casts where
 * appropriate to eliminate warnings given by gcc -Wall.
 *
 * Revision 1.29  1994/04/08  12:31:39  mjl
 * Removed driver interface handling of nopause (it was a bad idea).  Added
 * call to (*plsc->tidy) [if defined] in plP_tidy.  Added a function
 * plsMouseEH for setting the mouse event handler (contributed by Radey
 * Shouman).
 *
 * Revision 1.28  1994/03/23  08:02:48  mjl
 * Provided for handling more basic operations in the driver interface rather
 * than the drivers themselves (pls->nopause, resetting stream parameters
 * after a tidy, etc).  Added support for hardware fill -- if the device does
 * not support the operation, the function plfill_soft is used instead.
 * Pattern fill number set/get access functions added.
 *
 * Many debugging remnants from driver interface development removed.
 *
 * Many functions moved elsewhere (this file has gotten too large):
 *
 * 	plwarn plexit plcol0 plcol1 plrgb plrgb1 plscolbg plscol0 plgcol0
 * 	plscmap1 plscmap1f1 plscolor
 *
 * Revision 1.27  1994/01/25  06:23:26  mjl
 * Moved default setting of digits variables to the correct location.
 *
 * Revision 1.26  1994/01/18  06:01:38  mjl
 * Now set default number of digits in numeric labels for axis to be switched
 * to scientific notation.  Before this capability had to be enabled by the
 * user.  The number of digits defaults to x:4, y:4, z:3 (xy or xyz plots).
 *
 * Revision 1.25  1994/01/17  21:36:51  mjl
 * Added function c_plgcol0 for retrieving RGB color values from cmap0
 * entries.  User-contributed (I lost track of who sent it).
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
 * specific functions, the escape function is provided.  The command
 * stream gets duplicated to the plot buffer here.
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

enum {AT_BOP, DRAWING, AT_EOP};

/* Initialize device. */
/* The plot buffer must be called last */

void
plP_init(void)
{
    plsc->status = AT_EOP;

    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_init) (plsc);

    if (plsc->plbuf_write)
	plbuf_init(plsc);
}

/* End of page */
/* The plot buffer must be called first */
/* Ignore instruction if there's nothing drawn */

void
plP_eop(void)
{
    if (plsc->status != DRAWING)
	return;

    plsc->status = AT_EOP;

    if (plsc->plbuf_write)
	plbuf_eop(plsc);

    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_eop) (plsc);
}

/* Set up new page. */
/* The plot buffer must be called last */
/* Ignore if the bop was already issued. */
/* Print a warning if an eop wasn't issued previously */

void
plP_bop(void)
{
    if (plsc->status == AT_BOP)
	return;

    if (plsc->status != AT_EOP) 
	plwarn("plP_bop: missing call to pleop");

    plsc->status = AT_BOP;

    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_bop) (plsc);

    if (plsc->plbuf_write)
	plbuf_bop(plsc);
}

/* Tidy up device (flush buffers, close file, etc.) */

void
plP_tidy(void)
{
    if (plsc->tidy) {
	(*plsc->tidy) (plsc->tidy_data);
	plsc->tidy = NULL;
	plsc->tidy_data = NULL;
    }

    offset = plsc->device - 1;
    (*dispatch_table[offset].pl_tidy) (plsc);

    if (plsc->plbuf_write)
	plbuf_tidy(plsc);

    plsc->OutFile = NULL;
    free_mem(plsc->FileName);
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

    plsc->status = DRAWING;

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

    plsc->status = DRAWING;

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

    plsc->status = DRAWING;

    if (plsc->plbuf_write) {
	plsc->dev_npts = npts;
	plsc->dev_x = x;
	plsc->dev_y = y;
	plbuf_esc(plsc, PLESC_FILL, NULL);
    }

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

/* Here if the desired area fill capability isn't present, we mock up */
/* something in software */

static int foo;

static void
grfill(short *x, short *y, PLINT npts)
{
    if (plsc->patt == 0 && ! plsc->dev_fill0) {
	if ( ! foo) {
	    plwarn("Driver does not support hardware solid fills, switching to software fill.\n");
	    foo = 1;
	}
	plsc->patt = 8;
	plpsty(plsc->patt);
    }
    if (plsc->patt < 0 && ! plsc->dev_fill1) {
	if ( ! foo) {
	    plwarn("Driver does not support hardware pattern fills, switching to software fill.\n");
	    foo = 1;
	}
	plsc->patt = ABS(plsc->patt) % 8 + 1;
	plpsty(plsc->patt);
    }

    if (plsc->patt <= 0) {
	plsc->dev_npts = npts;
	plsc->dev_x = x;
	plsc->dev_y = y;

	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_FILL, NULL);
    }
    else
	plfill_soft(x, y, npts);
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
    PLINT i, x, y;

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
	    x = plsc->dioxax * xscl[i] + plsc->dioxay * yscl[i] + plsc->dioxb;
	    y = plsc->dioyax * xscl[i] + plsc->dioyay * yscl[i] + plsc->dioyb;
	    xscl[i] = x;
	    yscl[i] = y;
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
	if (plsc->difilt & PLDI_MAP)	/* Coordinate mapping */
	    calc_dimap();

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
 * Set plot orientation, specifying rotation in units of pi/2.
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
c_plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
	   PLFLT dimxpmm, PLFLT dimypmm)
{
    plsetvar(plsc->dimxmin, dimxmin);
    plsetvar(plsc->dimxmax, dimxmax);
    plsetvar(plsc->dimymin, dimymin);
    plsetvar(plsc->dimymax, dimymax);
    plsetvar(plsc->dimxpmm, dimxpmm);
    plsetvar(plsc->dimypmm, dimypmm);

    plsc->difilt |= PLDI_MAP;
    pldi_ini();
}

/*----------------------------------------------------------------------*\
 * void calc_dimap
 *
 * Set up transformation from metafile coordinates.  The size of the plot
 * is scaled so as to preserve aspect ratio.  This isn't intended to be a
 * general-purpose facility just yet (not sure why the user would need it,
 * for one).
\*----------------------------------------------------------------------*/

static void
calc_dimap()
{
    PLFLT lx, ly;
    PLINT pxmin, pxmax, pymin, pymax;
    PLFLT dimxlen, dimylen, pxlen, pylen;

    if ((plsc->dimxmin == plsc->phyxmi) && (plsc->dimxmax == plsc->phyxma) &&
	(plsc->dimymin == plsc->phyymi) && (plsc->dimymax == plsc->phyyma) &&
	(plsc->dimxpmm == plsc->xpmm) && (plsc->dimypmm == plsc->ypmm)) {
	plsc->difilt &= ~PLDI_MAP;
	return;
    }

/* Set default aspect ratio */

    lx = (plsc->dimxmax - plsc->dimxmin + 1) / plsc->dimxpmm;
    ly = (plsc->dimymax - plsc->dimymin + 1) / plsc->dimypmm;

    plsc->aspdev = lx / ly;

/* Build transformation to correct physical coordinates */

    dimxlen = plsc->dimxmax - plsc->dimxmin;
    dimylen = plsc->dimymax - plsc->dimymin;

    pxmin = plsc->phyxmi;
    pxmax = plsc->phyxma;
    pymin = plsc->phyymi;
    pymax = plsc->phyyma;
    pxlen = pxmax - pxmin;
    pylen = pymax - pymin;

    plsc->dimxax = pxlen / dimxlen;
    plsc->dimyay = pylen / dimylen;
    plsc->dimxb = pxmin - pxlen * plsc->dimxmin / dimxlen;
    plsc->dimyb = pymin - pylen * plsc->dimymin / dimylen;
}

/*----------------------------------------------------------------------*\
 * void plflush()
 *
 * Flushes the output stream.  Use sparingly, if at all.
\*----------------------------------------------------------------------*/

void
c_plflush(void)
{
    if (plsc->dev_flush) {
	offset = plsc->device - 1;
	(*dispatch_table[offset].pl_esc) (plsc, PLESC_FLUSH, NULL);
    }
    else {
	if (plsc->OutFile != NULL)
	    fflush(plsc->OutFile);
    }
}

/*----------------------------------------------------------------------*\
 * Startup routines.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
 * void plstar(nx, ny)
 *
 * Initialize plplot, passing in the windows/page settings.
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
 * Initialize plplot, passing the device name and windows/page settings. 
\*----------------------------------------------------------------------*/

void
c_plstart(const char *devname, PLINT nx, PLINT ny)
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
 * Initializes plplot, using preset or default options.
\*----------------------------------------------------------------------*/

void
c_plinit(void)
{
    PLFLT lx, ly;
    PLINT mk = 0, sp = 0, inc = 0, del = 2000;

    if (plsc->level != 0)
	plend1();

/* Set device number */

    plGetDev();

/* Stream number */

    plsc->ipls = ipls;

/* Initialize color maps */

    plCmap0_init();
    plCmap1_init();

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

/* Set up subpages */

    plP_subpInit();

/* Set up number of allowed digits before switching to scientific notation */
/* The user can always change this */

    if (plsc->xdigmax == 0)
	plsc->xdigmax = 4;

    if (plsc->ydigmax == 0)
	plsc->ydigmax = 4;

    if (plsc->zdigmax == 0)
	plsc->zdigmax = 3;

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
c_plend(void)
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
 * End a plotting session for the current stream only.  After the stream
 * is ended the memory associated with the stream's PLStream data
 * structure is freed (for stream > 0), and the stream counter is set to 0
 * (the default).
\*----------------------------------------------------------------------*/

void
c_plend1(void)
{
    if (plsc->level > 0) {
	plsc->level = 0;
	plP_eop();
	plP_tidy();
    }

/* Free all malloc'ed stream memory */

    free_mem(plsc->plwindow);
    free_mem(plsc->geometry);
    free_mem(plsc->dev);
    free_mem(plsc->BaseName);

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
		(int) strm, PL_NSTREAMS);
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
 *
 * Unfortunately, I /have/ to start at stream 1 and work upward, since
 * stream 0 is preallocated.  One of the BIG flaws in the PLplot API is
 * that no initial, library-opening call is required.  So stream 0 must be
 * preallocated, and there is no simple way of determining whether it is
 * already in use or not.
\*----------------------------------------------------------------------*/

void
c_plmkstrm(PLINT *p_strm)
{
    int i;

    for (i = 1; i < PL_NSTREAMS; i++) {
	if (pls[i] == NULL)
	    break;
    }

    if (i == PL_NSTREAMS) {
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

static void
cp_color(PLColor *to, PLColor *from)
{
    to->r = from->r;
    to->g = from->g;
    to->b = from->b;
}

void
c_plcpstrm(PLINT iplsr, PLINT flags)
{
    int i;
    PLStream *plsr;

    plsr = pls[iplsr];
    if (plsr == NULL) {
	fprintf(stderr, "plcpstrm: stream %d not in use\n", (int) iplsr);
	return;
    }

/* Plot buffer -- need to copy file pointer so that plreplot() works */
/* This also prevents inadvertent writes into the plot buffer */

    plsc->plbufFile = plsr->plbufFile;

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

/* Initialize if it hasn't been done yet. */

    if (plsc->level == 0)
	plinit();

/* Palettes */

    plsc->icol0 = plsr->icol0;
    plsc->ncol0 = plsr->ncol0;
    plsc->icol1 = plsr->icol1;
    plsc->ncol1 = plsr->ncol1;

    cp_color(&plsc->curcolor, &plsr->curcolor);
    for (i = 0; i < 16; i++) {
	plsc->cmap0setcol[i] = plsr->cmap0setcol[i];
	cp_color(&plsc->cmap0[i], &plsr->cmap0[i]);
    }
    for (i = 0; i < 256; i++) {
	cp_color(&plsc->cmap1[i], &plsr->cmap1[i]);
    }
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
	    printf(" <%2d> %-10s %s\n", i + 1, dispatch_table[i].pl_DevName,
		   dispatch_table[i].pl_MenuStr);
	}
	if (ipls == 0)
	    printf("\nEnter device number or keyword: ");
	else
	    printf("\nEnter device number or keyword (stream %d): ",
		   (int) ipls);

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
    strcpy(plsc->DevName, dispatch_table[dev - 1].pl_DevName);
}

/*----------------------------------------------------------------------*\
 * void c_plfont(ifont)
 *
 * Sets the global font flag to 'ifont'.
\*----------------------------------------------------------------------*/

void
c_plfont(PLINT ifont)
{
    if (plsc->level < 1) {
	plabort("plfont: Please call plinit first");
	return;
    }
    if (ifont < 1 || ifont > 4) {
	plabort("plfont: Invalid font");
	return;
    }

    font = ifont;
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
c_plreplot(void)
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

/* Get output device parameters. */

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

/* Set output device parameters.  Usually ignored by the driver. */

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

/* Set the number of subwindows in x and y */

void
c_plssub(PLINT nx, PLINT ny)
{
    if (nx > 0)
	plsc->nsubx = nx;
    if (ny > 0)
	plsc->nsuby = ny;

    if (plsc->level > 0)
	plP_subpInit();
}

/* Set the device (keyword) name */

void
c_plsdev(const char *devname)
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

/* Get the current stream pointer */

void
plgpls(PLStream **p_pls)
{
    *p_pls = plsc;
}

/* Set the function pointer for the keyboard event handler */

void
plsKeyEH(void (*KeyEH) (PLKey *, void *, int *), void *KeyEH_data)
{
    plsc->KeyEH = KeyEH;
    plsc->KeyEH_data = KeyEH_data;
}

/* Set the function pointer for the mouse event handler */

void
plsMouseEH(void (*MouseEH) (PLMouse *, void *, int *), void *MouseEH_data)
{
    plsc->MouseEH = MouseEH;
    plsc->MouseEH_data = MouseEH_data;
}

/* Set orientation.  Must be done before calling plinit. */

void
c_plsori(PLINT ori)
{
    plsdiori((PLFLT) ori);
}

/*
 * Set pen width.  Can be done any time, but before calling plinit is best
 * since otherwise it may be volatile (i.e. reset on next page advance). 
 * If width <= 0 or is unchanged by the call, nothing is done.
 */

void
c_plwid(PLINT width)
{
    if (width != plsc->width && width > 0) {
	plsc->width = width;

	if (plsc->level > 0) {
	    if ( ! plsc->widthlock) 
		plP_state(PLSTATE_WIDTH);
	}
    }
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

/* Set the output file pointer */

void
plgfile(FILE **p_file)
{
    *p_file = plsc->OutFile;
}

/* Get the output file pointer */

void
plsfile(FILE *file)
{
    plsc->OutFile = file;
}

/* Get the (current) output file name.  Must be preallocated to >80 bytes */
/* Beyond that, I truncate it.  You have been warned. */

void
c_plgfnam(char *fnam)
{
    strncpy(fnam, plsc->FileName, 79);
    fnam[79] = '\0';
}

/* Set the output file name. */

void
c_plsfnam(const char *fnam)
{
    plP_sfnam(plsc, fnam);
}

/* Set the pause (on end-of-page) status */

void
c_plspause(PLINT pause)
{
    plsc->nopause = ! pause;
}

/* Set the floating point precision (in number of places) in numeric labels. */

void
c_plprec(PLINT setp, PLINT prec)
{
    plsc->setpre = setp;
    plsc->precis = prec;
}

/* Get the floating point precision (in number of places) in numeric labels. */

void
plP_gprec(PLINT *p_setp, PLINT *p_prec)
{
    *p_setp = plsc->setpre;
    *p_prec = plsc->precis;
}

/*
 * Set the escape character for text strings.
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

/* Get the escape character for text strings. */

void
plgesc(char *p_esc)
{
    if (plsc->esc == '\0')
	plsc->esc = '#';

    *p_esc = plsc->esc;
}

/* Get the current library version number */
/* Note: you MUST have allocated space for this (80 characters is safe) */

void
c_plgver(char *p_ver)
{
    strcpy(p_ver, PLPLOT_VERSION);
}

/* Set inferior X window */

void
plsxwin(PLINT window_id)
{
    plsc->window_id = window_id;
}

/*----------------------------------------------------------------------*\
 *  These set/get information for family files, and may be called prior
 *  to plinit to set up the necessary parameters.  Arguments:
 *
 *	fam	familying flag (boolean)
 *	num	member number
 *	bmax	maximum member size
\*----------------------------------------------------------------------*/

/* Get family file parameters */

void
c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax)
{
    *p_fam = plsc->family;
    *p_num = plsc->member;
    *p_bmax = plsc->bytemax;
}

/* Set family file parameters */

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

/* Advance to the next family file on the next new page */

void
c_plfamadv(void)
{
    plsc->famadv = 1;
}

/*----------------------------------------------------------------------*\
 *  Interface routines for axis labling parameters.
 *  See pldtik.c for more info.
\*----------------------------------------------------------------------*/

/* Get x axis labeling parameters */

void
c_plgxax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->xdigmax;
    *p_digits = plsc->xdigits;
}

/* Set x axis labeling parameters */

void
c_plsxax(PLINT digmax, PLINT digits)
{
    plsc->xdigmax = digmax;
    plsc->xdigits = digits;
}

/* Get y axis labeling parameters */

void
c_plgyax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->ydigmax;
    *p_digits = plsc->ydigits;
}

/* Set y axis labeling parameters */

void
c_plsyax(PLINT digmax, PLINT digits)
{
    plsc->ydigmax = digmax;
    plsc->ydigits = digits;
}

/* Get z axis labeling parameters */

void
c_plgzax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = plsc->zdigmax;
    *p_digits = plsc->zdigits;
}

/* Set z axis labeling parameters */

void
c_plszax(PLINT digmax, PLINT digits)
{
    plsc->zdigmax = digmax;
    plsc->zdigits = digits;
}

/* Get character default height and current (scaled) height */

void
c_plgchr(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = plsc->chrdef;
    *p_ht = plsc->chrht;
}

/*----------------------------------------------------------------------*\
 *  These should not be called by the user.
\*----------------------------------------------------------------------*/

/* Get x-y domain in world coordinates for 3d plots */

void
plP_gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->domxmi;
    *p_xmax = plsc->domxma;
    *p_ymin = plsc->domymi;
    *p_ymax = plsc->domyma;
}

/* Get vertical (z) scale parameters for 3-d plot */

void
plP_grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax)
{
    *p_zscl = plsc->zzscl;
    *p_zmin = plsc->ranmi;
    *p_zmax = plsc->ranma;
}

/* Get parameters used in 3d plots */

void
plP_gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy, PLFLT *p_dyz)
{
    *p_dxx = plsc->cxx;
    *p_dxy = plsc->cxy;
    *p_dyx = plsc->cyx;
    *p_dyy = plsc->cyy;
    *p_dyz = plsc->cyz;
}

/* Get viewport boundaries in physical coordinates */

void
plP_gvpp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->vppxmi;
    *p_ixmax = plsc->vppxma;
    *p_iymin = plsc->vppymi;
    *p_iymax = plsc->vppyma;
}

/* Set viewport boundaries in physical coordinates */

void
plP_svpp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->vppxmi = ixmin;
    plsc->vppxma = ixmax;
    plsc->vppymi = iymin;
    plsc->vppyma = iymax;
}

/* Get subpage boundaries in physical coordinates */

void
plP_gspp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->sppxmi;
    *p_ixmax = plsc->sppxma;
    *p_iymin = plsc->sppymi;
    *p_iymax = plsc->sppyma;
}

/* Get clip boundaries in physical coordinates */

void
plP_gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->clpxmi;
    *p_ixmax = plsc->clpxma;
    *p_iymin = plsc->clpymi;
    *p_iymax = plsc->clpyma;
}

/* Set clip boundaries in physical coordinates */

void
plP_sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->clpxmi = ixmin;
    plsc->clpxma = ixmax;
    plsc->clpymi = iymin;
    plsc->clpyma = iymax;
}

/* Get physical device limits in physical coordinates */

void
plP_gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = plsc->phyxmi;
    *p_ixmax = plsc->phyxma;
    *p_iymin = plsc->phyymi;
    *p_iymax = plsc->phyyma;
}

/* Set physical device limits in physical coordinates */

void
plP_sphy(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    plsc->phyxmi = ixmin;
    plsc->phyxma = ixmax;
    plsc->phyymi = iymin;
    plsc->phyyma = iymax;
}

/* Get number of subpages on physical device and current subpage */

void
plP_gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs)
{
    *p_nx = plsc->nsubx;
    *p_ny = plsc->nsuby;
    *p_cs = plsc->cursub;
}

/* Set number of subpages on physical device and current subpage */

void
plP_ssub(PLINT nx, PLINT ny, PLINT cs)
{
    plsc->nsubx = nx;
    plsc->nsuby = ny;
    plsc->cursub = cs;
}

/* Get font and color attributes */

void
plP_gatt(PLINT *p_ifnt, PLINT *p_icol0)
{
    *p_ifnt = font;
    *p_icol0 = plsc->icol0;
}

/* Set font and color attributes */

void
plP_satt(PLINT ifnt, PLINT icol0)
{
    font = ifnt;
    plsc->icol0 = icol0;
}

/* Get subpage boundaries in normalized device coordinates */

void
plP_gspd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->spdxmi;
    *p_xmax = plsc->spdxma;
    *p_ymin = plsc->spdymi;
    *p_ymax = plsc->spdyma;
}

/* Get viewport boundaries in normalized device coordinates */

void
plP_gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->vpdxmi;
    *p_xmax = plsc->vpdxma;
    *p_ymin = plsc->vpdymi;
    *p_ymax = plsc->vpdyma;
}

/* Get viewport boundaries in normalized device coordinates */

void
plP_svpd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->vpdxmi = xmin;
    plsc->vpdxma = xmax;
    plsc->vpdymi = ymin;
    plsc->vpdyma = ymax;
}

/* Get viewport boundaries in world coordinates */

void
plP_gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = plsc->vpwxmi;
    *p_xmax = plsc->vpwxma;
    *p_ymin = plsc->vpwymi;
    *p_ymax = plsc->vpwyma;
}

/* Set viewport boundaries in world coordinates */

void
plP_svpw(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    plsc->vpwxmi = xmin;
    plsc->vpwxma = xmax;
    plsc->vpwymi = ymin;
    plsc->vpwyma = ymax;
}

/* Get number of pixels to a millimeter */

void
plP_gpixmm(PLFLT *p_x, PLFLT *p_y)
{
    *p_x = plsc->xpmm;
    *p_y = plsc->ypmm;
}

/* Set number of pixels to a millimeter */

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
    PLFLT mpxscl, mpyscl;

    plP_sphy(xmin, xmax, ymin, ymax);

    plsc->dpxscl = xmax - xmin;
    plsc->dpxoff = xmin;
    plsc->dpyscl = ymax - ymin;
    plsc->dpyoff = ymin;

    mpxscl = plsc->xpmm;
    if (xmax <= xmin)
	mpxscl = -plsc->xpmm;
    mpyscl = plsc->ypmm;
    if (ymax <= ymin)
	mpyscl = -plsc->ypmm;
    plP_smp(mpxscl, (PLFLT) (xmin), mpyscl, (PLFLT) (ymin));
}

/* Get transformation variables for world coordinates to mm */

void
plP_gwm(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->wmxscl;
    *p_xoff = plsc->wmxoff;
    *p_yscl = plsc->wmyscl;
    *p_yoff = plsc->wmyoff;
}

/* Set transformation variables for world coordinates to mm */

void
plP_swm(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->wmxscl = xscl;
    plsc->wmxoff = xoff;
    plsc->wmyscl = yscl;
    plsc->wmyoff = yoff;
}

/* Get transformation variables for millimeters from bottom left */

void
plP_gmp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = plsc->mpxscl;
    *p_xoff = plsc->mpxoff;
    *p_yscl = plsc->mpyscl;
    *p_yoff = plsc->mpyoff;
}

/* Set transformation variables for millimeters from bottom left */

void
plP_smp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    plsc->mpxscl = xscl;
    plsc->mpxoff = xoff;
    plsc->mpyscl = yscl;
    plsc->mpyoff = yoff;
}
