/* $Id$

	Functions to shade regions on the basis of value.
	Can be used to shade contour plots or alone.
	Copyright 1993 Wesley Ebisuzaki 
*/

/*----------------------------------------------------------------------*\
 * Call syntax for plshade():
 * 
 * void plshade(PLFLT *a, PLINT nx, PLINT ny, char *defined, 
 *	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, 
 * 	PLFLT shade_min, PLFLT shade_max, 
 * 	PLINT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width,
 * 	PLINT max_color, PLINT max_width, void (*fill)(), PLINT
 * 	rectangular, ...)
 * 
 * arguments:
 * 
 * 	PLFLT &(a[0][0])
 * 
 * Contains array to be plotted. The array must have been declared as
 * PLFLT a[nx][ny].  See following note on fortran-style arrays.
 * 
 * 	PLINT nx, ny
 * 
 * Dimension of array "a".
 * 
 * 	char &(defined[0][0])
 * 
 * Contains array of flags, 1 = data is valid, 0 = data is not valid.
 * This array determines which sections of the data is to be plotted.
 * This argument can be NULL if all the values are valid.  Must have been
 * declared as char defined[nx][ny].
 * 
 * 	PLFLT xmin, xmax, ymin, ymax
 * 
 * Defines the "grid" coordinates.  The data a[0][0] has a position of
 * (xmin,ymin).
 * 
 * 	void (*mapform)()
 * 
 * Transformation from `grid' coordinates to world coordinates.  This
 * pointer to a function can be NULL in which case the grid coordinates
 * are the same as the world coordinates.
 * 
 * 	PLFLT shade_min, shade_max
 * 
 * Defines the interval to be shaded. If shade_max <= shade_min, plshade
 * does nothing.
 * 
 *	PLINT sh_cmap, PLFLT sh_color, PLINT sh_width
 * 
 * Defines color map, color map index, and width used by the fill pattern.
 * 
 * 	PLINT min_color, min_width, max_color, max_width
 * 
 * Defines pen color, width used by the boundary of shaded region. The min
 * values are used for the shade_min boundary, and the max values are used
 * on the shade_max boundary.  Set color and width to zero for no plotted
 * boundaries.
 * 
 * 	void (*fill)()
 * 
 * Routine used to fill the region.  Use plfill.  Future version of plplot
 * may have other fill routines.
 * 
 * 	PLINT rectangular
 * 
 * Flag. Set to 1 if rectangles map to rectangles after (*mapform)() else
 * set to zero. If rectangular is set to 1, plshade tries to save time by
 * filling large rectangles.  This optimization fails if (*mapform)()
 * distorts the shape of rectangles.  For example a plot in polor
 * coordinates has to have rectangular set to zero.
 * 
 * Example mapform's:
 * 
 * Grid to world coordinate transformation.
 * This example goes from a r-theta to x-y for a polar plot.
 *
 * void mapform(PLINT n, PLFLT *x, PLFLT *y) {
 * 	int i;
 * 	double r, theta;
 * 	for (i = 0; i < n; i++) {
 * 	    r = x[i];
 * 	    theta = y[i];
 * 	    x[i] = r*cos(theta);
 * 	    y[i] = r*sin(theta);	
 * 	}
 * }
 * 
 * Grid was in cm, convert to world coordinates in inches.
 * Expands in x direction.
 *
 * void mapform(PLINT n, PLFLT *x, PLFLT *y) {
 * 	int i;
 * 	for (i = 0; i < n; i++) {
 * 		x[i] = (1.0 / 2.5) * x[i];
 * 		y[i] = (1.0 / 2.5) * y[i];
 * 	}
 * }
 *
\*----------------------------------------------------------------------*/

#include "plplot/plplotP.h"
#include <float.h>

#define MISSING_MIN_DEF (PLFLT) 1.0
#define MISSING_MAX_DEF (PLFLT) -1.0


#define NEG  1
#define POS  8
#define OK   0
#define UNDEF 64

#define linear(val1, val2, level)  ((level - val1) / (val2 - val1))

/* Global variables */

static PLFLT sh_max, sh_min;
static int min_points, max_points, n_point;
static int min_pts[4], max_pts[4];
static PLINT pen_col_min, pen_col_max;
static PLINT pen_wd_min, pen_wd_max;
static PLFLT int_val;
/* Function prototypes */

