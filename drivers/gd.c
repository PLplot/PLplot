/* $Id$
 
         PNG and JPEG device driver based on libgd
*/

/*
 * Driver supports a hack that manipulates the colour palette when 
 * a light background is selected. This is basically to make sure 
 * there are not two "whites" when -bg ffffff is issued at the
 * command line.
 *
 * Related to this change, there is an ability to swap the "new"
 * black colour (index 15) with the red colour (index 2) by issuing
 * the command line "-hack" option. I like this for web pages, because
 * I think that black looks nicer than red (on white) for default
 * plotting. That is why it can be enabled with -hack, in case you
 * don't like it working this way. 
 *
 * For me, these two changes make it easy to switch from a "screen friendly"
 * black background with red first plotting colour, to a "web friendly"
 * white background with a black first plotting colour. 
 *
 * These features are enabled on a driver level by defining 
 * "SWAP_BALCK_WHEN_WHITE". If you wan't the driver to behave 100% like other
 * drivers, comment out the define
 */
 
#define SWAP_BALCK_WHEN_WHITE 

#include "plplot/plDevs.h"

#if defined(PLD_png) || defined(PLD_jpeg)

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

#include <gd.h>

/* Prototypes for functions in this file. */

static void	fill_polygon	(PLStream *pls);
static void	setcmap		(PLStream *pls);
static void     plD_init_png_Dev(PLStream *pls);

/* top level declarations */

#define NCOLOURS 256    /* Hardwire this for now */

/* In an attempt to fix a problem with the hidden line removal functions
 * that results in hidden lines *not* being removed from "small" plot
 * pages (ie, like a normal video screen), a "virtual" page of much
 * greater size is used to trick the algorithm into working correctly.
 * If, in future, this gets fixed on its own, then don't define
 * "use_experimental_hidden_line_hack"
 */

#define use_experimental_hidden_line_hack


/* Struct to hold device-specific info. */

typedef struct {

	gdImagePtr im_out;                      /* Graphics pointer */
        PLINT pngx;               
        PLINT pngy;

/* GD does "funny" things with the colour map.
 * It can't guarantee that the colours will be where you think they are.
 * So we need this "colour_index" table to store where the colour we
 * requested happens to be. Messy, but it works.
 */

        int colour_index[256];                  /* Colour "index" table         */

        int colour;                             /* Current Colour               */
        int totcol;                             /* Total number of colours      */
        int ncol1;                              /* Actual size of ncol1 we got  */

	int scale;                              /* scaling factor to "blow up" to */
	                                        /* the "virtual" page in removing hidden lines*/

} png_Dev;

        
/*--------------------------------------------------------------------------*\
 * plD_init_png_Dev()
 *
\*--------------------------------------------------------------------------*/

