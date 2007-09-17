/* June 2, 2007

   Graphics drivers that are based on the Cairo / Pango Libraries.
    
   Copyright (C) 2007 Hazen Babcock

   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
   	
*/

//---------------------------------------------------------------------
// Header files
//---------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <cairo.h>
#include <pango/pangocairo.h>

// PLplot header files (must occur before driver-dependent includes)

#include "plplotP.h"
#include "drivers.h"

// Driver-dependent includes
#if defined(PLD_xcairo)
#include <cairo-xlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h> 
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#endif
#if defined(PLD_pdfcairo)
#include <cairo-pdf.h>
#endif
#if defined(PLD_pscairo)
#include <cairo-ps.h>
#endif
#if defined(PLD_svgcairo)
#include <cairo-svg.h>
#endif


//---------------------------------------------------------------------
// Constants & global (to this file) variables 
//---------------------------------------------------------------------

#define DPI 72
#define DOWNSCALE 0.1
#define PLCAIRO_DEFAULT_X 720
#define PLCAIRO_DEFAULT_Y 540

#define MAX_STRING_LEN 500
#define MAX_MARKUP_LEN MAX_STRING_LEN * 10

static int text_clipping;
static int text_anti_aliasing;
static int graphics_anti_aliasing;

static DrvOpt cairo_options[] = {{"text_clipping", DRV_INT, &text_clipping, "Use text clipping (text_clipping=0|1)"},
				 {"text_anti_aliasing", DRV_INT, &text_anti_aliasing, "Set desired text anti-aliasing (text_anti_aliasing=0|1|2|3). The numbers are in the same order as the cairo_antialias_t enumeration documented at http://cairographics.org/manual/cairo-cairo-t.html#cairo-antialias-t)"},
				 {"graphics_anti_aliasing", DRV_INT, &graphics_anti_aliasing, "Set desired graphics anti-aliasing (graphics_anti_aliasing=0|1|2|3). The numbers are in the same order as the cairo_antialias_t enumeration documented at http://cairographics.org/manual/cairo-cairo-t.html#cairo-antialias-t"},
                                 {NULL, DRV_INT, NULL, NULL}};

typedef struct {
  cairo_surface_t *cairoSurface;
  cairo_t *cairoContext;
  short text_clipping;
  short text_anti_aliasing;
  short graphics_anti_aliasing;
#if defined(PLD_xcairo)
  short exit_event_loop;
  Display *XDisplay;
  Window XWindow;
#endif
#if defined(PLD_memcairo)
  unsigned char *memory;
#endif
} PLCairo;

char* plD_DEVICE_INFO_cairo = 
#if defined(PLD_xcairo)
  "xcairo:Cairo X Windows Driver:1:cairo:59:xcairo\n"
#endif
#if defined(PLD_pdfcairo)
  "pdfcairo:Cairo PDF Driver:0:cairo:60:pdfcairo\n"
#endif
#if defined(PLD_pscairo)
  "pscairo:Cairo PS Driver:0:cairo:61:pscairo\n"
#endif
#if defined(PLD_svgcairo)
  "svgcairo:Cairo SVG Driver:0:cairo:62:svgcairo\n"
#endif
#if defined(PLD_pngcairo)
  "pngcairo:Cairo PNG Driver:0:cairo:63:pngcairo\n"
#endif
#if defined(PLD_memcairo)
  "memcairo:Cairo Memory Driver:0:cairo:64:memcairo\n"
#endif
;

//---------------------------------------------------------------------
// Font style and weight lookup tables (copied
// from the psttf driver).
//---------------------------------------------------------------------

#define NPANGOLOOKUP 5

const char *defaultFamilyLookup[NPANGOLOOKUP] = {
  "Arial,Bitstream-Vera-Sans,sans",
  "Times-Roman,Bitstream-Vera-Serif,serif",
  "Courier,Bitstream-Vera-Sans-Mono,monospace",
  "Arial,Bitstream-Vera-Sans,sans,serif",
  "Arial,Bitstream-Vera-Sans,sans,serif"
};

const char *envFamilyLookup[NPANGOLOOKUP] = {
  "PLPLOT_FREETYPE_SANS_FAMILY",
  "PLPLOT_FREETYPE_SERIF_FAMILY",
  "PLPLOT_FREETYPE_MONO_FAMILY",
  "PLPLOT_FREETYPE_SCRIPT_FAMILY",
  "PLPLOT_FREETYPE_SYMBOL_FAMILY"
};

char familyLookup[NPANGOLOOKUP][1024];

const char *weightLookup[2] = {
  "normal",
  "bold"
};

const char *styleLookup[3] = {
  "normal",
  "italic",
  "oblique"
};


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is common to all the Cairo Drivers
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// function declarations
//---------------------------------------------------------------------

/* General */

PLCairo *stream_and_font_setup(PLStream *, int);
cairo_status_t write_to_stream(void *, unsigned char *, unsigned int);

/* String processing */

static void proc_str(PLStream *, EscText *);
static char *ucs4_to_pango_markup_format(PLUNICODE *, int, float);
static void open_span_tag(char *, PLUNICODE, float, int);
static void close_span_tag(char *, int);

/* Graphics */

