#! /bin/sh

aclocal \
  && libtoolize --copy --ltdl --automake \
  && autoheader \
  && automake --add-missing --copy \
  && autoconf
