#!/bin/sh
# Test suite for f77demos.  
# This is called from plplot-test.sh with $f77dir, $driver, and 
# $dsuffix defined.
# Eventually, argument parsing will be added to the f77 api, and this
# file will be greatly simplified after that while allowing additional $options
# (such as familying for the png driver) to be enabled.
$f77dir/x01f <<EOF >/dev/null
$driver
x01f.$dsuffix
EOF
$f77dir/x02f <<EOF >/dev/null
$driver
x02f.$dsuffix
EOF
$f77dir/x03f <<EOF >/dev/null
$driver
x03f.$dsuffix
EOF
$f77dir/x04f <<EOF >/dev/null
$driver
x04f.$dsuffix
EOF
$f77dir/x05f <<EOF >/dev/null
$driver
x05f.$dsuffix
EOF
$f77dir/x06f <<EOF >/dev/null
$driver
x06f.$dsuffix
EOF
$f77dir/x07f <<EOF >/dev/null
$driver
x07f.$dsuffix
EOF
$f77dir/x08f <<EOF >/dev/null
$driver
x08f.$dsuffix
EOF
$f77dir/x09f <<EOF >/dev/null
$driver
x09f.$dsuffix
EOF
$f77dir/x10f <<EOF >/dev/null
$driver
x10f.$dsuffix
EOF
$f77dir/x11f <<EOF >/dev/null
$driver
x11f.$dsuffix
EOF
$f77dir/x12f <<EOF >/dev/null
$driver
x12f.$dsuffix
EOF
$f77dir/x13f <<EOF >/dev/null
$driver
x13f.$dsuffix
EOF
$f77dir/x16f <<EOF >/dev/null
$driver
x16f.$dsuffix
EOF
