#!/usr/bin/perl

use Getopt::Long;

Getopt::Long::Configure ("pass_through");
GetOptions qw(-locate! -h!);

$locate_mode = (defined $opt_locate) ? 1 : 0;

if (defined $opt_h) {
  print "$0 option:\n    -locate\t\tDo locate\n";
  push @ARGV, "-h";
}

do 'x01.pl';
