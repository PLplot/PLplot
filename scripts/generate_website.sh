#!/bin/sh
echo "Generate and upload all PLplot website components to a computer host that is accessible with ssh."
echo -n "Valid login name on ssh-accessible host? "
read USERNAME
echo -n "Valid group name on ssh-accessible host? "
read GROUPNAME
echo -n "ssh-accessible hostname? "
read HOSTNAME
echo "The following directory prefix on '$HOSTNAME' will be removed and then replaced by the complete PLplot website so be careful with what you specify"
echo -n "Expendable directory prefix on '$HOSTNAME'? " 
read WEBSITE_PREFIX
echo ""
echo "Summary:"
echo "USERNAME = $USERNAME"
echo "GROUPNAME = $GROUPNAME"
echo "HOSTNAME = $HOSTNAME"
echo "WEBSITE_PREFIX = $WEBSITE_PREFIX"
echo ""
echo "Last warning: if you specify 'yes' below, then the '$WEBSITE_PREFIX' directory on the remote host, '$HOSTNAME', will be removed and then replaced."
ANSWER=
while test "$ANSWER" != "yes" -a "$ANSWER" != "no"; do
    echo -n "Continue (yes/no)? "
    read ANSWER
done
if test "$ANSWER" = "no"; then
    echo "Immediate exit specified!"
    exit
fi

# Remove $WEBSITE_PREFIX to insure absolutely clean result.
echo ""
echo "Completely remove old website files."
ssh $USERNAME@$HOSTNAME rm -rf $WEBSITE_PREFIX

# Get down to the work of generating the components of the PLplot website
# and uploading them to the $WEBSITE_PREFIX directory on $HOSTNAME.

# Build a local version of the documentation.

# N.B. this build puts some documentation results into the source tree so
# we use a throwaway source tree for this.

rm -rf /tmp/plplotdoc
mkdir -p /tmp/plplotdoc/build
cd /tmp/plplotdoc
echo ""
echo "svn export PLplot trunk to local machine.  This download may take a while depending on your bandwidth...."
svn --quiet export https://plplot.svn.sourceforge.net/svnroot/plplot/trunk \
    plplot_source
echo ""
echo "Configure and build PLplot documentation.  This may take a while depending on your cpu speed...."
cd /tmp/plplotdoc/build
cmake \
    -DWWW_USER=$USERNAME \
    -DWWW_GROUP=$GROUPNAME \
    -DWWW_HOST=$HOSTNAME \
    -DWWW_DIR=$WEBSITE_PREFIX/htdocs/docbook-manual \
    -DDEFAULT_NO_BINDINGS=ON -DDEFAULT_NO_DEVICES=ON \
    -DPREBUILD_DIST=ON \
    -DBUILD_DOC=ON \
    ../plplot_source \
    >& cmake.out
make VERBOSE=1 -j3 prebuild_dist >& make_prebuild.out

echo ""
echo "Install the just-generated documentation to $WEBSITE_PREFIX/htdocs/docbook-manual on $HOSTNAME."
# This command completely removes WWW_DIR on $HOSTNAME
# so be careful how you specify the above -DWWW_DIR option.

cd /tmp/plplotdoc/build
make VERBOSE=1 www-install >& make_www_install.out

echo ""
echo "Build PLplot, PLplot examples, and screenshots of those examples.  This may take a while depending on your cpu speed...."

# N.B. this command completely removes WWW_DIR/htdocs/examples-data 
# on $HOSTNAME so be careful how you specify WWW_DIR!

cd /tmp/plplotdoc/plplot_source
WWW_USER=$USERNAME \
    WWW_GROUP=$GROUPNAME \
    WWW_HOST=$HOSTNAME \
    WWW_DIR=$WEBSITE_PREFIX \
    scripts/htdocs-gen_plot-examples.sh >& htdocs_gen.out

echo ""
echo "Finish up by copying static components of the website."
# Generate the website (other than the documentation and examples done above).
# N.B. this command removes certain files and subdirectories of
# WWW_DIR on $HOSTNAME so be careful how you specify WWW_DIR!

cd /tmp/plplotdoc/plplot_source/www
make \
    WWW_USER=$USERNAME \
    WWW_GROUP=$GROUPNAME \
    WWW_HOST=$HOSTNAME \
    WWW_DIR=$WEBSITE_PREFIX/htdocs >& make_www.out

# If all the above works like it should, and $HOSTNAME has apache and PHP
# installed properly, you should be able to browse the
# resulting complete website.  AWI verified this by specifying
# irwin, irwin, raven (his local machine), and /home/irwin/public_html/plplot
# for USERNAME, GROUPNAME, HOSTNAME, and WEBSITE_PREFIX for this script
# and browsing http://raven/~irwin/plplot/htdocs/ afterward.

# Once you are satisfied with the local version of the website, then upload
# it to the official PLplot SourceForge site using the new (as of
# 2008-09-19) rsync procedure documented in README.Release_Manager_Cookbook.
