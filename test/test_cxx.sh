#!/bin/sh
# Test suite for c++ examples.  
# This is called from plplot-test.sh with $cxxdir, $device, $dsuffix,
# and $options defined.

# To build the c++ examples before running this script do the following:
# pushd $cxxdir; make; popd 

# x01cc is a C++ alternative that has no corresponding examples for the
# other front ends.
$cxxdir/x01cc -dev $device -o x01cc.$dsuffix $options

# Do the standard non-interactive examples.
# skip 14, 17, and 20 because they are interactive.
# skip 21 because it delivers variable results depending on computer timing
# and load.
for index in 01 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19; do
  $cxxdir/x${index} -dev $device -o x${index}cxx.$dsuffix $options
done
