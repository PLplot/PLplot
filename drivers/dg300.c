/* $Id$
   $Log$
   Revision 1.13  1994/03/23 06:34:20  mjl
   All drivers: cleaned up by eliminating extraneous includes (stdio.h and
   stdlib.h now included automatically by plplotP.h), extraneous clears
   of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
   driver interface or driver initialization as appropriate).  Special
   handling for malloc includes eliminated (no longer needed) and malloc
   prototypes fixed as necessary.

 * Revision 1.12  1993/07/31  07:56:27  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.11  1993/07/16  22:11:13  mjl
 * Eliminated low-level coordinate scaling; now done by driver interface.
 *
 * Revision 1.10  1993/07/01  21:59:31  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*	dg300.c

	PLPLOT dg300 device driver.
*/
#ifdef DG300

#include "plplotP.h"
#include "drivers.h"

/* top level declarations */

#define  DGX    639
#define  DGY    239

struct termattr {
    unsigned char com[4];
    unsigned char rom[4];
    unsigned char ram[4];
    unsigned char con[5];
    unsigned char eor;
} termattr;

/*----------------------------------------------------------------------*\
* plD_init_dg()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_dg(PLStream *pls)
{
    /* Request terminal configuration report */
    printf("\n\036\107\051\n");
    scanf("%s", (char *) &termattr);
    while (getchar() != '\n');
    if (!strncmp((char *) &termattr.ram[0], "0000", 4)) {
	printf("Please wait while graphics interpreter is downloaded.\n");
	/* Need to download graphics interpreter. */
	system("cat  /usr/local/src/g300/g300gci110.tx");
    }

    /* Clear screen, Set pen color to green, Absolute positioning */
    printf("\036\107\063\060\n\036\107\155\061\n\036\107\151\060\n");
    printf("\036\107\042\061\n");

    pls->termin = 1;		/* is an interactive device */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    plP_setpxl((PLFLT) (3.316 * 16), (PLFLT) (1.655 * 16));
    plP_setphy(0, DGX * 16, 0, DGY * 16);
}

/*----------------------------------------------------------------------*\
* plD_line_dg()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_dg(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    printf("LINE %d %d %d %d\n", x1 >> 4, y1 >> 3, x2 >> 4, y2 >> 3);
}

/*----------------------------------------------------------------------*\
* plD_polyline_dg()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_dg(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_dg(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_dg()
*
* End of page.  User must hit a <CR> to continue.
\*----------------------------------------------------------------------*/

void
plD_eop_dg(PLStream *pls)
{
    /* Before clearing wait for CR */
    putchar('\007');
    fflush(stdout);
    while (getchar() != '\n');
    printf("ERASE\n");
}

/*----------------------------------------------------------------------*\
* plD_bop_dg()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_dg(PLStream *pls)
{
    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_dg()
*
* Close graphics file
\*----------------------------------------------------------------------*/

void
plD_tidy_dg(PLStream *pls)
{
    printf("\036\107\042\060\n");
    fflush(stdout);
}

/*----------------------------------------------------------------------*\
* plD_state_dg()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_dg(PLStream *pls, PLINT op)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_dg()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_dg(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int 
pldummy_dg300()
{
    return 0;
}

#endif				/* DG300 */
