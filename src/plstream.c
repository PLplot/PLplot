/* $Id$
   $Log$
   Revision 1.2  1993/02/23 05:21:09  mjl
   Eliminated negative orientations.  Recognized settings are now 0, 1, 2, 3
   (multiply by 90 degrees to get orientation).

 * Revision 1.1  1993/01/23  06:00:28  mjl
 * Added to hold functions that primarily deal with manipulation of stream
 * quantities, through a pointer to a stream passed as an argument.  You may
 * also think of it as a driver utility library.
 *

 * dispatch.c history:
 *
 * Revision 1.6  1992/11/07  07:48:36  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.5  1992/10/22  17:04:54  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.4  1992/10/20  20:12:26  mjl
 * Modified file open routine to open next family member file if requested
 * to do so.
 *
 * Revision 1.3  1992/09/30  18:24:50  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:38  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:31  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plstream.c

	Stream & device support functions.
*/

#include "plplot.h"

#include <stdio.h>
#include <string.h>
#include "plstream.h"

/* Functions only in this file */

static void plSclSetup(PLStream *, PLDev *);

static char line[80];

/*----------------------------------------------------------------------*\
* color_def()
*
* Initializes color table entries by RGB values.
* Does nothing if: color already set, or palette entry exceeds number
* of colors in the palette.
\*----------------------------------------------------------------------*/

static void
color_def(PLStream *pls, PLINT i, U_CHAR r, U_CHAR g, U_CHAR b)
{
    if (!pls->cmap0setcol[i] && (i < pls->ncol0)) {
	pls->cmap0[i].r = r;
	pls->cmap0[i].g = g;
	pls->cmap0[i].b = b;
	pls->cmap0setcol[i] = 1;
    }
}

/*----------------------------------------------------------------------*\
* plCmaps_init()
*
* Initializes color maps.
* Do not initialize if already done.
*
* Initial RGB values for color map 0 taken from HPUX 8.07 X-windows 
* rgb.txt file, and may not accurately represent the described colors on 
* all systems.
\*----------------------------------------------------------------------*/

void
plCmaps_init(PLStream *pls)
{
    int itype, ihfinit = 0;
    PLFLT param[6];

/* If the user hasn't specified, the number of colors default to 16 */

    if (pls->ncol0 == 0)
	pls->ncol0 = 16;

/* Color map 0 */

    color_def(pls, 0, 0, 0, 0);		/* black */
    color_def(pls, 1, 255, 0, 0);	/* red */
    color_def(pls, 2, 255, 255, 0);	/* yellow */
    color_def(pls, 3, 0, 255, 0);	/* green */
    color_def(pls, 4, 50, 191, 193);	/* aquamarine */
    color_def(pls, 5, 255, 181, 197);	/* pink */
    color_def(pls, 6, 245, 222, 179);	/* wheat */
    color_def(pls, 7, 126, 126, 126);	/* grey */
    color_def(pls, 8, 165, 42, 42);	/* brown */
    color_def(pls, 9, 0, 0, 255);	/* blue */
    color_def(pls, 10, 138, 43, 226);	/* Blue Violet */
    color_def(pls, 11, 0, 255, 255);	/* cyan */
    color_def(pls, 12, 25, 204, 223);	/* turquoise */
    color_def(pls, 13, 255, 0, 255);	/* magenta */
    color_def(pls, 14, 233, 150, 122);	/* salmon */
    color_def(pls, 15, 255, 255, 255);	/* white */

/* Color map 1 */

    if (!pls->cmap1set) {

	switch (ihfinit) {

	case 0:
	default:

/*
* The default map plots height levels at full color intensity, going from
* blue-violet (low), to green (medium), to red (high).
*/
	    itype = 0;		/* HLS model */
	    param[0] = 280.;	/* h -- low: blue-violet */
	    param[1] = -20.;	/* h -- high: red */
	    param[2] = 1.;	/* l -- low */
	    param[3] = 1.;	/* l -- high */
	    param[4] = 1.;	/* s -- low */
	    param[5] = 1.;	/* s -- high */
	    break;
	}

	plscm1f1(itype, param);
    }
}

/*----------------------------------------------------------------------*\
* plOpenFile()
*
* Opens file for output, prompting if not set.
* Prints extra newline at end to make output look better in batch runs.
\*----------------------------------------------------------------------*/

