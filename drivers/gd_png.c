/* $Id$
 
         PNG device driver.
*/
#include "plplot/plDevs.h"

#if defined(PLD_png) || defined(PLD_jpg)

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

#include "gd.h"

#ifndef bzero   /* not standard ANSI (boo hoo) */
#define bzero(a,b) memset(a,0,b)
#endif

/* Prototypes for functions in this file. */

static void	fill_polygon	(PLStream *pls);
static void	setcmap		(PLStream *pls);
static void     plD_init_png_Dev(PLStream *pls);

/* top level declarations */

#define NCOLOURS 256    /* Hardwire this for now */



/* Struct to hold device-specific info. */

typedef struct {

	gdImagePtr im_out;                      /* Graphics pointer */
        PLINT pngx;               
        PLINT pngy;

/* GD does "funny" things with the colour map.
 * It can't guarentee that the colours will be where you think they are.
 * So we need this "colour_index" table to store where the colour we
 * requested happens to be. Messy, but it works.
 */

        int colour_index[256];                  /* Colour "index" table         */

        int colour;                             /* Current Colour               */
        int totcol;                             /* Total number of colours      */

} png_Dev;

        
/*--------------------------------------------------------------------------*\
 * plD_init_png_Dev()
 *
\*--------------------------------------------------------------------------*/

static void
plD_init_png_Dev(PLStream *pls)
{
    png_Dev *dev;

/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(png_Dev));
    if (pls->dev == NULL)
	plexit("plD_init_png_Dev: Out of memory.");

    bzero(pls->dev,sizeof(png_Dev)); /* I'm lazy - quick way of setting everything to 0 */

    dev = (png_Dev *) pls->dev;

    dev->colour=1;
    dev->totcol=16;

    if ( (pls->dev_compression<=0)||(pls->dev_compression>99) )
       pls->dev_compression=90;

}

/*----------------------------------------------------------------------*\
 * plD_init_png()
 *
 * Initialize device.
\*----------------------------------------------------------------------*/

void plD_init_png(PLStream *pls)
{
    png_Dev *dev=NULL;

    pls->termin = 0;            /* Not an interactive device */
    pls->icol0 = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->dev_fill0 = 1;         /* Can do solid fills */
    pls->width = 1;

    if (!pls->colorset)
	pls->color = 1;         /* Is a color device */

/* Initialize family file info */
    plFamInit(pls);

/* Prompt for a file name if not already set */
    plOpenFile(pls);

/* Allocate and initialize device-specific data */
    plD_init_png_Dev(pls);
    dev=(png_Dev *)pls->dev;

      if (pls->xlength <= 0 || pls->ylength <=0)
      {
/* use default width, height of 800x600 if not specifed by -geometry option
 * or plspage */
	 plspage(0., 0., 800, 600, 0, 0);
      }

      dev->im_out = gdImageCreate(pls->xlength, pls->ylength);
      
      setcmap(pls);
      dev->totcol = 16;		/* Reset RGB map so we don't run out of
				   indicies */
      pls->graphx = GRAPHICS_MODE;
		

     dev->pngx = pls->xlength - 1;	/* should I use -1 or not??? */
     dev->pngy = pls->ylength - 1;

     if (pls->xdpi<=0) 
     {
/* This corresponds to a typical monitor resolution of 4 pixels/mm. */
	plspage(4.*25.4, 4.*25.4, 0, 0, 0, 0);
     }
     else
     {
	pls->ydpi=pls->xdpi;        /* Set X and Y dpi's to the same value */
     } 
/* Convert DPI to pixels/mm */
     plP_setpxl(pls->xdpi/25.4,pls->ydpi/25.4);

     plP_setphy(0, dev->pngx, 0, dev->pngy);

}

/*----------------------------------------------------------------------*\
 * plD_line_png()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_png(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    png_Dev *dev=(png_Dev *)pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    y1 = dev->pngy - y1;
    y2 = dev->pngy - y2;

    gdImageLine(dev->im_out, x1, y1, x2, y2, dev->colour_index[dev->colour]);	

}

/*----------------------------------------------------------------------*\
 * plD_polyline_png()
 *
 * Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_png(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_png(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}


/*----------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
png_Dev *dev=(png_Dev *)pls->dev;

    int i;
    gdPoint *points=NULL;
    
    if (pls->dev_npts < 1)
	return;

     points = malloc((size_t)pls->dev_npts * sizeof(gdPoint));

     for (i = 0; i < pls->dev_npts; i++) 
         {
	   points[i].x = pls->dev_x[i];
	   points[i].y = dev->pngy - pls->dev_y[i];
         }

   gdImageFilledPolygon(dev->im_out, points, pls->dev_npts, dev->colour_index[dev->colour]);
   free(points);

}

/*----------------------------------------------------------------------*\
 * setcmap()
 *
 * Sets up color palette.
\*----------------------------------------------------------------------*/

