#!/bin/sh
# Test suite for c++ examples.  
# This is called from plplot-test.sh with $cxxdir, $device, and 
# $dsuffix defined.

# To build the c++ examples before running this script do the following:
# pushd $cxxdir; make; popd 

# Eventually, argument parsing will be added to the c++ api, and this
# file will be greatly simplified after that while allowing additional $options
# (such as familying for the png device) to be enabled.
$cxxdir/x01cc <<EOF >/dev/null
$device
x01cc.$dsuffix
EOF
