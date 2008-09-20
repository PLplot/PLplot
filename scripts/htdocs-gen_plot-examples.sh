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
#     WWW_USER=joeplplotter scripts/htdocs-gen_plot-examples.sh
#
# It is also possible to override the variables WWW_GROUP, WWW_HOST,
# and WWW_DIR in the same way as with WWW_USER in the example above.
#
# To avoid rebuild launch it like this:
#     build=false scripts/htdocs-gen_plot-examples.sh


# a fresh make, with minimum bindings

build=${build:-true}

if test "$build" = true; then
#    make clean
#    make configure
#
#    ./configure --disable-static --enable-dyndrivers --prefix=/tmp/plplot \
#    --disable-cxx --disable-python --disable-java --disable-itcl \
#    --disable-octave --enable-f77 --enable-psttf --enable-psttfc
#
    rm -rf htdocsgen/build_dir htdocsgen/install
    mkdir -p htdocsgen/build_dir htdocsgen/install
    cd htdocsgen/build_dir
    # Specifically enable f77 and Ada so that examples will be generated.
    cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install \
	-DDEFAULT_NO_BINDINGS=ON -DENABLE_f77=ON -DENABLE_ada=ON\
    -DDEFAULT_NO_DEVICES=ON -DPLD_pngcairo=ON \
	../../
    make -j3
    make -j3 install
    cd ../..
fi

EXDIR=htdocs/examples-data
pushd htdocsgen/install/share/plplot*/examples/c
export cexamples_dir=`pwd`
make
popd

# hack, x20c needs lena in the current directory
cp examples/c/lena.pgm .

for exe in 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19 20 21 22 \
    23 24 25 26 27 28 29 30; do

    if test $exe = 08 -o $exe = 16 -o $exe = 20 -o $exe = 30; then
      # No cairo graphics AA (Yep, 1 turns it off).
	DRIVEROPT='-drvopt graphics_anti_aliasing=1'
    else
	if test $exe = 09 -o $exe = 21; then
        # Text clipping.
            DRIVEROPT='-drvopt text_clipping=1'
	else
        # Otherwise use default graphics AA which is full AA 
            DRIVEROPT=
	fi
    fi
    echo Working on example ${exe} using DRIVEROPT of $DRIVEROPT

    # generate standard and preview size images
    $cexamples_dir/x${exe}c -dev pngcairo -o x${exe} -fam -fflen 2 \
	$DRIVEROPT
    $cexamples_dir/x${exe}c -dev pngcairo -o prev-x${exe} -fam -fflen 2 \
	$DRIVEROPT -geometry 200x150

    # give png extension
    for i in `ls x${exe}.?? prev-x${exe}.??`; do
	mv $i $i.png;
    done;

    # move to www directory.
    echo populating www directory demo${exe}
    rm -rf $EXDIR/demo${exe}
    mkdir -p $EXDIR/demo${exe}
    mv *${exe}.??.png $EXDIR/demo${exe}
    # Note both Ada and fortran 77 examples grabbed from the installed
    # examples.
    for f in \
	$cexamples_dir/../ada/x${exe}a.adb \
        examples/c/x${exe}c.c \
	examples/c++/x${exe}.cc \
	$cexamples_dir/../f77/x${exe}f.f \
	examples/f95/x${exe}f.f90 \
        examples/java/x${exe}.java \
        examples/ocaml/x${exe}.ml \
        bindings/octave/demos/x${exe}c.m \
        examples/perl/x${exe}.pl \
        examples/python/xw${exe}.py \
        examples/tcl/x${exe}.tcl \
	; do
        if test -f $f ; then
            cp $f $EXDIR/demo${exe}
        else
            echo Example `basename $f` is not yet available \
                > $EXDIR/demo${exe}/`basename $f`
        fi
    done

    # rename executables, to avoid browsers trying to execute files
    # instead of showing them.
    (cd  $EXDIR/demo${exe};
	for j in \
	    *.adb \
            *.c \
	    *.cc \
	    *.f \
	    *.f90 \
	    *.java \
	    *.ml \
	    *.m \
	    *.pl \
	    *.py \
	    *.tcl \
	    ; do
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

WWW_GROUP=${WWW_GROUP:-plplot}
WWW_HOST=${WWW_HOST:-shell.sf.net}
WWW_USER_AT_HOST=${WWW_USER:+$WWW_USER@}$WWW_HOST
WWW_DIR=${WWW_DIR:-/home/groups/p/pl/plplot}

echo "ssh $WWW_USER_AT_HOST mkdir -p $WWW_DIR"
ssh $WWW_USER_AT_HOST mkdir -p $WWW_DIR
echo "ssh $WWW_USER_AT_HOST rm -rf $WWW_DIR/$EXDIR"
ssh $WWW_USER_AT_HOST rm -rf $WWW_DIR/$EXDIR
echo "scp $TARBALL $WWW_USER_AT_HOST:$WWW_DIR"
scp $TARBALL $WWW_USER_AT_HOST:$WWW_DIR
echo "ssh $WWW_USER_AT_HOST chmod g=u $WWW_DIR/$TARBALL"
ssh $WWW_USER_AT_HOST chmod g=u $WWW_DIR/$TARBALL
echo "ssh $WWW_USER_AT_HOST tar -x -z -C $WWW_DIR -f $WWW_DIR/$TARBALL"
ssh $WWW_USER_AT_HOST tar -x -z -C $WWW_DIR -f $WWW_DIR/$TARBALL
echo "ssh $WWW_USER_AT_HOST chgrp -R $WWW_GROUP $WWW_DIR/$EXDIR"
ssh $WWW_USER_AT_HOST chgrp -R $WWW_GROUP $WWW_DIR/$EXDIR
echo "ssh $WWW_USER_AT_HOST chmod -R g=u $WWW_DIR/$EXDIR"
ssh $WWW_USER_AT_HOST chmod -R g=u $WWW_DIR/$EXDIR
echo "ssh $WWW_USER_AT_HOST rm -f $WWW_DIR/$TARBALL"
ssh $WWW_USER_AT_HOST rm -f $WWW_DIR/$TARBALL
