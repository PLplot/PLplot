#!/bin/sh
# Test suite for octave demos.  
# This is called from plplot-test.sh with $octavedir, $driver, 
# $dsuffix and $options defined.
# Assumes that $option starts and are '-' separated.

# WARNING, 'octave' can be defined at 'configure' time, to
# allow for different installed versions. (work in progress)

octave -f -q -p $octavedir//: <<EOF
plplot_stub;
t = split("$options", "-"); t(1,:)="";
for i=1:rows(t)
    tt = deblank (t(i,:)); len = length(tt);
    ix = index(tt," ");
    if (ix == 0); ix = len; len = 0; endif
    plsetopt(tt(1:ix), tt(ix:len));
endfor

# p7 works OK with plmeta, e.g., but not ps or psc. pleop/plbop issue?
for i=[1:7 8 9 13 15 16];
   figure(i,"$driver",sprintf("p%d.$dsuffix",i));
   feval(sprintf("p%d",i))
   closefig
endfor
EOF
