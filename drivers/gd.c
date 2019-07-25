//       PNG, GIF, and JPEG device driver based on libgd
//
// Copyright (C) 2004  Joao Cardoso
// Copyright (C) 2002, 2003, 2004  Andrew Roach
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

//             GIF SUPPORT
//
//  Following the expiration of Unisys's worldwide patents on lzw compression
//  GD 2.0.28+ have reinstated support for GIFs, and so support for this
//  format has been added to the GD family of drivers. GIF's only support
//  1, 4 and 8 bit, so no truecolour. Why would you want GIFs though ? PNG is
//  a far superior format, not only giving you 1,4,8 and 24 bit, but also
//  better compression and just about all browsers now support them.
//

//
//  The GD drivers, PNG, GIF, and JPEG, support a number of different options
//  depending on the version of GD installed.
//
//  If you have installed GD Ver 2.+ you gain support for truecolour (24
//  bit, 16 millionish) modes as well as different line widths. These
//  capibilities are part of GD more so than the GD driver, so they aren't
//  available in any 1.? versions of the driver.
//
//  24 bit support is, by default, set to "auto" if you have V2.+ of GD.
//  What this means is the *driver* decides when to use 24 bit or 8 bit
//  modes for PNG files. The logic is rather simple - if you have less than
//  257 colours, it is set to 8 bit mode, if more then it's in 24 bit mode.
//  This should work fine for most people, most of the time, in most
//  situations; however, it can be overridden in case it has to via the
//  "-drvopt" command line switch. The png driver has two related settings:
//              8bit    and
//              24bit
//
//  If either of these command line toggles are set, that mode becomes the
//  standard used regardless of the number of colours used. It can be envoked
//  as follows:
//                 x08c -dev png -drvopt 8bit -fam -o 8bitpng
//                                      or
//                 x08c -dev png -drvopt 24bit -fam -o 24bitpng
//
//  NOTE:
//  The 24 bit PNG file is an RGBA file, not RGB - it includes alpha channel
//  (transparency). Transparency is set to opaque, but the fact it is an
//  RGBA and not an RGB might cause some problems with some viewers.
//  Sadly, I can't do anything about it... sorry.
//
//  GIF files can only have 256 colours, so naturally truecolour mode is not
//  supported for this sub-driver.
//
//  Stuff for GD V1.? as well as V2.+
//
//  optimise
//
//  From version 1.17 of the GD driver, a command line option has been
//  added to try and optimise the PNG files. If successful, the optimise
//  command will create 4 bit (16 colour) PNGs instead of 8 bit (256 colour)
//  ones. This results in slightly smaller files with no loss in any colour
//  information. The function has no real memory overhead, but does have a
//  slight speed hit in exchange for the optimisation. For example:
//         x08c -dev png -drvopt 8bit,optimise -fam -o 8bitpng
//  forces the png driver to make 8bit pngs, and will then optimise any PNG
//  images with 16 or less colours into a 4 bit PNG. Note, this DOESN'T WORK
//  WITH 24bit PNGs yet, and will never work with JPEGs.
//
//
//  Also as of version 1.17 of the GD driver, the options for palette
//  modification previously set with the command line option "-hack" have
//  now been moved to two options settable from the -drvopt switch.
//
//  def_black15
//
//  -drvopt def_black15 sets index 15, usually white, to black if index 0,
//  the background colour and usually black, has been set to white from the
//  command line option -bg
//
//  swp_red15
//
//  -drvopt swp_red15 swaps index 15, usually white, with index 1, which is
//  usually red. This might be desirable occasionally, but it is principally
//  included for cases when the background has been set on the command line
//  to white, and the "def_black15" option has been issued to redefine index
//  15 as black. By issuing a command like:
//                 x08c -dev png -bg ffffff -drvopt def_black15,swp_red15
//  the driver will set the background to white, then redefine index 15 of
//  cmap0, which is usually white to black, then swap index 2 (red) to 15
//  (white originally, now black), so at the end of the day, the "default"
//  plotting colour is now black. Why do all of this ? It is a very quick
//  way of making a nice web-friendly png without having to redefine the
//  cmaps within your program.
//
//  smoothlines
//
//  -drvopt smoothlines=1 turns on anti-aliased line and polygong drawing if
//  you are using a 24bit mode. Unfortunately gd doesn't honour line
//  width when anti-aliasing, so by default it is off.
//


#include "plDevs.h"

#if defined ( PLD_png ) || defined ( PLD_jpeg ) || defined ( PLD_gif )

#include "plplotP.h"
#include "drivers.h"

#include <gd.h>

//  Device info
//
//  Don't knoq if all this logic is necessary, but basically we are going to
//  start with all three sub-drivers present, then work out way down to two
//  and finally one of each.
//

PLDLLIMPEXP_DRIVER const char* plD_DEVICE_INFO_gd =
#if defined ( PLD_png )
    "png:PNG file:0:gd:39:png\n"
#endif
#if defined ( PLD_jpeg )
    "jpeg:JPEG file:0:gd:40:jpeg\n"
#endif
#if defined ( PLD_gif )
    "gif:GIF file:0:gd:47:gif\n"
#endif
;

#if GD2_VERS >= 2
#ifdef PL_HAVE_FREETYPE
#define SMOOTH_LINES_OK
#endif
#endif

#ifdef PL_HAVE_FREETYPE

//
//  Freetype support has been added to the GD family of drivers using the
//  plfreetype.c module, and implemented as a driver-specific optional extra
//  invoked via the -drvopt command line toggle. It uses the
//  "PLESC_HAS_TEXT" command for rendering within the driver.
//
//  Freetype support is turned on/off at compile time by defining
//  "PL_HAVE_FREETYPE".
//
//  To give the user some level of control over the fonts that are used,
//  environmental variables can be set to over-ride the definitions used by
//  the five default plplot fonts.
//
//  Freetype rendering is used with the command line "-drvopt text".
//  Anti-aliased fonts can be used by issuing "-drvopt text,smooth"
//

#include "plfreetype.h"

#endif

// Prototypes for functions in this file.

static void     fill_polygon( PLStream *pls );
static void     setcmap( PLStream *pls );
static void     plD_init_png_Dev( PLStream *pls );
static void     plD_gd_optimise( PLStream *pls );
static void     plD_black15_gd( PLStream *pls );
static void     plD_red15_gd( PLStream *pls );
#ifdef PLD_gif
static void     plD_init_gif_Dev( PLStream *pls );
#endif

#ifdef PL_HAVE_FREETYPE

static void plD_pixel_gd( PLStream *pls, short x, short y );
static PLINT plD_read_pixel_gd( PLStream *pls, short x, short y );
static void plD_set_pixel_gd( PLStream *pls, short x, short y, PLINT colour );
static void init_freetype_lv1( PLStream *pls );
static void init_freetype_lv2( PLStream *pls );

