#!/bin/sh
# Test suite for Tcl demos.  
# This is called from plplot-test.sh with $tcldir, $installbindir, $driver, 
# $dsuffix, and $options defined.
export results=`pwd`
cd $tcldir
$installbindir/pltcl -dev $driver -o $results/plot.$dsuffix $options <<EOF
plinit
source plot.tcl
plot stats.log {1 4}
plot stats.log
plot r.dat
plot plot.dat
exit
EOF
$installbindir/pltcl -dev $driver -o $results/plgrid.$dsuffix $options <<EOF
plinit
source plgrid.tcl
plgrid
exit
EOF
$installbindir/pltcl -dev $driver -o $results/tcldemos.$dsuffix $options <<EOF
plinit
source tcldemos.tcl
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
exit
EOF
