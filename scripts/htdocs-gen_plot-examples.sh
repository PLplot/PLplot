#! /bin/bash

# This script is intended to generate a tarball of source code and screenshots,
# 'htdocs_plot_examples.tgz', that will be part of the 'examples' web page,
#
# This script must be run in the top plplot source directory as:
#
#     scripts/htdocs-gen_plot-examples.sh
# 
# The tarball will be copied to SourceForge and unpacked at the www area.  
# If your user name (as given by the environment variable USER) is different
# as that at SF, then launch the script like this:
#     
#     USER=joeplplotter scripts/htdocs-gen_plot-examples.sh
#
# To avoid rebuild launch it like this:
#     build=false scripts/htdocs-gen_plot-examples.sh


# a fresh make, with minimum bindings

build=${build:-true}

if test "$build" = true; then
    make clean
    make configure

    ./configure --disable-static --enable-dyndrivers --prefix=/tmp/plplot \
    --disable-cxx --disable-python --disable-java --disable-itcl \
    --disable-octave --enable-f77

    make
    make install
fi

# remove htdocs directory, left from a previous script execution
rm -rf htdocs

# for all C demos executables:
(cd examples/c; make check)

# hack, x20c needs lena in the current directory
cp examples/c/lena.pgm .

EXDIR=htdocs/examples-data

for exe in 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19 20 21; do

    echo Working in example ${exe}

    # generate standard and preview size images
    examples/c/x${exe}c -dev png -o x${exe} -fam -fflen 2;
    examples/c/x${exe}c -dev png -o prev-x${exe} -fam -fflen 2 -geometry 200x150;

    # give png extension
    for i in `ls x${exe}.?? prev-x${exe}.??`; do
       mv $i $i.png;
    done;

    # move to www directory.
    echo populatting www directory demo${exe}
    mkdir -p $EXDIR/demo${exe}
    mv *${exe}.??.png $EXDIR/demo${exe}/
    cp examples/c/x${exe}c.c examples/tcl/x${exe}.tcl examples/java/x${exe}.java \
	examples/f77/x${exe}f.f bindings/octave/demos/x${exe}c.m \
	examples/python/xw${exe}.py $EXDIR/demo${exe}

    # rename executables, to avoid browsers trying to execute files instead of showing them.
    (cd  htdocs/examples-data/demo${exe};
    for j in *.c *.f *.m *.tcl *.java *.py; do
	    mv $j $j-
    done
    )  

done

# remove hack
rm -f lena.pgm

# create the tarball
TARBALL=htdocs-plot-demos.tgz
find htdocs -size 0 -exec rm {} \;
tar cvzf $TARBALL htdocs
rm -rf htdocs

# Transfer the tarball to Sourceforge and unpack it, such that the files will
# appear in the PLplot web site

WWW_HOST=${USER:+$USER@}shell.sf.net
WWW_DIR=/home/groups/p/pl/plplot

echo Copying the tarball to WWW site
scp $TARBALL $WWW_HOST:$WWW_DIR
echo Changing its permission to allow group access
ssh $WWW_HOST chmod g=u $WWW_DIR/$TARBALL
echo Unpacking the remote tarball
ssh $WWW_HOST tar -x -z -C $WWW_DIR -f $WWW_DIR/$TARBALL
echo Changing its group permissions of the remote demo direcotry
ssh $WWW_HOST chmod -R g=u $WWW_DIR/$EXDIR
echo Removing the remote tarball
ssh $WWW_HOST rm -f $WWW_DIR/$TARBALL
