#! /bin/bash

# This script is intended to generate a tarball of source code and screenshots,
# 'htdocs_plot_examples.tgz', that will be part of the 'examples' web page,
#
# The tarball must be unpacked at the top of the htdocs directory,
# i.e. 'htdocs' must appear when a 'ls' is issued.
# 
# The tarball can be copied to sf as, e.g.,
#    scp tmp/htdocs_plot_examples.tgz USER@login.sf.net:/home/groups/p/pl/plplot/htdocs_plot_examples.tgz
#

# This script must be run in the top plplot source directory as:
#     scripts/htdocs-gen_plot-examples.sh

# a fresh make, with minimum bindings
make clean
make configure

./configure --enable-dyndrivers --prefix=/usr/local --with-double --with-freetype \
--disable-cxx --disable-python --disable-java --disable-itcl --disable-octave --enable-f77

make
make cdemos

cd tmp

# remove htdocs directory, left from a previous script execution
rm -rf htdocs

# for all C demos executables:

for exe in 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19 20; do

    echo Working in example ${exe}

    # generate standard and preview size images
    ./x${exe}c -dev png -o x${exe} -fam -fflen 2;
    ./x${exe}c -dev png -o prev-x${exe} -fam -fflen 2 -geometry 200x150;

    # give png extension
    for i in `ls x${exe}.?? prev-x${exe}.??`; do
       mv $i $i.png;
    done;

    # move to www directory.
    echo populatting www directory demo${exe}
    mkdir -p htdocs/examples/demo${exe}
    mv *${exe}.??.png htdocs/examples/demo${exe}/
    cp x${exe}c.c ../examples/tcl/x${exe}.tcl ../examples/java/x${exe}.java \
	x${exe}f.f ../bindings/octave/demos/x${exe}c.m ../examples/python/xw${exe}.py \
	htdocs/examples/demo${exe}/

    # rename executables, to avoid browsers trying to execute files instead of showing them.
    (cd  htdocs/examples/demo${exe};
    for j in *.c *.f *.m *.tcl *.java *.py; do
	    mv $j $j-
    done
    )  

done

# create the tarball
tar cvzf htdocs_plot_examples.tgz htdocs
rm -rf htdocs
