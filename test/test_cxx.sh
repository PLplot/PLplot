#!/bin/sh
# Test suite for cxxdemos.  
# This is called from plplot-test.sh with $cxxdir, $driver, and 
# $dsuffix defined.
# Eventually, argument parsing will be added to the c++ api, and this
# file will be greatly simplified after that while allowing additional $options
# (such as familying for the png driver) to be enabled.
$cxxdir/x01cc <<EOF >/dev/null
$driver
x01cc.$dsuffix
EOF
