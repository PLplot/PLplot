/* ///////////////////////////////////////////////////////////////////
//  File:       emxvga.c                                     [emx+gcc]
//
//  Descript:   the SVGA display driver for DOS & OS/2
//
//  Library:    plplot
//
//  Requires:   emx0.8h + gcc 2.5.7
//              svgakit/vesakit 1.5 (Johannes Martin)
//
//  Public:     plD_init_vga()
//              plD_line_vga()
//              plD_polyline_vga()
//              plD_eop_vga()
//              plD_bop_vga()
//              plD_tidy_vga()
//              plD_state_vga()
//              plD_esc_vga()
//
//              pldummy_emxvga()
//
//  Private:    pause()
//              vga_graphics()
//              vga_text()
//              g_mode_detect()
//              fill_polygon()
//
//  Notes:
//  0)  Since there is no *standard* graphics mode text output for emx,
//      just draw a small carriage return <-| in the top-left corner.
//
//  1)  Only supports 256+ colour, VESA compliant, SVGA cards.
//
//  2)  uses `-geometry WxH+Xof+Yoff` to force a screen resolution
//      * if (Xoffset > Width), use Xoffset as the VESA mode number [decimal]
//      * else use Width to select a 256 colour VESA mode
//           Width == 480 --> 640x480 mode
//           Width == 400 --> 640x400 mode
//         or
//           Width == 640 and Height is the tiebreaker
//      * else start at a high resolution and work downwards
//
//  3)  Uses color map 0, color map 1 colour selection, does polygon fills
//
//  Revisions:
//  13 Apr 94   mjo     documented (mj olesen)
//   8 Jun 94   mjo     color map 0,1 color selection, color map 0,1
//                      state change (palette change), polygon fills.
/////////////////////////////////////////////////////////////////// */
#if defined (__EMX__) && defined (EMXVESA)      /* (emx+gcc) */

#include "plplotP.h"            /* _read_kbd() in <stdlib.h> */
#include "drivers.h"
#include <graph.h>

/* Function prototypes */
/* INDENT OFF */

static void     pause           (void);
static void     vga_text        (PLStream *);
static void     vga_graphics    (PLStream *);
static int      g_mode_detect   (PLStream *);
static void     fill_polygon    (PLStream *);

typedef struct {
    short       isPageDirty;    /* track if something has been plotted */
    short       VESAmode;       /* the VESA mode number */
    short       xlen;           /* g_xsize -1 */
    short       ylen;           /* g_ysize -1 */
    short       ncol1;          /* Number of cmap 1 colors permitted */
    short       curcolor;       /* Current pen color */
} vgaDev;

/* //////////////////////////////////////////////////////////////// */

/* Initializes cmap 0 */
#define Cmap0Init(p)    \
        g_vgapal( (unsigned char *)((p)->cmap0), 0, (p)->ncol0, 0 )

/* Initializes cmap 1 */
#define Cmap1Init(p)    \
        g_vgapal( (unsigned char *)((p)->cmap1), (p)->ncol0, \
                ((vgaDev *)(p)->dev)->ncol1, 0 )

#define PROMPT_COLOR    2       /* colour for pause, same as axis labels */

