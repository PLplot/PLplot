#! /bin/bash

function run {
  echo -n Running `$1 --version | sed q`...
  $*
  echo " done"
}

aclocal_opts="$*"
aclocal_opts=${aclocal_opts:="-I /usr/local/share/libtool/libltdl"}

run aclocal $aclocal_opts \
  && run autoheader \
  && run automake --add-missing --copy \
  && run libtoolize --copy --ltdl --automake \
  && run autoconf
