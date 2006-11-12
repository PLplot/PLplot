#!/bin/bash

# This will prepare a distribution tarball directly from the CVS sources of
# the PLplot project.
# CAVEAT: this script silently and completely deletes a directory called
# /tmp/plplot-dist-prep

# Copyright (C) 2003, 2004  Rafael Laboissiere
# Copyright (C) 2006 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# $Id$

usage () {
  local prog=`basename $0`
  echo "Usage: $prog [-n] [-u user] [-w remote dir] [-r branch] \\"
  echo "          [-c [-i prefix] [-o cmake options]] \\"
  echo "       $prog -d"
  echo "       $prog -h"
  echo
  echo "Option -n prevents building of the DocBook manual."
  echo "Option -u sets the user name used for CVS and SourceForge access."
  echo "Option -w sets the CVS root directory."
  echo "Option -r sets the -r option (revision tag) used for CVS retrieval."
  echo "When option -c is given, the generated tarball is"
  echo "  unpacked, configured with cmake and built with make, and"
  echo "  the ctest (build-tree tests) is run afterward."
  echo "  If the -i prefix option is specified in addition to -c,"
  echo "  the configuration is done with the specified install prefix"
  echo "  and make install is run after ctest."
  echo "  If the -o cmake options option is specified in addition to -c,"
  echo "  those options are used for the configuration of the unpacked"
  echo "  tarball."
  echo "Option -d prints the default values."
  echo "Option -h prints this usage information."
  exit $1
}

DOC_ARG=${DOC_ARG:--DBUILD_DOC=ON}
WWW_USER=${WWW_USER:-hbabcock}
CVSROOTDIR=${CVSROOTDIR:-plplot.cvs.sourceforge.net:/cvsroot/plplot}
BRANCH=${BRANCH:--D now}
# Just easier to always keep this temporary directory in the same location
CVSTMPDIR=/tmp/plplot-dist-prep

# Put here extra cmake options that should always be used when
# generating a tarball
config_opt=""

print_defaults () {
  local v
  for v in DOC_ARG WWW_USER CVSROOTDIR BRANCH ; do
    eval "echo $v=\\\"\$$v\\\""
  done
  exit 0
}

bypass_rtag=no
do_check=no
prefix=""

while getopts "bcdhi:no:r:u:w:" option
do
  case $option in
    b) bypass_rtag=yes ;;
    c) do_check=yes ;;
    d) print_defaults ;;
    h) usage 0 ;;
    i) test -n "$OPTARG" || usage 1 ; prefix=$OPTARG ;;
    n) DOC_ARG= ;;
    o) config_opt="$config_opt $OPTARG" ;;
    r) test -n "$OPTARG" || usage 1 ; BRANCH="-r $OPTARG" ;;
    u) test -n "$OPTARG" || usage 1 ; WWW_USER=$OPTARG ;;
    w) test -n "$OPTARG" || usage 1 ; CVSROOTDIR=$OPTARG ;;
    *) usage 1 ;;
  esac
done

cleanup ( ) {
    rm -rf $CVSTMPDIR
}

#trap "cleanup" 0 HUP INT QUIT PIPE TERM

test $bypass_rtag = yes \
  || date_tag=cvs-tarball_`date --utc +%Y-%m-%d-%H-%M-%S` \
  && echo "Tagging repository with $date_tag" \
  && cvs -d${WWW_USER}@$CVSROOTDIR rtag $BRANCH $date_tag plplot \
  && BRANCH="-r $date_tag"

cleanup
mkdir $CVSTMPDIR
cd $CVSTMPDIR
cvs -d${WWW_USER}@$CVSROOTDIR export $BRANCH plplot \
  && cd plplot \
  && cf/bootstrap.sh >& bootstrap.sh.out \
  && mkdir ../build_dir \
  && cd ../build_dir \
  && cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
     -DPREBUILD_DIST=ON ${DOC_ARG} \
     -DWWW_USER=${WWW_USER} ../plplot >& cmake.out \
  && echo "Making distribution." \
  && make prebuild_dist >& make_prebuild_dist.out \
  && make package_source >& make_package_source.out \
  && TARBALL=`ls plplot-*.tar.gz` \
  && DISTDIR=`echo $TARBALL | sed s/.tar.gz//` \
  && mv $TARBALL .. \
  && cd .. \
  && echo "CVS distribution tarball: $TARBALL" \
  && test "$do_check" = yes \
  && tar xfz $TARBALL \
  && mkdir ctest_build_dir \
  && ( cd ctest_build_dir \
       && cmake ${config_opt} -DCMAKE_INSTALL_PREFIX=${prefix} \
          -DCMAKE_VERBOSE_MAKEFILE=ON \
	  -DWWW_USER=${WWW_USER} -DBUILD_TEST=ON \
	  ../plplot >& cmake.out \
       && make >& make.out \
       && ctest >& ctest.out \
       && test -n "$prefix" \
       && rm -rf ${prefix} \
       && make install >& make_install.out )