#endif

// top level declarations

static int NCOLOURS = gdMaxColors;

// In an attempt to fix a problem with the hidden line removal functions
// that results in hidden lines *not* being removed from "small" plot
// pages (ie, like a normal video screen), a "virtual" page of much
// greater size is used to trick the algorithm into working correctly.
// If, in future, this gets fixed on its own, then don't define
// "use_experimental_hidden_line_hack"
//

#define use_experimental_hidden_line_hack

// I think the current version of Freetype supports up to a maximum of
// 128 grey levels for text smoothing. You can get quite acceptable
// results with as few as 4 grey-levels. Uusually only about 5 get used
// anyway, but the question is where, in the "grey spectrum" will they be ?
// Who knows ? The following define lets you set a maximum limit on the
// number of grey-levels used. It is really only here for the 24bit mode
// and could be set to 255, but that would slow things down and use more
// memory. 64 seems to be a nice compromise, but if you want to change it,
// then change it here.
//

#ifndef max_number_of_grey_levels_used_in_text_smoothing
#define max_number_of_grey_levels_used_in_text_smoothing    64
#endif

// Not present in versions before 2.0

#ifndef gdImagePalettePixel
#define gdImagePalettePixel( im, x, y )    ( im )->pixels[( y )][( x )]
#endif

#if GD2_VERS >= 2
int plToGdAlpha( PLFLT a )
{
    int tmp = (int) ( ( 1.0 - a ) * gdAlphaMax );
    return tmp;
}
#endif

// Struct to hold device-specific info.

typedef struct
{
    gdImagePtr    im_out;                       // Graphics pointer
    PLINT         pngx;
    PLINT         pngy;

    int           colour;                        // Current Colour
    int           totcol;                        // Total number of colours
    int           ncol1;                         // Actual size of ncol1 we got

    PLFLT         scale;                         // scaling factor to "blow up" to
                                                 // the "virtual" page in removing hidden lines

    int           optimise;                      // Flag used for 4bit pngs
    int           black15;                       // Flag used for forcing a black colour
    int           red15;                         // Flag for swapping red and 15

    unsigned char TRY_BLENDED_ANTIALIASING;      // Flag to try and set up BLENDED ANTIALIASING

#if GD2_VERS >= 2
    int           truecolour;                   // Flag to ALWAYS force 24 bit mode
    int           palette;                      // Flag to ALWAYS force  8 bit mode
    unsigned char smooth;                       // Flag to ask for line smoothing
#endif
} png_Dev;

void plD_init_png( PLStream * );
void plD_line_png( PLStream *, short, short, short, short );
void plD_polyline_png( PLStream *, short *, short *, PLINT );
void plD_eop_png( PLStream * );
void plD_eop_jpeg( PLStream * );
void plD_bop_png( PLStream * );
void plD_tidy_png( PLStream * );
void plD_state_png( PLStream *, PLINT );
void plD_esc_png( PLStream *, PLINT, void * );
#ifdef PLD_gif
void plD_init_gif( PLStream * );
void plD_eop_gif( PLStream * );
#endif

#ifdef PLD_png

void plD_dispatch_init_png( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = "PNG file";
    pdt->pl_DevName = "png";
#endif
    pdt->pl_type     = plDevType_FileOriented;
    pdt->pl_seq      = 39;
    pdt->pl_init     = (plD_init_fp) plD_init_png;
    pdt->pl_line     = (plD_line_fp) plD_line_png;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_png;
    pdt->pl_eop      = (plD_eop_fp) plD_eop_png;
    pdt->pl_bop      = (plD_bop_fp) plD_bop_png;
    pdt->pl_tidy     = (plD_tidy_fp) plD_tidy_png;
    pdt->pl_state    = (plD_state_fp) plD_state_png;
    pdt->pl_esc      = (plD_esc_fp) plD_esc_png;
}

#endif

#ifdef PLD_jpeg

void plD_dispatch_init_jpeg( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = "JPEG File";
    pdt->pl_DevName = "jpeg";
#endif
    pdt->pl_type     = plDevType_FileOriented;
    pdt->pl_seq      = 40;
    pdt->pl_init     = (plD_init_fp) plD_init_png;
    pdt->pl_line     = (plD_line_fp) plD_line_png;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_png;
    pdt->pl_eop      = (plD_eop_fp) plD_eop_jpeg;
    pdt->pl_bop      = (plD_bop_fp) plD_bop_png;
    pdt->pl_tidy     = (plD_tidy_fp) plD_tidy_png;
    pdt->pl_state    = (plD_state_fp) plD_state_png;
    pdt->pl_esc      = (plD_esc_fp) plD_esc_png;
}
#endif


#ifdef PLD_gif

void plD_dispatch_init_gif( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = "GIF File";
    pdt->pl_DevName = "gif";
#endif
    pdt->pl_type     = plDevType_FileOriented;
    pdt->pl_seq      = 47;
    pdt->pl_init     = (plD_init_fp) plD_init_gif;
    pdt->pl_line     = (plD_line_fp) plD_line_png;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_png;
    pdt->pl_eop      = (plD_eop_fp) plD_eop_gif;
    pdt->pl_bop      = (plD_bop_fp) plD_bop_png;
    pdt->pl_tidy     = (plD_tidy_fp) plD_tidy_png;
    pdt->pl_state    = (plD_state_fp) plD_state_png;
    pdt->pl_esc      = (plD_esc_fp) plD_esc_png;
}
#endif


//--------------------------------------------------------------------------
// plD_init_png_Dev()
//
//--------------------------------------------------------------------------

