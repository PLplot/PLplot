#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a tdistribution tarbal directly from the CVS sources of
# the PLplot project

SF_USER=${USER:-rlaboiss}
SF_CVSROOT=cvs.plplot.sourceforge.net:/cvsroot/plplot

CVSTMPDIR=plplot-cvs-tarball

rm -rf $CVSTMPDIR
cvs -d${SF_USER}@$SF_CVSROOT export -d$CVSTMPDIR -D now plplot
cd $CVSTMPDIR

VERSION=${1:+--version=$1}
./bootstrap.sh ${VERSION:---date-version}
./configure --enable-builddoc
make dist
TARBALL=`ls plplot-*.tar.gz`
mv $TARBALL ..
cd ..
rm -rf $CVSTMPDIR
echo "CVS distribution tarball: $TARBALL"
