/* ///////////////////////////////////////////////////////////////////
// $Id$
// $Log$
// Revision 1.2  1994/05/26 19:21:45  mjl
// Minor changes to bring up to spec with the other drivers.
//
 * Revision 1.1  1994/05/25  09:34:14  mjl
 * Consolidated HP pen plotter driver, contributed by Mark Olesen
 * (olesen@weber.me.queensu.ca).
 *
//
//  File:       hpgl.c
//
//  Descript:   hp7470 and hp7580 drivers
//
//  Library:    ---
//
//  Requires:   ---
//
//  Public:     plD_init_hp7470()
//              plD_init_hp7580()
//              plD_line_hpgl()
//              plD_polyline_hpgl()
//              plD_eop_hpgl()
//              plD_bop_hpgl()
//              plD_tidy_hpgl()
//              plD_state_hpgl()
//              plD_esc_hpgl()
//
//              pldummy_hpgl()
//
//  Private:    initialize_hpgl_pls()
//
//  Notes:      ---
//
//  Revisions:
//  13 Apr 94   mjo     documented, amalgamated hp7470 and hp7580 drivers
//                      hpgl commands changed to uppercase: makes HPGL
//                      interpreters happier and the plotters don't care
//
/////////////////////////////////////////////////////////////////// */
#if defined ( HP7470 ) || defined ( HP7580 )

#include "plplotP.h"
#include <stdio.h>
#include <string.h>
#include "drivers.h"

/* top level declarations */

/* Plotter sizes */

#define HP7470_XMIN       0
#define HP7470_XMAX   10299
#define HP7470_YMIN       0
#define HP7470_YMAX    7649

#define HP7580_XMIN  -4500
#define HP7580_XMAX   4500
#define HP7580_YMIN  -2790
#define HP7580_YMAX   2790

/*----------------------------------------------------------------------*\
* initialize_hpgl_pls()
*
* Initialize plot stream
\*----------------------------------------------------------------------*/

static void
initialize_hpgl_pls(PLStream *pls)
{
    pls->termin = 0;            /* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;

    if (!pls->colorset)
      pls->color = 1;

    plFamInit(pls);             /* Initialize family file info */
    plOpenFile(pls);            /* get file name if not already set */
}

/*----------------------------------------------------------------------*\
* plD_init_hp7470()
*
* Initialize device.
\*----------------------------------------------------------------------*/

#ifdef HP7470
void
plD_init_hp7470(PLStream *pls)
{
    PLDev *dev;

    initialize_hpgl_pls(pls);   /* initialize plot stream */

    dev = plAllocDev(pls);      /* Allocate device-specific data */

    dev->xold = UNDEFINED;      /* initialize device-specific data */
    dev->yold = UNDEFINED;
    dev->xmin = HP7470_XMIN;
    dev->xmax = HP7470_XMAX;
    dev->ymin = HP7470_YMIN;
    dev->ymax = HP7470_YMAX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setpxl((PLFLT) 40., (PLFLT) 40.);
    plP_setphy( dev->xmin, dev->xmax, dev->ymin, dev->ymax);
    fputs( "\x1b.I200;;17:\x1b.N;19:\x1b.M;;;10:IN;\n", pls->OutFile );
}
#endif          /* HP7470 */

/*----------------------------------------------------------------------*\
* plD_init_hp7580()
*
* Initialize device.
\*----------------------------------------------------------------------*/

#ifdef HP7580
void
plD_init_hp7580(PLStream *pls)
{
    PLDev *dev;

    initialize_hpgl_pls(pls);   /* initialize plot stream */

    dev = plAllocDev(pls);      /* Allocate device-specific data */

    dev->xold = UNDEFINED;      /* initialize device-specific data */
    dev->yold = UNDEFINED;
    dev->xmin = HP7580_XMIN;
    dev->xmax = HP7580_XMAX;
    dev->ymin = HP7580_YMIN;
    dev->ymax = HP7580_YMAX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setpxl((PLFLT) 40., (PLFLT) 40.);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

    fputs( "\x1b.I200;;17:\x1b.N;19:\x1b.M;;;10:IN;\n", pls->OutFile );
    fputs( "RO90;IP;SP4;PA;\n", pls->OutFile );
}
#endif  /* HP7580 */

/*----------------------------------------------------------------------*\
* plD_line_hpgl()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_hpgl(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    /* Write out old path */
    if (x1 != dev->xold || y1 != dev->yold)    /* Write out old path */
      pls->bytecnt += fprintf( pls->OutFile, "PU%d %d;", x1, y1 );

    /* Add new point to path */
    pls->bytecnt += fprintf( pls->OutFile, "PD%d %d\n", x2, y2);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
* plD_polyline_hpgl()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_hpgl(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  register PLINT i;
  PLDev *dev = (PLDev *) pls->dev;

  /* Write out old path */
  if (xa[0] != dev->xold || ya[0] != dev->yold)
    pls->bytecnt += fprintf( pls->OutFile, "PU%d %d;", xa[0], ya[0] );

  for (i = 1; i < npts; i++)        /* Add new point to path */
    pls->bytecnt += fprintf( pls->OutFile, "PD%d %d\n", xa[i], ya[i] );

  dev->xold = xa[ npts - 1 ];
  dev->yold = ya[ npts - 1 ];
}

/*----------------------------------------------------------------------*\
* plD_eop_hpgl()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_hpgl(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* plD_bop_hpgl()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_hpgl(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fputs( "PG;\n", pls->OutFile );
    if (!pls->termin)
        plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_hpgl()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_hpgl(PLStream *pls)
{
    fputs( "SP0\n", pls->OutFile );
    fclose(pls->OutFile);
}

/*----------------------------------------------------------------------*\
* plD_state_hpgl()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void
plD_state_hpgl(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
        break;

    case PLSTATE_COLOR0:
        if ( pls->icol0 < 1 || pls->icol0 > 8)
          fputs( "\nInvalid pen selection.", stderr );
        else
          fprintf( pls->OutFile, "SP%d %d\n", pls->icol0, pls->width );
        break;

    case PLSTATE_COLOR1:
        break;
    }
}

/*----------------------------------------------------------------------*\
* plD_esc_hpgl()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_hpgl(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int
pldummy_hpgl(void)
{
    return 0;
}

#endif                          /* HP7470 || HP7580 */
