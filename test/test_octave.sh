#!/bin/sh
# Test suite for octave demos.  
# This is called from plplot-test.sh with $octavedir, $driver, and 
# $dsuffix defined.

octave -f -q -p $octavedir//: <<EOF
#p7 # p7 works OK with plmeta, e.g., but not ps or psc. pleop/plbop issue?
for i=[1:6 8 9 13 15 16];
   figure(i,"$driver",sprintf("p%d.$dsuffix",i))
   feval(sprintf("p%d",i))
   closefig
endfor
EOF
