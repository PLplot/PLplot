#!/bin/bash

# WARNING: completely replaces htdocs by new version without
# backups other than the cvs repository.
# It is assumed that old versions of htdocs can be obtained using
# cvs export with a different tag or date

DOCBOOKDIR=htdocs/resources/docbook-manual
DEBIANDIR=htdocs/resources/debian
cd /home/groups/p/pl/plplot
if [ ! -d $DOCBOOKDIR ] ; then
  echo "Warning: The DocBook directory is missing in the htdocs hierarchy."
  echo "  Install it from the CVS checked out sources of the docbook module."
  DOCBOOKDIR=""
else  
  mv $DOCBOOKDIR docbook-manual-$$
fi  
test -d $DEBIANDIR && mv $DEBIANDIR debian-$$
if [ -z "$USER" ] ; then 
  echo -n "Login name for SourceForge CVS: "
  read USER
fi
rm -rf htdocs
CVS_RSH=ssh CVSROOT=$USER@cvs1:/cvsroot/plplot \
  cvs export -r HEAD -d htdocs www
test -n "$DOCBOOKDIR" && mv docbook-manual-$$ $DOCBOOKDIR
test -d $DEBIANDIR && mv $DEBIANDIR debian-$$
chmod -R g=u htdocs
