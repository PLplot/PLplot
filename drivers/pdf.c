/* December 29, 2006

	PLplot driver for PDF 

	Copyright (C) 2006 Werner Smekal

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
#include "plDevs.h"

#ifdef PLD_pdf

#include <stdarg.h>
#include <math.h>
#include <setjmp.h>

#include "hpdf.h"

/* PLplot header files */
#include "plplotP.h"
#include "drivers.h"

/* constants */

/* a4 page, 144 dpi */
#define DPI            144 
#define PDF_Default_X	 DPI*11.69
#define PDF_Default_Y	 DPI*8.27

#define MAX_STRING_LEN	1000

typedef struct {
  HPDF_Doc pdf;
  HPDF_Page page;
  FILE *pdfFile;
  int canvasXSize;
  int canvasYSize;
  
  /* font variables */
  HPDF_Font m_font;
  double fontSize;
  double fontScale;
  PLINT textWidth, textHeight, textDescent, textLeading;
  double yOffset;
  PLINT posX, posY;
  PLFLT rotation, cos_rot, sin_rot;
  PLFLT shear, cos_shear, sin_shear;  
} pdfdev;

/* local variables */
const char* plD_DEVICE_INFO_pdf = "pdf:Portable Document Format PDF:1:pdf:58:pdf";
static jmp_buf env;

//-----------------------------------------------
// function declarations
//-----------------------------------------------

/* General */

static short desired_offset( short, double );
static void poly_line(PLStream *pls, short *xa, short *ya, PLINT npts, short fill);

/* String processing */
static void process_string( PLStream *, EscText * );

/* PLplot interface functions */
void plD_dispatch_init_pdf      (PLDispatchTable *pdt);
void plD_init_pdf               (PLStream *);
void plD_line_pdf               (PLStream *, short, short, short, short);
void plD_polyline_pdf   		(PLStream *, short *, short *, PLINT);
void plD_eop_pdf                (PLStream *);
void plD_bop_pdf                (PLStream *);
void plD_tidy_pdf               (PLStream *);
void plD_state_pdf              (PLStream *, PLINT);
void plD_esc_pdf                (PLStream *, PLINT, void *);

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler( HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data )
{
  /* invoke longjmp() when an error has occurred */
  printf( "ERROR: error_no=%04X, detail_no=%d\n", (unsigned int)error_no, (int)detail_no );
  longjmp(env, 1);
}


//---------------------------------------------------------------------
// dispatch_init_init()
//
// Initialize device dispatch table
//----------------------------------------------------------------------

void plD_dispatch_init_pdf(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
   pdt->pl_MenuStr  = "Portable Document Format PDF";
   pdt->pl_DevName  = "pdf";
#endif
   pdt->pl_type     = plDevType_FileOriented;
   pdt->pl_seq      = 58;
   pdt->pl_init     = (plD_init_fp)     plD_init_pdf;
   pdt->pl_line     = (plD_line_fp)     plD_line_pdf;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_pdf;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_pdf;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_pdf;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_pdf;
   pdt->pl_state    = (plD_state_fp)    plD_state_pdf;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_pdf;
}

static int text=1;
DrvOpt pdf_options[] = {
  { "text", DRV_INT, &text, "Use own text routines (text=0|1)" },
  { NULL, DRV_INT, NULL, NULL}
};
  

