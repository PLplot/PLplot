#!/bin/bash

# Copyright (C) 2003  Rafael Laboissiere
# This script is in the Public Domain

# This will prepare a distribution tarbal directly from the CVS sources of
# the PLplot project.
# CAVEAT: this script silently and completely deletes a directory called 
# plplot-cvs-tarball existent in the current working directory.

SF_USER=${USER:-rlaboiss}
SF_CVSROOT=cvs.plplot.sourceforge.net:/cvsroot/plplot
VERSION=${1:+--version=$1}
CVSTMPDIR=plplot-cvs-tarball

cleanup ( ) {
    rm -rf $CVSTMPDIR
}

trap "cleanup" 0 HUP INT QUIT PIPE TERM

cleanup

cvs -d${SF_USER}@$SF_CVSROOT export -d$CVSTMPDIR -D now plplot \
  && cd $CVSTMPDIR \
  && ./bootstrap.sh ${VERSION:---date-version} \
  && ./configure --enable-builddoc --enable-gnome \
  && make \
  && make dist \
  && TARBALL=`ls plplot-*.tar.gz` \
  && mv $TARBALL .. \
  && cd .. \
  && echo "CVS distribution tarball: $TARBALL"
