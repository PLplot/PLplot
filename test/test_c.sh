#!/bin/sh
# Test suite for cdemos.  
# This is called from test.sh with $cdir, $driver, $dsuffix, and $options 
# defined.
$cdir/x01c -dev $driver -o x01c.$dsuffix $options
$cdir/x02c -dev $driver -o x02c.$dsuffix $options
$cdir/x03c -dev $driver -o x03c.$dsuffix $options
$cdir/x04c -dev $driver -o x04c.$dsuffix $options
$cdir/x05c -dev $driver -o x05c.$dsuffix $options
$cdir/x06c -dev $driver -o x06c.$dsuffix $options
$cdir/x07c -dev $driver -o x07c.$dsuffix $options
$cdir/x08c -dev $driver -o x08c.$dsuffix $options
$cdir/x09c -dev $driver -o x09c.$dsuffix $options
$cdir/x10c -dev $driver -o x10c.$dsuffix $options
$cdir/x11c -dev $driver -o x11c.$dsuffix $options
$cdir/x12c -dev $driver -o x12c.$dsuffix $options
$cdir/x13c -dev $driver -o x13c.$dsuffix $options
#skip x14c since uses tk driver.
$cdir/x15c -dev $driver -o x15c.$dsuffix $options
$cdir/x16c -dev $driver -o x16c.$dsuffix $options
$cdir/x17c -dev $driver -o x17c.$dsuffix $options
$cdir/x18c -dev $driver -o x18c.$dsuffix $options
$cdir/x19c -dev $driver -o x19c.$dsuffix $options
