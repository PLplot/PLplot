#!/bin/sh
# Test suite for Python demos.  
# This is called from plplot-test.sh with $pythondir, $device, $dsuffix, 
# and $options defined.
$pythondir/pythondemos.py -dev $device -o pythondemos.$dsuffix $options