static void set_current_context(PLStream *);
static void poly_line(PLStream *, short *, short *, PLINT);
static void rotate_cairo_surface(PLStream *, float, float, float, float, float, float);

/* PLplot interface functions */

// general
void plD_bop_cairo               (PLStream *);
void plD_eop_cairo               (PLStream *);
void plD_state_cairo             (PLStream *, PLINT);
void plD_esc_cairo               (PLStream *, PLINT, void *);
void plD_tidy_cairo              (PLStream *);
void plD_line_cairo              (PLStream *, short, short, short, short);
void plD_polyline_cairo          (PLStream *, short *, short *, PLINT);

//----------------------------------------------------------------------
// plD_bop_cairo()
//
// Set up for the next page.
//----------------------------------------------------------------------

void plD_bop_cairo(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  // Fill in the window with the background color.
  cairo_rectangle(aStream->cairoContext, 0.0, 0.0, pls->xlength, pls->ylength);
  cairo_set_source_rgb(aStream->cairoContext,
		       (double)pls->cmap0[0].r/255.0,
		       (double)pls->cmap0[0].g/255.0,
		       (double)pls->cmap0[0].b/255.0);
  cairo_fill(aStream->cairoContext);  
}

//---------------------------------------------------------------------
// plD_line_cairo()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//----------------------------------------------------------------------

void plD_line_cairo(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  set_current_context(pls);

  cairo_move_to(aStream->cairoContext, DOWNSCALE * (double) x1a, DOWNSCALE * (double) y1a);
  cairo_line_to(aStream->cairoContext, DOWNSCALE * (double) x2a, DOWNSCALE * (double) y2a);
  cairo_stroke(aStream->cairoContext);
}

//---------------------------------------------------------------------
// plD_polyline_cairo()
//
// Draw a polyline in the current color.
//---------------------------------------------------------------------

void plD_polyline_cairo(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  poly_line(pls, xa, ya, npts);
  cairo_stroke(aStream->cairoContext);
}

//---------------------------------------------------------------------
// plD_eop_cairo()
//
// Generic end of page.
//---------------------------------------------------------------------

void plD_eop_cairo(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;
  cairo_show_page(aStream->cairoContext);
}

//---------------------------------------------------------------------
// plD_tidy_cairo()
//
// General: Close graphics file or otherwise clean up.
//---------------------------------------------------------------------

void plD_tidy_cairo(PLStream *pls)
{
  PLCairo *aStream;
  
  aStream = (PLCairo *)pls->dev;

  // Free the cairo context and surface.
  cairo_destroy(aStream->cairoContext);
  cairo_surface_destroy(aStream->cairoSurface);
}

//---------------------------------------------------------------------
// plD_state_cairo()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//
// Nothing is done here because these attributes are acquired from 
// PLStream for each element that is drawn.
//---------------------------------------------------------------------

void plD_state_cairo(PLStream *pls, PLINT op)
{
}

//---------------------------------------------------------------------
// plD_esc_cairo()
//
// Generic escape function.
//---------------------------------------------------------------------

void plD_esc_cairo(PLStream *pls, PLINT op, void *ptr)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  switch(op)
    {
    case PLESC_FILL:     // filled polygon
      poly_line(pls, pls->dev_x, pls->dev_y, pls->dev_npts);
      cairo_fill(aStream->cairoContext);
      break;
    case PLESC_HAS_TEXT: // render rext
      proc_str(pls, (EscText *) ptr);
      break;
    }
}

//---------------------------------------------------------------------
// proc_str()
//
// Processes strings for display.
//---------------------------------------------------------------------

