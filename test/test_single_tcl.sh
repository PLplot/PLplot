#!/bin/sh
# usage:
# ./test_single_tcl.sh 01
# n.b. must use two digits to specify any number between 01 and 18, to get 
# any of the first, through 18th examples.
number=$1
shortnumber=$number
if test ${number:0:1} == "0"; then shortnumber=${number:1:1}; fi
./pltcl -dev psc -o x$number\t.ps <<EOF
plinit
source tcldemos.tcl
[format "%d" $shortnumber]
exit
EOF
