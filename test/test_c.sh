#!/bin/sh
# Test suite for c examples.  
# This is called from plplot-test.sh with $cdir, $device, $dsuffix, 
# and $options defined.

# To build the c examples before running this script do the following:
# pushd $cdir; make; popd 

$cdir/x01c -dev $device -o x01c.$dsuffix $options
$cdir/x02c -dev $device -o x02c.$dsuffix $options
$cdir/x03c -dev $device -o x03c.$dsuffix $options
$cdir/x04c -dev $device -o x04c.$dsuffix $options
$cdir/x05c -dev $device -o x05c.$dsuffix $options
$cdir/x06c -dev $device -o x06c.$dsuffix $options
$cdir/x07c -dev $device -o x07c.$dsuffix $options
$cdir/x08c -dev $device -o x08c.$dsuffix $options
$cdir/x09c -dev $device -o x09c.$dsuffix $options
$cdir/x10c -dev $device -o x10c.$dsuffix $options
$cdir/x11c -dev $device -o x11c.$dsuffix $options
$cdir/x12c -dev $device -o x12c.$dsuffix $options
$cdir/x13c -dev $device -o x13c.$dsuffix $options
#skip x14c since uses tk device.
$cdir/x15c -dev $device -o x15c.$dsuffix $options
$cdir/x16c -dev $device -o x16c.$dsuffix $options
#skip x17c which is only meant for interactive execution
$cdir/x18c -dev $device -o x18c.$dsuffix $options
$cdir/x19c -dev $device -o x19c.$dsuffix $options
#skip x20c which is only meant for interactive execution
$cdir/x21c -dev $device -o x21c.$dsuffix $options