void proc_str(PLStream *pls, EscText *args)
{
  int i;
  float fontSize;
  int textXExtent, textYExtent;
  char *textWithPangoMarkup;
  PLFLT rotation, shear, cos_rot, sin_rot, cos_shear, sin_shear;
  cairo_matrix_t *cairoTransformMatrix;
  cairo_font_options_t *cairoFontOptions;
  PangoContext *context;
  PangoLayout *layout;
  PangoFontDescription *fontDescription;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  set_current_context(pls);

  // Check that we got unicode, warning message and return if not
  if(args->unicode_array_len == 0){
    printf("Non unicode string passed to a cairo driver, ignoring\n");
    return;
  }
	
  // Check that unicode string isn't longer then the max we allow	
  if(args->unicode_array_len >= MAX_STRING_LEN){
    printf("Sorry, the cairo drivers only handles strings of length < %d\n", MAX_STRING_LEN);
    return;
  }

  // Calculate the font size (in pixels)
  fontSize = pls->chrht * DPI/25.4;

  // Convert the escape characters into the appropriate Pango markup
  textWithPangoMarkup = ucs4_to_pango_markup_format(args->unicode_array, args->unicode_array_len, fontSize);

  // Create the Pango text layout so we can figure out how big it is
  layout = pango_cairo_create_layout(aStream->cairoContext);
  pango_layout_set_markup(layout, textWithPangoMarkup, -1);
  pango_layout_get_pixel_size(layout, &textXExtent, &textYExtent);

  // Set font aliasing
  context = pango_layout_get_context(layout);
  cairoFontOptions = cairo_font_options_create();
  cairo_font_options_set_antialias(cairoFontOptions, aStream->text_anti_aliasing);
  pango_cairo_context_set_font_options(context, cairoFontOptions);
  pango_layout_context_changed(layout);
  cairo_font_options_destroy(cairoFontOptions);

  // Save current transform matrix & clipping region
  cairo_save(aStream->cairoContext);

  // Set up the clipping region if we are doing text clipping
  if(aStream->text_clipping){
    cairo_rectangle(aStream->cairoContext, DOWNSCALE * pls->clpxmi, DOWNSCALE * pls->clpymi, DOWNSCALE * (pls->clpxma - pls->clpxmi), DOWNSCALE * (pls->clpyma - pls->clpymi));
    cairo_clip(aStream->cairoContext);
  }

  // Move to the string reference point
  cairo_move_to(aStream->cairoContext, DOWNSCALE * (double) args->x, DOWNSCALE * (double) args->y);

  // Invert the coordinate system so that the text is drawn right side up
  cairoTransformMatrix = (cairo_matrix_t *) malloc (sizeof(cairo_matrix_t));
  cairo_matrix_init(cairoTransformMatrix, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0);
  cairo_transform(aStream->cairoContext, cairoTransformMatrix);

  // Extract rotation angle and shear from the PLplot tranformation matrix.
  // Compute sines and cosines of the angles as an optimization.
  plRotationShear(args->xform, &rotation, &shear);
  rotation -= pls->diorot * 3.14159 / 2.0;
  cos_rot = cos(rotation);
  sin_rot = sin(rotation);
  cos_shear = cos(shear);
  sin_shear = sin(shear);

  // Apply the transform matrix
  cairo_matrix_init(cairoTransformMatrix,             
		    cos_rot,
		    -sin_rot,
		    cos_rot * sin_shear + sin_rot * cos_shear,
		    -sin_rot * sin_shear + cos_rot * cos_shear,
		    0,0);
  cairo_transform(aStream->cairoContext, cairoTransformMatrix);
  free(cairoTransformMatrix);
  
  // Move to the text starting point
  cairo_rel_move_to(aStream->cairoContext, 
		    (double)(-1.0 * args->just * (double)textXExtent), 
		    (double)(-0.5 * textYExtent));

  // Render the text
  pango_cairo_show_layout(aStream->cairoContext, layout);

  // Restore the transform matrix to its state prior to the text transform.
  cairo_restore(aStream->cairoContext);
  
  // Free the layout object and the markup string.
  g_object_unref(layout);
  free(textWithPangoMarkup);
}

//---------------------------------------------------------------------
// ucs4_to_pango_markup_format()
//
// Converts the plplot string (in ucs4) to a utf8 string that includes
// pango markup.
//
// http://developer.gnome.org/doc/API/2.0/pango/PangoMarkupFormat.html
//---------------------------------------------------------------------

char *ucs4_to_pango_markup_format(PLUNICODE *ucs4, int ucs4Len, float fontSize)
{
  char plplotEsc;
  int i;
  int upDown = 0;
  PLUNICODE fci;
  char utf8[5];
  char *pangoMarkupString;

  // Will this be big enough? We might have lots of markup.
  pangoMarkupString = (char *) malloc (sizeof(char) * MAX_MARKUP_LEN);
  for(i = 0; i < MAX_MARKUP_LEN; i++){
    pangoMarkupString[i] = 0;
  }

  // Get PLplot escape character
  plgesc(&plplotEsc);

  // Get the curent font and open the first span tag
  plgfci(&fci);
  open_span_tag(pangoMarkupString, fci, fontSize, 0);

  // Parse the string to generate the tags
  i = 0;
  while (i < ucs4Len){
    // Try to avoid going off the end of the string
    if(strlen(pangoMarkupString) > (MAX_MARKUP_LEN - 50)){
      continue;
    }
    if (ucs4[i] < PL_FCI_MARK){	// not a font change
      if (ucs4[i] != (PLUNICODE)plplotEsc) {  // a character to display
	// we have to handle "<", ">" and "&" separately since they throw off the XML
	switch(ucs4[i])
	  {
	  case 38:
	    strcat(pangoMarkupString, "&#38;");
	    break;
	  case 60:
	    strcat(pangoMarkupString, "&#60;");
	    break;
	  case 62:
	    strcat(pangoMarkupString, "&#62;");
	    break;
	  default:
	    ucs4_to_utf8(ucs4[i],utf8);
	    strcat(pangoMarkupString, utf8);
	    break;
	  }
	i++;
	continue;
      }
      i++;
      if (ucs4[i] == (PLUNICODE)plplotEsc){   // a escape character to display
	ucs4_to_utf8(ucs4[i],utf8);
	strcat(pangoMarkupString, utf8);
	i++;
	continue;
      }
      else {
	if(ucs4[i] == (PLUNICODE)'u'){	// Superscript
	  if(upDown < 0){
	    strcat(pangoMarkupString, "</sub>");
	  } else {
	    strcat(pangoMarkupString, "<sup>");
	  }
	  upDown++;
	}
	if(ucs4[i] == (PLUNICODE)'d'){	// Subscript
	  if(upDown > 0){
	    strcat(pangoMarkupString, "</sup>");
	  } else {
	    strcat(pangoMarkupString, "<sub>");
	  }
	  upDown--;
	}
	i++;
      }
    }
    else { // a font change
      close_span_tag(pangoMarkupString, upDown);
      open_span_tag(pangoMarkupString, ucs4[i], fontSize, upDown);
      i++;
    }
  }

  // Close the last span tag.
  close_span_tag(pangoMarkupString, upDown);

  //printf("%s\n", pangoMarkupString);

  return pangoMarkupString;
}

