#!/bin/bash
#
# Script for uploading the CVS snapshot tarball to the PLplot web site
#
# Copyright (C) 2004  Rafael Laboissiere
# This script in the public domain

WWW_HOST=${WWW_HOST:-plplot.sf.net}
WWW_USER=${USER:-rlaboiss}
WWW_DIR=${WWW_DIR:-/home/groups/p/pl/plplot/htdocs/cvs-tarball}
GPGKEY=${GPGKEY:-0x4A5D72FE}

if test $# != 1 ; then
    echo "Usage: $0 tarball.tar.gz"
    exit 1
fi

TARBALL=$1
TARBALL_NAME=`basename $TARBALL`

gpg --default-key $GPGKEY --detach-sign --armor $TARBALL

INDEX=`tempfile --mode=0644`
DATE=`date --utc +"%Y-%m-%d %H:%M UTC"`

cat > $INDEX <<EOF
<html>
<head>
<title>PLplot CVS tarball</title>
</head>
<body>
<h2>Latest PLplot CVS snapshot distribution</h2>

<p> Tarball: <a href="$TARBALL_NAME">$TARBALL_NAME</a>.</p>
<p> Detached GPG signature: <a href="$TARBALL_NAME.asc">$TARBALL_NAME.asc</a>
(signed with key 
<a href="http://wwwkeys.pgp.net:11371/pks/lookup?op=index&search=$GPGKEY">$GPGKEY</a>)
</p>
<p> Uploaded by <a href="http://sf.net/users/$WWW_USER">$WWW_USER</a> 
on $DATE</p>

</body>
</html>
EOF

HOST=$WWW_USER@$WWW_HOST

ssh $HOST rm -rf $WWW_DIR
ssh $HOST mkdir $WWW_DIR
scp $TARBALL $TARBALL.asc $HOST:$WWW_DIR
scp $INDEX $HOST:$WWW_DIR/index.html
ssh $HOST chgrp -R plplot $WWW_DIR
ssh $HOST chmod -R g=u $WWW_DIR

rm -f $TARBALL.asc $INDEX