static void
plD_init_png_Dev( PLStream *pls )
{
    png_Dev *dev;

//  Stuff for the driver options, these vars are copied into the driver
//  structure so that everything is thread safe and reenterant.
//

    static int optimise = 0;
    static int black15  = 0;
    static int red15    = 0;
#if GD2_VERS >= 2
    static int truecolour  = 0;
    static int palette     = 0;
    static int smooth_line = 0;
#endif
#ifdef PL_HAVE_FREETYPE
    static int freetype    = 1;
    static int smooth_text = 1;
    FT_Data    *FT;
#endif

    DrvOpt gd_options[] = { { "optimise",    DRV_INT, &optimise,    "Optimise PNG palette when possible"                                                                                                    },
                            { "def_black15", DRV_INT, &black15,     "Define idx 15 as black. If the background is \"whiteish\" (from \"-bg\" option), force index 15 (traditionally white) to be \"black\"" },
                            { "swp_red15",   DRV_INT, &red15,       "Swap index 1 (usually red) and 1 (usually white); always done after \"black15\"; quite useful for quick changes to web pages"          },
#if GD2_VERS >= 2
                            { "8bit",        DRV_INT, &palette,     "Palette (8 bit) mode"                                                                                                                  },
                            { "24bit",       DRV_INT, &truecolour,  "Truecolor (24 bit) mode"                                                                                                               },
                            { "smoothlines", DRV_INT, &smooth_line, "Turn line Anti Aliasing on (1) or off (0)"                                                                                             },
#endif
#ifdef PL_HAVE_FREETYPE
                            { "text",        DRV_INT, &freetype,    "Use driver text (FreeType)"                                                                                                            },
                            { "smooth",      DRV_INT, &smooth_text, "Turn text smoothing on (1) or off (0)"                                                                                                 },
#endif
                            { NULL,          DRV_INT, NULL,         NULL                                                                                                                                    } };


// Allocate and initialize device-specific data

    if ( pls->dev != NULL )
        free( (void *) pls->dev );

    pls->dev = calloc( 1, (size_t) sizeof ( png_Dev ) );
    if ( pls->dev == NULL )
        plexit( "plD_init_png_Dev: Out of memory." );

    dev = (png_Dev *) pls->dev;

    dev->colour = 1;  // Set a fall back pen colour in case user doesn't


// Check for and set up driver options

    plParseDrvOpts( gd_options );

    dev->black15  = black15;
    dev->red15    = red15;
    dev->optimise = optimise;

#if GD2_VERS >= 2

    dev->palette    = palette;
    dev->truecolour = truecolour;



    if ( ( dev->truecolour > 0 ) && ( dev->palette > 0 ) )
        plwarn( "Selecting both \"truecolor\" AND \"palette\" driver options is contradictory, so\nI will just use my best judgment.\n" );
    else if ( dev->truecolour > 0 )
        NCOLOURS = 16777216;
    else if ( ( dev->truecolour == 0 ) && ( dev->palette == 0 ) && ( ( pls->ncol1 + pls->ncol0 ) > NCOLOURS ) )
    {
        NCOLOURS = 16777216;
    }

    if ( ( dev->palette == 0 ) && ( dev->optimise == 0 ) && ( smooth_line == 1 ) )
        dev->smooth = 1;                                                                            // Allow smoothing of lines if we have a truecolour device

#endif

#ifdef PL_HAVE_FREETYPE
    if ( freetype )
    {
        pls->dev_text    = 1; // want to draw text
        pls->dev_unicode = 1; // want unicode

        // As long as we aren't optimising, we'll try to use better antialaising
        // We can also only do this if the user wants smoothing, and hasn't
        // selected a palette mode.
        //


        init_freetype_lv1( pls );
        FT = (FT_Data *) pls->FT;
        FT->want_smooth_text = smooth_text > 0 ? 1 : 0;
        if ( ( dev->optimise == 0 ) && ( dev->palette == 0 ) && ( smooth_text != 0 ) )
        {
            FT->BLENDED_ANTIALIASING = 1;
            dev->truecolour          = 1;
        }
    }

#endif
}

//--------------------------------------------------------------------------
// plD_init_png()
//
// Initialize device.
//--------------------------------------------------------------------------

void plD_init_png( PLStream *pls )
{
    png_Dev *dev = NULL;

    pls->termin    = 0;         // Not an interactive device
    pls->icol0     = 1;
    pls->bytecnt   = 0;
    pls->page      = 0;
    pls->dev_fill0 = 1;         // Can do solid fills

    if ( !pls->colorset )
        pls->color = 1;         // Is a color device

// Initialize family file info
    plFamInit( pls );

// Prompt for a file name if not already set
    plOpenFile( pls );

// Allocate and initialize device-specific data
    plD_init_png_Dev( pls );
    dev = (png_Dev *) pls->dev;

    // set dpi and page size defaults if the user has not already set
    // these with -dpi or -geometry command line options or with
    // plspage.

    if ( pls->xdpi <= 0. || pls->ydpi <= 0. )
    {
        // Use recommended default pixels per inch.
        plspage( PLPLOT_DEFAULT_PIXELS_PER_INCH, PLPLOT_DEFAULT_PIXELS_PER_INCH, 0, 0, 0, 0 );
    }

    if ( pls->xlength == 0 || pls->ylength == 0 )
    {
        // Use recommended default pixel width and height.
        plspage( 0., 0., PLPLOT_DEFAULT_WIDTH_PIXELS, PLPLOT_DEFAULT_HEIGHT_PIXELS, 0, 0 );
    }

    pls->graphx = GRAPHICS_MODE;

    dev->pngx = pls->xlength - 1;       // should I use -1 or not???
    dev->pngy = pls->ylength - 1;

#ifdef use_experimental_hidden_line_hack

    if ( dev->pngx > dev->pngy ) // Work out the scaling factor for the
    {                            // "virtual" (oversized) page
        dev->scale = (PLFLT) ( PIXELS_X - 1 ) / (PLFLT) dev->pngx;
    }
    else
    {
        dev->scale = (PLFLT) PIXELS_Y / (PLFLT) dev->pngy;
    }
#else

    dev->scale = 1.;

#endif

// Convert DPI to pixels/mm
    plP_setpxl( dev->scale * pls->xdpi / PLPLOT_MM_PER_INCH, dev->scale * pls->ydpi / PLPLOT_MM_PER_INCH );

    plP_setphy( 0, dev->scale * dev->pngx, 0, dev->scale * dev->pngy );

#ifdef PL_HAVE_FREETYPE
    if ( pls->dev_text )
    {
        init_freetype_lv2( pls );
    }
#endif
}


#ifdef PLD_gif

//--------------------------------------------------------------------------
// plD_init_gif_Dev()
//
// We need a new initialiser for the GIF version of the GD driver because
// the GIF one does not support TRUECOLOUR
//--------------------------------------------------------------------------

