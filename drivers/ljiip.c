/* $Id$
 * $Log$
 * Revision 1.4  1994/07/19 22:30:21  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.3  1994/04/30  16:14:45  mjl
 * Fixed format field (%ld instead of %d) or introduced casts where
 * appropriate to eliminate warnings given by gcc -Wall.
 *
 * Revision 1.2  1994/04/09  03:10:43  furnish
 * Teeny typo correction to remove duplicate symbols which caused HP to
 * be unable to produce shared lib.
 *
 * Revision 1.1  1994/04/08  11:46:44  mjl
 * New LaserJet IIp driver by Wesley Ebisuzaki, based on old ljii driver.
 * Has compression and other optimizations.
*/

/*	ljiip.c

	PLPLOT Laser Jet IIp device driver.
	Based on old LJII driver, modifications by Wesley Ebisuzaki

	DPI = 300, 150, 100	(dots per inch)
		default: Amiga/Unix 300 dpi, MS-DOS 150 dpi
		higher = better output, more memory, longer to print
	GCMODE = 0, 2 (graphics compression mode)
		default: 2,  old laser jets should use 0
		compression can speed up the printing up to 3x

*/
#include "plDevs.h"

#ifdef PLD_ljii

#include "plplotP.h"
#include "drivers.h"
#include <math.h>
#include <string.h>

#ifdef __GO32__			/* dos386/djgpp */
#ifdef MSDOS
#undef MSDOS
#endif
#endif

/* Function prototypes */

static void setpoint(PLINT, PLINT);

/* top level declarations */

/* GCMODE = graphics compression mode, 0=old laser jets, 2=ljiip or later */
#define GCMODE   2

/* DPI = dots per inch, more dots=better plot, more memory, more time */
/* possible DPI = 75, 100, 150, 300 */
/* if DPI=300 then your machine must have a free 1Mb block of memory */

#define DPI      300

#ifdef MSDOS
#undef  DPI
#define DPI      150
#endif

#define CURX	 ((long) (DPI / 5))
#define CURY	 ((long) (DPI / 7))
#define XDOTS	 (376 * (DPI / 50))	/* # dots across */
#define YDOTS	 (500 * (DPI / 50))	/* # dots down */
#define JETX     (XDOTS-1)
#define JETY     (YDOTS-1)


#define BPROW	 (XDOTS/8L)	/* # bytes across */
#define MAX_WID	 8		/* max pen width in pixels */
#define BPROW1	 (BPROW + (MAX_WID+7)/8)
/* pen has width, make bitmap bigger */
#define NBYTES	 BPROW1*(YDOTS+MAX_WID)	/* total number of bytes */

/* Graphics control characters. */

#define ESC      0x1b
#define FF       0x0c

static char mask[8] =
{'\200', '\100', '\040', '\020', '\010', '\004', '\002', '\001'};

#ifndef MSDOS
#define _HUGE
#else
#define _HUGE _huge
#endif

static unsigned char _HUGE *bitmap;	/* points to memory area NBYTES in size */

/*----------------------------------------------------------------------*\
* plD_init_ljiip()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_ljiip(PLStream *pls)
{
    PLDev *dev;

    pls->termin = 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->color = 0;
    pls->width = DPI / 100;
    pls->bytecnt = 0;
    pls->page = 0;

/* Initialize family file info */

    plFamInit(pls);

/* Prompt for a file name if not already set */

    plOpenFile(pls);

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;

    /* number of pixels / mm */
    plP_setpxl((PLFLT) (DPI/25.4), (PLFLT) (DPI/25.4));

/* Rotate by 90 degrees since portrait mode addressing is used */

    dev->xmin = 0;
    dev->ymin = 0;
    dev->xmax = JETY;
    dev->ymax = JETX;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Allocate storage for bit map matrix */

#ifdef MSDOS
    if ((bitmap = (unsigned char _HUGE *) halloc((long) NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#else
    if ((bitmap = (unsigned char *) calloc(NBYTES, sizeof(char))) == NULL)
	plexit("Out of memory in call to calloc");
#endif

/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
}

/*----------------------------------------------------------------------*\
* plD_line_ljiip()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_ljiip(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    int abs_dx, abs_dy, dx, dy, incx, incy;
    int i, width, residual;
    PLINT j;
    width = pls->width;

    if (width > MAX_WID) width = MAX_WID;

/* Take mirror image, since PCL expects (0,0) to be at top left */

    y1 = dev->ymax - (y1 - dev->ymin);
    y2 = dev->ymax - (y2 - dev->ymin);

/* Rotate by 90 degrees */

    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x1, &y1);
    plRotPhy(1, dev->xmin, dev->ymin, dev->xmax, dev->ymax, &x2, &y2);

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0) {
	abs_dx = -dx;
	incx = -1;
    }
    else {
	abs_dx = dx;
	incx = 1;
    }
    if (dy < 0) {
	abs_dy = -dy;
	incy = -1;
    }
    else {
	abs_dy = dy;
	incy = 1;
    }
    if (width > 1) {
	for (i = 0; i < width; i++) {
	    for (j = 0; j < width; j++) {
	        setpoint((PLINT) (x1+i), (PLINT) (y1+j));
	        setpoint((PLINT) (x2+i), (PLINT) (y2+j));
	    }
	}
    }
    if (abs_dx >= abs_dy) {
	residual = -(abs_dx >> 1);
	if (width == 1) {
            for (i = 0; i <= abs_dx; i++, x1 += incx) {
                setpoint((PLINT) (x1), (PLINT) (y1));
                if ((residual += abs_dy) >= 0) {
                    residual -= abs_dx;
                    y1 += incy;
                }
            }
	}
	else {
	    for (i = 0; i <= abs_dx; i++, x1 += incx) {
	       for (j = 0; j < width; j++) {
	           setpoint((PLINT) (x1), (PLINT) (y1+j));
	           setpoint((PLINT) (x1+width-1), (PLINT) (y1+j));
	       }
	       if ((residual += abs_dy) >= 0) {
		   residual -= abs_dx;
		   y1 += incy;
	       }
	    }
	}
    }
    else {
	residual = -(abs_dy >> 1);
        if (width == 1) {
            for (i = 0; i <= abs_dy; i++, y1 += incy) {
                setpoint((PLINT) (x1), (PLINT) (y1));
                if ((residual += abs_dx) >= 0) {
                    residual -= abs_dy;
                    x1 += incx;
                }
            }
        }
        else {
            for (i = 0; i <= abs_dy; i++, y1 += incy) {
               for (j = 0; j < width; j++) {
                   setpoint((PLINT) (x1+j), (PLINT) (y1));
                   setpoint((PLINT) (x1+j), (PLINT) (y1+width-1));
               }
               if ((residual += abs_dx) >= 0) {
                   residual -= abs_dy;
                   x1 += incx;
               }
            }
        }
    }
}