static void 
set_cond(register int *cond, register PLFLT *a,
	 register const char *defined, register PLINT n);

static int 
find_interval(PLFLT a0, PLFLT a1, PLINT c0, PLINT c1, PLFLT *x);

static void 
poly(void (*fill) (PLINT, PLFLT *, PLFLT *),
     PLFLT *x, PLFLT *y, PLINT v1, PLINT v2, PLINT v3, PLINT v4);

static void 
big_recl(int *cond_code, register int ny, int dx, int dy,
	 int *ix, int *iy);

static void 
draw_boundary(PLINT slope, PLFLT *x, PLFLT *y);

static PLINT 
plctest(PLFLT *x, PLFLT level);

static PLINT 
plctestez(PLFLT *a, PLINT nx, PLINT ny, PLINT ix,
	  PLINT iy, PLFLT level);

static void
plshade_int(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	PLPointer c2eval_data, 
	const char *defined, PLFLT missing_min, PLFLT missing_max,
	PLINT nx, PLINT ny, 
	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLFLT shade_min, PLFLT shade_max,
	PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	PLINT min_color, PLINT min_width,
	PLINT max_color, PLINT max_width,
	void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data);

/*----------------------------------------------------------------------*\
 * plshade()
 *
 * Shade region.
 * This interface to plfshade() assumes the 2d function array is passed
 * via a (PLFLT **), and is column-dominant (normal C ordering).
\*----------------------------------------------------------------------*/

void c_plshade( PLFLT **a, PLINT nx, PLINT ny, const char *defined,
                 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                 PLFLT shade_min, PLFLT shade_max,
                 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
                 PLINT min_color, PLINT min_width,
                 PLINT max_color, PLINT max_width,
                 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
                 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
                 PLPointer pltr_data )
{
    PLfGrid2 grid;

    grid.f = a;
    grid.nx = nx;
    grid.ny = ny;

    plshade_int( plf2eval2, (PLPointer) &grid,
                 NULL, NULL,
/*	     plc2eval, (PLPointer) &cgrid,*/
                 defined, MISSING_MIN_DEF, MISSING_MAX_DEF, nx, ny, xmin, 
                 xmax, ymin, ymax, shade_min, shade_max,
                 sh_cmap, sh_color, sh_width,
                 min_color, min_width, max_color, max_width,
                 fill, rectangular, pltr, pltr_data );
}

/*----------------------------------------------------------------------*\
 * plshade1()
 *
 * Shade region.
 * This interface to plfshade() assumes the 2d function array is passed
 * via a (PLFLT *), and is column-dominant (normal C ordering).
\*----------------------------------------------------------------------*/

void c_plshade1( PLFLT *a, PLINT nx, PLINT ny, const char *defined,
                 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                 PLFLT shade_min, PLFLT shade_max,
                 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
                 PLINT min_color, PLINT min_width,
                 PLINT max_color, PLINT max_width,
                 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
                 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
                 PLPointer pltr_data )
{
    PLfGrid grid;

    grid.f = a;
    grid.nx = nx;
    grid.ny = ny;

    plshade_int( plf2eval, (PLPointer) &grid,
                 NULL, NULL,
/*	     plc2eval, (PLPointer) &cgrid,*/
                 defined, MISSING_MIN_DEF, MISSING_MAX_DEF, nx, ny, xmin, 
                 xmax, ymin, ymax, shade_min, shade_max,
                 sh_cmap, sh_color, sh_width,
                 min_color, min_width, max_color, max_width,
                 fill, rectangular, pltr, pltr_data );
}

/*----------------------------------------------------------------------*\
 * plfshade()
 *
 * Shade region.  
 * Array values are determined by the input function and the passed data.
\*----------------------------------------------------------------------*/

void 
plfshade(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	 PLPointer f2eval_data,
	 PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny, 
	 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data)
{
    plshade_int(f2eval,  f2eval_data, c2eval, c2eval_data, 
	 NULL, MISSING_MIN_DEF, MISSING_MAX_DEF,
	 nx, ny, xmin, xmax, ymin, ymax,
	 shade_min, shade_max, sh_cmap, sh_color, sh_width,
	 min_color, min_width, max_color, max_width,
	 fill, rectangular, pltr, pltr_data);
}


