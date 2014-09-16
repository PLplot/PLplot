#!/usr/bin/perl -w

while (<>) {
  if (/^set\(PLPLOT_VERSION ([^,)]+)/) {
    print $1;
    last;
  }
}
