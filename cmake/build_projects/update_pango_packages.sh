#!/bin/bash
# Update build configurations of pango packages that are described by
# GTK+ jhbuild build configuration

GTK_VERSION=3.9.4
rm -rf $GTK_VERSION
mkdir $GTK_VERSION
cd $GTK_VERSION
# This downloads all the relevant xml build configuration files
# for the given version of GTK
wget -nd  ftp://ftp.acc.umu.se/pub/GNOME/teams/releng/$GTK_VERSION/*
cd ..
./gtk_xml_recursive_process.py \
$GTK_VERSION/gnome-apps-$GTK_VERSION.modules \
>|gtk_packages_$GTK_VERSION.xml
rm -rf $GTK_VERSION
patch --no-backup-if-mismatch gtk_packages_$GTK_VERSION.xml patch_gtk_packages.xml

./gtk_transform.py pango 1 <gtk_packages_$GTK_VERSION.xml 1>| pango_packages.data
cmake -DFILENAME:FILEPATH=pango_packages.data -P configure_bp.cmake
