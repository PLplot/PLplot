#!/usr/bin/perl
 
$inpart = 0;
$intoc = 0;
while (<>) {
  if (/(<!DOCTYPE.*)\[(.*)/) {
    print "$1 \"\" [$2\n" ; next}
  if (/(.*)<part>(.*)/) {
    $inpart = 1;
    print "$1<!--<part>";
    $_ = "$2\n";
    redo;
  }
  if ($inpart and m{(.*)</title>(.*)}) {
    $inpart = 0;
    print "$1</title>-->";
    $_ = "$2\n";
    redo;
  }
  if (m{(.*)</part>(.*)}) {
    $inpart = 1;
    print "$1<!--</part>-->";
    $_ = "$2\n";
    redo;
  }
  if (/(.*)<toc>(.*)/) {
    $intoc = 1;
    print "$1<!--<toc>";
    $_ = "$2\n";
    redo;
  }
  if (m{(.*)</toc>(.*)}) {
    $intoc = 0;
    print "$1</toc>-->";
    $_ = "$2\n";
    redo;
  }
print;
}

