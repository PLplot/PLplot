# $Id$
# $Log$
# Revision 1.2  1994/01/17 21:30:23  mjl
# Improved security for Tcl-DP communication by disabling any further
# connections after the initial client/server handshaking is finished.
#
# Revision 1.1  1994/01/15  17:36:03  mjl
# Added to hold client side initialization procs.
#
#----------------------------------------------------------------------------
# PLPLOT TK/TCL graphics client procs
# Maurice LeBrun
# 14-Jan-1994
# IFS, University of Texas at Austin
#
# These are customizable (since autoloaded) initialization procs for the
# TK driver.  At present you should be very careful what you change,
# however, since this stuff is still evolving.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# plclient_init
#
# Initializes variables for plplot/TK/DP driver.
# These variables determine what plserver procs are invoked by the driver
# to do various jobs.  Specifically, they are:
#
# plserver_init_proc	proc to initialize plserver
# plw_create_proc	proc to create the plframe widget
# plw_init_proc		proc to initialize the plframe widget
# plw_flash_proc	proc to handle end of page condition
# plw_end_proc		proc to detach the plframe widget from the client 
#----------------------------------------------------------------------------

proc plclient_init {} {

    global plserver_init_proc plw_create_proc plw_init_proc \
	plw_start_proc plw_flash_proc plw_end_proc \
	dp client_host client_port update_proc

# This initializes plserver

    set plserver_init_proc	plserver_init

# These govern the plframe and associated widgets

    set plw_create_proc		plw_create
    set plw_init_proc		plw_init
    set plw_start_proc		plw_start
    set plw_flash_proc		plw_flash
    set plw_end_proc 		plw_end

# Set up communications port and other junk

    if { $dp } {
	set client_host [host_id]
	set client_port [dp_MakeRPCServer]
	set update_proc dp_update

    } else {
	set update_proc update
    }
}

#----------------------------------------------------------------------------
# plclient_link_init
#
# Initializes link to plserver assuming plserver already exists.
# Basically this means just sending info on the communications channel.
#----------------------------------------------------------------------------

proc plclient_link_init {} {
    global dp server client

    if { $dp } {
	global client_host client_port

	dp_RDO $server set client_host $client_host
	dp_RDO $server set client_port $client_port

    } else {
	global client_name

	send [list $server] "after 1 set client_name [list $client_name]"
    }

    wait_until {[info exists client]}
}

#----------------------------------------------------------------------------
# plclient_dp_init
#
# Initializes dp data link.
# In addition, as this is the last client/server connection needed, I
# disable further connections.
#----------------------------------------------------------------------------

proc plclient_dp_init {} {
    global server server_host plwindow client data_port data_sock 

    dp_RDO $server plw_dplink $plwindow $client

    wait_until {[info exists data_port]}
    set data_sock [lindex [dp_connect $server_host $data_port] 0]

    dp_Host -
}