/* INDENT ON */
/*----------------------------------------------------------------------*\
* g_mode_detect()
*
* try a variety of display resolutions
* not exactly autodetect ... but something
*
* check if `-geometry WxH+Xof+Yoff` was passed on the argument list
*
* if ( Xof > W )
*   use Xof as the VESA mode number (use cautiously)
* else if ( W > 0 && H > 0 )
*   use W and H to select a 256 colour VESA mode
* else
*   start at highest resolution 256 colour VESA mode and work down
*
\*----------------------------------------------------------------------*/
static int
g_mode_detect( PLStream *pls )
{                               /* ~~~~ */
  vgaDev *dev;
  int mode;
  dev = (vgaDev *) pls->dev;

/* if geometry was passed via arg list */
  if ( pls->xoffset > pls->xlength ) {
    mode = pls->xoffset;        /* offset specified = VESA mode */
    if ( !g_mode( mode ) ) {
      fprintf( stderr, "Can\'t set VESA mode %d (0x%x)\n", mode, mode );
      exit(0);
    }
  } else {                      /* xlength specified = X resolution */
    switch ( pls->xlength ) {
    case 1280:  mode = G1280x1024x256;  break;
    case 1024:  mode = G1024x768x256;   break;
    case 800:   mode = G800x600x256;    break;
    case 640:   mode = (pls->ylength<=400)? G640x400x256: G640x480x256; break;
    case 480:   mode = G640x480x256;    break;  /* easy way to get 640x480 */
    case 400:   mode = G640x400x256;    break;  /* easy way to get 640x400 */
    case 320:   mode = G320x200x256;    break;
    default:    mode = G1280x1024x256;  /* aim for a high resolution ! */
    }

    switch ( mode ) {           /* start high and work down */
    case G1280x1024x256:if(g_mode(mode=G1280x1024x256)) break;
    case G1024x768x256: if(g_mode(mode=G1024x768x256))  break;
    case G800x600x256:  if(g_mode(mode=G800x600x256))   break;
    case G640x480x256:  if(g_mode(mode=G640x480x256))   break;
    case G640x400x256:  if(g_mode(mode=G640x400x256))   break;
    case G320x200x256:  if(g_mode(mode=G320x200x256))   break;
    default:  mode = 0;      /* no vga card ? */
    }
  }

  dev->VESAmode = mode;
#ifdef SET_SIZE_EXPLICITLY
  { short x, y, c;              /* if you don't trust g_?size, g_colors */
    switch ( mode ) {
    case G1280x1024x256:x = 1280; y = 1024; c = 256; break;
    case G1024x768x256: x = 1024; y = 768;  c = 256; break;
    case G800x600x256:  x = 800;  y = 600;  c = 256; break;
    case G640x480x256:  x = 640;  y = 480;  c = 256; break;
    case G640x400x256:  x = 640;  y = 400;  c = 256; break;
    case G320x200x256:  x = 320;  y = 200;  c = 256; break;
    default:            x = y = c = 0;
    }
    pls->xlength  = x;
    pls->ylength  = y;
    dev->ncol1    = c;
  }
#else
  pls->xlength = g_xsize;
  pls->ylength = g_ysize;
  dev->ncol1 = g_colors;
#endif

  if ( pls->xlength && pls->ylength ) {
    dev->xlen = pls->xlength -1;
    dev->ylen = pls->ylength -1;
    if ( dev->ncol1 ) {
      dev->ncol1 -= pls->ncol0; /* subtract colours already in cmap0 */
      if ( dev->ncol1 > pls->ncol1 )
        dev->ncol1 = pls->ncol1;
    }
  } else {
    dev->xlen = dev->ylen = dev->ncol1 = 0;
  }

  return mode;
}


/*----------------------------------------------------------------------*\
* plD_init_vga()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
plD_init_vga(PLStream *pls)
{                               /* ~~~~ */
  vgaDev *dev;

  pls->termin = 1;              /* is an interactive terminal */
  pls->icol0 = 1;
  pls->width = 1;
  pls->bytecnt = 0;
  pls->page = 0;
  pls->graphx = 0;
  pls->dev_fill0 = 1;           /* Can do solid fills */

  if ( !pls->colorset )
    pls->color = 1;

/* Allocate and initialize device-specific data */
  if (pls->dev != NULL)
    free((void *) pls->dev);

  pls->dev = calloc(1, (size_t) sizeof(vgaDev));
  if (pls->dev == NULL)
    plexit("plD_init_vga: Out of memory.");
  dev = (vgaDev *) pls->dev;

  if ( !g_mode_detect(pls) )
    plexit( "plD_init_vga: No svga card?" );

  plP_setpxl( 3.0, 3.0 );       /* pixels per mm */
  plP_setphy(0, dev->xlen, 0, dev->ylen );

  pls->graphx = 0;
  vga_graphics(pls);
}

/*----------------------------------------------------------------------*\
* plD_line_vga()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_vga(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{                               /* ~~~~ */
    vgaDev *dev = (vgaDev *) pls->dev;
    dev->isPageDirty = 1;
    g_line ( (int) x1a, (int) (dev->ylen - y1a),
             (int) x2a, (int) (dev->ylen - y2a),
             dev->curcolor );
}

/*----------------------------------------------------------------------*\
* plD_polyline_vga()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_vga(PLStream *pls, short *xa, short *ya, PLINT npts)
{                               /* ~~~~ */
    PLINT i;
    vgaDev *dev = (vgaDev *) pls->dev;

    dev->isPageDirty = 1;
    for (i = 0; i < npts - 1; i++)
      g_line( (int) xa[i],      (int) (dev->ylen - ya[i]),
              (int) xa[i + 1],  (int) (dev->ylen - ya[i+1]),
              dev->curcolor );
}

/*----------------------------------------------------------------------*\
* plD_eop_vga()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_vga(PLStream *pls)
{                               /* ~~~~ */
  vgaDev *dev = (vgaDev *) pls->dev;
  if ( dev->isPageDirty ) {
    pause();
    g_clear( 0 );
    dev->isPageDirty = 0;
  }
}

/*----------------------------------------------------------------------*\
* plD_bop_vga()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
plD_bop_vga(PLStream *pls)
{                               /* ~~~~ */
  pls->page++;
  plD_eop_vga(pls);
}

