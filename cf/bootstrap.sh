#! /bin/bash
#
# Autootols bootstrap script for PLplot
#
# Copyright (C) 2003, 2004  Alan W. Irwin
# Copyright (C) 2003  Joao Cardoso
# Copyright (C) 2003, 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

# Filter "underquoted definitions" warnings from aclocal output.  This
# should not be necessary in teh distant future (today is 2004-02),
# when all the third party m4 macros will be properly quoted.

function filter {
  fgrep -v underquoted | fgrep -v "run info" | fgrep -v sources.redhat
  return 0
}

function run {
  echo -n Running `$1 --version | sed 1q`...
  $* 2>&1 | filter
  echo " done"
}

function usage {
  cat <<EOF
Usage: $0 [OPTIONS] [ACLOCAL OPTIONS]
Options:
  --version=VER
  --date-version
  --help
aclocal options usually look like:
  -I /usr/share/libtool/libltdl
EOF
  exit 0
}

# Check for Automake version >= 1.8.2
automake --version | sed 1q \
  | perl -ne '/((\d+)\.(\d+)\.(\d+))/; \
      die "'$0': Automake version is $1.  Version 1.8.2 or later is needed\n"
        if $2<1 or ($2==1 and $3<8) or ($2==1 and $3==8 and $4<2); \
      exit 0' || exit 1

version=""
aclocal_opts=""
date_version=no
set_date=no

while test $# -gt 0 ; do
  case $1 in
  --version=*)
    version=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'`
    set_date=yes
    ;;
  --date-version)
    date_version=yes
    set_date=yes
    ;;
  --help)
    usage
    ;;
  *)
    aclocal_opts="$aclocal_opts $1"
    ;;
  esac
  shift
done

if test "$aclocal_opts" = "" ; then
  prefix=`which libtool | sed 's:/bin/libtool::'`
  if test -n "$prefix" ; then
    acdir=${prefix}/share/libtool/libltdl
    if test -d $acdir ; then
      if test -f ${acdir}/aclocal.m4 ; then
        aclocal_opts="-I $acdir"
      fi
    fi
  fi
fi

aclocal_opts=${aclocal_opts:="-I /usr/share/libtool/libltdl"}

echo Using aclocal options: $aclocal_opts

curver=`grep ^AC_INIT configure.ac \
        | perl -ne 'if (/plplot, (\d+\.\d+\.\d+)/) {print $1}'`

if [ $date_version = yes ] ; then
  version=$curver.cvs.`date '+%Y%m%d'`
fi

if [ -n "$version" ] ; then
  echo -n "Patching configure.ac (version $version)... "
  perl -pi -e \
    's/^(AC_INIT\(plplot, )([^,)]+)/${1}'$version'/' \
    configure.ac
  echo done
fi

if [ $set_date = yes ] ; then
  date=`date +%Y-%m-%d`
  echo -n "Patching configure.ac (release date $date)... "
  perl -pi -e 's/^(RELEASE_DATE=).*/${1}'$date'/' configure.ac
  echo done
fi

aclocal_opts=${aclocal_opts:="-I /usr/share/libtool/libltdl"}

# aclocal -I option below must be given the absolute path of the cf/ dir,
# otherwise it is not considered as "external" to the project (this is
# the case for aclocal-1.8, at least)

run aclocal -I `pwd`/cf $aclocal_opts \
  && run autoheader \
  && rm -rf libltdl \
  && run libtoolize --force --copy --ltdl --automake \
  && run automake --add-missing --copy \
  && run autoconf \
  && ( echo -n "Regenerating libltdl/aclocal+configure..."; \
       cd libltdl ; \
       aclocal $aclocal_opts 2>&1 | filter && \
       automake ; \
       if [ ! -e configure.ac ] ; then \
           cp configure.in configure.ac ; \
           autoconf 2>/dev/null ; \
           rm -f configure.ac ; \
       else \
           autoconf 2>/dev/null ; \
       fi && \
       echo " done" )
