/* $Id$
 * $Log$
 * Revision 1.19  1995/03/17 00:14:25  mjl
 * Changed printf's to fprintf's to stdout or stderr.  Added plGinInit()
 * to fill in a PLGraphicsIn with appropriate initial values.
 *
 * Revision 1.18  1994/07/26  18:17:24  mjl
 * Added missing variable declaration and initializer.
 *
 * Revision 1.17  1994/07/26  09:00:46  mjl
 * Added a quick fix so that hitting a carriage return when prompted for
 * a filename no longer aborts the program.  Contributed by Mark Olesen.
 *
 * Revision 1.16  1994/06/30  18:22:17  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
*/

/*	plstream.c

	Miscellaneous stream & device support functions.
*/

#include "plplotP.h"

/*--------------------------------------------------------------------------*\
 * plcol_interp()
 *
 * Initializes device cmap 1 entry by interpolation from pls->cmap1
 * entries.  Returned PLColor is supposed to represent the i_th color
 * out of a total of ncol colors in the current color scheme.
\*--------------------------------------------------------------------------*/

void
plcol_interp(PLStream *pls, PLColor *newcolor, int i, int ncol)
{
    float x, delta;
    int il, ir;

    x = (double) (i * (pls->ncol1-1)) / (double) (ncol-1);
    il = x;
    ir = il + 1;
    delta = x - il;

    if (ir > pls->ncol1 || il < 0)
	fprintf(stderr, "Invalid color\n");

    else if (ir == pls->ncol1 || (delta == 0.)) {
	newcolor->r = pls->cmap1[il].r;
	newcolor->g = pls->cmap1[il].g;
	newcolor->b = pls->cmap1[il].b;
    }
    else {
	newcolor->r = (1.-delta) * pls->cmap1[il].r + delta * pls->cmap1[ir].r;
	newcolor->g = (1.-delta) * pls->cmap1[il].g + delta * pls->cmap1[ir].g;
	newcolor->b = (1.-delta) * pls->cmap1[il].b + delta * pls->cmap1[ir].b;
    }
}

/*--------------------------------------------------------------------------*\
 * plOpenFile()
 *
 * Opens file for output, prompting if not set.
 * Prints extra newline at end to make output look better in batch runs.
 * A file name of "-" indicates output to stdout.
\*--------------------------------------------------------------------------*/

#define MAX_NUM_TRIES	10
void
plOpenFile(PLStream *pls)
{
    int i = 0, count = 0;
    size_t len;
    char line[256];

    while (pls->OutFile == NULL) {

/* Setting pls->FileName = NULL forces creation of a new family member */
/* You should also free the memory associated with it if you do this */

	if (pls->family && pls->BaseName != NULL)
	    plP_getmember(pls);

/* Prompt if filename still not known */

	if (pls->FileName == NULL) {
	    do {
		fprintf(stdout, "Enter graphics output file name: ");
		fgets(line, sizeof(line), stdin);
		len = strlen(line);
		if (len)
		    len--;
		line[len] = '\0';	/* strip new-line */
		count++;		/* count zero entries */
	    } while (!len && count < MAX_NUM_TRIES);
	    plP_sfnam(pls, line);
	}

/* If name is "-", send to stdout */

	if ( ! strcmp(pls->FileName, "-")) {
	    pls->OutFile = stdout;
	    pls->output_type = 1;
	    break;
	}

/* Need this here again, for prompted family initialization */

	if (pls->family && pls->BaseName != NULL) 
	    plP_getmember(pls);

	if (i++ > 10)
	    plexit("Too many tries.");

	if ((pls->OutFile = fopen(pls->FileName, "wb+")) == NULL) 
	    fprintf(stdout, "Can't open %s.\n", pls->FileName);
	else
	    fprintf(stderr, "Created %s\n", pls->FileName);
    }
}

/*--------------------------------------------------------------------------*\
 * plP_getmember()
 *
 * Sets up next file member name (in pls->FileName), but does not open it.
\*--------------------------------------------------------------------------*/