//---------------------------------------------------------------------
// pdf_init()
//
// Initialize device
//----------------------------------------------------------------------
void plD_init_pdf(PLStream *pls)
{
  pdfdev* dev;

  /* allocate memory for the device storage */
  dev = (pdfdev*)calloc( 1, sizeof(pdfdev) );
	if( dev == NULL)
    plexit( "Insufficient memory\n" );
  pls->dev = (void*)dev;
   
  /* Check for and set up driver options */
  plParseDrvOpts( pdf_options );
        
  pls->termin = 0;			/* not an interactive device */
  pls->color = 1;		  	/* supports color */
  pls->width = 1;
  pls->bytecnt = 0;
#ifdef _DEBUG
  pls->verbose = 1;
  pls->debug = 1;
#else
  pls->verbose = 0;
  pls->debug = 0;
#endif

  if( text ) {
    pls->dev_text = 1;		  /* handles text        */
    pls->dev_unicode = 1; 	/* wants text as unicode */
  }

  pls->page = 0;
  pls->dev_fill0 = 1;		/* supports hardware solid fills */
  pls->dev_fill1 = 1;

  pls->graphx = GRAPHICS_MODE;

  if (!pls->colorset)
    pls->color = 1; 
 
   /* Set up device parameters */
   plP_setpxl( DPI/25.4, DPI/25.4 );           /* Pixels/mm. */

  /* Set the bounds for plotting.  default is PDF_Default_X x PDF_Default_Y unless otherwise specified. */
  if (pls->xlength <= 0 || pls->ylength <= 0){
    dev->canvasXSize = (PLINT)PDF_Default_X;
    dev->canvasYSize = (PLINT)PDF_Default_Y;
  } else {
    dev->canvasXSize = pls->xlength;
    dev->canvasYSize = pls->ylength;
  }   
  plP_setphy( 0, dev->canvasXSize, 0, dev->canvasYSize );
  
  /* Initialize family file info */
  /*plFamInit(pls);*/

  /* Prompt for a file name if not already set */
  plOpenFile( pls );
  dev->pdfFile = pls->OutFile;
   
  dev->pdf = HPDF_New( error_handler, NULL );
  if( !dev->pdf )
    plexit("ERROR: cannot create pdf object.\n");

  if( setjmp(env) ) {
    HPDF_Free( dev->pdf );
    plexit("ERROR: ???\n");
  }
}

//----------------------------------------------------------------------
// pdf_bop()
//
// Set up for the next page.
//----------------------------------------------------------------------

void plD_bop_pdf(PLStream *pls)
{
  pdfdev* dev = (pdfdev*)pls->dev;
  HPDF_REAL width, height;

  pls->page++;

  dev->page = HPDF_AddPage( dev->pdf );
  HPDF_Page_SetSize( dev->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE );
  HPDF_Page_Concat( dev->page, 72.0f/DPI, 0, 0, 72.0f/DPI, 0, 0);
  
   // set the background by drawing a rectangle that is the size of
   // of the canvas and filling it with the background color.
   
  HPDF_Page_SetRGBFill( dev->page, pls->cmap0[0].r/255.0, pls->cmap0[0].g/255.0, pls->cmap0[0].b/255.0 );  
  
  width=HPDF_Page_GetWidth( dev->page )*DPI/72.f; // TODO: why this?
  height=HPDF_Page_GetHeight( dev->page )*DPI/72.f;  // TODO: why this?
  HPDF_Page_MoveTo( dev->page, (HPDF_REAL)0.0, (HPDF_REAL)0.0 );
  HPDF_Page_LineTo( dev->page, width, (HPDF_REAL)0.0 );
  HPDF_Page_LineTo( dev->page, width, (HPDF_REAL)height );
  HPDF_Page_LineTo( dev->page, 0.0, (HPDF_REAL)height );
  HPDF_Page_Fill( dev->page );
}

//---------------------------------------------------------------------
// pdf_line()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//----------------------------------------------------------------------

void plD_line_pdf(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  pdfdev* dev = (pdfdev*)pls->dev;
  
  HPDF_Page_SetLineWidth( dev->page, pls->width );
  HPDF_Page_SetLineCap( dev->page, HPDF_ROUND_END );
  HPDF_Page_SetRGBStroke( dev->page, pls->curcolor.r/255.0, pls->curcolor.g/255.0, pls->curcolor.b/255.0 );  
  HPDF_Page_SetLineJoin( dev->page, HPDF_ROUND_JOIN );
  
  HPDF_Page_MoveTo( dev->page, (HPDF_REAL)x1a, (HPDF_REAL)y1a );
  HPDF_Page_LineTo( dev->page, (HPDF_REAL)x2a, (HPDF_REAL)y2a );
  HPDF_Page_Stroke( dev->page );
}

//---------------------------------------------------------------------
// pdf_polyline()
//
// Draw a polyline in the current color.
//---------------------------------------------------------------------

void plD_polyline_pdf(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  poly_line( pls, xa, ya, npts, 0 );
}

//---------------------------------------------------------------------
// pdf_eop()
//
// End of page
//---------------------------------------------------------------------

void plD_eop_pdf(PLStream *pls)
{
}

//---------------------------------------------------------------------
// pdf_tidy()
//
// Close graphics file or otherwise clean up.
//---------------------------------------------------------------------

