# $Id$
# $Log$
# Revision 1.1  1993/08/03 20:35:40  mjl
# A demo of alternate plserver startup.  Just sketched out so far.
#

#----------------------------------------------------------------------------
# PLPLOT TK/TCL demo: alternative to standard startup
# Maurice LeBrun
# 3-Aug-1993
# IFS, University of Texas at Austin
#----------------------------------------------------------------------------

set plserver_init_proc tkdemo1_init

#----------------------------------------------------------------------------
# tkdemo1_init
# Create the main window, menus, etc.
#----------------------------------------------------------------------------

proc tkdemo1_init {} {
    
    plserver_init
    plw_create .plw
}