/*----------------------------------------------------------------------*\
 * plshade_int()
 *
 * Shade region -- this routine does all the work
 *
 * This routine is internal so the arguments can and will change.
 * To retain some compatibility between versions, you must go through
 * some stub routine!
 *
 * 4/95
 *
 * new: missing_min, missing_max
 *
 *     if data <= missing_max and data >= missing_min
 *       then the data will beconsidered to be missing
 *     this allows 2nd way to set undefined points (good for ftn)
 *     if missing feature is not used, set missing_max < missing_min
 *
 * parameters:
 *
 * f2eval, f2eval_data:  data to plot
 * c2eval, c2eval_data:  defined mask (not implimented)
 * defined: defined mask (old API - implimented)
 * missing_min, missing_max: yet another way to set data to undefined
 * nx, ny: array dimensions
 * xmin, xmax, ymin, ymax: grid coordinates
 * shade_min, shade_max: shade region with values between ...
 * sh_cmap, sh_color, sh_width: shading parameters, width is only for hatching
 * min_color, min_width: line parameters for boundary (minimum)
 * max_color, max_width: line parameters for boundary (maximum)
 *     set min_width == 0 and max_width == 0 for no contours
 * fill: fill function, set to NULL for no shading (contour plot)
 * rectangular: flag set to 1 if pltr() maps rectangles to rectangles
 *     this helps optimize the plotting
 * pltr: function to map from grid to plot coordinates
 *
 *
\*----------------------------------------------------------------------*/

