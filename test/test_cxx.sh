#!/bin/sh
# Test suite for c++ examples.  
# This is called from plplot-test.sh with $cxxdir, $device, $dsuffix,
# and $options defined.

# To build the c++ examples before running this script do the following:
# pushd $cxxdir; make; popd 

$cxxdir/x01cc -dev $device -o x01cc.$dsuffix $options
$cxxdir/x01 -dev $device -o x01cxx.$dsuffix $options
$cxxdir/x02 -dev $device -o x02cxx.$dsuffix $options
$cxxdir/x03 -dev $device -o x03cxx.$dsuffix $options
$cxxdir/x04 -dev $device -o x04cxx.$dsuffix $options
$cxxdir/x05 -dev $device -o x05cxx.$dsuffix $options
$cxxdir/x06 -dev $device -o x06cxx.$dsuffix $options
$cxxdir/x07 -dev $device -o x07cxx.$dsuffix $options
$cxxdir/x08 -dev $device -o x08cxx.$dsuffix $options
$cxxdir/x09 -dev $device -o x09cxx.$dsuffix $options
$cxxdir/x10 -dev $device -o x10cxx.$dsuffix $options
$cxxdir/x11 -dev $device -o x11cxx.$dsuffix $options
$cxxdir/x12 -dev $device -o x12cxx.$dsuffix $options
$cxxdir/x13 -dev $device -o x13cxx.$dsuffix $options
#skip x14 since uses tk device.
$cxxdir/x15 -dev $device -o x15cxx.$dsuffix $options
$cxxdir/x16 -dev $device -o x16cxx.$dsuffix $options
#skip x17 which is only meant for interactive execution
$cxxdir/x18 -dev $device -o x18cxx.$dsuffix $options
$cxxdir/x19 -dev $device -o x19cxx.$dsuffix $options
#skip x20 which is only meant for interactive execution
$cxxdir/x21 -dev $device -o x21cxx.$dsuffix $options