//---------------------------------------------------------------------
// open_span_tag
//
// 1. Opens a span tag with the appropriate font description given the
//    current fci.
// 2. Add the appropriate number of <sub> or <sup> tags to bring us
//    back to our current sub/super-script level.
//---------------------------------------------------------------------

void open_span_tag(char *pangoMarkupString, PLUNICODE fci, float fontSize, int upDown)
{
  int i;
  unsigned char fontFamily, fontStyle, fontWeight;
  char openTag[200];

  // Generate the font info for the open tag & concatenate this
  // onto the markup string.
  plP_fci2hex(fci, &fontFamily, PL_FCI_FAMILY);
  plP_fci2hex(fci, &fontStyle, PL_FCI_STYLE);
  plP_fci2hex(fci, &fontWeight, PL_FCI_WEIGHT);
  sprintf(openTag, "<span font_desc=\"%s %.2f\" ", familyLookup[fontFamily], fontSize);
  strcat(pangoMarkupString, openTag);

  sprintf(openTag, "style=\"%s\" ", styleLookup[fontStyle]);
  strcat(pangoMarkupString, openTag);

  sprintf(openTag, "weight=\"%s\">", weightLookup[fontWeight]);
  strcat(pangoMarkupString, openTag);

  // Move to the right sub/super-script level
  if(upDown > 0){
    while(upDown > 0){
      strcat(pangoMarkupString, "<sup>");
      upDown--;
    }
  }
  if(upDown < 0){
    while(upDown < 0){
      strcat(pangoMarkupString, "<sub>");
      upDown++;
    }
  }
}

//---------------------------------------------------------------------
// close_span_tag
//
// Close a span tag & brings us down to zero sub/super-script level.
//---------------------------------------------------------------------

void close_span_tag(char *pangoMarkupString, int upDown)
{
  if(upDown > 0){
    while(upDown > 0){
      strcat(pangoMarkupString, "</sup>");
      upDown--;
    }
  }
  if(upDown < 0){
    while(upDown < 0){
      strcat(pangoMarkupString, "</sub>");
      upDown++;
    }
  }

  strcat(pangoMarkupString, "</span>");
}

//---------------------------------------------------------------------
// write_to_stream()
//
// Writes data to a open file stream. This function is passed to the
// Cairo file IO devices.
//---------------------------------------------------------------------

cairo_status_t write_to_stream(void *filePointer, unsigned char *data, unsigned int length)
{
  int bytes_written;

  bytes_written = fwrite(data, 1, length, (FILE*) filePointer);
  if(bytes_written == length){
    return CAIRO_STATUS_SUCCESS;
  }
  else{
    return CAIRO_STATUS_WRITE_ERROR;
  }
}

//---------------------------------------------------------------------
// stream_and_font_setup()
//
// Initializes the PLStream structure for the cairo devices.
// Initializes the font lookup table.
// Checks for cairo specific user options.
// Returns a new PLCairo structure.
//---------------------------------------------------------------------

PLCairo *stream_and_font_setup(PLStream *pls, int interactive)
{
  int i;
  char *a;
  PLCairo *aStream;

  // Stream setup
  pls->termin = interactive; /* Interactive device */
  pls->dev_flush = 1;        /* Handles flushes */
  pls->color = 1;            /* Supports color */
  pls->dev_text = 1;         /* Handles text */
  pls->dev_unicode = 1;      /* Wants unicode text */
  pls->page = 0;
  pls->dev_fill0 = 1;        /* Supports hardware solid fills */
  pls->plbuf_write = 1;      /* Activate plot buffer */
  
  plP_setpxl(DPI/25.4/DOWNSCALE, DPI/25.4/DOWNSCALE);
  
  if (pls->xlength <= 0 || pls->ylength <= 0){
    pls->xlength = PLCAIRO_DEFAULT_X;
    pls->ylength = PLCAIRO_DEFAULT_Y;
  }
  plP_setphy((PLINT) 0, (PLINT) pls->xlength / DOWNSCALE, (PLINT) 0, (PLINT) pls->ylength / DOWNSCALE);

  // Initialize font table with either enviroment variables or defaults.
  // This was copied from the psttf driver.
  for(i=0;i<NPANGOLOOKUP;i++){
    if((a = getenv(envFamilyLookup[i])) != NULL){
      strcpy(familyLookup[i],a);
    }
    else {
      strcpy(familyLookup[i],defaultFamilyLookup[i]);
    }
  }

  // Allocate a cairo stream structure
  aStream = malloc(sizeof(PLCairo));

  // Set text clipping off as this makes the driver pretty slow
  aStream->text_clipping = 0;
  text_clipping = 0;
  text_anti_aliasing = 0;     // use 'default' text aliasing by default
  graphics_anti_aliasing = 0; // use 'default' graphics aliasing by default

  // Check for cairo specific options
  plParseDrvOpts(cairo_options);

  // Turn on text clipping if the user desires this
  if(text_clipping){
    aStream->text_clipping = 1;
  }

  // Record users desired text and graphics aliasing
  aStream->text_anti_aliasing = text_anti_aliasing;
  aStream->graphics_anti_aliasing = graphics_anti_aliasing;

  return aStream;
}

