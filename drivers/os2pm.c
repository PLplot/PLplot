/* $Id$
   $Log$
   Revision 1.7  1993/07/01 21:59:41  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.6  1993/03/15  21:39:14  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.5  1993/03/03  19:42:03  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.4  1993/02/22  23:10:59  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.3  1993/01/23  05:41:48  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.2  1992/11/07  07:48:45  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.1  1992/05/20  21:32:38  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
	os2pm.c

	Geoffrey Furnish
	9-22-91
	
	This driver sends plplot commands to the OS/2 PM PLPLOT Server.

	The Geoffrey Furnish Standard Disclaimer:
	"I hate any C compiler that isn't ANSI compliant, and I refuse to waste
	my time trying to support such garbage.  If you can't compile with an
	ANSI compiler, then don't expect this to work.  No appologies,
	now or ever."

	25 March 1992
	VERSION 1.0
*/

/* Some screwy thing with VOID goes wrong if this comes after the
   the os2.h stuff. */

#include "plplotP.h"

#define INCL_BASE
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include "drivers.h"

/* top level declarations */

static USHORT	rv;
static HFILE	hf;
static short	cnt;

static PLINT	xold = UNDEFINED;
static PLINT	yold = UNDEFINED;
	  
#include "pmdefs.h"

typedef	PLINT	COMMAND_ID;
typedef PLINT * CPARAMS;

static void	write_command( COMMAND_ID cid, CPARAMS p );

/*----------------------------------------------------------------------*\
*  os2_setup()
*
* Set up device.
\*----------------------------------------------------------------------*/

void os2_setup( PLStream *pls )
{
}

/*----------------------------------------------------------------------*\
*  os2_orient()
*
* Set up orientation.
\*----------------------------------------------------------------------*/

void os2_orient( PLStream *pls )
{
}

/*----------------------------------------------------------------------*\
*  plD_init_os2()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void	plD_init_os2( PLStream *pls )
{
    USHORT	usAction;
    UCHAR	c = (UCHAR) INITIALIZE;

    pls->termin =- 0;		/* not an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 1;

    if (!pls->colorset)
        pls->color = 1;

    plP_setpxl( (PLFLT) PIXEL_RES_X, (PLFLT) PIXEL_RES_Y );
    plP_setphy( 0, PLMETA_X, 0, PLMETA_Y );

    rv = DosOpen( PIPE_NAME,		// name of the pipe.
		&hf,			// address of file handle.
		&usAction,		// action taken.
		0L,			// size of file.
		FILE_NORMAL,		// file attribute.
		FILE_OPEN,		// open the file.
		OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
		0L );
    if (rv)
	plexit( "Unable to open connection to PM server.\n" );
	
    write_command( c, NULL );
}

/*----------------------------------------------------------------------*\
*  plD_line_os2()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void plD_line_os2( PLStream *pls, 
			short x1, short y1, short x2, short y2 )
{
	UCHAR c;
	PLINT	cp[4];

	if (	x1 < 0 || x1 > PLMETA_X ||
		x2 < 0 || x2 > PLMETA_X ||
		y1 < 0 || y1 > PLMETA_Y ||
		y2 < 0 || y2 > PLMETA_Y 	) {
		printf( "Something is out of bounds." );
	}

/* If continuation of previous line send the LINETO command, which uses 
   the previous (x,y) point as it's starting location.  This results in a
   storage reduction of not quite 50%, since the instruction length for
   a LINETO is 5/9 of that for the LINE command, and given that most 
   graphics applications use this command heavily.

   Still not quite as efficient as tektronix format since we also send the
   command each time (so shortest command is 25% larger), but a heck of
   a lot easier to implement than the tek method.
*/
	if(x1 == xold && y1 == yold) {

		c = (UCHAR) LINETO;
		cp[0]=x2;
		cp[1]=y2;
		write_command( c, cp );
	}
	else {
		c = (UCHAR) LINE;
		cp[0] = x1;
		cp[1] = y1;
		cp[2] = x2;
		cp[3] = y2;
		write_command( c, cp );
	}
	xold = x2;
	yold = y2;
}
  
/*----------------------------------------------------------------------*\
* plD_polyline_os2()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void 
plD_polyline_os2 (PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i=0; i<npts-1; i++) 
      plD_line_os2( pls, xa[i], ya[i], xa[i+1], ya[i+1] );
}

/*----------------------------------------------------------------------*\
*  plD_eop_os2()
*
*  Clear page.
\*----------------------------------------------------------------------*/

void	plD_eop_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) CLEAR;

	write_command( c, NULL );
}

/*----------------------------------------------------------------------*\
*  plD_bop_os2()
*
*  Advance to next page.
\*----------------------------------------------------------------------*/

