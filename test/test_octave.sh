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

#plot equivalent of x??c examples.
#For file output 14 and 17 are not suitable, and 19 is not done
#(and should probably be dropped anyway since the map stuff is not
#in the API that is supposed to be common to all front ends.)
for i=[1:13 15 16 18];
    cmd = sprintf("x%.2dc",i);
    t = split("$options", "-"); t(1,:)="";
    for j=1:rows(t)
        tt = deblank (t(j,:)); len = length(tt);
	ix = index(tt," ");
	if (ix == 0); ix = len; len = 0; endif
	arg = tt(1:ix)
	value = tt(ix:len)
	plSetOpt(tt(1:ix), tt(ix:len));
    endfor
    device="$driver";
    plSetOpt("dev", device);
#o trailer on filename e.g., x01o.ps) to distinguish from other 
#common examples.
    file = sprintf("x%.2do.$dsuffix",i);
    plSetOpt("o", file);
    eval(cmd);
endfor
EOF