static void
setcmap(PLStream *pls)
{
    int i, ncol1, ncol0;
    PLColor cmap1col;
    png_Dev *dev=(png_Dev *)pls->dev;

       ncol0=pls->ncol0;

/* Initialize cmap 0 colors */

    for (i = 0; i < ncol0; i++)
        {
        dev->colour_index[i]=gdImageColorAllocate(dev->im_out,
                                   pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);

        }

/* Initialize any remaining slots for cmap1 */


    ncol1 = NCOLOURS;
    for (i = 0; i < ncol1; i++) {
	plcol_interp(pls, &cmap1col, i, ncol1);
        dev->colour_index[i + pls->ncol0]=gdImageColorAllocate(dev->im_out,
                                   cmap1col.r, cmap1col.g, cmap1col.b);
    }


}


/*----------------------------------------------------------------------*\
 * plD_state_png()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_png(PLStream *pls, PLINT op)
{
png_Dev *dev=(png_Dev *)pls->dev;

    switch (op) {

    case PLSTATE_COLOR0:
	dev->colour = pls->icol0;
	if (dev->colour == PL_RGB_COLOR) {
	    int r = pls->curcolor.r;
	    int g = pls->curcolor.g;
	    int b = pls->curcolor.b;
	    if (dev->totcol < 256) 
	       {
                dev->colour_index[++dev->totcol]=gdImageColorAllocate(dev->im_out,r, g, b);
		dev->colour = dev->totcol;
	       }

	}
	break;

    case PLSTATE_COLOR1:
        { int icol1, ncol1, r, g, b;

        if ((ncol1 = MIN(NCOLOURS -pls->ncol0, pls->ncol1)) < 1)
            break;

        icol1 = pls->ncol0 + (pls->icol1 * (ncol1-1)) / (pls->ncol1-1);
        r = pls->curcolor.r;
        g = pls->curcolor.g;
        b = pls->curcolor.b;
        
        /* We have to "deallocate" any colour that we want to redefine
         * I don't know what implications this has all up, but you seem
         * to have to do it to make things work the way you expect them too.
         */
         
        gdImageColorDeallocate(dev->im_out,dev->colour_index[icol1]);
        dev->colour_index[icol1]=gdImageColorAllocate(dev->im_out,r, g, b);
 	dev->colour = icol1;
	}
	break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
	if (pls->color)
	    setcmap(pls);
	break;
    }
}


/*----------------------------------------------------------------------*\
 * plD_esc_png()
 *
 * Escape function.
\*----------------------------------------------------------------------*/

void plD_esc_png(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

      case PLESC_FILL:  /* fill */
	fill_polygon(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
 * plD_bop_png()
 *
 * Set up for the next page.
 * Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void plD_bop_png(PLStream *pls)
{
    png_Dev *dev=(png_Dev *)pls->dev;

    plGetFam(pls);
/* force new file if pls->family set for all subsequent calls to plGetFam 
 * n.b. putting this after plGetFam call is important since plinit calls
 * bop, and you don't want the familying sequence started until after
 * that first call to bop.*/
    pls->famadv = 1;

    pls->page++;

    dev->im_out = gdImageCreate(pls->xlength, pls->ylength);
    setcmap(pls);

    dev->totcol = 16;

}

/*----------------------------------------------------------------------*\
 * plD_tidy_png()
 *
 * Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void plD_tidy_png(PLStream *pls)
{
   png_Dev *dev=(png_Dev *)pls->dev;

   fclose(pls->OutFile);
   free_mem (pls->dev);

}

#ifdef PLD_png

/*----------------------------------------------------------------------*\
 * plD_eop_png()
 *
 * End of page.
\*----------------------------------------------------------------------*/

void plD_eop_png(PLStream *pls)
{
png_Dev *dev=(png_Dev *)pls->dev;
    if (pls->family || pls->page == 1) {
       gdImagePng(dev->im_out, pls->OutFile);
       gdImageDestroy(dev->im_out);
    }
}

#endif

#ifdef PLD_jpg

/*----------------------------------------------------------------------*\
 * plD_eop_jpg()
 *
 * End of page.
\*----------------------------------------------------------------------*/

void plD_eop_jpg(PLStream *pls)
{
png_Dev *dev=(png_Dev *)pls->dev;
    if (pls->family || pls->page == 1) {
       gdImageJpeg(dev->im_out, pls->OutFile, pls->dev_compression);
       gdImageDestroy(dev->im_out);
    }
}

#endif

/*#endif*/


#else
int 
pldummy_png()
{
    return 0;
}

#endif				/* PNG */
