# $Id$
# $Log$
# Revision 1.1.2.1  2001/04/19 12:31:45  rlaboiss
# First merge against MAIN
#
# Revision 1.1  1993/12/21 10:34:17  mjl
# Added to be a customizable initialization proc for the TK driver.
# Represents a much simpler way to change the behavior than the previous
# (through plplot flags) method.  It requires some more work, however.
#

#----------------------------------------------------------------------------
# pltk_init
#
# Initializes variables for plplot/TK driver.
# These variables determine what plserver procs are invoked by the driver
# to do various jobs.  Specifically, they are:
#
# plserver_init_proc	proc to initialize plserver
# plw_create_proc	proc to create the plframe widget
# plw_init_proc		proc to initialize the plframe widget
# plw_flash_proc	proc to handle end of page condition
# plw_end_proc		proc to detach the plframe widget from the client 
#----------------------------------------------------------------------------

proc pltk_init {} {

    global plserver_init
    global plw_create plw_init plw_flash plw_end

# This initializes plserver

    set plserver_init_proc	plserver_init

# These govern the plframe and associated widgets

    set plw_create_proc		plw_create
    set plw_init_proc		plw_init
    set plw_flash_proc		plw_flash
    set plw_end_proc 		plw_end
}