static void
plshade_int(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	PLPointer c2eval_data, 
	const char *defined, PLFLT missing_min, PLFLT missing_max,
	PLINT nx, PLINT ny, 
	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLFLT shade_min, PLFLT shade_max,
	PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	PLINT min_color, PLINT min_width,
	PLINT max_color, PLINT max_width,
	void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data)
{

    PLINT init_width, n, slope = 0, ix, iy;
    int count, i, j, nxny;
    PLFLT *a, *a0, *a1, dx, dy;
    PLFLT x[8], y[8], xp[2], tx, ty;
    char *new_defined;
    int *c, *c0, *c1;

    if (plsc->level < 3) {
	plabort("plfshade: window must be set up first");
	return;
    }

    if (nx <= 0 || ny <= 0) {
	plabort("plfshade: nx and ny must be positive");
	return;
    }

    if (shade_min >= shade_max) {
	plabort("plfshade: shade_max must exceed shade_min");
	return;
    }

    if (pltr == NULL || pltr_data == NULL)
	rectangular = 1;

    int_val = shade_max - shade_min;
    init_width = plsc->width;

    pen_col_min = min_color;
    pen_col_max = max_color;

    pen_wd_min = min_width;
    pen_wd_max = max_width;

    plstyl((PLINT) 0, NULL, NULL);
    plwid(sh_width); 
    if (fill != NULL) {
        switch (sh_cmap) {
        case 0:
            plcol0((PLINT) sh_color);
	    break;
        case 1:
	    plcol1(sh_color);
	    break;
        default:
	    plabort("plfshade: invalid color map selection");
	    return;
        }
    }
    /* alloc space for value array, and initialize */
    /* This is only a temporary kludge */
    nxny = nx * ny;
    if ((a = (PLFLT *) malloc(nxny * sizeof(PLFLT))) == NULL) {
	plabort("plfshade: unable to allocate memory for value array");
	return;
    }

    for (ix = 0; ix < nx; ix++) 
	for (iy = 0; iy < ny; iy++) 
	    a[iy + ix*ny] = f2eval(ix, iy, f2eval_data);

    /* alloc space for condition codes */

    if ((c = (int *) malloc(nxny * sizeof(int))) == NULL) {
	plabort("plfshade: unable to allocate memory for condition codes");
	free(a);
	return;
    }

    sh_min = shade_min;
    sh_max = shade_max;

    /* setup new_defined */
    if (defined == NULL && missing_max < missing_min) {
	new_defined = NULL;
    }
    else {
        if ((new_defined = (char *) malloc(nxny * sizeof(char))) == NULL) {
	    plabort("plfshade: unable to allocate memory for condition codes");
	    free(c);
	    free(a);
            return;
	}
	if (defined == NULL) {
	    for (i = 0; i < nxny; i++) new_defined[i] = 1;
	}
	else {
	    memcpy(new_defined, defined, nxny);
	}
	for (i = 0; i < nxny; i++) {
	    if (new_defined[i] == 1 && a[i] >= missing_min &&
		a[i] <= missing_max) new_defined[i] = 0;
	}
    }

    /* Ignore defined array for now */

    set_cond(c, a, new_defined, nxny);
    dx = (xmax - xmin) / (nx - 1);
    dy = (ymax - ymin) / (ny - 1);
    a0 = a;
    a1 = a + ny;
    c0 = c;
    c1 = c + ny;

    for (ix = 0; ix < nx - 1; ix++) {

	for (iy = 0; iy < ny - 1; iy++) {

	    count = c0[iy] + c0[iy + 1] + c1[iy] + c1[iy + 1];

	    /* No filling needs to be done for these cases */

	    if (count >= UNDEF)
		continue;
	    if (count == 4 * POS)
		continue;
	    if (count == 4 * NEG)
		continue;

	    /* Entire rectangle can be filled */

	    if (count == 4 * OK) {
		/* find bigest rectangle that fits */
		if (rectangular) {
		    big_recl(c0 + iy, ny, nx - ix, ny - iy, &i, &j);
		}
		else {
		    i = j = 1;
		}
		x[0] = x[1] = ix;
		x[2] = x[3] = ix+i;
		y[0] = y[3] = iy;
		y[1] = y[2] = iy+j;

		if (pltr && pltr_data) {
		    for (i = 0; i < 4; i++) {
			(*pltr) (x[i], y[i], &tx, &ty, pltr_data);
			x[i] = tx;
			y[i] = ty;
		    }
		}
		else {
		    for (i = 0; i < 4; i++) {
			x[i] = xmin + x[i]*dx;
			y[i] = ymin + y[i]*dy;
		    }
		}
		if (fill)
		    (*fill) ((PLINT) 4, x, y);
		iy += j - 1;
		continue;
	    }

	    /* Only part of rectangle can be filled */

	    n_point = min_points = max_points = 0;
	    n = find_interval(a0[iy], a0[iy + 1], c0[iy], c0[iy + 1], xp);
	    for (j = 0; j < n; j++) {
		x[j] = ix;
		y[j] = iy + xp[j];
	    }

	    i = find_interval(a0[iy + 1], a1[iy + 1],
			      c0[iy + 1], c1[iy + 1], xp);

	    for (j = 0; j < i; j++) {
		x[j + n] = ix + xp[j];
		y[j + n] = iy + 1;
	    }
	    n += i;

	    i = find_interval(a1[iy + 1], a1[iy], c1[iy + 1], c1[iy], xp);
	    for (j = 0; j < i; j++) {
		x[n + j] = ix + 1;
		y[n + j] = iy + 1 - xp[j];
	    }
	    n += i;

	    i = find_interval(a1[iy], a0[iy], c1[iy], c0[iy], xp);
	    for (j = 0; j < i; j++) {
		x[n + j] = ix + 1 - xp[j];
		y[n + j] = iy;
	    }
	    n += i;

	    if (pltr && pltr_data) {
		for (i = 0; i < n; i++) {
		    (*pltr) (x[i], y[i], &tx, &ty, pltr_data);
		    x[i] = tx;
		    y[i] = ty;
		}
	    }
	    else {
		for (i = 0; i < n; i++) {
		    x[i] = xmin + x[i]*dx;
		    y[i] = ymin + y[i]*dy;
		}
	    }

	    if (min_points == 4)
		slope = plctestez(a, nx, ny, ix, iy, shade_min);
	    if (max_points == 4)
		slope = plctestez(a, nx, ny, ix, iy, shade_max);

	    /* n = number of end of line segments */
	    /* min_points = number times shade_min meets edge */
	    /* max_points = number times shade_max meets edge */

	    /* special cases: check number of times a contour is in a box */

	    switch ((min_points << 3) + max_points) {
	      case 000:
	      case 020:
	      case 002:
	      case 022:
		if (fill && n > 0)
		    (*fill) (n, x, y);
		break;
	      case 040:	/* 2 contour lines in box */
	      case 004:
		if (n != 6)
		    fprintf(stderr, "plfshade err n=%d !6", (int) n);
		if (slope == 1 && c0[iy] == OK) {
		    if (fill)
			(*fill) (n, x, y);
		}
		else if (slope == 1) {
		    poly(fill, x, y, 0, 1, 2, -1);
		    poly(fill, x, y, 3, 4, 5, -1);
		}
		else if (c0[iy + 1] == OK) {
		    if (fill)
			(*fill) (n, x, y);
		}
		else {
		    poly(fill, x, y, 0, 1, 5, -1);
		    poly(fill, x, y, 2, 3, 4, -1);
		}
		break;
	      case 044:
		if (n != 8)
		    fprintf(stderr, "plfshade err n=%d !8", (int) n);
		if (slope == 1) {
		    poly(fill, x, y, 0, 1, 2, 3);
		    poly(fill, x, y, 4, 5, 6, 7);
		}
		else {
		    poly(fill, x, y, 0, 1, 6, 7);
		    poly(fill, x, y, 2, 3, 4, 5);
		}
		break;
	      case 024:
	      case 042:
		/* 3 contours */
		if (n != 7)
		    fprintf(stderr, "plfshade err n=%d !7", (int) n);

		if ((c0[iy] == OK || c1[iy+1] == OK) && slope == 1) {
		    if (fill)
		        (*fill) (n, x, y);
		}
		else if ((c0[iy+1] == OK || c1[iy] == OK) && slope == 0) {
		    if (fill)
		        (*fill) (n, x, y);
		}

		else if (c0[iy] == OK) {
		    poly(fill, x, y, 0, 1, 6, -1);
		    poly(fill, x, y, 2, 3, 4, 5);
		}
		else if (c0[iy+1] == OK) {
		    poly(fill, x, y, 0, 1, 2, -1);
		    poly(fill, x, y, 3, 4, 5, 6);
		}
		else if (c1[iy+1] == OK) {
		    poly(fill, x, y, 0, 1, 5, 6);
		    poly(fill, x, y, 2, 3, 4, -1);
		}
		else if (c1[iy] == OK) {
		    poly(fill, x, y, 0, 1, 2, 3);
		    poly(fill, x, y, 4, 5, 6, -1);
		}
		else {
		    fprintf(stderr, "plfshade err logic case 024:042\n");
		}
		break;
	      default:
		fprintf(stderr, "prog err switch\n");
		break;
	    }
	    draw_boundary(slope, x, y);

	    if (fill != NULL) {
	        plwid(sh_width);
		if (sh_cmap == 0) plcol0((PLINT) sh_color);
		else if (sh_cmap == 1) plcol1(sh_color);
	    }
	}

	a0 = a1;
	c0 = c1;
	a1 += ny;
	c1 += ny;
    }

    free(c);
    free(a);
    if (new_defined) free(new_defined);
    plwid(init_width);
}

