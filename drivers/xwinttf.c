/* March 13, 2007

   PLplot driver for X Windows that uses the Cairo graphics library for
   rendering graphics and Pango for rendering text.
 
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

//---------------------------------------------
// Header files, defines and local variables
//---------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <pango/pangocairo.h>

/* PLplot header files */

#include "plplotP.h"
#include "drivers.h"

/* constants */

#define MAXPAGES 50
#define DPI 72
#define XWINTTF_DEFAULT_X 720
#define XWINTTF_DEFAULT_Y 540
#define MAX_STRING_LEN 500
#define MAX_MARKUP_LEN MAX_STRING_LEN * 10

/* local variables */

char* plD_DEVICE_INFO_xwinttf = "xwinttf:TrueType X Windows Driver:1:xwinttf:59:xwinttf";

static int currentPage = -1;
static int XScreen;
static Display *XDisplay = NULL;
static Window rootWindow ;
static Window XWindow[MAXPAGES];

static int windowXSize = XWINTTF_DEFAULT_X;
static int windowYSize = XWINTTF_DEFAULT_Y;

static cairo_surface_t *cairoSurface[MAXPAGES];
static cairo_t *cairoContext[MAXPAGES];

//-----------------------------------------------
// Font style and weight lookup tables (copied
// from the psttf driver).
//-----------------------------------------------

#define NPANGOLOOKUP 5

