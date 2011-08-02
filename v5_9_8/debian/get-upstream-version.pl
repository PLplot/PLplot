#!/usr/bin/perl -w

while (<>) {
  if (/^set\(VERSION ([^,)]+)/) {
    print $1;
    last;
  }
}
