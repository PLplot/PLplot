/* $Id$
 * $Log$
 * Revision 1.2  1995/05/15 07:56:43  mjl
 * Include malloc.h (for debugging malloc) if DEBUGGING_MALLOC is defined.
 *
 * Revision 1.1  1995/05/07  02:24:30  mjl
 * Added to help with debugging support.  Includes macros as well as a
 * static function pldebug, which has a stdarg (vargarg) based argument list.
 * Debugging output is only enabled if DEBUG is defined prior to inclusion of
 * this file and the debug stream variable is set (can be set by -debug command
 * line option).
 *
*/

/*
    pldebug.h

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
	pltext();
	va_start(args, fname);

    /* print out name of caller and source file */

	fprintf(stderr, "%s (%s): ", fname, __FILE__);

    /* print out remainder of message */

	vfprintf(stderr, (char *) va_arg(args, char *), args);
	va_end(args);
	plgra();
    }
#endif
}

#endif	/* __PLDEBUG_H__ */
