/* $Id$
   $Log$
   Revision 1.4  1995/06/13 03:58:09  mjl
   Fixes for 4.99j on the Amiga.

 * Revision 1.3  1994/03/23  08:55:27  mjl
 * Header file rearrangement.
 *
 * Revision 1.2  1993/07/01  21:59:49  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible plplot functions
 * now start with "pl"; device driver functions start with "plD_".
 *
 * Revision 1.1  1993/03/15  21:34:19  mjl
 * Reorganization and update of Amiga drivers.  Window driver now uses Amiga
 * OS 2.0 capabilities.
 *
*/

/*	pla_gstskel.c

	Skeleton file for use with SAS/C 6.x, for building global symbol
	table. 
*/

#include "plplot.h"
#include "plamiga.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
