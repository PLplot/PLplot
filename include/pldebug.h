/* $Id$

    Copyright (C) 1995 by Maurice J. LeBrun

    Debugging support for PLplot.

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 
*/

#ifndef __PLDEBUG_H__
#define __PLDEBUG_H__

#include <stdarg.h>

/* For the truly desperate debugging task */

#ifdef DEBUG_ENTER
#define dbug_enter(a) \
if (plsc->debug) \
    fprintf(stderr, "Entered %s (%s)\n", a, __FILE__);

#else
#define dbug_enter(a)
#endif

/* If we're using a debugging malloc, include the header file here */

#ifdef DEBUGGING_MALLOC
#include <malloc.h>
#endif

/*--------------------------------------------------------------------------*\
 * pldebug()
 *
 * Included into every plplot source file to control debugging output.  To
 * enable printing of debugging output, you must #define DEBUG before
 * including plplotP.h or specify -DDEBUG in the compile line.  When running
 * the program you must in addition specify -debug.  This allows debugging
 * output to be available when asked for.
 *
 * Syntax:
 *	pldebug(function_name, format, arg1, arg2...);
\*--------------------------------------------------------------------------*/

static void
pldebug( const char *fname, ... )
{
#ifdef DEBUG
    va_list args;

    if (plsc->debug) {
	c_pltext();
	va_start(args, fname);

    /* print out name of caller and source file */

	fprintf(stderr, "%s (%s): ", fname, __FILE__);

    /* print out remainder of message */

	vfprintf(stderr, (char *) va_arg(args, char *), args);
	va_end(args);
	c_plgra();
    }
#endif
}

#endif	/* __PLDEBUG_H__ */
