#! /bin/bash

# File: cvsdir.sh
# Description: Save and restore CVS dirs under current dir.  This is
#   useful for genrating a Debian package without the CVS
#   administrative directories, but getting them restored later.
# Author: Rafael Laboissière <rafael@debian.org>
# Created on: Mon Feb 12 18:06:37 CET 2001
# Last modified on: Mon Feb 12 18:07:48 CET 2001
# $Id$


cvsdirs=`find . -type d -and -name CVS`
cvstar=../cvs.tar

case "$1" in
  save) 
    if [ -n "$cvsdirs" ] ; then 
      tar cf $cvstar $cvsdirs
      rm -rf $cvsdirs
    else
      echo "W: No CVS dirs under ." 1>&2
    fi
  ;;
  restore)
    if [ -f $cvstar ] ; then
      tar xf $cvstar
    else
      echo "E: File $cvstar not found" 1>&2
    fi
  ;;
  *)
    cat 1>&2 <<EOF
Usage $0 <save|restore>
EOF
  ;;
esac
		    
