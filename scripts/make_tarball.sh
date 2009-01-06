#!/bin/bash

# This will prepare a distribution tarball directly from the svn sources of
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
  echo "Usage: $prog -t tagname [-n] [-w URL] \\"
  echo "          [-c [-i prefix] [-o cmake options]] \\"
  echo "       $prog -d"
  echo "       $prog -h"
  echo
  echo "Option -t is mandatory and is used for specifying the subdirectory"
  echo "  of the svn URL.  By default the svn URL points to the tags"
  echo "  subdirectory so normally the -t option is a version tag"
  echo "  (e.g., v5_7_4) used for the _already committed_ tags subdirectory"
  echo "  of the release existing at the SourceForge svn repository."
  echo "Option -n prevents pre-building anything including the DocBook manual."
  echo "Option -w sets the svn repository URL."
  echo "When option -c is given, the generated tarball is"
  echo "  unpacked, configured with cmake and built with make, and"
  echo "  the ctest (build-tree tests) is run afterward."
  echo "  If the -i prefix option is specified in addition to -c,"
  echo "  the configuration is done with the specified install prefix"
  echo "  and make install is run after ctest."
  echo "  WARNING: the prefix directory is completely removed before the"
  echo "  install so be careful what you specify for the -i option."
  echo "  If the -o cmake options option is specified in addition to -c,"
  echo "  those options are used for the configuration of the unpacked"
  echo "  tarball."
  echo "Option -d prints the default values."
  echo "Option -h prints this usage information."
  exit $1
}

DOC_ARG=${DOC_ARG:--DBUILD_DOC=ON}
SVN_URL=${SVN_URL:-https://plplot.svn.sourceforge.net/svnroot/plplot/tags}
# Just easier to always keep this temporary directory in the same location
SVNTMPDIR=/tmp/plplot-dist-prep

# Put here extra cmake options that should always be used when
# generating a tarball
config_opt=""

# Put here extra make options
# Parallel builds for extra speed on multiprocessing boxes
make_opt="-j3"

print_defaults () {
  local v
  for v in DOC_ARG SVN_URL ; do
    eval "echo $v=\\\"\$$v\\\""
  done
  exit 0
}

do_check=no
prefix=""
PREBUILD_ARG="-DPREBUILD_DIST=ON"
PREBUILT_DOC_ARG="-DPREBUILT_DOC=ON"
do_prebuild_dist=yes

while getopts "cdhi:no:t:u:w:" option
do
  case $option in
    c) do_check=yes ;;
    d) print_defaults ;;
    h) usage 0 ;;
    i) test -n "$OPTARG" || usage 1 ; prefix=$OPTARG ;;
    n) DOC_ARG=;PREBUILD_ARG=;PREBUILT_DOC_ARG=;do_prebuild_dist=no ;;
    o) config_opt="$config_opt $OPTARG" ;;
    t) test -n "$OPTARG" || usage 1 ; TAG=$OPTARG ;;
    w) test -n "$OPTARG" || usage 1 ; SVN_URL=$OPTARG ;;
    *) usage 1 ;;
  esac
done

test -n "$TAG" || usage 1

cleanup ( ) {
    rm -rf $SVNTMPDIR
}

#trap "cleanup" 0 HUP INT QUIT PIPE TERM

cleanup
mkdir $SVNTMPDIR
cd $SVNTMPDIR
svn export $SVN_URL/$TAG plplot \
  && mkdir build_dir \
  && cd build_dir \
  && cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
     ${PREBUILD_ARG} ${DOC_ARG} \
     -DWWW_USER:STRING=${WWW_USER} ../plplot >& cmake.out \
  && echo "Making distribution." \
  && (if [ "$do_prebuild_dist" = "yes" ] ; then
        make ${make_opt} prebuild_dist >& make_prebuild_dist.out
      fi) \
  && make ${make_opt} package_source >& make_package_source.out \
  && TARBALL=`ls plplot-*.tar.gz` \
  && DISTDIR=`echo $TARBALL | sed s/.tar.gz//` \
  && mv $TARBALL .. \
  && cd .. \
  && echo "distribution tarball: $TARBALL" \
  && test "$do_check" = "yes" \
  && tar xfz $TARBALL \
  && mkdir ctest_build_dir \
  && ( cd ctest_build_dir \
       && cmake ${config_opt} -DCMAKE_INSTALL_PREFIX:PATH=${prefix} \
          -DCMAKE_VERBOSE_MAKEFILE=ON ${PREBUILT_DOC_ARG} \
	  -DWWW_USER:STRING=${WWW_USER} -DBUILD_TEST=ON \
	  ../plplot >& cmake.out \
       && make ${make_opt} >& make.out \
       && ctest >& ctest.out \
       && test -n "$prefix" \
       && rm -rf ${prefix} \
       && make ${make_opt} install >& make_install.out )
