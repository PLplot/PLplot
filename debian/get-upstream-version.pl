#!/usr/bin/perl -w

while (<>) {
  if (/^AM_INIT_AUTOMAKE\(plplot, ([^)]+)\)/) {
    print $1;
    last;
  }
}
