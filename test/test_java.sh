#!/bin/sh
# Test suite for java examples.  
# This is called from plplot-test.sh with $device, $dsuffix, 
# and $options defined.
# N.B. $CLASSPATH must be defined properly and the path must point
# to the java and javac commands for this to work.

# The java examples were automatically built in the make install step
# by cd $prefix/lib
# javac -d java java/plplot/examples/*.java.
# This is equivalent to
# javac -d $CLASSPATH $CLASSPATH/plplot/examples/*.java
# but means that $CLASSPATH does not have to be set at build
# or install time like it does to execute these pre-compiled examples.

# Do the standard non-interactive examples.
# skip 14, 17, and 20 because they are interactive and not implemented.
# skip 21 because it delivers variable results depending on computer timing
# and load and is not implemented.
for index in 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19; do
  java plplot.examples.x${index} -dev $device -o x${index}j.$dsuffix $options
done
