#!/bin/sh
# Test suite for Python demos.  
# This is called from plplot-test.sh with $pythondir, $driver, $dsuffix, 
# and $options defined.
$pythondir/pythondemos.py -dev $driver -o pythondemos.$dsuffix $options
