/* $Id$
 * $Log$
 * Revision 1.8  2004/01/17 16:41:37  rlaboiss
 * This jumbo cvs commit is the result of a herculean task to update the
 * copyright notices of most of the source files of PLplot.
 *
 * I hope that I did not introduce any serious breakage.  This seems plausible,
 * because I did changes only at the beginning of the files and only in
 * commented lines.  However, I used a semi-automatic procedure for doing that
 * and some bugs may have slipped into the files.  The sources still build okay
 * (at least the Debian packages) and I can install the libraries, and compile
 * and run some C examples.  That is not enough, so PLEASE TEST!
 *
 * This is what I did in more detail:
 *
 * 1) From the CVS ChangeLog, I considered all the files that changed since the
 *    last release (5.2.1).  I then added the line:
 *
 *        Copyright (C) 2004  Joe C. V. S. Committer
 *
 *    where the name of the developper who did the CVS commit is included.
 *
 * 2) Notice that the year in the line above should read 2004, even if the
 *    changes were done last year, since Copyright years refer to when the
 *    software is released.
 *
 * 3) For the same reason, I did not bother adding missing Copyright entries
 *    for previous authors of each file.  One copyright holder is enough for
 *    our purposes.
 *
 * 4) I also reformatted and reworded the License Conditions text.  This
 *    included changing things like "This file is free software" to "PLplot is
 *    free sofware" and so on.
 *
 * 5) Finally, some files were lacking License notices, especially the src/*.c.
 *    I added them and put in the Copyright holder line the name of the
 *    developer who did the last cvs commit for the file.
 *
 * If you wish to know where your name appears, you can run from the
 * $(top_builddir):
 *
 *     grep -ri "copyright.*2004".
 *
 *
 * Copyright (C) 2004  Joao Cardoso
 *
 * This file is part of PLplot.
 *
 * PLplot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Library Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PLplot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with PLplot; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Revision 1.7  2003/10/29 19:40:54  jcard
 * Mostly cosmetic changes that enable plplot to compiled with (almost) no warnings, even with gcc -Wall.
 *
 * Most changes are just casts, and most of them are tcl/tk related. For tcl/tk-8.4, no warnings occurs.
 * Also tested with tcl/tk-8.3, where some warnings remain.
 * There are no java/f77/cxx/python/octave changes.
 *
 * Revision 1.6  2002/12/03 08:39:22  airwin
 * Merged AT branch into MAIN, a new configuration era has started
 *
 * Revision 1.5.2.1  2002/11/29 22:58:14  airwin
 * Finish AT tcl binding.  These are only small fairly non-consequential
 * refinements that I did while trying to debug the extended search problem
 * (which had a simple solution which I will discuss on list).
 *
 * Prepare the start of the tk and tk-x-plat bindings by dropping "plplot/"
 * from the #include statements in the source files.
 *
 * Revision 1.5  2002/07/12 09:31:38  vincentdarley
 * tcl interaction cleanup
 *
 * Revision 1.4  2002/07/11 08:52:03  vincentdarley
 * resolve name conflict between tk, tk-x-plat
 *
 * Revision 1.3  2002/07/10 09:52:38  vincentdarley
 * resolve name clashes, and sync pltools.tcl
 *
 * Revision 1.2  2002/07/04 15:14:54  vincentdarley
 * handle plplot version info directly
 *
 * Revision 1.1  2002/07/02 09:03:44  vincentdarley
 * x-platform tk code
 *
 * Revision 1.2.4.1  2000/07/28 17:48:10  airwin
 * AWI: pltk patch changes to existing files
 *
 * Revision 1.2  1998/11/19 04:32:46  furnish
 * Hack out a dumb little diagnostic which has been being printed for a
 * couple of years, but whose utility has long since expired.
 *
 * Revision 1.1  1996/10/25  19:21:21  furnish
 * New package bootstrap facility, contributed by Vince Darley, further
 * modified by Geoff Furnish.  Still needs work.
 *
 *
 * Revision 1.25  1996/11/7 vince

 * This file is derived from the old tkshell.c, and implements the Pltk init
 * function.  It can be loaded into any Tcl7.5 interpreter, and requires some
 * .tcl library files to be in known places.  Set your environment variables
 * to make sure of this.
 *
 * Old changes see 'tkshell.c'.  This file should be used _instead_ of
 * tkshell.c, if building the 'libPltk.so' shared library for dynamic loading
 * into Tcl.
 */

/*
 * tkshell.c
 * Maurice LeBrun
 * 6-May-93
 *
 * A miscellaneous assortment of Tcl support functions.
 */

#include "plserver.h"

#ifdef BUILD_Plplot
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_Vfs */

/*----------------------------------------------------------------------*\
 * Plplotter_Init
 *
 * Initialization routine for extended wish'es.
 * Creates the plframe, wait_until, and host_id (w/Tcl-DP only)
 * commands.  The more basic Plplot-Tcl initialization is handled by
 * the Plbasicinit function called from here.
\*----------------------------------------------------------------------*/

EXTERN int
Plplotter_Init( Tcl_Interp *interp )
{
#ifdef USE_TCL_STUBS
    /*
     * We hard-wire 8.1 here, rather than TCL_VERSION, TK_VERSION because
     * we really don't mind which version of Tcl, Tk we use as long as it
     * is 8.1 or newer.  Otherwise if we compiled against 8.2, we couldn't
     * be loaded into 8.1
     */
    Tcl_InitStubs(interp,"8.1",0);
#endif
#ifdef USE_TK_STUBS
    Tk_InitStubs(interp,"8.1",0);
#endif
    /* This must be before any other Tcl related calls */
    if (PlbasicInit(interp) != TCL_OK) {
	return TCL_ERROR;
    }

    /*
     * Note, the old technique of:
     *    main = Tk_MainWindow(interp);
     * and then passing main to 'plframe' as the clientdata can
     * cause unusual problems, probably due to bugs in Tk on
     * some platforms, when the 'main window' doesn't yet exist
     * properly by the time we are called.  Since plframe only
     * uses the value in one place (each time a new frame is
     * created), we simply use 'Tk_MainWindow' in plframe, and
     * avoid the startup problems.
     */

/* plframe -- PLplot graphing widget */

    Tcl_CreateCommand( interp, "plframe", (Tcl_CmdProc*) plPlotterCmd,
		       (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

    Tcl_PkgProvide(interp,"Plplotter",VERSION);
    return TCL_OK;
}

