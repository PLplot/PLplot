/* $Id$
 * $Log$
 * Revision 1.13  1994/03/23 08:32:34  mjl
 * Moved color and color map handling functions into plctrl.c.
 * Changed file open routine to support new options for sequencing family
 * member files.
 *
 * Revision 1.12  1994/01/15  17:28:59  mjl
 * Added include of pdf.h.
 *
 * Revision 1.11  1993/12/06  07:46:54  mjl
 * More modifications to support new color model.
 *
 * Revision 1.10  1993/09/24  20:33:28  furnish
 * Went wild with "const correctness".  Can now pass a C++ String type to
 * most (all that I know of) PLPLOT functions.  This works b/c String has
 * an implicit conversion to const char *.  Now that PLPLOT routines take
 * const char * rather than char *, use from C++ is much easier.
*/

/*	plstream.c

	Stream & device support functions.
*/

#include "plplotP.h"
#include "plstream.h"
#include <string.h>

/*----------------------------------------------------------------------*\
* plcol_interp()
*
* Initializes device cmap 1 entry by interpolation from pls->cmap1
* entries.  Returned PLColor is supposed to represent the i_th color
* out of a total of ncol colors in the current color scheme.
\*----------------------------------------------------------------------*/

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

    else if (ir == pls->ncol1) {
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

/*----------------------------------------------------------------------*\
* plOpenFile()
*
* Opens file for output, prompting if not set.
* Prints extra newline at end to make output look better in batch runs.
* A file name of "-" indicates output to stdout.
\*----------------------------------------------------------------------*/

void
plOpenFile(PLStream *pls)
{
    int i = 0;
    char line[256];

    while (pls->OutFile == NULL) {

/* Setting pls->FileName = NULL forces creation of a new family member */
/* You should also free the memory associated with it if you do this */

	if (pls->family && pls->BaseName != NULL) 
	    plP_getmember(pls);

/* Prompt if filename still not known */

	if (pls->FileName == NULL) {
	    printf("Enter desired name for graphics output file: ");
	    fgets(line, sizeof(line), stdin);
	    line[strlen(line) - 1] = '\0';

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
	    printf("Can't open %s.\n", pls->FileName);
	else
	    fprintf(stderr, "Created %s\n", pls->FileName);
    }
}

/*----------------------------------------------------------------------*\
* plP_getmember()
*
* Sets up next file member name (in pls->FileName), but does not open it.
\*----------------------------------------------------------------------*/

void
plP_getmember(PLStream *pls)
{
    char tmp[256];

    if (pls->FileName == NULL)
	pls->FileName = (char *) malloc(10 + strlen(pls->BaseName));

    sprintf(tmp, "%s.%%0%1ii", pls->BaseName, pls->fflen);
    sprintf(pls->FileName, tmp, pls->member);
}

/*----------------------------------------------------------------------*\
* plP_sfnam()
*
* Sets up file name & family stem name.
* Reserve some extra space (5 chars) to hold an optional member number.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plFamInit()
*
* Initializes family file parameters.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plGetFam()
*
* Starts new member file of family file set if necessary.
*
* Note each member file is a complete graphics file (can be printed
* individually), although 'plrender' will treat a family as a single
* logical file if given the family name instead of the member name.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plRotPhy()
*
* Rotates physical coordinates if necessary for given orientation.
* Each time orient is incremented, the plot is rotated 90 deg clockwise.
* Note: this is now used only to rotate by 90 degrees for devices that
* expect portrait mode.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* plAllocDev()
*
* Allocates a standard PLDev structure for device-specific data, stores
* the address in pls->dev, and returns the address as well.
\*----------------------------------------------------------------------*/

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

