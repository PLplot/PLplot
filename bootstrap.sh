#! /bin/sh

aclocal \
  && touch config.h.in \
  && rm config.h.in \
  && touch config.h.in \
  && libtoolize --copy --ltdl --automake\
  && automake --add-missing --copy\
  && autoheader \
  && autoconf


