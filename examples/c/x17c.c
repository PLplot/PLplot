/* $Id$
 * $Log$
 * Revision 1.1  1994/04/08 12:08:54  mjl
 * Preliminary stab at a strip chart demo (doesn't work yet).
 *
*/

/* Demonstration program for PLPLOT: */

/* Plots a simple stripchart.
 * Eventually I want a really cool demo here: slowly evolving plots of
 * say density and temperature.  These only need to get updated every so
 * often.  And then at the bottom some strip charts of energy or such
 * that are continually updated.
 */

#include <plplot.h>
#include <math.h>

/* Data declarations for stripcharts. */

typedef struct {
    PLINT npts, nptsmax;
    PLFLT *x, *y;
    PLFLT xmin, xmax, ymin, ymax, xjump, xlen;

    char *xspec, *yspec, *labx, *laby, *labtop;
    PLINT colbox, colline, collab;
} PLStrip;

static int sid;				/* strip id number */
static PLStrip *strip[100];		/* Array of pointers */
static PLStrip *stripc;			/* current strip chart */

/* Create 1d stripchart */

void
plstripc(PLINT *id, char *xspec, char *yspec,
	 PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
	 PLINT colbox, PLINT colline, PLINT collab,
	 char *labx, char *laby, char *labtop);

/* Generates a complete stripchart plot.  */

void
plstrip_gen(PLStrip *strip);

/* Add a point to a stripchart.  */

void
plstripa(PLINT id, PLFLT x, PLFLT y);

/* Deletes and releases memory used by a stripchart.  */

void
plstripd(PLINT id);

/* Miscellaneous */

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/*----------------------------------------------------------------------*\
* main program
\*----------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
    PLINT id, n, nsteps = 1000;
    PLFLT y, ymin, ymax;
    PLFLT t, tmin, tmax, tjump, dt;
    PLINT colbox, collab, colline;

/* plplot initialization */
/* Parse and process command line arguments */

    (void) plParseInternalOpts(&argc, argv, PL_PARSE_FULL);

    plSetInternalOpt("db", "");
    plSetInternalOpt("np", "");

/* Initialize plplot */

    plinit();

/* Create strip chart */

/* User sets up plot completely except for window and data 
 * Eventually settings in place when strip chart is created will be
 * remembered so that multiple strip charts can be used simultaneously.
 */

    pladv(0);

/* For now just a standard viewport */

    plvsta();
    plcol(4);

/* Specify some reasonable defaults for ymin and ymax */
/* The plot will grow automatically if needed (but not shrink) */

    ymin = -1.0;
    ymax =  1.0;

/* Specify initial tmin and tmax -- this determines length of window.
/* Also specify maximum jump in t */
/* This can accomodate adaptive timesteps */

    tmin = 0.;
    tmax = 100.;
    tjump = 25.;

/* Axes options same as plbox. */
/* Only automatic tick generation and label placement allowed */
/* Eventually I'll make this fancier */

    colbox = 1;
    colline = 4;
    collab = 3;

    plstripc(&id, "bcnst", "bcnstv",
	     tmin, tmax, tjump, ymin, ymax,
	     colbox, colline, collab,
	     "t", "d", "displacement vs time");

/* This is to represent a loop over time */
/* Let's try a random walk process */

    y = 0.0;
    dt = 0.1;
    for (n = 0; n < nsteps; n++) {
	t = n * dt;
	y = y + rand();

	plstripa(id, t, y);
    }

/* Destroy strip chart and it's memory */

    plstripd(id);

    plend();
    exit(0);
}

/*----------------------------------------------------------------------*\
* plstripc
*
* Create 1d stripchart.
\*----------------------------------------------------------------------*/

