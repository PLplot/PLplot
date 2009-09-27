#!/bin/bash

# $Id$

# This script will run uncrustify on all source files registered in
# the lists below and summarize which uncrustified files are
# different.  Also there are options to view the differences in detail
# and/or apply them.  This script must be run from the top-level
# directory of the PLplot source tree.

# Copyright (C) 2009 Alan W. Irwin
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

usage()
{
echo '
Usage: ./style_source.sh [OPTIONS]

Options:
   [--view]         Show detailed style differences in source code.
   [--noview]       Summarize style differences in source code (default).
   [--apply]        Apply style differences to source code (POWERFUL).
   [--noapply]      Do not apply style differences to source code (default).
   [--help]         Show this message.

'
   exit $1
}

# Figure out what script options were specified by the user.
# Defaults
export view=OFF
export apply=OFF

while test $# -gt 0; do

    case $1 in
	--view)
	    view=ON
	    ;;
	--noview)
	    view=OFF
	    ;;
	--apply)
	    apply=ON
	    ;;
	--noapply)
	    apply=OFF
	    ;;
	--help)
            usage 0 1>&2
            ;;
	*)
            usage 1 1>&2
            ;;
    esac
    shift
done

version=`uncrustify --version`
if [ "$version" != "uncrustify 0.53" ] ; then
    echo "
Detected uncrustify version = '$version'.
This script only works with uncrustify 0.53."
    exit 1
fi

if [ ! -f src/plcore.c ] ; then 
    echo "This script can only be run from PLplot top-level source tree."
    exit 1
fi

if [ "$apply" = "ON" ] ; then
    echo '
The --apply option is POWERFUL and will replace _all_ files mentioned in
previous runs of style_source.sh with their uncrustified versions.
'
    ANSWER=
    while [ "$ANSWER" != "yes" -a "$ANSWER" != "no" ] ; do
	echo -n "Continue (yes/no)? "
	read ANSWER
    done
    if [ "$ANSWER" = "no" ] ; then
	echo "Immediate exit specified!"
	exit
    fi

fi

export csource_LIST="src/*.c src/*.h include/plplot.h"

export cppsource_LIST="bindings/c++/plstream.cc  bindings/c++/plstream.h include/qt.h"

for csource in $csource_LIST ; do
    uncrustify -c uncrustify.cfg -q -l c < $csource | cmp --quiet $csource -
    if [ "$?" -ne 0 ] ; then
	ls $csource
	if [ "$view" = "ON" ] ; then
	    uncrustify -c uncrustify.cfg -q -l c < $csource | diff -au $csource -
	fi

	if [ "$apply" = "ON" ] ; then
	    uncrustify -c uncrustify.cfg -q -l c --no-backup $csource
	fi
    fi
done

for cppsource in $cppsource_LIST ; do
    uncrustify -c uncrustify.cfg -q -l cpp < $cppsource | cmp --quiet $cppsource -
    if [ "$?" -ne 0 ] ; then
	ls $cppsource
	if [ "$view" = "ON" ] ; then
	    uncrustify -c uncrustify.cfg -q -l cpp < $cppsource | diff -au $cppsource -
	fi

	if [ "$apply" = "ON" ] ; then
	    uncrustify -c uncrustify.cfg -q -l cpp --no-backup $cppsource
	fi
    fi
done