/*----------------------------------------------------------------------*\
* plD_tidy_vga()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_vga(PLStream *pls)
{                               /* ~~~~ */
  vga_text(pls);
  pls->page = 0;
  pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* plD_state_vga()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void
plD_state_vga(PLStream *pls, PLINT op)
{                               /* ~~~~ */
  vgaDev *dev = (vgaDev *) pls->dev;

  switch (op) {
  case PLSTATE_WIDTH:   /* pen width */
    break;

  case PLSTATE_COLOR0:  /* switch to a color from cmap 0 */
    dev->curcolor = pls->icol0;
    break;

  case PLSTATE_COLOR1:  /* switch to a color from cmap 1 */
    dev->curcolor = (pls->icol1 * (dev->ncol1-1)) / (pls->ncol1-1)
        + pls->ncol0;
    break;

  case PLSTATE_FILL:    /* set area fill attribute */
    break;

  case PLSTATE_CMAP0:   /* change to cmap 0 */
    Cmap0Init(pls);
    break;

  case PLSTATE_CMAP1:   /* change to cmap 1 */
    Cmap1Init(pls);
    break;
  }
}

/*----------------------------------------------------------------------*\
* plD_esc_vga()
*
* Escape function.
*
* Functions:
*	PLESC_TEXT	change to text mode
*	PLESC_GRAPH	change to graphics mode
*	PLESC_FILL	Fill polygon
*
\*----------------------------------------------------------------------*/

void
plD_esc_vga(PLStream *pls, PLINT op, void *ptr)
{                               /* ~~~~ */
  switch (op) {
  case PLESC_TEXT:
    vga_text(pls);
    break;

  case PLESC_GRAPH:
    vga_graphics(pls);
    break;

  case PLESC_FILL:
    fill_polygon(pls);
    break;
  }
}

/*----------------------------------------------------------------------*\
* vga_graphics()
*
* Switch to graphis mode.
\*----------------------------------------------------------------------*/

static void
vga_graphics(PLStream *pls)
{                               /* ~~~~ */
  vgaDev *dev = (vgaDev *) pls->dev;

  if ( !pls->graphx ) {
    g_mode( dev->VESAmode );
    Cmap0Init(pls);
    Cmap1Init(pls);
    if ( pls->curcmap ) {       /* colour map 1 */
      dev->curcolor = (pls->icol1 * (dev->ncol1-1)) / (pls->ncol1-1)
                + pls->ncol0;
    } else {
      dev->curcolor = pls->icol0;
    }
    pls->graphx = 1;
    dev->isPageDirty = 0;
  }
}


/*----------------------------------------------------------------------*\
* vga_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

static void
vga_text(PLStream *pls)
{                               /* ~~~~ */
  vgaDev *dev = (vgaDev *) pls->dev;
  if ( pls->graphx ) {
    if ( dev->isPageDirty )
      pause();
    g_mode( GTEXT );

    pls->graphx = 0;
    dev->isPageDirty = 0;
  }
}

/*----------------------------------------------------------------------*\
* fill_polygon()
*
* Fill polygon described in points pls->dev_x[] and pls->dev_y[].
* Only solid color fill supported.
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{                               /* ~~~~ */
  int x[PL_MAXPOLY], y[PL_MAXPOLY], n;
  vgaDev *dev = (vgaDev *) pls->dev;

  if (pls->dev_npts > PL_MAXPOLY)
    plexit("Error -- too many points in polygon\n");

  for (n = 0; n < pls->dev_npts; n++) {
    x[n] = (int) pls->dev_x[n];
    y[n] = (int) (dev->ylen - pls->dev_y[n]);
  }

  g_polygon( x, y, n, (dev->curcolor), G_FILL );
}

/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
*
* someday, real text - but it works
\*----------------------------------------------------------------------*/

static const int
  Px[] = { 0x10, 0x10, 0x00, 0x20, 0x20 },      /* <-| prompt */
  Py[] = { 0x00, 0x10, 0x08, 0x08, 0x00 };

static void
pause(void)
{                               /* ~~~~ */
  g_polygon( Px, Py, 3, PROMPT_COLOR, G_FILL );
  g_line( Px[2], Py[2], Px[3], Py[3], PROMPT_COLOR );
  g_line( Px[3], Py[3], Px[4], Py[4], PROMPT_COLOR );

  if ( ! _read_kbd(0, 1, 0) )
    _read_kbd(0, 1, 0);
}

#else
int
pldummy_emxvga()
{                               /* ~~~~ */
  return 0;
}

#endif                          /* (__EMX__) && (EMXVESA) */
/* ///////////////////// end of file (c source) /////////////////// */
