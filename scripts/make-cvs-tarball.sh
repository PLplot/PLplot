#!/bin/bash

# This will prepare a distribution tarbal directly from the CVS sources of
# the PLplot project.
# CAVEAT: this script silently and completely deletes a directory called
# plplot-cvs-tarball existent in the current working directory.

# Copyright (C) 2003, 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

SF_USER=${USER:-rlaboiss}
SF_CVSROOT=cvs.plplot.sourceforge.net:/cvsroot/plplot
VERSION=${1:+--version=$1}
BRANCH=${2:+-r $2}
BRANCH=${BRANCH:--D now}
CVSTMPDIR=plplot-cvs-tarball

cleanup ( ) {
    rm -rf $CVSTMPDIR
}

trap "cleanup" 0 HUP INT QUIT PIPE TERM

cleanup

cvs -d${SF_USER}@$SF_CVSROOT export -d$CVSTMPDIR -D now plplot \
  && cd $CVSTMPDIR \
  && ./bootstrap.sh ${VERSION:---date-version} \
  && ./configure --enable-builddoc \
  && make dist \
  && TARBALL=`ls plplot-*.tar.gz` \
  && mv $TARBALL .. \
  && cd .. \
  && echo "CVS distribution tarball: $TARBALL"
