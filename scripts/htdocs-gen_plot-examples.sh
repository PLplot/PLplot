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
    # Specifically enable f77 so that examples will be generated.
    cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install \
    -DENABLE_cxx=OFF -DENABLE_f77=ON -DENABLE_f95=OFF \
    -DENABLE_python=OFF -DENABLE_java=OFF \
    -DENABLE_tcl=OFF -DENABLE_octave=OFF \
    -DDEFAULT_NO_DEVICES=ON -DPLD_png=ON -DPLD_psttf=ON \
    ../../
    make
    make install
    cd ../..
fi

EXDIR=htdocs/examples-data
pushd htdocsgen/install/share/plplot*/examples/c
export cexamples_dir=`pwd`
make
popd

# hack, x20c needs lena in the current directory
cp examples/c/lena.pgm .

for exe in 01 02 03 04 05 06 07 08 09 10 11 12 13 15 16 18 19 20 21 22 23 24 25; do

    echo Working on example ${exe}

    # generate standard and preview size images
    if test $exe != 24 ; then
        if test $exe = 02 ; then
            SMOOTH=0
	else
	    SMOOTH=1
	fi
        $cexamples_dir/x${exe}c -dev png -drvopt text,smooth=$SMOOTH \
	    -o x${exe} -fam -fflen 2;
        $cexamples_dir/x${exe}c -dev png -drvopt text,smooth=$SMOOTH \
	    -o prev-x${exe} -fam -fflen 2 -geometry 200x150;
    else
      $cexamples_dir/x24c -dev psttfc -o test.ps
      convert -rotate 90 test.ps -geometry 800x600 x24.01.png
      convert -rotate 90 test.ps -geometry 200x150 prev-x24.01.png
      rm test.ps
      # convert command needs the png suffix, but now take it off to
      # be consistent with script below which puts it back on again.
      mv x24.01.png x24.01
      mv prev-x24.01.png prev-x24.01
    fi

    # give png extension
    for i in `ls x${exe}.?? prev-x${exe}.??`; do
       mv $i $i.png;
    done;

    # move to www directory.
    echo populating www directory demo${exe}
    rm -rf $EXDIR/demo${exe}
    mkdir -p $EXDIR/demo${exe}
    mv *${exe}.??.png $EXDIR/demo${exe}
    # Note fortran 77 examples grabbed from installed examples.
    for f in examples/c/x${exe}c.c examples/tcl/x${exe}.tcl		\
             examples/java/x${exe}.java cexamples_dir/../f77/x${exe}f.f		\
             bindings/octave/demos/x${exe}c.m				\
             examples/python/xw${exe}.py examples/c++/x${exe}.cc	\
             examples/perl/x${exe}.pl ; do
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
    for j in *.c *.cc *.f *.m *.tcl *.java *.py *.pl; do
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
WWW_HOST=${WWW_HOST:-${WWW_USER:+$WWW_USER@}shell.sf.net}
WWW_DIR=${WWW_DIR:-/home/groups/p/pl/plplot}

echo Removing remote examples directory
ssh $WWW_HOST rm -rf $WWW_DIR/$EXDIR
echo Copying the tarball to WWW site
scp $TARBALL $WWW_HOST:$WWW_DIR
echo Changing its permission to allow group access
ssh $WWW_HOST chmod g=u $WWW_DIR/$TARBALL
echo Unpacking the remote tarball
ssh $WWW_HOST tar -x -z -C $WWW_DIR -f $WWW_DIR/$TARBALL
echo Changing group of the remote examples directory
ssh $WWW_HOST chgrp -R $WWW_GROUP $WWW_DIR/$EXDIR
echo Changing group permissions of the remote examples directory
ssh $WWW_HOST chmod -R g=u $WWW_DIR/$EXDIR
echo Removing the remote tarball
ssh $WWW_HOST rm -f $WWW_DIR/$TARBALL
 