//---------------------------------------------------------------------
// set_current_context()
//
// Updates the cairo graphics context with the current values in
// PLStream.
//---------------------------------------------------------------------

void set_current_context(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;
  cairo_set_source_rgb(aStream->cairoContext,
		       (double)pls->curcolor.r/255.0, 
  		       (double)pls->curcolor.g/255.0,
  		       (double)pls->curcolor.b/255.0);
  // In Cairo, zero width lines are not hairlines, they are completely invisible.
  if(pls->width < 1){
    cairo_set_line_width(aStream->cairoContext, 1.0);
  } else{
    cairo_set_line_width(aStream->cairoContext, (double) pls->width);
  }
}

//---------------------------------------------------------------------
// poly_line()
//
// Draws a multi-segmented line. It is then up to the calling function
// to decide whether to just draw the line, or fill in the area
// enclosed by the line.
//---------------------------------------------------------------------

void poly_line(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  int i;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  set_current_context(pls);
  
  cairo_move_to(aStream->cairoContext, DOWNSCALE * (double) xa[0], DOWNSCALE * (double) ya[0]);
  for(i=1;i<npts;i++){
    cairo_line_to(aStream->cairoContext, DOWNSCALE * (double) xa[i], DOWNSCALE * (double) ya[i]);
  }
}

//---------------------------------------------------------------------
// rotate_cairo_surface()
//
// Rotates the cairo surface to the appropriate orientation.
//---------------------------------------------------------------------

void rotate_cairo_surface(PLStream *pls, float x11, float x12, float x21, float x22, float x0, float y0)
{
  cairo_matrix_t *matrix;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  matrix = (cairo_matrix_t *) malloc (sizeof(cairo_matrix_t));
  cairo_matrix_init(matrix, x11, x12, x21, x22, x0, y0);
  cairo_transform(aStream->cairoContext, matrix);
  free(matrix);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the xcairo driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_xcairo)

static int XScreen;
static Window rootWindow;

void plD_dispatch_init_xcairo    (PLDispatchTable *pdt);
void plD_init_xcairo             (PLStream *);
void plD_eop_xcairo              (PLStream *);
void plD_tidy_xcairo             (PLStream *);
void plD_esc_xcairo              (PLStream *, PLINT, void *);
static void xcairo_get_cursor    (PLStream *, PLGraphicsIn *);

//---------------------------------------------------------------------
// plD_dispatch_init_xcairo()
//
// xcairo dispatch table initialization.
//----------------------------------------------------------------------

void plD_dispatch_init_xcairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo X Windows Driver";
   pdt->pl_DevName  = "xcairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 59;
   pdt->pl_init     = (plD_init_fp)     plD_init_xcairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_xcairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_cairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_xcairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_xcairo;
}

//---------------------------------------------------------------------
// plD_init_xcairo()
//
// Initialize Cairo X Windows device.
//----------------------------------------------------------------------

