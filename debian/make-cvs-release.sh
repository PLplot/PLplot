#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a PLplot source tree for a release <version>.cvs.<date>
# of Debian packages.  Run it from the top dir.

today=`date '+%Y%m%d'`

echo -n "Patching configure.ac ... "
perl -pi -e \
  's/^(AM_INIT_AUTOMAKE\(plplot, )(\d\.\d\.\d)([^)]+\))/$1$2'.cvs.$today')/' \
  configure.ac
echo done

version=`perl -ne \
           'if(/^AM_INIT_AUTOMAKE\(plplot, ([^)]+)\)/){print $1;last}' \
           < configure.ac`

echo -n "Patching debian/rules ... "
perl -pi -e 's/(^version\s*:=\s*)([^\s]+)/${1}'$version'/' debian/rules
echo done

./bootstrap.sh
debian/rules config
make dist
mv plplot-$version.tar.gz ../plplot_$version.orig.tar.gz