void	plD_bop_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) PAGE;

	xold = UNDEFINED;
	yold = UNDEFINED;

	write_command( c, NULL );
}

/*----------------------------------------------------------------------*\
*  plD_tidy_os2()
*
*  Close graphics file
\*----------------------------------------------------------------------*/

void	plD_tidy_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) CLOSE;
	
	write_command( c, NULL );
		 
	DosClose( hf );
	pls->fileset = 0;
}

/*----------------------------------------------------------------------*\
*  plD_color_os2()
*
*  Set pen color.
\*----------------------------------------------------------------------*/

void	plD_color_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) NEW_COLOR;
	
	write_command( c, &pls->icol0 );
}

/*----------------------------------------------------------------------*\
*  plD_text_os2()
*
*  Switch to text mode.
\*----------------------------------------------------------------------*/

void	plD_text_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) SWITCH_TO_TEXT;

	write_command( c, NULL );
}

/*----------------------------------------------------------------------*\
*  plD_graph_os2()
*
*  Switch to graphics mode.
\*----------------------------------------------------------------------*/

void	plD_graph_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) SWITCH_TO_GRAPH;

	write_command( c, NULL );
}

/*----------------------------------------------------------------------*\
*  plD_width_os2()
*
*  Set pen width.
\*----------------------------------------------------------------------*/

void	plD_width_os2( PLStream *pls )
{
	UCHAR c = (UCHAR) NEW_WIDTH;
	
	write_command( c, &pls->width );
}

/*----------------------------------------------------------------------*\
*  plD_esc_os2()
*
*  Escape function.  Note that any data written must be in device
*  independent form to maintain the transportability of the metafile.
\*----------------------------------------------------------------------*/

void	plD_esc_os2( PLStream *pls, PLINT op, void *ptr )
{
	UCHAR c = (UCHAR) ESCAPE;
	float *color;
	unsigned long ired, igreen, iblue;
	unsigned long	pmrgb;

	write_command( c, NULL );

	switch (op) {
	    case PL_SET_RGB: 
		c = (UCHAR) ESC_RGB;
		color = (float *) &ptr[0];
		ired =	min(256,max(0,(int)255.*color[0]));
		igreen= min(256,max(0,(int)255.*color[1]));
		iblue = min(256,max(0,(int)255.*color[2]));
		pmrgb	= (ired   & 0xff) << 16 |
			  (igreen & 0xff) <<  8 |
			  (iblue  & 0xff);
		write_command( c, &pmrgb );
		//printf( "Changing to RGB value %lx \n", pmrgb );
		break;
	
	    default:
		c = (UCHAR) ESC_NOOP;
		write_command( c, NULL );
	}
}

/*----------------------------------------------------------------------*\
* Support routines
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
*	write_command()
*	
*	Function to write a command to the command pipe, and to flush
*	the pipe when full.
\*----------------------------------------------------------------------*/

void	write_command( COMMAND_ID cid,	CPARAMS p )
{
    static int i=0;
    static PLINT buffer[ PIPE_BUFFER_SIZE ];
    static PLINT cnt = 0;
    
    i++;

    buffer[cnt++] = cid;
    switch( cid ) {
	case LINE:
	    buffer[cnt++] = *p++;
	    buffer[cnt++] = *p++;
	    buffer[cnt++] = *p++;
	    buffer[cnt++] = *p++;
	    break;

	case LINETO:
	    buffer[cnt++] = *p++;
	    buffer[cnt++] = *p++;
	    break;

	case NEW_COLOR:
	case NEW_WIDTH:
	case ESC_RGB:
	    buffer[cnt++] = *p++;
	    break;

	case INITIALIZE:
	case CLOSE:
	case SWITCH_TO_TEXT:
	case SWITCH_TO_GRAPH:
	case CLEAR:
	case PAGE:
	case ESC_NOOP:
	    break;
	
	case ESCAPE:
	    break;

	default:
	    printf( "Unknown command type--no params passed\n" );
	    break;
    }
    if (cnt >= (.9 * PIPE_BUFFER_SIZE) || cid == CLOSE) {
	short rv1, rv2, bytes1, bytes2;
	
	rv1 = DosWrite( hf, &cnt, sizeof( PLINT ), &bytes1 );
	rv2 = DosWrite( hf, buffer, (USHORT) (cnt * sizeof(PLINT)), &bytes2 );
	if (!rv1 && !rv2) 
	    /* printf( "%d, %d bytes were written.\n", bytes1, bytes2 ) */ ;
	else 
	    printf( "----> write to pipe failed <----\n" );

	if (bytes1 != 4 || bytes2 != cnt*sizeof(PLINT) )
	    printf( "*** Bogus # of bytes written ***\n" );

	cnt=0;
    }
}
