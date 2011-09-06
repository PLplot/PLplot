#! /usr/bin/env perl
 
# Massage the SWIG generated octave bindings to add in the required 
# visibility pragmas for gcc to make -fvisibility=hidden work ok.
#
# (C) Andrew Ross 2011
#
# This file is part of PLplot and release under the terms of the GNU GPL.
# No warranties.

die "Usage:\n  $0 plplot_octaveOCTAVE.cxx\n"
  if scalar @ARGV != 1;

my ($srcfile) = @ARGV;

open (SRCFILE, "< $srcfile")
  or die "$0: cannot open file $srcfile";

my $in_func = 0;
  
while (<SRCFILE>) {
  if (/^DEFUN_DLD/) {
    $in_func = 1;
    print "#if defined(__GNUC__) && __GNUC__ > 3\n#pragma GCC visibility push(default)\n#endif\n";
  }
  print $_;
  if (/^}/ && $in_func == 1) {
    print "#if defined(__GNUC__) && __GNUC__ > 3\n#pragma GCC visibility pop\n#endif\n";
    $in_func = 0;
  }
}