/*----------------------------------------------------------------------*\
 * set_cond()
 *
 * Fills out condition code array.
\*----------------------------------------------------------------------*/

static void 
set_cond(register int *cond, register PLFLT *a,
	 register const char *defined, register PLINT n)
{
    if (defined) {
	while (n--) {
	    if (*defined++ == 0)
		*cond++ = UNDEF;
	    else if (*a < sh_min)
		*cond++ = NEG;
	    else if (*a > sh_max)
		*cond++ = POS;
	    else
		*cond++ = OK;
	    a++;
	}
    }
    else {
	while (n--) {
	    if (*a < sh_min)
		*cond++ = NEG;
	    else if (*a > sh_max)
		*cond++ = POS;
	    else
		*cond++ = OK;
	    a++;
	}
    }
}

/*----------------------------------------------------------------------*\
 * find_interval()
 *
 * Two points x(0) = a0, (condition code c0) x(1) = a1, (condition code c1)
 * return interval on the line that are shaded
 * 
 * returns 0 : no points to be shaded 1 : x[0] <= x < 1 is the interval 2 :
 * x[0] <= x <= x[1] < 1 interval to be shaded n_point, max_points,
 * min_points are incremented location of min/max_points are stored 
\*----------------------------------------------------------------------*/

static int 
find_interval(PLFLT a0, PLFLT a1, PLINT c0, PLINT c1, PLFLT *x)
{
    register int n;

    n = 0;
    if (c0 == OK) {
	x[n++] = 0.0;
	n_point++;
    }
    if (c0 == c1)
	return n;

    if (c0 == NEG || c1 == POS) {
	if (c0 == NEG) {
	    x[n++] = linear(a0, a1, sh_min);
	    min_pts[min_points++] = n_point++;
	}
	if (c1 == POS) {
	    x[n++] = linear(a0, a1, sh_max);
	    max_pts[max_points++] = n_point++;
	}
    }
    if (c0 == POS || c1 == NEG) {
	if (c0 == POS) {
	    x[n++] = linear(a0, a1, sh_max);
	    max_pts[max_points++] = n_point++;
	}
	if (c1 == NEG) {
	    x[n++] = linear(a0, a1, sh_min);
	    min_pts[min_points++] = n_point++;
	}
    }
    return n;
}

