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

java plplot.examples.x01 -dev $device -o x01j.$dsuffix $options
java plplot.examples.x02 -dev $device -o x02j.$dsuffix $options
java plplot.examples.x03 -dev $device -o x03j.$dsuffix $options
java plplot.examples.x04 -dev $device -o x04j.$dsuffix $options
java plplot.examples.x05 -dev $device -o x05j.$dsuffix $options
java plplot.examples.x06 -dev $device -o x06j.$dsuffix $options
java plplot.examples.x07 -dev $device -o x07j.$dsuffix $options
java plplot.examples.x08 -dev $device -o x08j.$dsuffix $options
java plplot.examples.x09 -dev $device -o x09j.$dsuffix $options
java plplot.examples.x10 -dev $device -o x10j.$dsuffix $options
java plplot.examples.x11 -dev $device -o x11j.$dsuffix $options
java plplot.examples.x12 -dev $device -o x12j.$dsuffix $options
java plplot.examples.x13 -dev $device -o x13j.$dsuffix $options
java plplot.examples.x14 -dev $device -o x14j.$dsuffix $options
java plplot.examples.x15 -dev $device -o x15j.$dsuffix $options
java plplot.examples.x16 -dev $device -o x16j.$dsuffix $options
java plplot.examples.x17 -dev $device -o x17j.$dsuffix $options
java plplot.examples.x18 -dev $device -o x18j.$dsuffix $options
java plplot.examples.x19 -dev $device -o x19j.$dsuffix $options
