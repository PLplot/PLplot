#----------------------------------------------------------------------------
# "plot" proc.
#
# A minimal implementation of the GNUPLOT "plot" command.
#----------------------------------------------------------------------------

proc plot {file {title {}}} {

# Open and read file

    set fileid [open $file]
    set contents [read $fileid]

# Fill up the data arrays

    set n [expr [llength $contents] / 2]
    matrix x f $n
    matrix y f $n

    for {set i 0} {$i < $n} {incr i} {
	x $i = [lindex $contents [expr 2*$i]]
	y $i = [lindex $contents [expr 2*$i+1]]
    }

# Get data min/max

    set xmin [x 0]; set xmax [x 0]
    set ymin [y 0]; set ymax [y 0]

    for {set i 1} {$i < $n} {incr i} {
	if {[expr [x $i] < $xmin]} then {set xmin [x $i]}
	if {[expr [x $i] > $xmax]} then {set xmax [x $i]}
	if {[expr [y $i] < $ymin]} then {set ymin [y $i]}
	if {[expr [y $i] > $ymax]} then {set ymax [y $i]}
    }

# Set up the plot

    plcol 1
    plenv $xmin $xmax $ymin $ymax 0 0
    plcol 2
    pllab "x" "y" "$title"

# Plot the data

    plcol 3
    plline $n x y

# An eop is nice here.

    pleop
}
