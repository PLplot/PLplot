#! /bin/bash

www_host=shell1.sourceforge.net
www_dir=/home/groups/p/pl/plplot/htdocs/resources/debian
www_user=rlaboiss
www_group=plplot

rcp_command=scp

tmp_dir=${TMPDIR:-/var/tmp}/install-www

#www-
usage()
{
  cat <<EOF
Usage: $0 [OPTIONS]
Options:
        [--release=RELEASE]
        [--host=REMOTE_HOST]
        [--dir=REMOTE_DIR]
        [--user=REMOTE_USER]
        [--group=REMOTE_GROUP]
        [--help]
EOF
  exit $1
}

while test $# -gt 0; do
  case "$1" in
  -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
  *) optarg= ;;
  esac

  case $1 in
    --release=*)
      release=$optarg
      ;;
    --host=*)
      www_host=$optarg
      ;;
    --dir=*)
      www_dir=$optarg
      ;;
    --user=*)
      www_user=$optarg
      ;;
    --group=*)
      www_group=$optarg
      ;;
    --help)
      usage 1 1>&2
      ;;
  esac
  shift
done

if [ -z "$release" ] ; then
  if [ ! -f debian/changelog ] ; then
    echo "File debian/changelog not found!" 1>&2
    exit 1
  fi
  release=`perl -ne 'if(/\((.*)\)/){print $1;exit}' < debian/changelog`
fi

rsh_command="ssh $www_user@$www_host"

tarball=dist.tar.gz

rm -rf $tmp_dir
mkdir -p $tmp_dir
cp ../*${release}*.{deb,dsc,diff.gz} $tmp_dir
cp ../*`echo $release | sed 's/-.*//'`.orig.tar.gz $tmp_dir
cp debian/Release $tmp_dir
touch $tmp_dir/override
pwd=`pwd`
( cd $tmp_dir 
  dpkg-scanpackages . override | gzip -9 -c > Packages.gz
  dpkg-scansources . override | gzip -9 -c > Sources.gz
  rm -f override
  ls | fgrep -v index | eperl -k $pwd/debian/index.html.in > index.html
  tar cfz $tarball `ls`
)

$rsh_command rm -rf $www_dir
$rsh_command mkdir -p $www_dir
$rcp_command $tmp_dir/dist.tar.gz $www_user@$www_host:$www_dir 
$rsh_command tar xfz $www_dir/$tarball -C $www_dir
$rsh_command chgrp -R $www_group $www_dir
$rsh_command chmod -R g=u $www_dir
$rsh_command rm -f $www_dir/$tarball