const char *defaultFamilyLookup[NPANGOLOOKUP] = {
  "Sans",
  "Serif",
  "Monospace",
  "Sans",
  "Sans"
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

//-----------------------------------------------
// function declarations
//-----------------------------------------------

/* String processing */

static void proc_str(PLStream *, EscText *);
static char *ucs4_to_pango_markup_format(PLUNICODE *, int, float);
static void open_span_tag(char *, PLUNICODE, float, int);
static void close_span_tag(char *, int);

/* Graphics */

static void get_cursor(PLStream *, PLGraphicsIn *);
static void set_current_context(PLStream *);
static void poly_line(PLStream *, short *, short *, PLINT);

/* PLplot interface functions */

void plD_dispatch_init_xwinttf      (PLDispatchTable *pdt);
void plD_init_xwinttf               (PLStream *);
void plD_line_xwinttf               (PLStream *, short, short, short, short);
void plD_polyline_xwinttf   	    (PLStream *, short *, short *, PLINT);
void plD_eop_xwinttf                (PLStream *);
void plD_bop_xwinttf                (PLStream *);
void plD_tidy_xwinttf               (PLStream *);
void plD_state_xwinttf              (PLStream *, PLINT);
void plD_esc_xwinttf                (PLStream *, PLINT, void *);

//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

void plD_dispatch_init_xwinttf(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "TrueType X Windows Driver";
   pdt->pl_DevName  = "xwinttf";
#endif
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 59;
   pdt->pl_init     = (plD_init_fp)     plD_init_xwinttf;
   pdt->pl_line     = (plD_line_fp)     plD_line_xwinttf;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_xwinttf;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_xwinttf;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_xwinttf;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_xwinttf;
   pdt->pl_state    = (plD_state_fp)    plD_state_xwinttf;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_xwinttf;
}

//---------------------------------------------------------------------
// xwinttf_init()
//
// Initialize device
//----------------------------------------------------------------------

void plD_init_xwinttf(PLStream *pls)
{
  int i;
  char *a;

  pls->termin = 1;		/* Interactive device */
  pls->dev_flush = 1;		/* Handles flushes */
  pls->color = 1;		/* Supports color */
  pls->dev_text = 1;		/* Handles text */
  pls->dev_unicode = 1; 	/* Wants unicode text */
  pls->page = 0;
  pls->dev_fill0 = 1;		/* Supports hardware solid fills */
  
  plP_setpxl(DPI/25.4, DPI/25.4);
  
  if (pls->xlength <= 0 || pls->ylength <= 0){
    plP_setphy((PLINT) 0, (PLINT) XWINTTF_DEFAULT_X, (PLINT) 0, XWINTTF_DEFAULT_Y);
  } else {
    windowXSize = pls->xlength;
    windowYSize = pls->ylength;
    plP_setphy((PLINT) 0, (PLINT) pls->xlength, (PLINT) 0, (PLINT) pls->ylength);
  }

  XDisplay = XOpenDisplay(NULL);
  if(XDisplay == NULL){
    printf("Failed to open X Windows display\n");
    // some sort of error here
  }
  XScreen = DefaultScreen(XDisplay);
  rootWindow = RootWindow(XDisplay, XScreen);

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

  // Set all cairo pointers to NULL
  for(i=0;i<MAXPAGES;i++){
    cairoSurface[i] = NULL;
    cairoContext[i] = NULL;
  }
}

//----------------------------------------------------------------------
// xwinttf_bop()
//
// Set up for the next page.
//----------------------------------------------------------------------

void plD_bop_xwinttf(PLStream *pls)
{
  char plotTitle[40];
  cairo_matrix_t *flipVertical;
  Visual *defaultVisual;
  XGCValues values;

  // Increment the page number.
  currentPage++;
  if(currentPage > MAXPAGES){
    currentPage = 0;
  }
  pls->page++;

  // Initialize plot title
  sprintf(plotTitle, "PLplot (%d)", currentPage);

  // Create a X Window.
  XWindow[currentPage] = XCreateSimpleWindow(XDisplay, rootWindow, 0, 0, windowXSize, windowYSize, 
					     1, BlackPixel(XDisplay, XScreen), BlackPixel(XDisplay, XScreen));
  XStoreName(XDisplay, XWindow[currentPage], plotTitle);
  XSelectInput(XDisplay, XWindow[currentPage], NoEventMask);
  XMapWindow(XDisplay, XWindow[currentPage]);

  // Create an cairo surface & context that are associated with the window.
  defaultVisual = DefaultVisual(XDisplay, 0);
  cairoSurface[currentPage] = cairo_xlib_surface_create(XDisplay, XWindow[currentPage], defaultVisual, windowXSize, windowYSize);
  cairoContext[currentPage] = cairo_create(cairoSurface[currentPage]);

  // Fill in the window with the background color.
  cairo_rectangle(cairoContext[currentPage], 0.0, 0.0, windowXSize, windowYSize);
  cairo_set_source_rgb(cairoContext[currentPage],
		       (double)pls->cmap0[0].r/255.0,
		       (double)pls->cmap0[0].g/255.0,
		       (double)pls->cmap0[0].b/255.0);
  cairo_fill(cairoContext[currentPage]);

  // Invert the coordinate system so the graphs are drawn right side up.
  flipVertical = (cairo_matrix_t *) malloc (sizeof(cairo_matrix_t));
  cairo_matrix_init(flipVertical, 1.0, 0.0, 0.0, -1.0, 0.0, windowYSize);
  cairo_set_matrix(cairoContext[currentPage], flipVertical);
  free(flipVertical);
}

//---------------------------------------------------------------------
// xwinttf_line()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//----------------------------------------------------------------------

void plD_line_xwinttf(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  set_current_context(pls);

  cairo_move_to(cairoContext[currentPage], (double) x1a, (double) y1a);
  cairo_line_to(cairoContext[currentPage], (double) x2a, (double) y2a);
  cairo_stroke(cairoContext[currentPage]);
}

//---------------------------------------------------------------------
// xwinttf_polyline()
//
// Draw a polyline in the current color.
//---------------------------------------------------------------------

void plD_polyline_xwinttf(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  poly_line(pls, xa, ya, npts);
  cairo_stroke(cairoContext[currentPage]);
}

//---------------------------------------------------------------------
// xwinttf_eop()
//
// End of page
//---------------------------------------------------------------------

void plD_eop_xwinttf(PLStream *pls)
{
  XFlush(XDisplay);
}

//---------------------------------------------------------------------
// xwinttf_tidy()
//
// Close graphics file or otherwise clean up.
//---------------------------------------------------------------------

void plD_tidy_xwinttf(PLStream *pls)
{
  int i;

  printf("Key <Return> to finish\n");
  getc(stdin);

  for(i=0;i<MAXPAGES;i++){
    if(cairoContext[i] != NULL){
      cairo_destroy(cairoContext[i]);
      cairo_surface_destroy(cairoSurface[i]);
    }
  }

  XCloseDisplay(XDisplay);
  free(XDisplay);
}

//---------------------------------------------------------------------
// plD_state_xwinttf()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//
// Nothing is done here because these attributes are acquired from 
// PLStream for each element that is drawn.
//---------------------------------------------------------------------

void plD_state_xwinttf(PLStream *pls, PLINT op)
{
}

//---------------------------------------------------------------------
// xwinttf_esc()
//
// Escape function.
//---------------------------------------------------------------------

void plD_esc_xwinttf(PLStream *pls, PLINT op, void *ptr)
{
  switch(op)
    {
    case PLESC_FILL:  // filled polygon
      poly_line(pls, pls->dev_x, pls->dev_y, pls->dev_npts);
      cairo_fill(cairoContext[currentPage]);
      break;
    case PLESC_HAS_TEXT: // render rext
      proc_str(pls, (EscText *) ptr);
      break;
    case PLESC_FLUSH: // forced update of the window
      XFlush(XDisplay);
      break;
    case PLESC_GETC:                // get cursor position
      XFlush(XDisplay);
      get_cursor(pls, (PLGraphicsIn*)ptr);
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
  PLFLT rotation, shear, cos_rot, sin_rot, sin_shear;
  cairo_matrix_t *cairoTransformMatrix;
  PangoLayout *layout;
  PangoFontDescription *fontDescription;

  set_current_context(pls);

  // Check that we got unicode, warning message and return if not
  if(args->unicode_array_len == 0){
    printf("Non unicode string passed to xwinttf driver, ignoring\n");
    return;
  }
	
  // Check that unicode string isn't longer then the max we allow	
  if(args->unicode_array_len >= MAX_STRING_LEN){
    printf("Sorry, the xwinttf driver only handles strings of length < %d\n", MAX_STRING_LEN);
    return;
  }

  // Calculate the font size (in pixels)
  fontSize = pls->chrht * DPI/25.4;

  // Convert the escape characters into the appropriate Pango markup
  textWithPangoMarkup = ucs4_to_pango_markup_format(args->unicode_array, args->unicode_array_len, fontSize);

  // Create the Pango text layout so we can figure out how big it is
  layout = pango_cairo_create_layout(cairoContext[currentPage]);
  pango_layout_set_markup(layout, textWithPangoMarkup, -1);
  // fontDescription = pango_font_description_from_string(fontString);
  // pango_layout_set_font_description(layout, fontDescription);
  pango_layout_get_pixel_size(layout, &textXExtent, &textYExtent);

  // Move to the string reference point
  cairo_move_to(cairoContext[currentPage], (double) args->x, (double) args->y);

  // Save current transform matrix
  cairo_save(cairoContext[currentPage]);

  // Invert the coordinate system so that the text is drawn right side up
  cairoTransformMatrix = (cairo_matrix_t *) malloc (sizeof(cairo_matrix_t));
  cairo_matrix_init(cairoTransformMatrix, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0);
  cairo_transform(cairoContext[currentPage], cairoTransformMatrix);

  // Extract rotation angle and shear from the PLplot tranformation matrix.
  // Compute sines and cosines of the angles as an optimization.
  plRotationShear(args->xform, &rotation, &shear);
  cos_rot = cos(rotation);
  sin_rot = sin(rotation);
  sin_shear = sin(shear);

  // Apply the transform matrix
  cairo_matrix_init(cairoTransformMatrix,
		    cos_rot,
		    -sin_rot,
		    cos_rot * sin_shear + sin_rot,
		    -sin_rot * sin_shear + cos_rot,
		    0,0);
  cairo_transform(cairoContext[currentPage], cairoTransformMatrix);
  free(cairoTransformMatrix);

  // Move to the text starting point
  cairo_rel_move_to(cairoContext[currentPage], 
		    (double)(-1.0 * args->just * (double)textXExtent), 
		    (double)(-0.5 * textYExtent));

  // Render the text
  pango_cairo_show_layout(cairoContext[currentPage], layout);

  // Restore the transform matrix to its state prior to the text transform.
  cairo_restore(cairoContext[currentPage]);
  
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
// Close a span tag & brings us down to zero sub/super-script level
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
// get_cursor()
//
// returns the location of the next mouse click
//---------------------------------------------------------------------

void get_cursor(PLStream *pls, PLGraphicsIn *gin)
{
  XEvent mouseEvent;
  Cursor xHairCursor;

  // Initialize PLplot mouse event structure
  plGinInit(gin);

  // Create cross hair cursor & switch to using it
  xHairCursor = XCreateFontCursor(XDisplay, XC_crosshair);
  XDefineCursor(XDisplay, XWindow[currentPage], xHairCursor);

  // Get the next mouse button release event
  XSelectInput(XDisplay, XWindow[currentPage], ButtonReleaseMask);
  XMaskEvent(XDisplay, ButtonReleaseMask, &mouseEvent);
  XSelectInput(XDisplay, XWindow[currentPage], NoEventMask);

  // Update PLplot's mouse event structure
  gin->button = 0;
  gin->pX = mouseEvent.xbutton.x;
  gin->pY = mouseEvent.xbutton.y;
  gin->dX = (PLFLT)mouseEvent.xbutton.x/((PLFLT)(pls->xlength));
  gin->dY = (PLFLT)mouseEvent.xbutton.y/((PLFLT)(pls->ylength));

  // Switch back to normal cursor
  XUndefineCursor(XDisplay, XWindow[currentPage]);
  XFlush(XDisplay);
}

//---------------------------------------------------------------------
// set_current_context()
//
// Updates the cairo graphics context with the current values in
// PLStream.
//---------------------------------------------------------------------

void set_current_context(PLStream *pls)
{
  cairo_set_source_rgb(cairoContext[currentPage],
		       (double)pls->curcolor.r/255.0, 
  		       (double)pls->curcolor.g/255.0,
  		       (double)pls->curcolor.b/255.0);
  // In Cairo, zero width lines are not hairlines, they are completely invisible.
  if(pls->width < 1){
    cairo_set_line_width(cairoContext[currentPage], 1.0);
  } else{
    cairo_set_line_width(cairoContext[currentPage], (double) pls->width);
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

  set_current_context(pls);
  
  cairo_move_to(cairoContext[currentPage], (double) xa[0], (double) ya[0]);
  for(i=1;i<npts;i++){
    cairo_line_to(cairoContext[currentPage], (double) xa[i], (double) ya[i]);
  }
}