void plD_init_xcairo(PLStream *pls)
{
  char plotTitle[40];
  Visual *defaultVisual;
  XGCValues values;
  PLCairo *aStream;

  // Setup the PLStream and the font lookup table
  aStream = stream_and_font_setup(pls, 1);

  // X Windows setup
  aStream->XDisplay = NULL;
  aStream->XDisplay = XOpenDisplay(NULL);
  if(aStream->XDisplay == NULL){
    printf("Failed to open X Windows display\n");
    // some sort of error here
  }
  XScreen = DefaultScreen(aStream->XDisplay);
  rootWindow = RootWindow(aStream->XDisplay, XScreen);
  aStream->exit_event_loop = 0;

  // Initialize plot title
  sprintf(plotTitle, "PLplot");
    
  // Create a X Window.
  aStream->XWindow = XCreateSimpleWindow(aStream->XDisplay, rootWindow, 0, 0, pls->xlength, pls->ylength, 
					    1, BlackPixel(aStream->XDisplay, XScreen), BlackPixel(aStream->XDisplay, XScreen));
  XStoreName(aStream->XDisplay, aStream->XWindow, plotTitle);
  XSelectInput(aStream->XDisplay, aStream->XWindow, NoEventMask);
  XMapWindow(aStream->XDisplay, aStream->XWindow);

  // Create an cairo surface & context that are associated with the X window.
  defaultVisual = DefaultVisual(aStream->XDisplay, 0);
  aStream->cairoSurface = cairo_xlib_surface_create(aStream->XDisplay, aStream->XWindow, defaultVisual, pls->xlength, pls->ylength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Save the pointer to the structure in the PLplot stream
  pls->dev = aStream;

  // Invert the surface so that the graphs are drawn right side up.
  rotate_cairo_surface(pls, 1.0, 0.0, 0.0, -1.0, 0.0, pls->ylength);

  // Set graphics aliasing
  cairo_set_antialias(aStream->cairoContext, aStream->graphics_anti_aliasing);
}

//---------------------------------------------------------------------
// plD_eop_xcairo()
//
// X Windows specific end of page.
//---------------------------------------------------------------------

void plD_eop_xcairo(PLStream *pls)
{
  int number_chars;
  long event_mask;
  char event_string[10];
  KeySym keysym;
  XComposeStatus cs;
  XEvent event;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  XFlush(aStream->XDisplay);

  // Loop, handling selected events, till the user elects to close the plot.
  printf("Click on the plot and key <Return> to exit.\n");
  event_mask = ButtonPressMask | KeyPressMask | ExposureMask;
  XSelectInput(aStream->XDisplay, aStream->XWindow, event_mask);
  while(!aStream->exit_event_loop){
    XWindowEvent(aStream->XDisplay, aStream->XWindow, event_mask, &event);
    switch(event.type){
    case KeyPress:
      number_chars = XLookupString((XKeyEvent *)&event, event_string, 10, &keysym, &cs);
      event_string[number_chars] = '\0';
      if(keysym == XK_Return){
	aStream->exit_event_loop = 1;
      }
      break;
    case Expose:
      plD_bop_cairo(pls);
      plRemakePlot(pls);
      XFlush(aStream->XDisplay);
      break;
    }
  }
  aStream->exit_event_loop = 0;

  //  printf("Key <Return> to finish\n");
  //getc(stdin);
}

//---------------------------------------------------------------------
// plD_tidy_xcairo()
//
// X Windows: close graphics file or otherwise clean up.
//---------------------------------------------------------------------

void plD_tidy_xcairo(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  plD_tidy_cairo(pls);

  // Close the window and the display.
  XFlush(aStream->XDisplay);
  XDestroyWindow(aStream->XDisplay, aStream->XWindow);

  XCloseDisplay(aStream->XDisplay);
}

//---------------------------------------------------------------------
// plD_esc_xcairo()
//
// Escape function, specialized for the xcairo driver
//---------------------------------------------------------------------

void plD_esc_xcairo(PLStream *pls, PLINT op, void *ptr)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  switch(op)
    {
    case PLESC_FILL:     // filled polygon
      poly_line(pls, pls->dev_x, pls->dev_y, pls->dev_npts);
      cairo_fill(aStream->cairoContext);
      break;
    case PLESC_HAS_TEXT: // render rext
      proc_str(pls, (EscText *) ptr);
      break;
    case PLESC_FLUSH:    // forced update of the window
      XFlush(aStream->XDisplay);
      break;
    case PLESC_GETC:     // get cursor position
      XFlush(aStream->XDisplay);
      xcairo_get_cursor(pls, (PLGraphicsIn*)ptr);
      break;
    }
}

//---------------------------------------------------------------------
// xcairo_get_cursor()
//
// X Windows: returns the location of the next mouse click or key press.
//---------------------------------------------------------------------

void xcairo_get_cursor(PLStream *pls, PLGraphicsIn *gin)
{
  int number_chars;
  KeySym keysym;
  XComposeStatus cs;
  XEvent event;
  XButtonEvent *xButtonEvent;
  Cursor xHairCursor;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  // Initialize PLplot mouse event structure
  plGinInit(gin);

  // Create cross hair cursor & switch to using it
  xHairCursor = XCreateFontCursor(aStream->XDisplay, XC_crosshair);
  XDefineCursor(aStream->XDisplay, aStream->XWindow, xHairCursor);

  // Get the next mouse button release or key press event
  XSelectInput(aStream->XDisplay, aStream->XWindow, ButtonReleaseMask | KeyPressMask);
  XMaskEvent(aStream->XDisplay, ButtonReleaseMask | KeyPressMask, &event);
  XSelectInput(aStream->XDisplay, aStream->XWindow, NoEventMask);

  // Get key pressed (if any)
  if(event.type == KeyPress){
    number_chars = XLookupString((XKeyEvent *)&event, gin->string, 10, &keysym, &cs);
    gin->string[number_chars] = '\0';
    switch (keysym){
    case XK_BackSpace:
    case XK_Tab:
    case XK_Linefeed:
    case XK_Return:
    case XK_Escape:
    case XK_Delete:
      gin->keysym = 0xFF & keysym;
      break;
    default:
      gin->keysym = keysym;
    }
  }
  else {
    gin->string[0] = '\0';
    gin->keysym = 0x20;
  }

  // Update PLplot's mouse event structure
  xButtonEvent = (XButtonEvent *)&event;
  gin->state = xButtonEvent->state;
  gin->button = xButtonEvent->button;
  gin->pX = event.xbutton.x;
  gin->pY = event.xbutton.y;
  gin->dX = (PLFLT)event.xbutton.x/((PLFLT)(pls->xlength));
  gin->dY = (PLFLT)event.xbutton.y/((PLFLT)(pls->ylength));

  // Switch back to normal cursor
  XUndefineCursor(aStream->XDisplay, aStream->XWindow);
  XFlush(aStream->XDisplay);
}

#endif


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the cairo PDF driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_pdfcairo)