static void
plD_init_gif_Dev( PLStream *pls )
{
    png_Dev *dev;

//  Stuff for the driver options, these vars are copied into the driver
//  structure so that everything is thread safe and reenterant.
//

    static int black15 = 0;
    static int red15   = 0;
#ifdef PL_HAVE_FREETYPE
    static int freetype    = 1;
    static int smooth_text = 0;
    FT_Data    *FT;
#endif

    DrvOpt gd_options[] = { { "def_black15", DRV_INT, &black15,     "Define idx 15 as black. If the background is \"whiteish\" (from \"-bg\" option), force index 15 (traditionally white) to be \"black\"" },
                            { "swp_red15",   DRV_INT, &red15,       "Swap index 1 (usually red) and 1 (usually white); always done after \"black15\"; quite useful for quick changes to web pages"          },
#ifdef PL_HAVE_FREETYPE
                            { "text",        DRV_INT, &freetype,    "Use driver text (FreeType)"                                                                                                            },
                            { "smooth",      DRV_INT, &smooth_text, "Turn text smoothing on (1) or off (0)"                                                                                                 },
#endif
                            { NULL,          DRV_INT, NULL,         NULL                                                                                                                                    } };


// Allocate and initialize device-specific data

    if ( pls->dev != NULL )
        free( (void *) pls->dev );

    pls->dev = calloc( 1, (size_t) sizeof ( png_Dev ) );
    if ( pls->dev == NULL )
        plexit( "plD_init_gif_Dev: Out of memory." );

    dev = (png_Dev *) pls->dev;

    dev->colour = 1;  // Set a fall back pen colour in case user doesn't

// Check for and set up driver options

    plParseDrvOpts( gd_options );

    dev->black15 = black15;
    dev->red15   = red15;

    dev->optimise   = 0;  // Optimise does not work for GIFs... should, but it doesn't
    dev->palette    = 1;  // Always use palette mode for GIF files
    dev->truecolour = 0;  // Never have truecolour in GIFS

#ifdef PL_HAVE_FREETYPE
    if ( freetype )
    {
        pls->dev_text    = 1; // want to draw text
        pls->dev_unicode = 1; // want unicode

        init_freetype_lv1( pls );
        FT = (FT_Data *) pls->FT;

        FT->want_smooth_text = smooth_text > 0 ? 1 : 0;
    }

#endif
}

//--------------------------------------------------------------------------
// plD_init_gif()
//
// Initialize device.
//--------------------------------------------------------------------------

void plD_init_gif( PLStream *pls )
{
    png_Dev *dev = NULL;

    pls->termin    = 0;         // Not an interactive device
    pls->icol0     = 1;
    pls->bytecnt   = 0;
    pls->page      = 0;
    pls->dev_fill0 = 1;         // Can do solid fills

    if ( !pls->colorset )
        pls->color = 1;         // Is a color device

// Initialize family file info
    plFamInit( pls );

// Prompt for a file name if not already set
    plOpenFile( pls );

// Allocate and initialize device-specific data
    plD_init_gif_Dev( pls );
    dev = (png_Dev *) pls->dev;

    // set dpi and page size defaults if the user has not already set
    // these with -dpi or -geometry command line options or with
    // plspage.

    if ( pls->xdpi <= 0. || pls->ydpi <= 0. )
    {
        // Use recommended default pixels per inch.
        plspage( PLPLOT_DEFAULT_PIXELS_PER_INCH, PLPLOT_DEFAULT_PIXELS_PER_INCH, 0, 0, 0, 0 );
    }

    if ( pls->xlength == 0 || pls->ylength == 0 )
    {
        // Use recommended default pixel width and height.
        plspage( 0., 0., PLPLOT_DEFAULT_WIDTH_PIXELS, PLPLOT_DEFAULT_HEIGHT_PIXELS, 0, 0 );
    }

    pls->graphx = GRAPHICS_MODE;

    dev->pngx = pls->xlength - 1;       // should I use -1 or not???
    dev->pngy = pls->ylength - 1;

#ifdef use_experimental_hidden_line_hack

    if ( dev->pngx > dev->pngy ) // Work out the scaling factor for the
    {                            // "virtual" (oversized) page
        dev->scale = (PLFLT) ( PIXELS_X - 1 ) / (PLFLT) dev->pngx;
    }
    else
    {
        dev->scale = (PLFLT) PIXELS_Y / (PLFLT) dev->pngy;
    }
#else

    dev->scale = 1.;

#endif


// Convert DPI to pixels/mm
    plP_setpxl( dev->scale * pls->xdpi / PLPLOT_MM_PER_INCH, dev->scale * pls->ydpi / PLPLOT_MM_PER_INCH );

    plP_setphy( 0, dev->scale * dev->pngx, 0, dev->scale * dev->pngy );

#ifdef PL_HAVE_FREETYPE
    if ( pls->dev_text )
    {
        init_freetype_lv2( pls );
    }
#endif
}

#endif


//--------------------------------------------------------------------------
// plD_line_png()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//--------------------------------------------------------------------------

void
plD_line_png( PLStream *pls, short x1a, short y1a, short x2a, short y2a )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int     x1 = x1a / dev->scale, y1 = y1a / dev->scale, x2 = x2a / dev->scale, y2 = y2a / dev->scale;
    y1 = dev->pngy - y1;
    y2 = dev->pngy - y2;

    #ifdef SMOOTH_LINES_OK
    if ( dev->smooth == 1 )
    {
        gdImageSetAntiAliased( dev->im_out, dev->colour );
        gdImageLine( dev->im_out, x1, y1, x2, y2, gdAntiAliased );
    }
    else
    {
        gdImageLine( dev->im_out, x1, y1, x2, y2, dev->colour );
    }
    #else
    gdImageLine( dev->im_out, x1, y1, x2, y2, dev->colour );
    #endif
}

//--------------------------------------------------------------------------
// plD_polyline_png()
//
// Draw a polyline in the current color.
//--------------------------------------------------------------------------

void
plD_polyline_png( PLStream *pls, short *xa, short *ya, PLINT npts )
{
    PLINT i;

    for ( i = 0; i < npts - 1; i++ )
        plD_line_png( pls, xa[i], ya[i], xa[i + 1], ya[i + 1] );
}


//--------------------------------------------------------------------------
// fill_polygon()
//
// Fill polygon described in points pls->dev_x[] and pls->dev_y[].
//--------------------------------------------------------------------------

static void
fill_polygon( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;

    int     i;
    gdPoint *points = NULL;

    if ( pls->dev_npts < 1 )
        return;

    points = malloc( (size_t) pls->dev_npts * sizeof ( gdPoint ) );

    for ( i = 0; i < pls->dev_npts; i++ )
    {
        points[i].x = pls->dev_x[i] / dev->scale;
        points[i].y = dev->pngy - ( pls->dev_y[i] / dev->scale );
    }

    #ifdef SMOOTH_LINES_OK
    if ( dev->smooth == 1 )
    {
        gdImageSetAntiAliased( dev->im_out, dev->colour );
        gdImageFilledPolygon( dev->im_out, points, pls->dev_npts, gdAntiAliased );
    }
    else
    {
        gdImageFilledPolygon( dev->im_out, points, pls->dev_npts, dev->colour );
    }
    #else
    gdImageFilledPolygon( dev->im_out, points, pls->dev_npts, dev->colour );
    #endif

    free( points );
}

//--------------------------------------------------------------------------
// setcmap()
//
// Sets up color palette.
//--------------------------------------------------------------------------