void
plOpenFile(PLStream *pls)
{
    int i = 0;

    while (!pls->OutFile) {
	if (!pls->fileset) {
	    printf("Enter desired name for graphics output file: ");
	    fgets(line, sizeof(line), stdin);
	    line[strlen(line) - 1] = '\0';

	    strncpy(pls->FileName, line, sizeof(pls->FileName) - 1);
	    pls->FileName[sizeof(pls->FileName) - 1] = '\0';
	    pls->fileset = 1;
	    strcpy(pls->FamilyName, pls->FileName);
	}
	if (pls->family) {
	    (void) sprintf(pls->FileName, "%s.%i", pls->FamilyName, pls->member);
	}

	if (i++ > 10)
	    plexit("Too many tries.");

	if ((pls->OutFile = fopen(pls->FileName, BINARY_WRITE)) == NULL) {
	    printf("Can't open %s.\n", pls->FileName);
	    pls->fileset = 0;
	}
	else
	    printf("Created %s\n", pls->FileName);
    }
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
	if (!pls->member)
	    pls->member = 1;
	if (!pls->bytemax)
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
	    grtidy();
	    pls->fileset = 1;
	    pls->member++;
	    pls->famadv = 0;
	    grinit();
	    return;
	}
    }
}

/*----------------------------------------------------------------------*\
* plGetInt()
*
* Prompts human to input an integer in response to given message.
\*----------------------------------------------------------------------*/

PLINT
plGetInt(char *s)
{
    PLINT m;
    int i = 0;

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atoi(line);
	return (m);
#else
	if (sscanf(line, "%d", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
    return (0);
}

/*----------------------------------------------------------------------*\
* plGetFlt()
*
* Prompts human to input a float in response to given message.
\*----------------------------------------------------------------------*/

PLFLT
plGetFlt(char *s)
{
    PLFLT m;
    int i = 0;

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atof(line);
	return (m);
#else
	if (sscanf(line, "%f", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
    return (0.);
}

/*----------------------------------------------------------------------*\
* plRotPhy()
*
* Rotates physical coordinates if necessary for given orientation.
\*----------------------------------------------------------------------*/

void
plRotPhy(PLINT orient, PLDev *dev, int *px1, int *py1, int *px2, int *py2)
{
    int x1, y1, x2, y2;

    x1 = *px1;
    y1 = *py1;
    x2 = *px2;
    y2 = *py2;

    switch (orient) {

    case 1:
	*px1 = dev->xmin + (y1 - dev->ymin);
	*py1 = dev->ymin + (dev->xmax - x1);
	*px2 = dev->xmin + (y2 - dev->ymin);
	*py2 = dev->ymin + (dev->xmax - x2);
	break;

    case 2:
	*px1 = dev->xmin + (dev->xmax - x1);
	*py1 = dev->ymin + (dev->ymax - y1);
	*px2 = dev->xmin + (dev->xmax - x2);
	*py2 = dev->ymin + (dev->ymax - y2);
	break;

    case 3:
	*px1 = dev->xmin + (dev->ymax - y1);
	*py1 = dev->ymin + (x1 - dev->xmin);
	*px2 = dev->xmin + (dev->ymax - y2);
	*py2 = dev->ymin + (x2 - dev->xmin);
	break;

    default:
	break;			/* do nothing */
    }
}

/*----------------------------------------------------------------------*\
* plSclPhy()
*
* Scales physical coordinates to get a specified aspect ratio on plot.
\*----------------------------------------------------------------------*/

void
plSclPhy(PLStream *pls, PLDev *dev, int *px1, int *py1, int *px2, int *py2)
{
    int x1o, y1o, x2o, y2o;

    if (pls->aspect <= 0.)	/* Failsafe (should never happen) */
	return;

    x1o = *px1;
    y1o = *py1;
    x2o = *px2;
    y2o = *py2;

    if (pls->aspectset) {
	plSclSetup(pls, dev);
	pls->aspectset = 0;
    }
    *px1 = (int) (dev->sclxmin + (x1o - dev->xmin) * dev->rsclx);
    *px2 = (int) (dev->sclxmin + (x2o - dev->xmin) * dev->rsclx);
    *py1 = (int) (dev->sclymin + (y1o - dev->ymin) * dev->rscly);
    *py2 = (int) (dev->sclymin + (y2o - dev->ymin) * dev->rscly);
}

/*----------------------------------------------------------------------*\
* plSclSetup()
*
* Gets physical scaling factors in response to given aspect ratio request.
* Largest box that satisfies given aspect ratio that fits on the page
* or subpage will be used.
\*----------------------------------------------------------------------*/

static void
plSclSetup(PLStream *pls, PLDev *dev)
{
    PLINT sclxlen, sclylen, devxlen, devylen;

/* Get global aspect scaling factors */

    devxlen = dev->xmax - dev->xmin;
    devylen = dev->ymax - dev->ymin;

    sclylen = devylen;
    sclxlen = sclylen / pls->aspect;
    if (sclxlen > devxlen) {
	sclxlen = devxlen;
	sclylen = sclxlen * pls->aspect;
    }

    dev->sclxmin = dev->xmin + (dev->xmax - sclxlen) / 2.0;
    dev->sclymin = dev->ymin + (dev->ymax - sclylen) / 2.0;

    dev->rsclx = (double) sclxlen / (double) devxlen;
    dev->rscly = (double) sclylen / (double) devylen;
}
