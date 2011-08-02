#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a PLplot source tree for a release <version>.cvs.<date>
# of Debian packages.  Run it from the top dir.

test -f Makefile && make maintainer-clean
if [ "$1" = "" ] ; then
  optver=--date-version
else
  optver="--version=$1"
fi
./bootstrap.sh $optver
rm -f config.status
fakeroot debian/rules clean
./configure --enable-builddoc --enable-python --enable-octave --enable-f77
make dist
version=`debian/get-upstream-version.pl < configure.ac`
mv plplot-$version.tar.gz ../plplot_$version.orig.tar.gz
