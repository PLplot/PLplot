#! /bin/sh

cd . && aclocal
cd . && automake --add-missing --gnu Makefile src/Makefile bin/Makefile
cd . && autoconf
