#!/bin/sh
# Test suite for octave demos.  
# This is called from plplot-test.sh with $octavedir, $driver, and 
# $dsuffix defined.

octave -f -q -p $octavedir//: <<EOF
figure(1, "$driver", "octave.$dsuffix");
p1
p2
p3
p4
p5
p6
#p7 # p7 works OK with plmeta, e.g., but not ps or psc. pleop/plbop issue?
p8
p9
p13
p15
p16
closefig
EOF
