#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a tdistribution tarbal directly from the CVS sources of
# the PLplot project

SF_USER=${USER:-rlaboiss}
SF_CVSROOT=cvs.plplot.sourceforge.net:/cvsroot/plplot

CVSTMPDIR=plplot-cvs-tarball

rm -rf $CVSTMPDIR
cvs -d${SF_USER}@$SF_CVSROOT co -d$CVSTMPDIR  plplot
cd $CVSTMPDIR
rm -rf `find . -name CVS`

VERSION=${1:+--version=$1}
echo ./bootstrap.sh ${VERSION:---date-version}
./configure --enable-builddoc --enable-python --enable-octave --enable-f77
make dist
TARBALL=`ls plplot-*.tar.gz`
mv $TARBALL ..
cd ..
rm -rf $CVSTMPDIR
echo "CVS distribution tarball: $TARBALL"
