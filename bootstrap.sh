#! /bin/sh

aclocal \
  && touch include/plConfig.h.in \
  && automake --add-missing \
  && autoheader \
  && autoconf


