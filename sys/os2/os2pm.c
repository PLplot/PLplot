/* $Id$
 * $Log$
 * Revision 1.2  1996/10/22 18:21:57  furnish
 * Update the OS/2 driver with fixes supplied by Thorsten Feiweier.
 *
 * Revision 1.1  1994/05/25  09:27:01  mjl
 * Decided the os2/ subdir of sys/ was a better place for this.
 *
 * Revision 1.9  1994/03/23  06:34:33  mjl
 * All drivers: cleaned up by eliminating extraneous includes (stdio.h and
 * stdlib.h now included automatically by plplotP.h), extraneous clears
 * of pls->fileset, pls->page, and pls->OutFile = NULL (now handled in
 * driver interface or driver initialization as appropriate).  Special
 * handling for malloc includes eliminated (no longer needed) and malloc
 * prototypes fixed as necessary.
 *
 * Revision 1.8  1993/07/31  07:56:38  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.7  1993/07/01  21:59:41  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
*/

/*
	os2pm.c

	Geoffrey Furnish
	9-22-91
	
	This driver sends plplot commands to the OS/2 PM PLPLOT Server.

	25 March 1992
	VERSION 1.0
*/

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define INCL_BASE
#include <os2.h>

#include "plplotP.h"
#include "drivers.h"
#include "pmdefs.h"

/* top level declarations */

static USHORT	rv;
static HFILE	hf;

static int i=0;
static PLINT buffer[ PIPE_BUFFER_SIZE ];
static PLINT cnt = 0;
static PLINT escape[2] = {0, 0};

static PLINT	xold = UNDEFINED;
static PLINT	yold = UNDEFINED;
	  
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
    ULONG	usAction;
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
}

/*----------------------------------------------------------------------*\
* plD_state_os2()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_os2(PLStream *pls, PLINT op)
{
  UCHAR c;
  /* write_command( CHANGE_STATE, NULL ); */



    switch (op) {

    case PLSTATE_WIDTH:
      c = (UCHAR) NEW_WIDTH;
      escape[0] = (PLINT) pls->width;
      // write_command( c, NULL);
      // write_command( (UCHAR) op, (CPARAMS) a );
      write_command ( c, NULL);
      break;

    case PLSTATE_COLOR0:
      c = (UCHAR) NEW_COLOR;
      escape[0] = (PLINT) pls->icol0;
      // write_command( c, NULL);
      // write_command( (UCHAR) op, (CPARAMS) a );
      write_command ( c, NULL);
      break;

    case PLSTATE_COLOR1:
      break;
    }
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

	// write_command( c, NULL );

	switch (op) {
	    case PLESC_SET_RGB: 
		// c = (UCHAR) ESC_RGB;
		color = (float *) ptr; //&ptr[0];
		ired =	min(256,max(0,(int)255.*color[0]));
		igreen= min(256,max(0,(int)255.*color[1]));
		iblue = min(256,max(0,(int)255.*color[2]));
		pmrgb	= (ired   & 0xff) << 16 |
			  (igreen & 0xff) <<  8 |
			  (iblue  & 0xff);
		escape[0] = (PLINT) ESC_RGB;
		escape[1] = (PLINT) pmrgb;
		write_command( c, NULL );
		//printf( "Changing to RGB value %lx \n", pmrgb );
		break;
	
	    default:
		// c = (UCHAR) ESC_NOOP;
		escape[0] = (PLINT) ESC_NOOP;
		write_command( c, NULL );
		break;
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
	     buffer[cnt++] = escape[0];
	     break;

	case INITIALIZE:
	case CLOSE:
	case SWITCH_TO_TEXT:
	case SWITCH_TO_GRAPH:
	case CLEAR:
	case PAGE:
	  break;
	
	case ESCAPE:
	  buffer[cnt++] = escape[0];
	  switch( escape[0]) {
	  case (PLINT) ESC_NOOP:
	    break;
	  case (PLINT) ESC_RGB:
	    buffer[cnt++] = escape[1];
	  default:
	    printf ("Unknown escape sequence\n");
	    break;
	  }
	  break;

	default:
	    printf( "Unknown command type--no params passed\n" );
	    break;
    }
    if (cnt >= (.9 * PIPE_BUFFER_SIZE) || cid == CLOSE) {
	short rv1, rv2;
	ULONG bytes1 = 0, bytes2 = 0;
	
	rv1 = DosWrite( hf, &cnt, sizeof( PLINT ), &bytes1 );
	rv2 = DosWrite( hf, buffer, (USHORT) (cnt * sizeof(PLINT)), &bytes2 );
	if (!rv1 && !rv2) 
	    /* printf( "%d, %d bytes were written.\n", bytes1, bytes2 ) */ ;
	else 
	    printf( "----> write to pipe failed <---- %i %i\n", rv1, rv2 );

	if (bytes1 != sizeof(PLINT) || bytes2 != cnt*sizeof(PLINT) )
	    printf( "*** Bogus # of bytes written ***\n" );

	cnt=0;
  }
}