static void
plD_init_png_Dev(PLStream *pls)
{
    png_Dev *dev;
    int i;
    
/* Allocate and initialize device-specific data */

    if (pls->dev != NULL)
	free((void *) pls->dev);

    pls->dev = calloc(1, (size_t) sizeof(png_Dev));
    if (pls->dev == NULL)
	plexit("plD_init_png_Dev: Out of memory.");

    dev = (png_Dev *) pls->dev;

    dev->colour=1;  /* Set a fall back pen colour in case user doesn't */
 
/*
 * To try and fix the problem colourmap problems, I will try something new.
 * The colourmap index will be set to "-8888" and that way I will know when
 * a colour has been allocated, so I can manually deallocate it.
 */
 
    for (i=0;i<256;++i) dev->colour_index[i]=-8888;

/*
 *  Set the compression/quality level for JPEG files
 *  The higher the value, the bigger/better the image is
 */
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

     pls->graphx = GRAPHICS_MODE;

     dev->pngx = pls->xlength - 1;	/* should I use -1 or not??? */
     dev->pngy = pls->ylength - 1;

#ifdef use_experimental_hidden_line_hack

     if (dev->pngx>dev->pngy)    /* Work out the scaling factor for the  */
        {                        /* "virtual" (oversized) page           */
        dev->scale=PIXELS_X/dev->pngx;
        }
     else
        {
        dev->scale=PIXELS_Y/dev->pngy;
        }
#else

     dev->scale=1;

#endif   

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
     plP_setpxl(dev->scale*pls->xdpi/25.4,dev->scale*pls->ydpi/25.4);

     plP_setphy(0, dev->scale*dev->pngx, 0, dev->scale*dev->pngy);

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
    int x1 = x1a/dev->scale, y1 = y1a/dev->scale, x2 = x2a/dev->scale, y2 = y2a/dev->scale;
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
	   points[i].x = pls->dev_x[i]/dev->scale;
	   points[i].y = dev->pngy - (pls->dev_y[i]/dev->scale);
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
    int i, ncol1=pls->ncol1;
    int ncol0=pls->ncol0, total_colours;
    PLColor cmap1col;
    png_Dev *dev=(png_Dev *)pls->dev;
    PLFLT tmp_colour_pos;

/*
 * In theory when you call "gdImageDestroy()" the colourmap for that image
 * should be reset. Just to make absolutely sure it dies, and is completely
 * cleared out, the next bit of code will go through and try to flush out
 * all the colours, even if they should already have been flushed.
 */
 
    for (i=0;i<NCOLOURS;++i)
        {
         if (dev->colour_index[i]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[i]);
             dev->colour_index[i]=-8888;
            }
        }

    if (ncol0>NCOLOURS/2)     /* Check for ridiculous number of colours */
       {                      /* in ncol0, and appropriately adjust the */ 
        plwarn("Too many colours in cmap0.");     /* number, issuing a  */ 
        ncol0=NCOLOURS/2;                         /* warning if it does */
        pls->ncol0=ncol0;
       }

    dev->totcol=0;       /* Reset the number of colours counter to zero */
    
    total_colours=ncol0+ncol1;  /* Work out how many colours are wanted */

    if (total_colours>NCOLOURS)     /* Do some rather modest error      */
       {                            /* checking to make sure that       */
        total_colours=NCOLOURS;     /* we are not defining more colours */
        ncol1=total_colours-ncol0;  /* than we have room for.           */

        if (ncol1<=0)
           {
            plexit("Problem setting colourmap in PNG or JPEG driver.");
           }
       }
 
 dev->ncol1=ncol1;  /* The actual size of ncol1, regardless of what was asked. 
                     * This is dependent on colour slots available.
                     * It might well be the same as ncol1.
                     */
 
/* Initialize cmap 0 colors */

if (ncol0>0)  /* make sure the program actually asked for cmap0 first */
   {
    for (i = 0; i < ncol0; i++)
        {
        dev->colour_index[i]=gdImageColorAllocate(dev->im_out,
                                   pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);
        ++dev->totcol; /* count the number of colours we use as we use them */
        }

#ifdef SWAP_BALCK_WHEN_WHITE

/* 
 * Do a kludge to add a "black" colour back to the palette if the
 * background is "almost white" (ie changed through -bg).
 * 
 * Also includes an "optional" change to swap the red colour (1) with the 
 * black colour (15), which is off by default. (I don't like the red being
 * the 'default' colour "1" on a "white" background, or for that matter
 * yellow being "2", but I can live more with yellow at number two.)
 * Just use "-hack" from the command line to make it take effect.
 *
 */

if ((pls->cmap0[0].r>227)&&(pls->cmap0[0].g>227)&&(pls->cmap0[0].b>227))
   {
    if (pls->hack!=1)
       {
       if (dev->colour_index[15]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[15]);
             dev->colour_index[15]=gdImageColorAllocate(dev->im_out,0, 0, 0);
            } 
       }
    else
       {
       if (dev->colour_index[15]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[15]);
             dev->colour_index[15]=gdImageColorAllocate(dev->im_out, 
                              pls->cmap0[1].r, pls->cmap0[1].g, pls->cmap0[1].b);
            } 

       if (dev->colour_index[1]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[1]);
             dev->colour_index[1]=gdImageColorAllocate(dev->im_out,0,0,0); 
            } 
        
                              
      }
   }

#endif

  }

/* Initialize any remaining slots for cmap1 */


