#! /usr/bin/perl
# File: included-html-files.pl
# Description: Extract names of included HTML files from a DocBook main file
# Author: Rafael Laboissière <rafael@icp.inpg.fr>
# Created on: Thu May 20 11:47:08 CEST 1999
# Last modified on: Thu May 20 12:02:38 CEST 1999
# $Id$

while (<>) {
  if (/HREF="(.*\.html)"/) {
    print "$1\n";
  }
}
