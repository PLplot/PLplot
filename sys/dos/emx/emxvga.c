/* ///////////////////////////////////////////////////////////////////
//  File:       emxvga.c                                     [emx+gcc]
//
//  Descript:   the SVGA display driver for DOS & OS/2
//
//  Library:    plplot
//
//  Requires:   emx0.8h + gcc 2.5.7
//              svgakit/vesakit 1.5 (Johannes Martin)
//              c_plmtex() from plstring.c
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
//
//
//  Notes:
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
//  3)  Supports the PLPLOT RGB escape function.
//       ie., draw lines of any color you like.
//
//  4)  Currently, there is no bitmapped text output for emx :(
//      Instead, use c_plmtex() in the pause() function
//
//  Revisions:
//  13 Apr 94   mjo     documented (mj olesen)
//  -- --- --   ---     ---
/////////////////////////////////////////////////////////////////// */
#if defined (__EMX__) && defined (EMXVESA)      /* (emx+gcc) */

#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"
#include <graph.h>
#include <stdlib.h>             /* for _read_kbd() */

static void     pause           (void);
static void     vga_text        (PLStream *);
static void     vga_graphics    (PLStream *);
static int      g_mode_detect   (PLStream *);

#ifndef TRUE
  #define TRUE 1                /* define truth */
  #define FALSE 0
#endif

static short
    isPageDirty = FALSE,
    DetectedVideoMode,
    vgaXres,
    vgaYres,
    CurrentColour = 1,
    TotalColours = 16;

static unsigned char palette[][3] = {
   {  0,   0,   0 },    /* {   0,   0,   0 },   // black */
   { 63,   0,   0 },    /* { 255,   0,   0 },   // red */
   { 63,  63,   0 },    /* { 255, 255,   0 },   // yellow */
   {  0,  63,   0 },    /* {   0, 255,   0 },   // green */
   { 32,  63,  53 },    /* { 127, 255, 212 },   // acquamarine */
   { 63,  48,  51 },    /* { 255, 192, 203 },   // pink */
   { 61,  55,  46 },    /* { 245, 222, 179 },   // wheat */
   { 48,  48,  48 },    /* { 190, 190, 190 },   // grey */
   { 41,  10,  10 },    /* { 165,  42,  42 },   // brown */
   {  0,   0,  63 },    /* {   0,   0, 255 },   // blue */
   { 34,  11,  56 },    /* { 138,  43, 226 },   // Blue Violet */
   {  0,  63,  63 },    /* {   0, 255, 255 },   // cyan */
   { 16,  56,  52 },    /* {  64, 224, 208 },   // turquoise */
   { 63,   0,  63 },    /* { 255,   0, 255 },   // magenta */
   { 62,  32,  36 },    /* { 250, 128, 114 },   // salmon */
   { 63,  63,  63 },    /* { 255, 255, 255 },   // white */
   {  0,   0,   0 },    /* {   0,   0,   0 },   // black */
};

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
  int mode;
                /* if geometry was passed via arg list */
  if ( pls->xoffset > pls->xlength ) {
    mode = pls->xoffset;
    if ( !g_mode( mode ) ) {
      fprintf( stderr, "Can\'t set VESA mode %d (0x%x)\n", mode, mode );
      exit(0);
    }
    vgaXres = g_xsize;
    vgaYres = g_ysize;
  } else {
    switch ( pls->xlength ) {
    case 1280:
      mode = G1280x1024x256;
      break;
    case 1024:
      mode = G1024x768x256;
      break;
    case 800:
      mode = G800x600x256;
      break;
    case 640:
      mode = ( pls->ylength <= 400 )? G640x400x256 : G640x480x256;
      break;
    case 480:                   /* easy way to get 640x480 */
      mode = G640x480x256;
      break;
    case 400:                   /* easy way to get 640x480 */
      mode = G640x400x256;
      break;
    case 320:
      mode = G320x200x256;
      break;
    default:
      mode = G1280x1024x256;    /* aim for a high resolution ! */
      break;
    }

    switch ( mode ) {
      case G1280x1024x256:if ( g_mode(mode = G1280x1024x256) )    break;
      case G1024x768x256: if ( g_mode(mode = G1024x768x256) )     break;
      case G800x600x256:  if ( g_mode(mode = G800x600x256) )      break;
      case G640x480x256:  if ( g_mode(mode = G640x480x256) )      break;
      case G640x400x256:  if ( g_mode(mode = G640x400x256) )      break;
      case G320x200x256:  if ( g_mode(mode = G320x200x256) )      break;
      default:
        mode = 0;               /* no vga card ? */
    }
    vgaXres = g_xsize;
    vgaYres = g_ysize;
#ifdef SET_SIZE_EXPLICITLY      /* if you don't trust g_?size */
    switch ( mode ) {
      case G1280x1024x256:vgaXres = 1280; vgaYres = 1024; break;
      case G1024x768x256: vgaXres = 1024; vgaYres = 768;  break;
      case G800x600x256:  vgaXres = 800;  vgaYres = 600;  break;
      case G640x480x256:  vgaXres = 640;  vgaYres = 480;  break;
      case G640x400x256:  vgaXres = 640;  vgaYres = 400;  break;
      case G320x200x256:  vgaXres = 320;  vgaYres = 200;  break;
      default:            vgaXres = vgaYres = 0;
    }
#endif
  }

  if ( vgaXres && vgaYres ) {
    pls->xlength = vgaXres--;
    pls->ylength = vgaYres--;
  } else {
    pls->xlength = pls->ylength = 0;
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
  pls->termin = 1;              /* is an interactive terminal */
  pls->icol0 = 1;
  pls->width = 1;
  pls->bytecnt = 0;
  pls->page = 0;
  pls->graphx = FALSE;

  if (!pls->colorset)
      pls->color = 1;

  DetectedVideoMode = g_mode_detect(pls);

  if ( !DetectedVideoMode ) {
    g_vgapal( (unsigned char *) palette, 0, TotalColours = 16, 1 );

    pls->graphx = TRUE;
    isPageDirty = FALSE;
  }

  plP_setpxl(2.5, 2.5);         /* My best guess.  Seems to work okay. */

  plP_setphy(0, vgaXres, 0, vgaYres );

}

