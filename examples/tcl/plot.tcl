#----------------------------------------------------------------------------
# $Id$
# Interactive plotting driver for pltcl (plplot-enhanced tclsh)
# 
# Maurice LeBrun
# 10/18/95
# DejaNews, Inc.
#
# Generates a line plot given a file of ASCII data (similar to the GNUPLOT
# "plot" command).  Mostly for quicky plots, i.e. if you need something
# more sophisticated, just copy this and modify to taste!
#
# Usage:
#	plot [data file]
#
# Two powerful mechanisms are used to allow customization of the output.
# First, local plotting options may be specified in the data file itself.
# Comment lines of the form "# set <foo> <bar>" are stripped of their
# leading "# " and evaluated.  The local plot options are given below.
#
# Second, a global associative array -- plopts -- can be used to set the
# local plotting options as well.  E.g. to set "xlab", set plopt(xlab) to
# the desired value at global scope.  Values specified this way take
# precedence over those set in the data file in order to allow interactive
# customization without having to edit the data file.
#
# Local plot options are as follows:
#
#	Name	Meaning		Default value
#	----	-------		-------------
#	file	data file	(none) [1]
#	title	plot title	(none)
#	xlab	x-axis label	x
#	ylab	y-axis label	y
#	xopt	x axis options	bcnst (passed to plbox)
#	yopt	y axis options	bcnstv (same)
#	lsty	line style	1 [2]
#	poin	point symbol	-1 or 1, depending on value of lsty [3]
#	xmin	min x value	(data minimum - x)
#	xmax	max x value	(data maximum - x)
#	ymin	min y value	(data minimum - y)
#	ymax	max y value	(data maximum - y)
#
# control parameters:
#
#	noeop - if set, the plot is not ended (so you can overlay stuff)
#
# Notes:
# 1. The data file MUST be specified, either through a global variable
#    or via the invocation line.  Otherwise we return with an error.
#
# 2. Line styles are as follows:
#	 0 : none (only points plotted)
#	 1 : continuous
#	2-8: built-in plplot broken line pattern
#
# 3. Default is -1, which means do not plot points separately, however, if
#    a line style of -1 is specified, default is 1 (single point).  To see
#    all of the valid point types, refer to the manual.  Here's just a few:
#	0	box
#	1	dot
#	2	cross
#	3	asterisk
#	4	circle
#	5	x
#	6	box (what, again?)
#	7	triangle
#
#----------------------------------------------------------------------------
# $Log$
# Revision 1.3  1995/10/19 00:04:37  mjl
# Massively rewritten, is now quite powerful.  For a full features list, see
# the internal documentation.
#
#----------------------------------------------------------------------------

proc plot {{file {}}} {
    global plopt

# Make sure there is a data file

    if {$file == ""} {
	if {[info exists plopt(file)]} {
	    set file $plopt(file)
	} else {
	    error "Error: no data file specified"
	}
    }

# Set up plot option defaults

    set title	""
    set xlab	x
    set ylab	y
    set xopt	bcnst
    set yopt	bcnstv
    set lsty	1
    set poin	-1

# Turn off pause on xwin devices since it's really not necessary for a
# single plot application.

    plgdev device
    switch $device {
	xwin	-
	tk	-
	dp	{plspause 0}
    }

# Open file and read into data arrays

    set matlen 200
    set matinc 200
    matrix x f $matlen
    matrix y f $matlen

    set fileid [open $file]
    set n 0
    while {[expr [gets $fileid input] > -1]} {
	set firstchar [string index [lindex $input 0] 0]
	if {$firstchar == ""} continue
	if {$firstchar == "\#"} {
	    set nextword [lindex $input 1]
	    if {"$nextword" == "set"} {
		set command [lrange $input 1 end]
		eval $command
	    } else {
		continue
	    }
	}
	if {$n == $matlen} {
	    incr matlen $matinc
	    x redim $matlen
	    y redim $matlen
	}

	x $n = [lindex $input 0]
	y $n = [lindex $input 1]
	incr n
    }
    close $fileid

    puts "Read $n data points"

# Get data min/max

    if {[info exists xmin] == 0} {
	set xmin [x 0]
	for {set i 1} {$i < $n} {incr i} {
	    if {[expr [x $i] < $xmin]} then {set xmin [x $i]}
	}
    }

    if {[info exists xmax] == 0} {
	set xmax [x 0]
	for {set i 1} {$i < $n} {incr i} {
	    if {[expr [x $i] > $xmax]} then {set xmax [x $i]}
	}
    }

    if {[info exists ymin] == 0} {
	set ymin [y 0]
	for {set i 1} {$i < $n} {incr i} {
	    if {[expr [y $i] < $ymin]} then {set ymin [y $i]}
	}
    }

    if {[info exists ymax] == 0} {
	set ymax [y 0]
	for {set i 1} {$i < $n} {incr i} {
	    if {[expr [y $i] > $ymax]} then {set ymax [y $i]}
	}
    }

# Set up plot options

    if {[info exists plopt(title)]}	{set title $plopt(title)}
    if {[info exists plopt(xlab)]}	{set xlab $plopt(xlab)}
    if {[info exists plopt(ylab)]}	{set ylab $plopt(ylab)}
    if {[info exists plopt(xopt)]}	{set xopt $plopt(xopt)}
    if {[info exists plopt(yopt)]}	{set yopt $plopt(yopt)}
    if {[info exists plopt(lsty)]}	{set lsty $plopt(lsty)}
    if {[info exists plopt(poin)]}	{set poin $plopt(poin)}
    if {[info exists plopt(xmin)]}	{set xmin $plopt(xmin)}
    if {[info exists plopt(xmax)]}	{set xmax $plopt(xmax)}
    if {[info exists plopt(ymin)]}	{set ymin $plopt(ymin)}
    if {[info exists plopt(ymax)]}	{set ymax $plopt(ymax)}
    if {[info exists plopt(noeop)]}	{set noeop $plopt(noeop)}

# Set up the plot

    plbop

    plvpor 0.15 0.85 0.1 0.9
    plwind $xmin $xmax $ymin $ymax

    plcol 1
    pllsty 1
    plbox $xopt 0.0 0 $yopt 0.0 0

    plcol 2
    pllab "$xlab" "$ylab" "$title"

# Plot the data

    plcol 3

    if { $lsty > 0 } {
	pllsty $lsty
	plline $n x y

    } else {
	if { $poin < 0 } {set poin 1}
	plpoin $n x y $poin
    }

# End the page (see note about pause above).

    pllsty 1

    if {[info exists noeop] == 0} pleop
}
