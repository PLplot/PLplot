/* $Id$
 * $Log$
 * Revision 1.1  1993/07/28 05:52:30  mjl
 * Added to provide function prototypes and anything else necessary to
 * use X utility functions in xwin.c driver (used by plframe.c at present).
 *
*/

/*	xwin.h

	Holds prototypes of xwin driver functions that need to be called
	directly from the plframe widget.  
*/

void  PLColor_to_XColor		(PLColor *, XColor *);
void  PLColor_from_XColor	(PLColor *, XColor *);