void
plstripc(PLINT *id, char *xspec, char *yspec,
	 PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
	 PLINT colbox, PLINT colline, PLINT collab,
	 char *labx, char *laby, char *labtop)
{
    int i;

/* Get a free strip id and allocate it */

    for (i = 0; i < 100; i++) {
	if (strip[i] == NULL)
	    break;
    }

    if (i == 0) {
	fprintf(stderr, "plstripc: Cannot create new strip chart\n");
	*id = -1;
	return;
    }
    else {
	sid = *id = i;
	strip[sid] = (PLStrip *) malloc((size_t) sizeof(PLStrip));
	if (strip[sid] == NULL)
	    plexit("plstripc: Out of memory.");

	memset((char *) strip[sid], 0, sizeof(PLStrip));
    }

/* Fill up the struct with all relevant info */

    stripc = strip[sid];

    stripc->npts = 0;
    stripc->nptsmax = 100;
    stripc->x = (PLFLT *) malloc((size_t) sizeof(PLFLT) * stripc->nptsmax);;
    stripc->y = (PLFLT *) malloc((size_t) sizeof(PLFLT) * stripc->nptsmax);;
    if (stripc->x == NULL || stripc->y == NULL)
	plexit("plstripc: Out of memory.");

    stripc->xmin = xmin;
    stripc->xmax = xmax;
    stripc->ymin = ymin;
    stripc->ymax = ymax;
    stripc->xjump = xjump;
    stripc->xlen = xmax - xmin;
    stripc->xspec = xspec;
    stripc->yspec = yspec;
    stripc->labx = labx;
    stripc->laby = laby;
    stripc->labtop = labtop;
    stripc->colbox = colbox;
    stripc->colline = colline;
    stripc->collab = collab;

/* Generate the plot */

    plstrip_gen(stripc);
}

/*----------------------------------------------------------------------*\
* plstrip_gen
*
* Generates a complete stripchart plot.  Used either initially or
* during rescaling.
\*----------------------------------------------------------------------*/

void
plstrip_gen(PLStrip *strip)
{

/* Set up window */

    plwind(strip->xmin, strip->xmax, strip->ymin, strip->ymax);

/* Draw box */

    plcol(strip->colbox);
    plbox(strip->xspec, 0.0, 0, strip->yspec, 0.0, 0);

    plcol(strip->collab);
    pllab(strip->labx, strip->laby, strip->labtop);

    if (strip->npts > 0) {
	plcol(strip->colline);
	plline(strip->npts, strip->x, strip->y);
    }
}

/*----------------------------------------------------------------------*\
* plstripa
*
* Add a point to a stripchart.  
* Allocates memory and rescales as necessary.
\*----------------------------------------------------------------------*/

void
plstripa(PLINT id, PLFLT x, PLFLT y)
{
    int i, istart;

    stripc = strip[id];

/* Add new point, allocating memory if necessary */

    if (++stripc->npts > stripc->nptsmax) {
	stripc->nptsmax += 32;
	stripc->x = (PLFLT *) realloc((void *) stripc->x, stripc->nptsmax);
	stripc->y = (PLFLT *) realloc((void *) stripc->y, stripc->nptsmax);
    }
    stripc->x[stripc->npts-1] = x;
    stripc->y[stripc->npts-1] = y;

    stripc->xmax = x;
    stripc->ymax = MAX(y, stripc->ymax);
    stripc->ymin = MIN(y, stripc->ymin);

/* Now either plot new point or regenerate plot */

    if (stripc->xmax - stripc->xmin < stripc->xlen) {
	plP_movwor(stripc->x[stripc->npts-2], stripc->y[stripc->npts-2]);
	plP_drawor(stripc->x[stripc->npts-1], stripc->y[stripc->npts-1]);
    }
    else {

/* Regenerating plot */
/* First push back the x scale */

	stripc->xmin -= stripc->xjump;
	stripc->xmax -= stripc->xjump;
	istart = 0;
	while (stripc->x[istart] - stripc->xjump < 0.)
	    istart++;

	stripc->npts -= istart;
	for (i = 0; i < stripc->npts; i++) {
	    stripc->x[i] = stripc->x[i+istart];
	    stripc->y[i] = stripc->y[i+istart];
	}

	stripc->xmin = stripc->x[0];
	stripc->xmax = stripc->x[stripc->npts];

/* Now do the real work */

	plstrip_gen(stripc);
    }
}

/*----------------------------------------------------------------------*\
* plstripd
*
* Deletes and releases memory used by a stripchart.  
\*----------------------------------------------------------------------*/

void
plstripd(PLINT id)
{
    stripc = strip[id];

    free((void *) stripc->x);
    free((void *) stripc->y);
    free((void *) stripc);
}