void plD_tidy_pdf(PLStream *pls)
{
  pdfdev* dev = (pdfdev*)pls->dev;

  /* save the document to a stream */
  HPDF_SaveToStream( dev->pdf );
  fprintf( stderr, "the size of data is %d\n", HPDF_GetStreamSize(dev->pdf) );

  /* rewind the stream. */
  HPDF_ResetStream( dev->pdf );

  /* get the data from the stream and output it to stdout. */
  for (;;) {
    HPDF_BYTE buf[4096]; // TODO: not good
    HPDF_UINT32 size = 4096;
    HPDF_STATUS ret = HPDF_ReadFromStream( dev->pdf, buf, &size );

    if( size == 0 )
      break;

    if( fwrite(buf, size, 1, dev->pdfFile) != 1)
      plexit( "ERROR: Cannot write to file!" );
  }
  
  fclose( dev->pdfFile );
  
  /* cleanup */
  HPDF_Free( dev->pdf );
}

//---------------------------------------------------------------------
// plD_state_pdf()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//
// Nothing is done here because these attributes are aquired from 
// PLStream for each element that is drawn.
//---------------------------------------------------------------------

void plD_state_pdf(PLStream *pls, PLINT op)
{
}

//---------------------------------------------------------------------
// pdf_esc()
//
// Escape function.
//---------------------------------------------------------------------
void plD_esc_pdf(PLStream *pls, PLINT op, void *ptr)
{
  switch( op )
  {
    case PLESC_FILL:      // fill polygon
      poly_line( pls, pls->dev_x, pls->dev_y, pls->dev_npts, 1 );
      break;
    case PLESC_HAS_TEXT:  // render text
      process_string( pls, (EscText*)ptr );
      break;
  }
}


//---------------------------------------------------------------------
// poly_line()
//
// Handles drawing filled and unfilled polygons
//---------------------------------------------------------------------
void poly_line( PLStream *pls, short *xa, short *ya, PLINT npts, short fill )
{
  pdfdev* dev = (pdfdev*)pls->dev;
  PLINT i;
  
  HPDF_Page_SetLineWidth( dev->page, pls->width );
  HPDF_Page_SetLineCap( dev->page, HPDF_ROUND_END );
  HPDF_Page_SetLineJoin( dev->page, HPDF_ROUND_JOIN );
  HPDF_Page_SetRGBStroke( dev->page, pls->curcolor.r/255.0, pls->curcolor.g/255.0, pls->curcolor.b/255.0 );  
  HPDF_Page_SetRGBFill( dev->page, pls->curcolor.r/255.0, pls->curcolor.g/255.0, pls->curcolor.b/255.0 );  
  
  HPDF_Page_MoveTo( dev->page, (HPDF_REAL)xa[0], (HPDF_REAL)ya[0] );
  for( i=1; i<npts; i++ )
    HPDF_Page_LineTo( dev->page, (HPDF_REAL)xa[i], (HPDF_REAL)ya[i] );

  if( fill==1 )
    HPDF_Page_FillStroke( dev->page );
  else
    HPDF_Page_Stroke( dev->page );
}

void PSDrawTextToCanvas( pdfdev* dev, char* utf8_string, short drawText )
{
}


void PSSetFont( pdfdev* dev, PLUNICODE fci )
{
  unsigned char fontFamily, fontStyle, fontWeight;

  plP_fci2hex( fci, &fontFamily, PL_FCI_FAMILY );
  plP_fci2hex( fci, &fontStyle, PL_FCI_STYLE );
  plP_fci2hex( fci, &fontWeight, PL_FCI_WEIGHT );  

  dev->m_font = HPDF_GetFont( dev->page, "Courier", "StandardEncoding" );
}


