#!/bin/bash
# Update build configurations of added packages that are described by
# gtk_packages_add.xml

rm -f add_packages.data
touch add_packages.data
# Ignore libxml2 since that is sucked in by the libxslt dependency.
PACKAGE_LIST="libffi intltool pixman gperf libxslt"
for PACKAGE in $PACKAGE_LIST; do
  ./gtk_transform.py $PACKAGE 1 <gtk_packages_add.xml 1>> add_packages.data
done
cmake -DFILENAME:FILEPATH=add_packages.data -P configure_bp.cmake