/*----------------------------------------------------------------------*\
* plD_line_vga()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_vga(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{                               /* ~~~~ */
    isPageDirty = TRUE;
    g_line ( (int) x1a, (int) (vgaYres - y1a),
             (int) x2a, (int) (vgaYres - y2a), CurrentColour );
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
    isPageDirty = TRUE;
    for (i = 0; i < npts - 1; i++)
      g_line( (int) xa[i],      (int) (vgaYres - ya[i]),
              (int) xa[i + 1],  (int) (vgaYres - ya[i+1]),
              CurrentColour );
}

/*----------------------------------------------------------------------*\
* plD_eop_vga()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_vga(PLStream *pls)
{                               /* ~~~~ */
  if ( isPageDirty ) {
    pause();
    g_clear( 0 );
    isPageDirty = FALSE;
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
  switch (op) {
  case PLSTATE_WIDTH:
    break;

  case PLSTATE_COLOR0:
    CurrentColour = pls->icol0;
    if ( CurrentColour == PL_RGB_COLOR && TotalColours < 255 ) {
      unsigned char rgb[3];
      rgb[0] = pls->curcolor.r;
      rgb[1] = pls->curcolor.g;
      rgb[2] = pls->curcolor.b;
      CurrentColour = ++TotalColours;
      g_vgapal( rgb, CurrentColour, 1, 1 );
    }
    break;

  case PLSTATE_COLOR1:
    break;
  }
}

/*----------------------------------------------------------------------*\
* plD_esc_vga()
*
* Escape function.
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
  }
}

/*----------------------------------------------------------------------*\
* Note: Switching to graphics mode.
*
* NOTE:  Moving to a new page causes the RGB map to be reset so that
* there will continue to be room.  This could conceivably cause a problem
* if an RGB escape was used to start drawing in a new color, and then
* it was expected that this would persist accross a page break.  If
* this happens, it will be necessary to rethink the logic of how this
* is handled.  Could wrap the index, for example.  This would require
* saving the RGB info used so far, which is not currently done.
\*----------------------------------------------------------------------*/

static void
vga_graphics(PLStream *pls)
{                               /* ~~~~ */
  if ( !pls->graphx ) {
    g_mode( DetectedVideoMode );
    g_vgapal( (unsigned char *) palette, 0, TotalColours = 16, 1 );
    pls->graphx = TRUE;
    isPageDirty = FALSE;
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
  if ( pls->graphx ) {
    if ( isPageDirty )
      pause();
    g_mode( GTEXT );

    pls->graphx = FALSE;
    isPageDirty = FALSE;
  }
}



/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
*
* someday, real text - but it works
\*----------------------------------------------------------------------*/

static void
pause(void)
{                               /* ~~~~ */
  c_plmtex( "lv", (PLFLT) 0, (PLFLT) 0, (PLFLT) 0, "Pause->" );
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