void PSDrawText( pdfdev* dev, PLUNICODE* ucs4, int ucs4Len, short drawText )
{
  int i = 0;
  char utf8_string[MAX_STRING_LEN];
  char utf8[5];
  char plplotEsc;
  PLUNICODE fci;

  memset( utf8_string, '\0', MAX_STRING_LEN );

  /* Get PLplot escape character */
  plgesc( &plplotEsc );

  /* Get the curent font */
  dev->fontScale = 1.0;
  dev->yOffset = 0.0;
  plgfci( &fci );
  PSSetFont( dev, fci );
  dev->textWidth=0;
  dev->textHeight=0;

  while( i < ucs4Len ) {
    if( ucs4[i] < PL_FCI_MARK ) {	/* not a font change */
      if( ucs4[i] != (PLUNICODE)plplotEsc ) {  /* a character to display */
        ucs4_to_utf8( ucs4[i], utf8 );
        strcat( utf8_string, utf8 );
      	i++;
      	continue;
      }
      i++;
      if( ucs4[i] == (PLUNICODE)plplotEsc ) {   /* a escape character to display */
        ucs4_to_utf8( ucs4[i], utf8 );
        strcat( utf8_string, utf8 );
        i++;
        continue;
      } else {
      	if( ucs4[i] == (PLUNICODE)'u' ) {	/* Superscript */
          // draw string so far
          PSDrawTextToCanvas( dev, utf8_string, drawText );
          
          // change font scale
      		if( dev->yOffset<0.0 )
            dev->fontScale *= 1.25;  /* Subscript scaling parameter */
      		else
            dev->fontScale *= 0.8;  /* Subscript scaling parameter */
          PSSetFont( dev, fci );

      		dev->yOffset += dev->fontSize * dev->fontScale / 2.;
      	}
      	if( ucs4[i] == (PLUNICODE)'d' ) {	/* Subscript */
          double old_fontScale=dev->fontScale;
          // draw string so far
          PSDrawTextToCanvas( dev, utf8_string, drawText );

          // change font scale
      		if( dev->yOffset>0.0 )
            dev->fontScale *= 1.25;  /* Subscript scaling parameter */
      		else
            dev->fontScale *= 0.8;  /* Subscript scaling parameter */
          PSSetFont( dev, fci );

      		dev->yOffset -= dev->fontSize * old_fontScale / 2.;
      	}
      	if( ucs4[i] == (PLUNICODE)'-' ) {	/* underline */
          // draw string so far
          PSDrawTextToCanvas( dev, utf8_string, drawText );

          //dev->underlined = !dev->underlined; 
          PSSetFont( dev, fci );
      	}
      	if( ucs4[i] == (PLUNICODE)'+' ) {	/* overline */
          /* not implemented yet */
        }
        i++;
      }
    } else { /* a font change */
      // draw string so far
      PSDrawTextToCanvas( dev, utf8_string, drawText );

      // get new font
      fci = ucs4[i];
      PSSetFont( dev, fci );
      i++;
    }
  }

  PSDrawTextToCanvas( dev, utf8_string, drawText );
}



//---------------------------------------------------------------------
// process_string()
//
// Handles drawing filled and unfilled polygons
//---------------------------------------------------------------------
void process_string( PLStream* pls, EscText* args )
{
  pdfdev* dev = (pdfdev*)pls->dev;

  /* Check that we got unicode, warning message and return if not */
  if( args->unicode_array_len == 0 ) {
    printf( "Non unicode string passed to a cairo driver, ignoring\n" );
    return;
  }
	
  /* Check that unicode string isn't longer then the max we allow */
  if( args->unicode_array_len >= 500 ) {
    printf( "Sorry, the wxWidgets drivers only handles strings of length < %d\n", 500 );
    return;
  }
  
  /* Calculate the font size (in pixels) */
  dev->fontSize = pls->chrht * DPI/25.4 * 1.3;
  
  /* calculate rotation of text */
  plRotationShear( args->xform, &dev->rotation, &dev->shear );
  dev->rotation -= pls->diorot * M_PI / 2.0;
  dev->cos_rot = cos( dev->rotation );
  dev->sin_rot = sin( dev->rotation );

  dev->posX = args->x;
  dev->posY = args->y;
  //PSDrawText( dev, args->unicode_array, args->unicode_array_len, false );
  
  dev->posX = args->x-(args->just*dev->textWidth)*dev->cos_rot-(0.5*dev->textHeight)*dev->sin_rot;
  dev->posY = args->y-(args->just*dev->textWidth)*dev->sin_rot+(0.5*dev->textHeight)*dev->cos_rot;
  //PSDrawText( dev, args->unicode_array, args->unicode_array_len, true );
}

#else

int 
pldummy_pdf()
{
  return 0;
}

#endif				/* PLD_pdf */