void
plP_getmember(PLStream *pls)
{
    char tmp[256];

    if (pls->FileName == NULL)
	pls->FileName = (char *) malloc(10 + strlen(pls->BaseName));

    sprintf(tmp, "%s.%%0%1ii", pls->BaseName, (int) pls->fflen);
    sprintf(pls->FileName, tmp, pls->member);
}

/*--------------------------------------------------------------------------*\
 * plP_sfnam()
 *
 * Sets up file name & family stem name.
 * Reserve some extra space (5 chars) to hold an optional member number.
\*--------------------------------------------------------------------------*/

void
plP_sfnam(PLStream *pls, const char *fnam)
{
    pls->OutFile = NULL;

    if (pls->FileName != NULL)
	free((void *) pls->FileName);

    pls->FileName = (char *) malloc(10 + strlen(fnam));

    strcpy(pls->FileName, fnam);

    if (pls->BaseName != NULL)
	free((void *) pls->BaseName);

    pls->BaseName = (char *) malloc(10 + strlen(fnam));

    strcpy(pls->BaseName, fnam);
}

/*--------------------------------------------------------------------------*\
 * plFamInit()
 *
 * Initializes family file parameters.
\*--------------------------------------------------------------------------*/

void
plFamInit(PLStream *pls)
{
    if (pls->family) {
	pls->bytecnt = 0;
	if ( ! pls->member)
	    pls->member = 1;
	if ( ! pls->finc)
	    pls->finc = 1;
	if ( ! pls->fflen)
	    pls->fflen = 1;
	if ( ! pls->bytemax)
	    pls->bytemax = PL_FILESIZE_KB * 1000;
    }
}

/*--------------------------------------------------------------------------*\
 * plGetFam()
 *
 * Starts new member file of family file set if necessary.
 *
 * Note each member file is a complete graphics file (can be printed
 * individually), although 'plrender' will treat a family as a single
 * logical file if given the family name instead of the member name.
\*--------------------------------------------------------------------------*/

void
plGetFam(PLStream *pls)
{
    if (pls->family) {
	if (pls->bytecnt > pls->bytemax || pls->famadv) {
	    plP_tidy();
	    pls->member += pls->finc;
	    pls->famadv = 0;
	    plP_init();
	    return;
	}
    }
}

/*--------------------------------------------------------------------------*\
 * plRotPhy()
 *
 * Rotates physical coordinates if necessary for given orientation.
 * Each time orient is incremented, the plot is rotated 90 deg clockwise.
 * Note: this is now used only to rotate by 90 degrees for devices that
 * expect portrait mode.
\*--------------------------------------------------------------------------*/

void
plRotPhy(PLINT orient, PLINT xmin, PLINT ymin, PLINT xmax, PLINT ymax,
	 int *px, int *py)
{
    int x, y;

    x = *px;
    y = *py;

    switch (orient%4) {

    case 1:
	*px = xmin + (y - ymin);
	*py = ymin + (xmax - x);
	break;

    case 2:
	*px = xmin + (xmax - x);
	*py = ymin + (ymax - y);
	break;

    case 3:
	*px = xmin + (ymax - y);
	*py = ymin + (x - xmin);
	break;

    default:
	break;			/* do nothing */
    }
}

/*--------------------------------------------------------------------------*\
 * plAllocDev()
 *
 * Allocates a standard PLDev structure for device-specific data, stores
 * the address in pls->dev, and returns the address as well.
\*--------------------------------------------------------------------------*/

PLDev *
plAllocDev(PLStream *pls)
{
    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(PLDev));
    if (pls->dev == NULL)
	plexit("plAllocDev: cannot allocate memory\n");

    return (PLDev *) pls->dev;
}

/*--------------------------------------------------------------------------*\
 * plGinInit()
 *
 * Just fills in the PLGraphicsIn with appropriate initial values.
\*--------------------------------------------------------------------------*/

void
plGinInit(PLGraphicsIn *gin)
{
    gin->type = 0;
    gin->state = 0;
    gin->keysym = 0;
    gin->button = 0;
    gin->string[0] = '\0';
    gin->pX = gin->pY = -1;
    gin->dX = gin->dY = 0.;
    gin->wX = gin->wY = 0.;
}