/*----------------------------------------------------------------------*\
 * poly()
 *
 * Draws a polygon from points in x[] and y[].
 * Point selected by v1..v4 
\*----------------------------------------------------------------------*/

static void 
poly(void (*fill) (PLINT, PLFLT *, PLFLT *),
     PLFLT *x, PLFLT *y, PLINT v1, PLINT v2, PLINT v3, PLINT v4)
{
    register PLINT n = 0;
    PLFLT xx[4], yy[4];

    if (fill == NULL)
	return;
    if (v1 >= 0) {
	xx[n] = x[v1];
	yy[n++] = y[v1];
    }
    if (v2 >= 0) {
	xx[n] = x[v2];
	yy[n++] = y[v2];
    }
    if (v3 >= 0) {
	xx[n] = x[v3];
	yy[n++] = y[v3];
    }
    if (v4 >= 0) {
	xx[n] = x[v4];
	yy[n++] = y[v4];
    }
    (*fill) (n, xx, yy);
}

/*----------------------------------------------------------------------*\
 * big_recl()
 *
 * find a big rectangle for shading
 *
 * 2 goals: minimize calls to (*fill)()
 *  keep ratio 1:3 for biggest rectangle
 *
 * only called by plshade()
 *
 * assumed that a 1 x 1 square already fits
 *
 * c[] = condition codes
 * ny = c[1][0] == c[ny]  (you know what I mean)
 *
 * returns ix, iy = length of rectangle in grid units
 *
 * ix < dx - 1
 * iy < dy - 1
 *
 * If iy == 1 -> ix = 1 (so that cond code can be set to skip)
\*----------------------------------------------------------------------*/

#define RATIO 3
#define COND(x,y) cond_code[x*ny + y]

static void 
big_recl(int *cond_code, register int ny, int dx, int dy,
	 int *ix, int *iy)
{

    int ok_x, ok_y, j;
    register int i, x, y;
    register int *cond;

    /* ok_x = ok to expand in x direction */
    /* x = current number of points in x direction */

    ok_x = ok_y = 1;
    x = y = 2;

    while (ok_x || ok_y) {
#ifdef RATIO
	if (RATIO * x <= y || RATIO * y <= x)
	    break;
#endif
	if (ok_y) {
	    /* expand in vertical */
	    ok_y = 0;
	    if (y == dy)
		continue;
	    cond = &COND(0, y);
	    for (i = 0; i < x; i++) {
		if (*cond != OK)
		    break;
		cond += ny;
	    }
	    if (i == x) {
		/* row is ok */
		y++;
		ok_y = 1;
	    }
	}
	if (ok_x) {
	    if (y == 2)
		break;
	    /* expand in x direction */
	    ok_x = 0;
	    if (x == dx)
		continue;
	    cond = &COND(x, 0);
	    for (i = 0; i < y; i++) {
		if (*cond++ != OK)
		    break;
	    }
	    if (i == y) {
		/* column is OK */
		x++;
		ok_x = 1;
	    }
	}
    }

    /* found the largest rectangle of 'ix' by 'iy' */
    *ix = --x;
    *iy = --y;

    /* set condition code to UNDEF in interior of rectangle */

    for (i = 1; i < x; i++) {
	cond = &COND(i, 1);
	for (j = 1; j < y; j++) {
	    *cond++ = UNDEF;
	}
    }
}

/*----------------------------------------------------------------------*\
 * draw_boundary()
 *
 * Draw boundaries of contour regions based on min_pts[], and max_pts[].
\*----------------------------------------------------------------------*/