static void
setcmap( PLStream *pls )
{
    int     i, ncol1 = pls->ncol1;
    int     ncol0 = pls->ncol0, total_colours;
    PLColor cmap1col;
    png_Dev *dev = (png_Dev *) pls->dev;
    PLFLT   tmp_colour_pos;

//
// Yuckky fix to get rid of the previosuly allocated palette from the
// GD image
//

    if ( dev->im_out != NULL )
    {
        for ( i = 0; i < 256; i++ )
        {
            gdImageColorDeallocate( dev->im_out, i );
        }
    }

    if ( ncol0 > NCOLOURS / 2 )                     // Check for ridiculous number of colours
    {                                               // in ncol0, and appropriately adjust the
        plwarn( "Too many colours in cmap0." );     // number, issuing a
        ncol0      = NCOLOURS / 2;                  // warning if it does
        pls->ncol0 = ncol0;
    }

    dev->totcol = 0;                           // Reset the number of colours counter to zero

    total_colours = ncol0 + ncol1;             // Work out how many colours are wanted

    if ( total_colours > NCOLOURS )            // Do some rather modest error
    {                                          // checking to make sure that
        total_colours = NCOLOURS;              // we are not defining more colours
        ncol1         = total_colours - ncol0; // than we have room for.

        if ( ncol1 <= 0 )
        {
            plexit( "Problem setting colourmap in PNG or JPEG driver." );
        }
    }

    dev->ncol1 = ncol1; // The actual size of ncol1, regardless of what was asked.
                        // This is dependent on colour slots available.
                        // It might well be the same as ncol1.
                        //

// Initialize cmap 0 colors

    if ( ( ncol0 > 0 ) && ( dev->im_out != NULL ) ) // make sure the program actually asked for cmap0 first
    {
        for ( i = 0; i < ncol0; i++ )
        {
#if GD2_VERS >= 2
            gdImageColorAllocateAlpha( dev->im_out,
                pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b,
                plToGdAlpha( pls->cmap0[i].a ) );
#else
            gdImageColorAllocate( dev->im_out,
                pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b );
#endif
            ++dev->totcol; // count the number of colours we use as we use them
        }
    }

// Initialize any remaining slots for cmap1


    if ( ( ncol1 > 0 ) && ( dev->im_out != NULL ) ) // make sure that we want to define cmap1 first
    {
        for ( i = 0; i < ncol1; i++ )
        {
            if ( ncol1 < pls->ncol1 ) // Check the dynamic range of colours
            {
                //
                // Ok, now if we have less colour slots available than are being
                // defined by pls->ncol1, then we still want to use the full
                // dynamic range of cmap1 as best we can, so what we do is work
                // out an approximation to the index in the full dynamic range
                // in cases when pls->ncol1 exceeds the number of free colours.
                //

                tmp_colour_pos = i > 0 ? pls->ncol1 * ( (PLFLT) i / ncol1 ) : 0;
                plcol_interp( pls, &cmap1col, (int) tmp_colour_pos, pls->ncol1 );
            }
            else
            {
                plcol_interp( pls, &cmap1col, i, ncol1 );
            }


#if GD2_VERS >= 2
            gdImageColorAllocateAlpha( dev->im_out,
                cmap1col.r, cmap1col.g, cmap1col.b,
                plToGdAlpha( cmap1col.a ) );
#else
            gdImageColorAllocate( dev->im_out,
                cmap1col.r, cmap1col.g, cmap1col.b );
#endif

            ++dev->totcol; // count the number of colours we use as we go
        }
    }
}


//--------------------------------------------------------------------------
// plD_state_png()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//--------------------------------------------------------------------------

void
plD_state_png( PLStream *pls, PLINT op )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    PLFLT   tmp_colour_pos;
#if GD2_VERS >= 2
    long    temp_col;
#endif


    switch ( op )
    {
#if GD2_VERS >= 2
    case PLSTATE_WIDTH:
        gdImageSetThickness( dev->im_out, pls->width );
        break;
#endif

    case PLSTATE_COLOR0:
#if GD2_VERS >= 2

        if ( ( pls->icol0 == PL_RGB_COLOR ) ||         //  Should never happen since PL_RGB_COLOR is deprecated, but here for backwards compatibility
             ( gdImageTrueColor( dev->im_out ) ) )     //  We will do this if we are in "TrueColour" mode
        {
            if ( ( dev->totcol < NCOLOURS ) ||         // See if there are slots left, if so we will allocate a new colour
                 ( gdImageTrueColor( dev->im_out ) ) ) // In TrueColour mode we allocate each colour as we come to it
            {
                // Next allocate a new colour to a temporary slot since what we do with it will vary depending on if its a palette index or truecolour
#if GD2_VERS >= 2
                temp_col = gdImageColorAllocateAlpha( dev->im_out, pls->curcolor.r,
                    pls->curcolor.g, pls->curcolor.b,
                    plToGdAlpha( pls->curcolor.a ) );
#else
                temp_col = gdImageColorAllocate( dev->im_out, pls->curcolor.r,
                    pls->curcolor.g, pls->curcolor.b );
#endif

                if ( gdImageTrueColor( dev->im_out ) )
                    dev->colour = temp_col;     // If it's truecolour, then we will directly set dev->colour to our "new" colour
                else
                {
                    dev->colour = dev->totcol;   // or else, we will just set it to the last colour
                    dev->totcol++;               // Bump the total colours for next time round
                }
            }
        }
        else   // just a normal colour allocate, so don't worry about the above stuff, just grab the index
        {
            dev->colour = pls->icol0;
        }

#else
        dev->colour = pls->icol0;
        if ( dev->colour == PL_RGB_COLOR )
        {
            if ( dev->totcol < NCOLOURS )
            {
#if GD2_VERS >= 2
                gdImageColorAllocateAlpha( dev->im_out, pls->curcolor.r,
                    pls->curcolor.g, pls->curcolor.b,
                    plToGdAlpha( pls->curcolor.a ) );
#else
                gdImageColorAllocate( dev->im_out, pls->curcolor.r,
                    pls->curcolor.g, pls->curcolor.b );
#endif
                dev->colour = dev->totcol;
            }
        }
#endif
        break;

    case PLSTATE_COLOR1:

#if GD2_VERS >= 2
        if ( !gdImageTrueColor( dev->im_out ) )
        {
#endif
        //
        // Start by checking to see if we have to compensate for cases where
        // we don't have the full dynamic range of cmap1 at our disposal
        //
        if ( dev->ncol1 < pls->ncol1 )
        {
            tmp_colour_pos = dev->ncol1 * ( (PLFLT) pls->icol1 / ( pls->ncol1 > 0 ? pls->ncol1 : 1 ) );
            dev->colour    = pls->ncol0 + (int) tmp_colour_pos;
        }
        else
            dev->colour = pls->ncol0 + pls->icol1;
#if GD2_VERS >= 2
    }
    else        // it is a truecolour image
    {
#if GD2_VERS >= 2
        dev->colour = gdTrueColorAlpha( pls->curcolor.r, pls->curcolor.g,
            pls->curcolor.b,
            plToGdAlpha( pls->curcolor.a ) );
#else
        dev->colour = gdTrueColor( pls->curcolor.r, pls->curcolor.g,
            pls->curcolor.b );
#endif
    }
#endif
        break;


    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:

#if GD2_VERS >= 2
        if ( ( dev->im_out != NULL ) && !gdImageTrueColor( dev->im_out ) )
        {
#endif

        //
        //  Code to redefine the entire palette
        //


        if ( pls->color )
            setcmap( pls );

#if GD2_VERS >= 2
    }
#endif

        break;
    }
}


