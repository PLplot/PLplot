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

CVSREL=.cvs.`date '+%Y%m%d'`

echo -n "Patching configure.ac ... "
perl -pi -e \
  's/^(AM_INIT_AUTOMAKE\(plplot, )(\d\.\d\.\d)([^)]*\))/$1$2'$CVSREL')/' \
  configure.ac
echo done

./bootstrap.sh
./configure --enable-docbook
( cd doc/docbook ; make )
make dist
TARBALL=`ls plplot-*.tar.gz`
mv $TARBALL ..
cd ..
rm -rf $CVSTMPDIR
echo "CVS distribution tarball: $TARBALL"