void plD_dispatch_init_pdfcairo  (PLDispatchTable *pdt);
void plD_init_pdfcairo           (PLStream *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

// pdfcairo
void plD_dispatch_init_pdfcairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo PDF Driver";
   pdt->pl_DevName  = "pdfcairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 60;
   pdt->pl_init     = (plD_init_fp)     plD_init_pdfcairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_cairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_cairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_cairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_cairo;
}

//---------------------------------------------------------------------
// plD_init_pdfcairo()
//
// Initialize Cairo PDF device
//----------------------------------------------------------------------

void plD_init_pdfcairo(PLStream *pls)
{
  PLCairo *aStream;

  // Setup the PLStream and the font lookup table
  aStream = stream_and_font_setup(pls, 0);

  // Prompt for a file name if not already set.
  plOpenFile(pls);

  // Create an cairo surface & context for PDF file.
  aStream->cairoSurface = cairo_pdf_surface_create_for_stream((cairo_write_func_t)write_to_stream, pls->OutFile, (double)pls->xlength, (double)pls->ylength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Save the pointer to the structure in the PLplot stream
  pls->dev = aStream;

  // Invert the surface so that the graphs are drawn right side up.
  rotate_cairo_surface(pls, 1.0, 0.0, 0.0, -1.0, 0.0, pls->ylength);

  // Set graphics aliasing
  cairo_set_antialias(aStream->cairoContext, aStream->graphics_anti_aliasing);
}

#endif


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the cairo PS driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_pscairo)

void plD_dispatch_init_pscairo  (PLDispatchTable *pdt);
void plD_init_pscairo           (PLStream *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

// pscairo
void plD_dispatch_init_pscairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo PS Driver";
   pdt->pl_DevName  = "pscairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 61;
   pdt->pl_init     = (plD_init_fp)     plD_init_pscairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_cairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_cairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_cairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_cairo;
}

//---------------------------------------------------------------------
// plD_init_pscairo()
//
// Initialize Cairo PS device
//----------------------------------------------------------------------

void plD_init_pscairo(PLStream *pls)
{
  PLCairo *aStream;

  // Setup the PLStream and the font lookup table
  aStream = stream_and_font_setup(pls, 0);

  // Prompt for a file name if not already set.
  plOpenFile(pls);

  // Create an cairo surface & context for PS file.
  aStream->cairoSurface = cairo_ps_surface_create_for_stream((cairo_write_func_t)write_to_stream, pls->OutFile, (double)pls->ylength, (double)pls->xlength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Save the pointer to the structure in the PLplot stream
  pls->dev = aStream;

  // Handle portrait or landscape
  if(pls->portrait){
    plsdiori(1);
    pls->freeaspect = 1;
  }
  rotate_cairo_surface(pls, 0.0, -1.0, -1.0, 0.0, pls->ylength, pls->xlength);
}


#endif


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the cairo SVG driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_svgcairo)

void plD_dispatch_init_svgcairo  (PLDispatchTable *pdt);
void plD_init_svgcairo           (PLStream *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

// svgcairo
void plD_dispatch_init_svgcairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo SVG Driver";
   pdt->pl_DevName  = "svgcairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 62;
   pdt->pl_init     = (plD_init_fp)     plD_init_svgcairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_cairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_cairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_cairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_cairo;
}

//---------------------------------------------------------------------
// plD_init_svgcairo()
//
// Initialize Cairo SVG device
//----------------------------------------------------------------------

void plD_init_svgcairo(PLStream *pls)
{
  PLCairo *aStream;

  // Setup the PLStream and the font lookup table
  aStream = stream_and_font_setup(pls, 0);

  // Prompt for a file name if not already set.
  plOpenFile(pls);

  // Create an cairo surface & context for SVG file.
  aStream->cairoSurface = cairo_svg_surface_create_for_stream((cairo_write_func_t)write_to_stream, pls->OutFile, (double)pls->xlength, (double)pls->ylength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Save the pointer to the structure in the PLplot stream
  pls->dev = aStream;

  // Invert the surface so that the graphs are drawn right side up.
  rotate_cairo_surface(pls, 1.0, 0.0, 0.0, -1.0, 0.0, pls->ylength);

  // Set graphics aliasing
  cairo_set_antialias(aStream->cairoContext, aStream->graphics_anti_aliasing);
}

#endif


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the cairo PNG driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_pngcairo)

void plD_dispatch_init_pngcairo  (PLDispatchTable *pdt);
void plD_init_pngcairo           (PLStream *);
void plD_bop_pngcairo            (PLStream *);
void plD_eop_pngcairo            (PLStream *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

// pngcairo
void plD_dispatch_init_pngcairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo PNG Driver";
   pdt->pl_DevName  = "pngcairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 63;
   pdt->pl_init     = (plD_init_fp)     plD_init_pngcairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_pngcairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_pngcairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_cairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_cairo;
}

//---------------------------------------------------------------------
// plD_init_pngcairo()
//
// Initialize Cairo PNG device
//----------------------------------------------------------------------