/*----------------------------------------------------------------------*\
* plD_polyline_ljiip()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_ljiip(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_ljiip(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* plD_eop_ljiip()
*
* End of page.(prints it here).
\*----------------------------------------------------------------------*/

void
plD_eop_ljiip(PLStream *pls)
{
    PLINT j;
    unsigned char _HUGE *p;
#if GCMODE > 0
    int i, iy, last, n, jmax;
    unsigned char _HUGE t_buf[BPROW*2];
    unsigned char c;
#endif

    /* PCL III setup: ref. Deskjet Plus Printer Owner's Manual */

    fprintf(pls->OutFile,"\033*rB");      	/* end raster graphics */
    fprintf(pls->OutFile,"\033*t%3dR", DPI);	/* set DPI */

#if GCMODE != 0
    fprintf(pls->OutFile,"\033*r%dS", XDOTS);	/* raster width */
    fprintf(pls->OutFile,"\033*b%1dM", GCMODE); /* graphics mode */
#endif

    /* First move cursor to origin */

    fprintf(pls->OutFile,"\033*p%ldX", CURX);
    fprintf(pls->OutFile,"\033*p%ldY", CURY);
    fprintf(pls->OutFile,"\033*r0A");		/* start graphics */

    /* Write out raster data */

#if GCMODE == 0
    for (j = 0, p = bitmap; j < YDOTS; j++, p += BPROW1) {
	fprintf(pls->OutFile,"\033*b>%dW", BPROW);
	fwrite(p, BPROW, sizeof(char), pls->OutFile);
    }
#endif
#if GCMODE == 2
    for (iy = 0, p = bitmap; iy < YDOTS; iy++, p += BPROW1) {

	/* find last non-zero byte */
	last = BPROW - 1;
	while (last > 0 && p[last] == 0) last--;
	last++;

	/* translate to mode 2, save results in t_buf[] */
	i = n = 0;
	while (i < last) {
	    c = p[i];
	    jmax = i + 127;
	    jmax = last < jmax ? last : jmax;
	    if (i < last - 2 && (c == p[i+1]) && (c == p[i+2])) {
		j = i + 3;
		while (j < jmax && c == p[j]) j++;
		t_buf[n++] = (i-j+1) & 0xff;
		t_buf[n++] = c;
		i = j;
	    }
	    else {
		for (j = i + 1; j < jmax; j++) {
		    if (j < last - 2 && (p[j] == p[j+1]) &&
			(p[j+1] == p[j+2]) ) break;
		}
		t_buf[n++] = j - i - 1;
		while (i < j) {
		    t_buf[n++] = p[i++];
		}
		/* i = j; */
	    }
	}
	fprintf(pls->OutFile,"\033*b%dW", (int) n);
	fwrite(t_buf, (int) n, sizeof(char), pls->OutFile);
    }
#endif

    pls->bytecnt += NBYTES;

    /* End raster graphics and send Form Feed */
    fprintf(pls->OutFile, "\033*rB");
    fprintf(pls->OutFile, "%c", FF);

    /* Finally, clear out bitmap storage area */
    memset((void *) bitmap, '\0', NBYTES);
}

/*----------------------------------------------------------------------*\
* plD_bop_ljiip()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_ljiip(PLStream *pls)
{
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*----------------------------------------------------------------------*\
* plD_tidy_ljiip()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_ljiip(PLStream *pls)
{
/* Reset Printer */

    fprintf(pls->OutFile, "%cE", ESC);
    fclose(pls->OutFile);
    free((char *) bitmap);
}

/*----------------------------------------------------------------------*\
* plD_state_ljiip()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_ljiip(PLStream *pls, PLINT op)
{
}

/*----------------------------------------------------------------------*\
* plD_esc_ljiip()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_ljiip(PLStream *pls, PLINT op, void *ptr)
{
}

/*----------------------------------------------------------------------*\
* setpoint()
*
* Sets a bit in the bitmap.
\*----------------------------------------------------------------------*/

static void
setpoint(PLINT x, PLINT y)
{
    PLINT index;
    index = x / 8 + y * BPROW1;
    *(bitmap + index) = *(bitmap + index) | mask[x % 8];
}

#else
int 
pldummy_ljiip()
{
    return 0;
}

#endif				/* PLD_ljii */
