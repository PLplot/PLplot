#!/bin/sh
# Test suite for Tcl examples.
#
# Copyright (C) 2004  Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

# This is called from plplot-test.sh with $tcldir, $device, $dsuffix,
# and $options defined.
results=`pwd`
export results
cd $tcldir
pltcl -dev $device -o $results/plot.$dsuffix $options <<EOF
plinit
source plot.tcl
plot stats.log {1 4}
plot stats.log
plot r.dat
plot plot.dat
exit
EOF
pltcl -dev $device -o $results/plgrid.$dsuffix $options <<EOF
plinit
source plgrid.tcl
plgrid
exit
EOF
# Skip 14th example because requires two output files.
# Skip 17th example because it is not implemented (and if it was, it
# would be interactive only).
pltcl -dev $device -o $results/tcldemos.$dsuffix $options <<EOF
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
15
16
18
exit
EOF
