#!/usr/bin/perl -w

while (<>) {
  if (/^AC_INIT\(plplot, ([^,)]+)/) {
    print $1;
    last;
  }
}