//--------------------------------------------------------------------------
// plD_esc_png()
//
// Escape function.
//--------------------------------------------------------------------------

void plD_esc_png( PLStream *pls, PLINT op, void *ptr )
{
    switch ( op )
    {
    case PLESC_FILL:    // fill
        fill_polygon( pls );
        break;

#ifdef PL_HAVE_FREETYPE
    case PLESC_HAS_TEXT:
        plD_render_freetype_text( pls, (EscText *) ptr );
        break;
#endif
    }
}

//--------------------------------------------------------------------------
// plD_bop_png()
//
// Set up for the next page.
// Advance to next family file if necessary (file output).
//--------------------------------------------------------------------------

void plD_bop_png( PLStream *pls )
{
    png_Dev *dev;

    plGetFam( pls );
// force new file if pls->family set for all subsequent calls to plGetFam
// n.b. putting this after plGetFam call is important since plinit calls
// bop, and you don't want the familying sequence started until after
// that first call to bop.

// n.b. pls->dev can change because of an indirect call to plD_init_png
// from plGetFam if familying is enabled.  Thus, wait to define dev until
// now.

    dev = (png_Dev *) pls->dev;

    pls->famadv = 1;

    pls->page++;

    if ( dev->black15 )
        plD_black15_gd( pls );
    if ( dev->red15 )
        plD_red15_gd( pls );

#if GD2_VERS >= 2
    if ( ( ( ( ( dev->truecolour > 0 ) && ( dev->palette > 0 ) ) ||     // In an EXTREMELY convaluted
             ( ( dev->truecolour == 0 ) && ( dev->palette == 0 ) ) ) && // manner, all this is just
           ( ( pls->ncol1 + pls->ncol0 ) <= 256 ) ) ||                  // asking the question, do we
         ( ( ( dev->palette > 0 ) && ( dev->truecolour == 0 ) ) ) )     // want truecolour or not ?
    {
#endif

    dev->im_out = gdImageCreate( pls->xlength, pls->ylength );

    setcmap( pls );

#if GD2_VERS >= 2
}
else
{
    dev->im_out = gdImageCreateTrueColor( pls->xlength, pls->ylength );
    plP_state( PLSTATE_COLOR0 );

//
// In truecolour mode, the background colour GD makes is ALWAYS black, so to
// "simulate" (stimulate?) a background colour other than black, we will just
// draw a dirty big rectange covering the whole image and colour it in
// whatever colour cmap0[0] happens to be.
//
// Question to C gurus: while it is slightly illogical and ugly, would:
//   if ((pls->cmap0[0].r+pls->cmap0[0].g+pls->cmap0[0].b)!=0)
// be more computationally efficient than:
//   if ((pls->cmap0[0].r!=0)||(pls->cmap0[0].g!=0)||(pls->cmap0[0].b!=0))
//  ???
//

    if ( ( pls->cmap0[0].r != 0 ) || ( pls->cmap0[0].g != 0 ) ||
         ( pls->cmap0[0].b != 0 ) || ( pls->cmap0[0].a != 0.0 ) )
    {
        gdImageFilledRectangle( dev->im_out, 0, 0, pls->xlength - 1, pls->ylength - 1,
            gdTrueColorAlpha( pls->cmap0[0].r, pls->cmap0[0].g,
                pls->cmap0[0].b,
                plToGdAlpha( pls->cmap0[0].a ) ) );
    }
}


// This ensures the line width is set correctly at the beginning of
//    each page

plD_state_png( pls, PLSTATE_WIDTH );

#endif
}

//--------------------------------------------------------------------------
// plD_tidy_png()
//
// Close graphics file or otherwise clean up.
//--------------------------------------------------------------------------

void plD_tidy_png( PLStream *pls )
{
#ifdef PL_HAVE_FREETYPE
    if ( pls->dev_text )
    {
        plD_FreeType_Destroy( pls );
    }
#endif

    plCloseFile( pls );
    free_mem( pls->dev );
}

//--------------------------------------------------------------------------
// plD_black15_gd()
//
//  This small function simply redefines index 15 of cmap0, which is
//  usually set to white, to black, but only if index 0, which is usually
//  black, has been redefined to white (for example, through -bg).
//
//--------------------------------------------------------------------------

void plD_black15_gd( PLStream *pls )
{
    if ( pls->ncol0 > 15 )
    {
        if ( ( pls->cmap0[0].r > 227 ) && ( pls->cmap0[0].g > 227 ) && ( pls->cmap0[0].b > 227 ) )
        {
            pls->cmap0[15].r = 0;
            pls->cmap0[15].g = 0;
            pls->cmap0[15].b = 0;
        }
    }
}


//--------------------------------------------------------------------------
// plD_red15_gd()
//
//
//  This function swaps index 1, often the default plotting colour, with
//  index 15, the last defined colour.
//
//  Colour 15 is usually white, and 1 is usually red, so swapping the two
//  might be desirable occasionally, but it is principally here for cases
//  when the background has been set on the command line to white, and the
//  "def_black15" option has been issued to redefine index 15 as black. By
//  issuing a command like
//
//      ... -bg ffffff -drvopt def_black15,swp_red15
//
//  the driver will set the background to white, then redefine index 15 of
//  cmap0, which is usually white to black, then swap index 2 (red) to 15
//  (white originally, now black), so at the end of the day, the "default"
//  plotting colour is now black. Why do all of this ? It is a very quick
//  way of making a nice web-friendly png without having to redefine the
//  cmaps within your program.
//
//  If you don't like it, don't use it !
//
//--------------------------------------------------------------------------

void plD_red15_gd( PLStream *pls )
{
    char r = pls->cmap0[1].r;
    char g = pls->cmap0[1].g;
    char b = pls->cmap0[1].b;

    if ( pls->ncol0 > 15 )
    {
        pls->cmap0[1].r = pls->cmap0[15].r;
        pls->cmap0[1].g = pls->cmap0[15].r;
        pls->cmap0[1].b = pls->cmap0[15].r;

        pls->cmap0[15].r = r;
        pls->cmap0[15].g = g;
        pls->cmap0[15].b = b;
    }
}


