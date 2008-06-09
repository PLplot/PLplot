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
//#define NEED_PLDEBUG
//#define DEBUG
#include "plplotP.h"
#include "drivers.h"
#include "plunicode-type1.h"
#include "plfci-type1.h"

/* constants */

/* a4 page, 144 dpi */
#define DPI            (144) 
#define PDF_Default_X	 (DPI*11.69)
#define PDF_Default_Y	 (DPI*8.27)

#define MAX_STRING_LEN	1000

typedef struct {
  HPDF_Doc pdf;
  HPDF_Page page;
  FILE *pdfFile;
  int canvasXSize;
  int canvasYSize;
  
  /* font variables */
  HPDF_Font m_font;
 	int nlookup;
	const Unicode_to_Type1_table *lookup; 
  HPDF_REAL fontSize;
  HPDF_REAL fontScale;
  HPDF_REAL textWidth, textHeight;
  HPDF_REAL yOffset;
  HPDF_REAL textRed, textGreen, textBlue;
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


/* driver specific options */
static PLINT text=1;
static PLINT compress=1;
static PLINT hrshsym = 1;

DrvOpt pdf_options[] = {
  { "text", DRV_INT, &text, "Use own text routines (text=0|1)" },
  { "compress", DRV_INT, &compress, "Compress pdf output (compress=0|1)" },
  { "hrshsym", DRV_INT, &hrshsym, "Use Hershey symbol set (hrshsym=0|1)" },
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

#ifdef DEBUG
  pls->verbose = 1;
  pls->debug = 1;
#else
  pls->verbose = 0;
  pls->debug = 0;
#endif

  if( text ) {
    pls->dev_text = 1;		  /* handles text        */
    pls->dev_unicode = 1; 	/* wants text as unicode */
    if( hrshsym )
      pls->dev_hrshsym = 1;    
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

  if( compress )
    HPDF_SetCompressionMode( dev->pdf, HPDF_COMP_ALL );
    
  if( setjmp(env) ) {
    HPDF_Free( dev->pdf );
    plexit("ERROR: ???\n");
  }
}

/***********************************************************************
 * pdf_bop( PLStream *pls )
 *
 * Set up for the next page.
 ***********************************************************************/
void plD_bop_pdf( PLStream *pls )
{
  pdfdev* dev = (pdfdev*)pls->dev;
  HPDF_REAL width, height;

  pls->page++;

  dev->page = HPDF_AddPage( dev->pdf );
  HPDF_Page_SetSize( dev->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE );
  HPDF_Page_Concat( dev->page, 72.0f/DPI, 0, 0, 72.0f/DPI, 0, 0);
  
   // set the background by drawing a rectangle that is the size of
   // of the canvas and filling it with the background color.
  HPDF_Page_SetRGBFill( dev->page, (HPDF_REAL)(pls->cmap0[0].r/255.0),
                        (HPDF_REAL)(pls->cmap0[0].g/255.0), (HPDF_REAL)(pls->cmap0[0].b/255.0) );  
  
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
  short xa[2], ya[2];
  
  xa[0]=x1a; xa[1]=x2a;
  ya[0]=y1a; ya[1]=y2a;

  poly_line( pls, xa, ya, 2, 0 );
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
  
  HPDF_Page_SetLineWidth( dev->page, (HPDF_REAL)(pls->width) );
  HPDF_Page_SetLineCap( dev->page, HPDF_ROUND_END );
  HPDF_Page_SetLineJoin( dev->page, HPDF_ROUND_JOIN );
  HPDF_Page_SetRGBStroke( dev->page, (HPDF_REAL)(pls->curcolor.r/255.0),
                          (HPDF_REAL)(pls->curcolor.g/255.0), (HPDF_REAL)(pls->curcolor.b/255.0) );  
  HPDF_Page_SetRGBFill( dev->page, (HPDF_REAL)(pls->curcolor.r/255.0),
                        (HPDF_REAL)(pls->curcolor.g/255.0), (HPDF_REAL)(pls->curcolor.b/255.0) );  
  
  HPDF_Page_MoveTo( dev->page, (HPDF_REAL)xa[0], (HPDF_REAL)ya[0] );
  for( i=1; i<npts; i++ )
    HPDF_Page_LineTo( dev->page, (HPDF_REAL)xa[i], (HPDF_REAL)ya[i] );

  if( fill==1 )
    HPDF_Page_FillStroke( dev->page );
  else
    HPDF_Page_Stroke( dev->page );
}


/*--------------------------------------------------------------------------*\
 *  unsigned char plunicode2type1 (const PLUNICODE index, 
 *       const Unicode_to_Type1_table lookup[], const int number_of_entries)
 *
 *  Function takes an input unicode index, looks through the lookup
 *  table (which must be sorted by PLUNICODE Unicode), then returns the 
 *  corresponding Type1 code in the lookup table.  If the Unicode index is
 *  not present the returned value is 32 (which is normally a blank
 *  for Type 1 fonts).
\*--------------------------------------------------------------------------*/
static unsigned char plunicode2type1( const PLUNICODE index,
                                      const Unicode_to_Type1_table lookup[],
                                      const int nlookup )
{
  int jlo = -1, jmid, jhi = nlookup;
  
  while( jhi - jlo > 1 ) {
    /* Note that although jlo or jhi can be just outside valid
     * range (see initialization above) because of while condition
     * jlo < jmid < jhi and jmid must be in valid range.
     */
  	jmid = (jlo+jhi)/2;
  	if( index > lookup[jmid].Unicode )
  	  jlo = jmid;
  	else if( index < lookup[jmid].Unicode )
  	  jhi = jmid;
  	else
  	  /* We have found it!
  	   * index == lookup[jmid].Unicode 
  	   */
  	  return( lookup[jmid].Type1 );
  }
  /* jlo is invalid or it is valid and index > lookup[jlo].Unicode.
   * jhi is invalid or it is valid and index < lookup[jhi].Unicode.
   * All these conditions together imply index cannot be found in lookup.
   * Mark with 32 (which is normally a blank in type 1 fonts).
   */
  return( 32 );
}


void PSDrawTextToCanvas( pdfdev* dev, char* type1_string, short drawText )
{
  HPDF_REAL th;

  if( drawText ) {
    HPDF_Page_BeginText( dev->page );
    HPDF_Page_SetTextRenderingMode( dev->page, HPDF_FILL );
    HPDF_Page_SetRGBFill( dev->page, dev->textRed, dev->textGreen, dev->textBlue );
    HPDF_Page_MoveTextPos( dev->page, dev->textWidth, dev->yOffset );
    HPDF_Page_ShowText( dev->page, type1_string );
    HPDF_Page_EndText( dev->page );
  }

  /* determine text width and height */
  dev->textWidth += HPDF_Page_TextWidth( dev->page, type1_string );
  th = (HPDF_REAL)(HPDF_Font_GetCapHeight( dev->m_font )*dev->fontSize/1000.0);
  dev->textHeight = dev->textHeight>(th+dev->yOffset) ? dev->textHeight : (th+dev->yOffset);
  
  /* clear string */
  memset( type1_string, '\0', MAX_STRING_LEN );
}


void PSSetFont( pdfdev* dev, PLUNICODE fci )
{
  char *font;

  /* convert the fci to Base14/Type1 font information */
  font = plP_FCI2FontName( fci, Type1Lookup, N_Type1Lookup );
  
  if( !(dev->m_font = HPDF_GetFont(dev->pdf, font, NULL)) )
    plexit( "ERROR: Couldn't open font\n" );
  HPDF_Page_SetFontAndSize( dev->page, dev->m_font, dev->fontSize );
  
	if( !strcmp(font, "Symbol") ) {
	   dev->nlookup = number_of_entries_in_unicode_to_symbol_table;
	   dev->lookup = unicode_to_symbol_lookup_table;
	}	else {
	   dev->nlookup = number_of_entries_in_unicode_to_standard_table;
	   dev->lookup = unicode_to_standard_lookup_table;
	}
}


void PSDrawText( pdfdev* dev, PLUNICODE* ucs4, int ucs4Len, short drawText )
{
  int i, s;
  unsigned char type1_string[MAX_STRING_LEN];
  char plplotEsc;
  PLUNICODE fci;

  memset( type1_string, '\0', MAX_STRING_LEN );

  /* Get PLplot escape character */
  plgesc( &plplotEsc );

  /* Get the curent font */
  dev->fontScale = 1.0;
  dev->yOffset = 0.0;
  plgfci( &fci );
  PSSetFont( dev, fci );
  dev->textWidth=0;
  dev->textHeight=0;

  i=0; s=0;
  while( i < ucs4Len ) {
    if( ucs4[i] < PL_FCI_MARK ) {	/* not a font change */
      if( ucs4[i] != (PLUNICODE)plplotEsc ) {  /* a character to display */
        type1_string[s++] = plunicode2type1( ucs4[i], dev->lookup, dev->nlookup);
      	i++;
      	continue;
      }
      i++;
      if( ucs4[i] == (PLUNICODE)plplotEsc ) {   /* a escape character to display */
        type1_string[s++] = plunicode2type1( ucs4[i], dev->lookup, dev->nlookup);
        i++;
        continue;
      } else {
      	if( ucs4[i] == (PLUNICODE)'u' ) {	/* Superscript */
          // draw string so far
          PSDrawTextToCanvas( dev, type1_string, drawText );
          s=0;
          
          // change font scale
      		if( dev->yOffset<0.0 )
            dev->fontScale *= (HPDF_REAL)1.25;  /* Subscript scaling parameter */
      		else
            dev->fontScale *= (HPDF_REAL)0.8;  /* Subscript scaling parameter */
          PSSetFont( dev, fci );

      		dev->yOffset += dev->fontSize * dev->fontScale / (HPDF_REAL)2.;
      	}
      	if( ucs4[i] == (PLUNICODE)'d' ) {	/* Subscript */
          HPDF_REAL old_fontScale=dev->fontScale;
          // draw string so far
          PSDrawTextToCanvas( dev, type1_string, drawText );
          s=0;

          // change font scale
      		if( dev->yOffset>0.0 )
            dev->fontScale *= (HPDF_REAL)1.25;  /* Subscript scaling parameter */
      		else
            dev->fontScale *= (HPDF_REAL)0.8;  /* Subscript scaling parameter */
          PSSetFont( dev, fci );

      		dev->yOffset -= dev->fontSize * old_fontScale / (HPDF_REAL)2.;
      	}
      	if( ucs4[i] == (PLUNICODE)'-' ) {	/* underline */
          // draw string so far
          PSDrawTextToCanvas( dev, type1_string, drawText );
          s=0;

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
      PSDrawTextToCanvas( dev, type1_string, drawText );
      s=0;

      // get new font
      fci = ucs4[i];
      PSSetFont( dev, fci );
      i++;
    }
  }

  PSDrawTextToCanvas( dev, type1_string, drawText );
}


//---------------------------------------------------------------------
// process_string()
//
// Handles drawing filled and unfilled polygons
//---------------------------------------------------------------------
void process_string( PLStream* pls, EscText* args )
{
  pdfdev* dev = (pdfdev*)pls->dev;
  PLFLT rotation, shear;
  HPDF_REAL cos_rot, sin_rot, cos_shear, sin_shear;  

  /* Check that we got unicode, warning message and return if not */
  if( args->unicode_array_len == 0 ) {
    printf( "Non unicode string passed to a pdf driver, ignoring\n" );
    return;
  }
	
  /* Check that unicode string isn't longer then the max we allow */
  if( args->unicode_array_len >= MAX_STRING_LEN ) {
    printf( "Sorry, the pdf drivers only handles strings of length < %d\n", MAX_STRING_LEN );
    return;
  }
  
  /* Calculate the font size (in pixels) */
  dev->fontSize =(HPDF_REAL)( pls->chrht * DPI/25.4 * 1.6);
  
  /* text color */
  dev->textRed=(HPDF_REAL)(pls->curcolor.r/255.0);
  dev->textGreen=(HPDF_REAL)(pls->curcolor.g/255.0);
  dev->textBlue=(HPDF_REAL)(pls->curcolor.b/255.0);
  
  /* calculate transformation matrix (rotation and shear of text) */
  plRotationShear( args->xform, &rotation, &shear );
  rotation -= pls->diorot * M_PI / 2.0;
  cos_rot = (HPDF_REAL)cos( rotation );
  sin_rot = (HPDF_REAL)sin( rotation );
  cos_shear = (HPDF_REAL)cos( shear );
  sin_shear = (HPDF_REAL)sin( shear );

  /* calculate text extend -> stored in dev->textWidth and dev->textHeight */
  PSDrawText( dev, args->unicode_array, args->unicode_array_len, 0 );

  /* apply transformation matrix and draw text */
  HPDF_Page_GSave( dev->page );
  HPDF_Page_Concat( dev->page, cos_rot, sin_rot,
                               -cos_rot * sin_shear - sin_rot * cos_shear,
                               -sin_rot * sin_shear + cos_rot * cos_shear,
                               (HPDF_REAL)(args->x), (HPDF_REAL)(args->y) );
  HPDF_Page_Concat( dev->page, (HPDF_REAL)1.0, (HPDF_REAL)0.0, (HPDF_REAL)0.0, (HPDF_REAL)1.0,
                    (HPDF_REAL)(-args->just*dev->textWidth), (HPDF_REAL)(-0.5*dev->textHeight) );
  PSDrawText( dev, args->unicode_array, args->unicode_array_len, 1 );
  HPDF_Page_GRestore( dev->page );
}

#else

int pldummy_pdf()
{
  return 0;
}

#endif				/* PLD_pdf */

