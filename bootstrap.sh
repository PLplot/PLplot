#! /bin/bash
#
# Autootols bootstrap script for PLplot
#
# This file is part of PLplot, and is released under the GNU LGPL.
# No warranties.
#
# Copyright (C) 2003  Alan W. Irwin
# Copyright (C) 2003  Joao Cardoso
# Copyright (C) 2003  Rafael Laboissiere

function run {
  echo -n Running `$1 --version | sed q`...
  $*
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

curver=`grep ^AM_INIT_AUTOMAKE configure.ac \
        | perl -ne 'if (/plplot, (\d+\.\d+\.\d+)/) {print $1}'`

if [ $date_version = yes ] ; then
  version=$curver.cvs.`date '+%Y%m%d'`
fi

if [ -n "$version" ] ; then
  echo -n "Patching configure.ac (version $version)... "
  perl -pi -e \
    's/^(AM_INIT_AUTOMAKE\(plplot, )([^)]*\))/${1}'$version')/' \
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

run aclocal $aclocal_opts \
  && run autoheader \
  && run libtoolize --force --copy --ltdl --automake \
  && run automake --add-missing --copy \
  && run autoconf \
  && ( echo -n "Regenerating libltdl/configure..."; \
       cd libltdl ; \
       if [ ! -e configure.ac ] ; then \
           cp configure.in configure.ac ; \
           autoconf 2>/dev/null ; \
           rm -f configure.ac ; \
       else \
           autoconf 2>/dev/null ; \
       fi ; \
       echo " done" )