if (ncol1>0)    /* make sure that we want to define cmap1 first */
   {
    for (i = 0; i < ncol1; i++) 
        {

         if (ncol1<pls->ncol1)       /* Check the dynamic range of colours */
            {

             /*
              * Ok, now if we have less colour slots available than are being
              * defined by pls->ncol1, then we still want to use the full
              * dynamic range of cmap1 as best we can, so what we do is work
              * out an approximation to the index in the full dynamic range
              * in cases when pls->ncol1 exceeds the number of free colours.
              */

             tmp_colour_pos= i>0 ? pls->ncol1*((PLFLT)i/ncol1) : 0;
             plcol_interp(pls, &cmap1col, (int) tmp_colour_pos, pls->ncol1);
             
            }
         else
            {
             plcol_interp(pls, &cmap1col, i, ncol1);
            }


         dev->colour_index[i + pls->ncol0]=gdImageColorAllocate(dev->im_out,
                                   cmap1col.r, cmap1col.g, cmap1col.b);
                                   
         ++dev->totcol; /* count the number of colours we use as we go */
        }
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
PLFLT tmp_colour_pos;

    switch (op) {

    case PLSTATE_COLOR0:
	dev->colour = pls->icol0;
	if (dev->colour == PL_RGB_COLOR) {
	    int r = pls->curcolor.r;
	    int g = pls->curcolor.g;
	    int b = pls->curcolor.b;
	    if (dev->totcol < NCOLOURS) 
	       {
                if (dev->colour_index[dev->totcol+1]!=-8888) /* Should not ever be necessary  */
                   {                                         /* But won't hurt to be sure */
                    gdImageColorDeallocate(dev->im_out,dev->colour_index[dev->totcol+1]);
                   } 
	        
                dev->colour_index[++dev->totcol]=gdImageColorAllocate(dev->im_out,r, g, b);
		dev->colour = dev->totcol;
	       }

	}
	break;

    case PLSTATE_COLOR1:
        /*
         * Start by checking to see if we have to compensate for cases where
         * we don't have the full dynamic range of cmap1 at out disposal
         */
        if (dev->ncol1<pls->ncol1)   
           {
           tmp_colour_pos=dev->ncol1*((PLFLT)pls->icol1/(pls->ncol1>0 ? pls->ncol1 : 1));
           dev->colour = pls->ncol0 + (int)tmp_colour_pos;
           }
        else
           dev->colour = pls->ncol0 + pls->icol1;
        
	break;
	
	
    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
    /*
     *  Code to redfeine the entire palette
     */
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
    png_Dev *dev;

    plGetFam(pls);
/* force new file if pls->family set for all subsequent calls to plGetFam 
 * n.b. putting this after plGetFam call is important since plinit calls
 * bop, and you don't want the familying sequence started until after
 * that first call to bop.*/

/* n.b. pls->dev can change because of an indirect call to plD_init_png 
 * from plGetFam if familying is enabled.  Thus, wait to define dev until
 * now. */

    dev = (png_Dev *) pls->dev;

    pls->famadv = 1;

    pls->page++;

    dev->im_out = gdImageCreate(pls->xlength, pls->ylength);
    setcmap(pls);


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
int i;

    if (pls->family || pls->page == 1) {
       gdImagePng(dev->im_out, pls->OutFile);

/*
 * In theory when you call "gdImageDestroy()" the colourmap for that image
 * should be reset. Just to make absolutely sure it dies, and is completely
 * cleared out, the next bit of code will go through and try to flush out
 * all the colours, even if they should already have been flushed.
 */
 
    for (i=0;i<NCOLOURS;++i)
        {
         if (dev->colour_index[i]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[i]);
             dev->colour_index[i]=-8888;
            }
        }

       gdImageDestroy(dev->im_out);
    }
}

#endif

#ifdef PLD_jpeg

/*----------------------------------------------------------------------*\
 * plD_eop_jpeg()
 *
 * End of page.
\*----------------------------------------------------------------------*/

void plD_eop_jpeg(PLStream *pls)
{
png_Dev *dev=(png_Dev *)pls->dev;
int i;

    if (pls->family || pls->page == 1) {
       gdImageJpeg(dev->im_out, pls->OutFile, pls->dev_compression);
 
    for (i=0;i<NCOLOURS;++i)
        {
         if (dev->colour_index[i]!=-8888)
            {
             gdImageColorDeallocate(dev->im_out,dev->colour_index[i]);
             dev->colour_index[i]=-8888;
            }
        }

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
