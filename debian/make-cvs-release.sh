#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a PLplot source tree for a release <version>.cvs.<date>
# of Debian packages.  Run it from the top dir.

if test "$1" = "-u" ; then
  cvsrel=
else
  cvsrel=.cvs.`date '+%Y%m%d'`
fi

echo -n "Patching configure.ac ... "
perl -pi -e \
  's/^(AM_INIT_AUTOMAKE\(plplot, )(\d\.\d\.\d)([^)]*\))/$1$2'$cvsrel')/' \
  configure.ac
echo done

if test "$1" != "-u" ; then
  ./bootstrap.sh
  debian/rules config
  make dist
  version=`debian/get-upstream-version.pl < configure.ac`
  mv plplot-$version.tar.gz ../plplot_$version.orig.tar.gz
fi