static void 
draw_boundary(PLINT slope, PLFLT *x, PLFLT *y)
{
    int i;

    if (pen_col_min != 0 && pen_wd_min != 0 && min_points != 0) {
	plcol0(pen_col_min);
	plwid(pen_wd_min);
	if (min_points == 4 && slope == 0) {
	    /* swap points 1 and 3 */
	    i = min_pts[1];
	    min_pts[1] = min_pts[3];
	    min_pts[3] = i;
	}
	pljoin(x[min_pts[0]], y[min_pts[0]], x[min_pts[1]], y[min_pts[1]]);
	if (min_points == 4) {
	    pljoin(x[min_pts[2]], y[min_pts[2]], x[min_pts[3]],
		   y[min_pts[3]]);
	}
    }
    if (pen_col_max != 0 && pen_wd_max != 0 && max_points != 0) {
	plcol0(pen_col_max);
	plwid(pen_wd_max);
	if (max_points == 4 && slope == 0) {
	    /* swap points 1 and 3 */
	    i = max_pts[1];
	    max_pts[1] = max_pts[3];
	    max_pts[3] = i;
	}
	pljoin(x[max_pts[0]], y[max_pts[0]], x[max_pts[1]], y[max_pts[1]]);
	if (max_points == 4) {
	    pljoin(x[max_pts[2]], y[max_pts[2]], x[max_pts[3]],
		   y[max_pts[3]]);
	}
    }
}

/*----------------------------------------------------------------------*\
 *
 * plctest( &(x[0][0]), PLFLT level)
 * where x was defined as PLFLT x[4][4];
 *
 * determines if the contours associated with level have
 * positive slope or negative slope in the box:
 *
 *  (2,3)   (3,3)
 *
 *  (2,2)   (3,2)
 *
 * this is heuristic and can be changed by the user
 *
 * return 1 if positive slope
 *        0 if negative slope
 *
 * algorithmn:
 *      1st test:
 *	find length of contours assuming positive and negative slopes
 *      if the length of the negative slope contours is much bigger
 *	than the positive slope, then the slope is positive.
 *      (and vice versa)
 *      (this test tries to minimize the length of contours)
 *
 *      2nd test:
 *      if abs((top-right corner) - (bottom left corner)) >
 *	   abs((top-left corner) - (bottom right corner)) ) then
 *		return negatiave slope.
 *      (this test tries to keep the slope for different contour levels
 *	the same)
\*----------------------------------------------------------------------*/

#define X(a,b) (x[a*4+b])
#define POSITIVE_SLOPE (PLINT) 1
#define NEGATIVE_SLOPE (PLINT) 0
#define RATIO_SQ 6.0

static PLINT 
plctest(PLFLT *x, PLFLT level)
{
    int i, j;
    double t[4], sorted[4], temp;

    sorted[0] = t[0] = X(1,1);
    sorted[1] = t[1] = X(2,2);
    sorted[2] = t[2] = X(1,2);
    sorted[3] = t[3] = X(2,1);

    for (j = 1; j < 4; j++) {
	temp = sorted[j];
	i = j - 1;
	while (i >= 0 && sorted[i] > temp) {
	    sorted[i+1] = sorted[i];
	    i--;
	}
	sorted[i+1] = temp;
    }
    /* sorted[0] == min */

    /* find min contour */
    temp = int_val * ceil(sorted[0]/int_val);
    if (temp < sorted[1]) {
	/* one contour line */
	for (i = 0; i < 4; i++) {
	    if (t[i] < temp) return i/2;
	}
    }
	
    /* find max contour */
    temp = int_val * floor(sorted[3]/int_val);
    if (temp > sorted[2]) {
	/* one contour line */
	for (i = 0; i < 4; i++) {
	    if (t[i] > temp) return i/2;
	}
    }
    /* nothing better to do - be consistant */
    return POSITIVE_SLOPE;
}

/*----------------------------------------------------------------------*\
 * plctestez
 *
 * second routine - easier to use
 * fills in x[4][4] and calls plctest
 *
 * test location a[ix][iy] (lower left corner)
\*----------------------------------------------------------------------*/

static PLINT 
plctestez(PLFLT *a, PLINT nx, PLINT ny, PLINT ix,
	  PLINT iy, PLFLT level)
{

    PLFLT x[4][4];
    int i, j, ii, jj;

    for (i = 0; i < 4; i++) {
	ii = ix + i - 1;
	ii = MAX(0, ii);
	ii = MIN(ii, nx - 1);
	for (j = 0; j < 4; j++) {
	    jj = iy + j - 1;
	    jj = MAX(0, jj);
	    jj = MIN(jj, ny - 1);
	    x[i][j] = a[ii * ny + jj];
	}
    }
    return plctest(&(x[0][0]), level);
}
