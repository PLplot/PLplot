#! /bin/bash

function run {
  echo -n Running `$1 --version | sed q`...
  $*
  echo " done"
}
  
run aclocal \
  && run libtoolize --copy --ltdl --automake \
  && run autoheader \
  && run automake --add-missing --copy \
  && run autoconf