void plD_init_pngcairo(PLStream *pls)
{
  PLCairo *aStream;

  // Setup the PLStream and the font lookup table and allocate a cairo 
  // stream structure.
  //
  // NOTE: The check below is necessary since, in family mode, this function
  //   will be called multiple times. While you might think that it is
  //   sufficient to update what *should* be the only pointer to the contents
  //   of pls->dev, i.e. the pointer pls->dev itself, it appears that
  //   something else somewhere else is also pointing to pls->dev. If you
  //   change what pls->dev points to then you will get a "bus error", from
  //   which I infer the existence of said bad stale pointer.
  //
  if(pls->dev == NULL){
    aStream = stream_and_font_setup(pls, 0);
  } else {
    stream_and_font_setup(pls, 0);
    aStream = pls->dev;
  }

  // Initialize family file info
  plFamInit(pls);

  // Prompt for a file name if not already set.
  plOpenFile(pls);

  // Save the pointer to the structure in the PLplot stream
  pls->dev = aStream;

  // Create a new cairo surface & context for PNG file.
  aStream->cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, (double)pls->xlength, (double)pls->ylength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Invert the surface so that the graphs are drawn right side up.
  rotate_cairo_surface(pls, 1.0, 0.0, 0.0, -1.0, 0.0, pls->ylength);

  // Set graphics aliasing
  cairo_set_antialias(aStream->cairoContext, aStream->graphics_anti_aliasing);
}

//----------------------------------------------------------------------
// plD_bop_pngcairo()
//
// PNG: Set up for the next page.
//----------------------------------------------------------------------

void plD_bop_pngcairo(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;

  // Plot familying stuff. Not really understood, just copying gd.c
  plGetFam(pls);
  pls->famadv = 1;
  pls->page++;

  // Fill in the window with the background color.
  cairo_rectangle(aStream->cairoContext, 0.0, 0.0, pls->xlength, pls->ylength);
  cairo_set_source_rgb(aStream->cairoContext,
		       (double)pls->cmap0[0].r/255.0,
		       (double)pls->cmap0[0].g/255.0,
		       (double)pls->cmap0[0].b/255.0);
  cairo_fill(aStream->cairoContext);
}

//---------------------------------------------------------------------
// plD_eop_pngcairo()
//
// PNG: End of page.
//---------------------------------------------------------------------

void plD_eop_pngcairo(PLStream *pls)
{
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;
  cairo_surface_write_to_png_stream(aStream->cairoSurface, (cairo_write_func_t)write_to_stream, pls->OutFile);
}

#endif


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// That which is specific to the cairo memory driver.
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined(PLD_memcairo)

void plD_dispatch_init_memcairo  (PLDispatchTable *pdt);
void plD_init_memcairo           (PLStream *);
void plD_eop_memcairo            (PLStream *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

// memcairo
void plD_dispatch_init_memcairo(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Cairo memory driver";
   pdt->pl_DevName  = "memcairo";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 64;
   pdt->pl_init     = (plD_init_fp)     plD_init_memcairo;
   pdt->pl_line     = (plD_line_fp)     plD_line_cairo;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_cairo;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_memcairo;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_cairo;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_cairo;
   pdt->pl_state    = (plD_state_fp)    plD_state_cairo;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_cairo;
}

//---------------------------------------------------------------------
// plD_init_memcairo()
//
// Initialize Cairo memory device
//----------------------------------------------------------------------

void plD_init_memcairo(PLStream *pls)
{
  PLCairo *aStream;

  // Set the plot size to the memory buffer size, on the off chance
  // that they are different.
  pls->xlength = pls->phyxma;
  pls->ylength = pls->phyyma;
  
  // Setup the PLStream and the font lookup table
  aStream = stream_and_font_setup(pls, 0);

  // Check that user supplied us with some memory to draw in
  if(pls->dev == NULL){
    plexit("Must call plsmem first to set user plotting area!");
  }
  
  // Save a pointer to the memory.
  aStream->memory = pls->dev;

  // Create a cairo surface & context.
  aStream->cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, (double)pls->xlength, (double)pls->ylength);
  aStream->cairoContext = cairo_create(aStream->cairoSurface);

  // Save the pointer to the structure in the PLplot stream.
  // Note that this wipes out the direct pointer to the memory buffer.
  pls->dev = aStream;

  // Invert the surface so that the graphs are drawn right side up.
  rotate_cairo_surface(pls, 1.0, 0.0, 0.0, -1.0, 0.0, pls->ylength);

  // Set graphics aliasing
  cairo_set_antialias(aStream->cairoContext, aStream->graphics_anti_aliasing);
}
      
//---------------------------------------------------------------------
// plD_eop_memcairo()
//
// Memory device specific end of page. This copies the contents
// of the cairo surface into the user supplied memory buffer.
//---------------------------------------------------------------------

void plD_eop_memcairo(PLStream *pls)
{
  int i;
  unsigned char *memory;
  unsigned char *cairo_surface_data;
  PLCairo *aStream;

  aStream = (PLCairo *)pls->dev;
  memory = aStream->memory;
  cairo_surface_data = cairo_image_surface_get_data(aStream->cairoSurface);

  for(i = 0;i < (pls->phyxma * pls->phyyma); i++){
    memory[0] = cairo_surface_data[1];
    memory[1] = cairo_surface_data[2];
    memory[2] = cairo_surface_data[3];
    memory += 3;
    cairo_surface_data += 4;
  }
}
                                       
#endif
