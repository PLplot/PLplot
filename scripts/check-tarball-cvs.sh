#/bin/sh -e

# First try on a script to check the discrepancies between the distributed
# tarball obtained with "make dist" and the CVS tree.  This script will
# send to stdout a list of files that are present in a freshly checked out
# CVS tree and absent from the tarball.
#
# Run it from the top directory.
#
# Written by Rafael Laboissiere on 2003-31-01

VERSION=`perl -ne \
           'if(/AM_INIT_AUTOMAKE\(plplot, ([0-9.]+)/){print "$1\n"; last}'\
           < configure.ac`
TOPDIR=plplot-$VERSION
TARBALL=$TOPDIR.tar.gz

make dist > /dev/null
cvs co plplot > /dev/null

for f in `find plplot -type f | sed 's|^plplot/||'` ; do
  if [ -z "`tar tfz $TARBALL $TOPDIR/$f 2>/dev/null`" ] ; then
    echo $f
  fi
done

rm -rf $TARBALL plplot
