#! /bin/sh

aclocal \
  && touch include/plConfig.h.in \
  && libtoolize --copy --ltdl --automake\
  && automake --add-missing --copy\
  && autoheader \
  && autoconf


