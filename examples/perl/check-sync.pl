#!/usr/bin/perl -w

# Check if the perl examples are in sync with the C examples
#
# Run like this: check-sync.pl *.pl
#
# Copyright (C) 2006  Rafael Laboisssiere
# This file is part of PLplot and is distributed under the terms of the
# GNU Lesser General Public License, version 2.1 or later.  No warranties.

(my $prog = $0) =~ s:.*/::;

if (scalar @ARGV == 0) {
  print "Usage: $prog file [file ...]\n";
  exit 0;
}

sub compver {
  my $v1 = shift;
  my $v2 = shift;
  my @V1 = split (/\./, $v1);
  my @V2 = split (/\./, $v2);
  my $n1 = scalar @V1;
  my $n2 = scalar @V2;
  my $retval = 0;
  for (my $i = 0; $i < $n1 and $i < $n2; $i++) {
    $retval = (int $V1 [$i] <=> int $V2 [$i]);
    last if $retval != 0;
  }
  $retval = ($n1 <=> $n2)
    if $retval == 0;
  return $retval;
}

my %currver;

open (ENTRIES, "< ../c/CVS/Entries")
  or die "$prog: Cannot open Entries file";

while (<ENTRIES>) {
  $currver {$1} = $2
    if m{^/(x\d\dc.c)/([\d.]+)};
}

foreach my $file (@ARGV) {
  open (IN, "< $file")
    or die "$prog: Cannot open $file.";
  while (<IN>) {
    if (/SYNC: (x\d\dc.c) ([\d.]+)/) {
      my $cfile = $1;
      my $ver = $2;
      my $cver = $currver {$cfile};
      print "$file: sync with ",
            compver ($ver, $cver) == 0
              ? "$cfile $cver"
              : "$ver, but $cfile version is $cver",
            "\n";
      next;
    }
  }
}