//--------------------------------------------------------------------------
// plD_gd_optimise()
//
//
//  This function pretty much does exactly what it says - it optimises the
//  PNG file. It does this by checking to see if all the allocated colours
//  were actually used. If they were not, then it deallocates them. This
//  function often results in the PNG file being saved as a 4 bit (16
//  colour) PNG rather than an 8 bit (256 colour) PNG. The file size
//  difference is not huge, not as great as for GIFs for example (I think
//  most of the saving comes from removing redundant entries from the
//  palette entry in the header); however some modest size savings occur.
//
//  The function isn't always successful - the optimiser will always
//  deallocate unused colours as it finds them, but GD will only deallocate
//  them "for real" until 16 colours are used up, and then stop since it
//  doesn't make a difference if you have 17 colours or 255 colours. The
//  result of this is you may end up with an image using say, 130 colours,
//  but you will have 240 colour entries, some of which aren't used, and
//  aren't blanked out.
//
//  Another side-effect of this function is the relative position of the
//  colour indices MAY shift as colours are deallocated. I really don't
//  think this should worry anyone, but if it does, don't optimise the
//  image !
//
//--------------------------------------------------------------------------

void plD_gd_optimise( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int i, j;
    char *bbuf;

    bbuf = calloc( 256, (size_t) 1 ); // Allocate a buffer to "check off" colours as they are used
    if ( bbuf == NULL )
        plexit( "plD_gd_optimise: Out of memory." );

    for ( i = 0; i < ( pls->xlength - 1 ); i++ )        // Walk through the image pixel by pixel
    {                                                   // checking to see what colour it is
        for ( j = 0; j < ( pls->ylength - 1 ); j++ )    // and adding it to the list of used colours
        {
            bbuf[gdImagePalettePixel( dev->im_out, i, j )] = 1;
        }
    }

    for ( i = 0; i < 256; i++ ) // next walk over the colours and deallocate
    {                           // unused ones
        if ( bbuf[i] == 0 )
            gdImageColorDeallocate( dev->im_out, i );
    }

    free( bbuf );
}


#ifdef PLD_png

//--------------------------------------------------------------------------
// plD_eop_png()
//
// End of page.
//--------------------------------------------------------------------------

void plD_eop_png( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int im_size  = 0;
    int png_compression;
    void *im_ptr = NULL;
    size_t nwrite;

    if ( pls->family || pls->page == 1 )
    {
        if ( dev->optimise )
        {
#if GD2_VERS >= 2
            if ( ( ( ( ( dev->truecolour > 0 ) && ( dev->palette > 0 ) ) ||     // In an EXTREMELY convaluted
                     ( ( dev->truecolour == 0 ) && ( dev->palette == 0 ) ) ) && // manner, all this is just
                   ( ( pls->ncol1 + pls->ncol0 ) <= 256 ) ) ||                  // asking the question, do we
                 ( ( ( dev->palette > 0 ) && ( dev->truecolour == 0 ) ) ) )     // want truecolour or not ?
            {
#endif
            plD_gd_optimise( pls );

#if GD2_VERS >= 2
        }
#endif
        }


        // image is written to output file by the driver
        // since if the gd.dll is linked to a different c
        // lib a crash occurs - this fix works also in Linux
        // gdImagePng(dev->im_out, pls->OutFile);
       #if GD2_VERS >= 2

        //Set the compression/quality level for PNG files.
        // pls->dev_compression values of 1-9 translate to the zlib compression values 1-9
        // pls->dev_compression values 10 <= compression <= 99 are divided by 10 to get the zlib
        // compression value. Values <=0 or greater than 99 are set to 90 which
        // translates to a zlib compression value of 9, the highest quality
        // of compression or smallest file size or largest computer time required
        // to achieve the compression.  Smaller zlib compression values correspond
        // to lower qualities of compression (larger file size), but lower
        // computer times as well.

        png_compression = ( ( pls->dev_compression <= 0 ) || ( pls->dev_compression > 99 ) ) ? 90 : pls->dev_compression;
        png_compression = ( png_compression > 9 ) ? ( png_compression / 10 ) : png_compression;
        im_ptr          = gdImagePngPtrEx( dev->im_out, &im_size, png_compression );
       #else
        im_ptr = gdImagePngPtr( dev->im_out, &im_size );
       #endif
        if ( im_ptr )
        {
            nwrite = fwrite( im_ptr, sizeof ( char ), im_size, pls->OutFile );
            if ( nwrite != im_size )
                plabort( "gd driver: Error writing png file" );
            gdFree( im_ptr );
        }

        gdImageDestroy( dev->im_out );
        dev->im_out = NULL;
    }
}

#endif

#ifdef PL_HAVE_FREETYPE

//--------------------------------------------------------------------------
//  void plD_pixel_gd (PLStream *pls, short x, short y)
//
//  callback function, of type "plD_pixel_fp", which specifies how a single
//  pixel is set in the current colour.
//--------------------------------------------------------------------------

void plD_pixel_gd( PLStream *pls, short x, short y )
{
    png_Dev *dev = (png_Dev *) pls->dev;

    gdImageSetPixel( dev->im_out, x, y, dev->colour );
}

//--------------------------------------------------------------------------
//  void plD_set_pixel_gd (PLStream *pls, short x, short y)
//
//  callback function, of type "plD_pixel_fp", which specifies how a single
//  pixel is set directly to hardware, using the colour provided
//--------------------------------------------------------------------------

void plD_set_pixel_gd( PLStream *pls, short x, short y, PLINT colour )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int R, G, B;
    int Colour;

    G = GetGValue( colour );
    R = GetRValue( colour );
    B = GetBValue( colour );

    Colour = gdImageColorResolve( dev->im_out, R, G, B );
    gdImageSetPixel( dev->im_out, x, y, Colour );
}

//--------------------------------------------------------------------------
//  PLINT plD_read_pixel_gd (PLStream *pls, short x, short y)
//
//  callback function, of type "plD_read_pixel_gd", which specifies how a
//  single pixel's RGB is read (in the destination context), then
//  returns an RGB encoded int with the info for blending.
//--------------------------------------------------------------------------

PLINT plD_read_pixel_gd( PLStream *pls, short x, short y )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    PLINT colour;
    unsigned char R, G, B;

    colour = gdImageGetTrueColorPixel( dev->im_out, x, y );

    R = gdTrueColorGetRed( colour );
    G = gdTrueColorGetGreen( colour );
    B = gdTrueColorGetBlue( colour );

    colour = RGB( R, G, B );
    return ( colour );
}


