#! /bin/bash

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

while test $# -gt 0 ; do
  case $1 in
  --version=*)
    version=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'`
    ;;
  --date-version)
    date_version=yes
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

curver=`grep ^AM_INIT_AUTOMAKE configure.ac \
        | perl -ne 'if (/plplot, (\d+\.\d+\.\d+)/) {print $1}'`

if [ $date_version = yes ] ; then
  version=$curver.cvs.`date '+%Y%m%d'`
fi

if [ -n "$version" ] ; then
  echo -n "Patching configure.ac ... "
  perl -pi -e \
    's/^(AM_INIT_AUTOMAKE\(plplot, )([^)]*\))/${1}'$version')/' \
    configure.ac
  echo done
fi

aclocal_opts=${aclocal_opts:="-I /usr/share/libtool/libltdl"}

run aclocal $aclocal_opts \
  && run autoheader \
  && run automake --add-missing --copy \
  && run libtoolize --copy --ltdl --automake \
  && run autoconf
