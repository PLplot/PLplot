#!/bin/sh
# Test suite for f77 examples.  
# This is called from plplot-test.sh with $f77dir, $device, and 
# $dsuffix defined.

# To build the f77 examples before running this script do the following:
# pushd $f77dir; make; popd 

# Eventually, argument parsing will be added to the f77 api, and this
# file will be greatly simplified after that while allowing additional $options
# (such as familying for the png device) to be enabled.
$f77dir/x01f <<EOF >/dev/null
$device
x01f.$dsuffix
EOF
$f77dir/x02f <<EOF >/dev/null
$device
x02f.$dsuffix
EOF
$f77dir/x03f <<EOF >/dev/null
$device
x03f.$dsuffix
EOF
$f77dir/x04f <<EOF >/dev/null
$device
x04f.$dsuffix
EOF
$f77dir/x05f <<EOF >/dev/null
$device
x05f.$dsuffix
EOF
$f77dir/x06f <<EOF >/dev/null
$device
x06f.$dsuffix
EOF
$f77dir/x07f <<EOF >/dev/null
$device
x07f.$dsuffix
EOF
$f77dir/x08f <<EOF >/dev/null
$device
x08f.$dsuffix
EOF
$f77dir/x09f <<EOF >/dev/null
$device
x09f.$dsuffix
EOF
$f77dir/x10f <<EOF >/dev/null
$device
x10f.$dsuffix
EOF
$f77dir/x11f <<EOF >/dev/null
$device
x11f.$dsuffix
EOF
$f77dir/x12f <<EOF >/dev/null
$device
x12f.$dsuffix
EOF
$f77dir/x13f <<EOF >/dev/null
$device
x13f.$dsuffix
EOF
$f77dir/x16f <<EOF >/dev/null
$device
x16f.$dsuffix
EOF