//--------------------------------------------------------------------------
//  void init_freetype_lv1 (PLStream *pls)
//
//  "level 1" initialisation of the freetype library.
//  "Level 1" initialisation calls plD_FreeType_init(pls) which allocates
//  memory to the pls->FT structure, then sets up the pixel callback
//  function.
//--------------------------------------------------------------------------

static void init_freetype_lv1( PLStream *pls )
{
    FT_Data *FT;

    plD_FreeType_init( pls );

    FT             = (FT_Data *) pls->FT;
    FT->pixel      = (plD_pixel_fp) plD_pixel_gd;
    FT->read_pixel = (plD_read_pixel_fp) plD_read_pixel_gd;
    FT->set_pixel  = (plD_set_pixel_fp) plD_set_pixel_gd;
}

//--------------------------------------------------------------------------
//  void init_freetype_lv2 (PLStream *pls)
//
//  "Level 2" initialisation of the freetype library.
//  "Level 2" fills in a few setting that aren't public until after the
//  graphics sub-syetm has been initialised.
//  The "level 2" initialisation fills in a few things that are defined
//  later in the initialisation process for the GD driver.
//
//  FT->scale is a scaling factor to convert co-ordinates. This is used by
//  the GD and other drivers to scale back a larger virtual page and this
//  eliminate the "hidden line removal bug". Set it to 1 if your device
//  doesn't have scaling.
//
//  Some coordinate systems have zero on the bottom, others have zero on
//  the top. Freetype does it one way, and most everything else does it the
//  other. To make sure everything is working ok, we have to "flip" the
//  coordinates, and to do this we need to know how big in the Y dimension
//  the page is, and whether we have to invert the page or leave it alone.
//
//  FT->ymax specifies the size of the page FT->invert_y=1 tells us to
//  invert the y-coordinates, FT->invert_y=0 will not invert the
//  coordinates.
//--------------------------------------------------------------------------

static void init_freetype_lv2( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    FT_Data *FT  = (FT_Data *) pls->FT;

    FT->scale       = dev->scale;
    FT->ymax        = dev->pngy;
    FT->invert_y    = 1;
    FT->smooth_text = 0;

    if ( ( FT->want_smooth_text == 1 ) && ( FT->BLENDED_ANTIALIASING == 0 ) ) // do we want to at least *try* for smoothing ?
    {
        FT->ncol0_org   = pls->ncol0;                                         // save a copy of the original size of ncol0
        FT->ncol0_xtra  = NCOLOURS - ( pls->ncol1 + pls->ncol0 );             // work out how many free slots we have
        FT->ncol0_width = FT->ncol0_xtra / ( pls->ncol0 - 1 );                // find out how many different shades of anti-aliasing we can do
        if ( FT->ncol0_width > 4 )                                            // are there enough colour slots free for text smoothing ?
        {
            if ( FT->ncol0_width > max_number_of_grey_levels_used_in_text_smoothing )
                FT->ncol0_width = max_number_of_grey_levels_used_in_text_smoothing;           // set a maximum number of shades
            plscmap0n( FT->ncol0_org + ( FT->ncol0_width * pls->ncol0 ) );                    // redefine the size of cmap0
// the level manipulations are to turn off the plP_state(PLSTATE_CMAP0)
// call in plscmap0 which (a) leads to segfaults since the GD image is
// not defined at this point and (b) would be inefficient in any case since
// setcmap is always called later (see plD_bop_png) to update the driver
// color palette to be consistent with cmap0.
            {
                PLINT level_save;
                level_save = pls->level;
                pls->level = 0;
                pl_set_extended_cmap0( pls, FT->ncol0_width, FT->ncol0_org ); // call the function to add the extra cmap0 entries and calculate stuff
                pls->level = level_save;
            }
            FT->smooth_text = 1; // Yippee ! We had success setting up the extended cmap0
        }
        else
            plwarn( "Insufficient colour slots available in CMAP0 to do text smoothing." );
    }
    else if ( ( FT->want_smooth_text == 1 ) && ( FT->BLENDED_ANTIALIASING == 1 ) ) // If we have a truecolour device, we wont even bother trying to change the palette
    {
        FT->smooth_text = 1;
    }
}

#endif


#ifdef PLD_jpeg

//--------------------------------------------------------------------------
// plD_eop_jpeg()
//
// End of page.
//--------------------------------------------------------------------------

void plD_eop_jpeg( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int im_size  = 0;
    void *im_ptr = NULL;
    size_t nwrite;
    int jpeg_compression;

    if ( pls->family || pls->page == 1 )
    {
        //  Set the compression/quality level for JPEG files
        //  The higher the value, the bigger/better the image is
        //
        if ( ( pls->dev_compression <= 0 ) || ( pls->dev_compression > 99 ) )
            jpeg_compression = 90;
        else
            jpeg_compression = pls->dev_compression;

        // image is written to output file by the driver
        // since if the gd.dll is linked to a different c
        // lib a crash occurs - this fix works also in Linux
        // gdImageJpeg(dev->im_out, pls->OutFile, jpeg_compression);
        im_ptr = gdImageJpegPtr( dev->im_out, &im_size, jpeg_compression );
        if ( im_ptr )
        {
            nwrite = fwrite( im_ptr, sizeof ( char ), im_size, pls->OutFile );
            if ( nwrite != im_size )
                plabort( "gd driver: Error writing png file" );
            gdFree( im_ptr );
        }

        gdImageDestroy( dev->im_out );
        dev->im_out = NULL;
    }
}

#endif

#ifdef PLD_gif

//--------------------------------------------------------------------------
// plD_eop_gif()
//
// End of page.
//--------------------------------------------------------------------------

void plD_eop_gif( PLStream *pls )
{
    png_Dev *dev = (png_Dev *) pls->dev;
    int im_size  = 0;
    void *im_ptr = NULL;
    size_t nwrite;

    if ( pls->family || pls->page == 1 )
    {
        // image is written to output file by the driver
        // since if the gd.dll is linked to a different c
        // lib a crash occurs - this fix works also in Linux
        // gdImageGif(dev->im_out, pls->OutFile);
        im_ptr = gdImageGifPtr( dev->im_out, &im_size );
        if ( im_ptr )
        {
            nwrite = fwrite( im_ptr, sizeof ( char ), im_size, pls->OutFile );
            if ( nwrite != im_size )
                plabort( "gd driver: Error writing png file" );
            gdFree( im_ptr );
        }

        gdImageDestroy( dev->im_out );
        dev->im_out = NULL;
    }
}

#endif


//#endif


#else
int
pldummy_png()
{
    return 0;
}

#endif                          // PNG
