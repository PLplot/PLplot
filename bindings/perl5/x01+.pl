#!/usr/bin/perl

use Getopt::Long;

Getopt::Long::Configure ("pass_through");
GetOptions qw(-locate! -fontset=i -xor! -h!);

$locate_mode = (defined $opt_locate) ? 1 : 0;

if (defined $opt_h) {
  print "$0 options:\n" .
        "    -locate\t\t Turns on test of API locate function\n" .
	"    -fontset\t\t Selects stroke font set (0 or 1, def:1)\n" .
	"    -xor\t\t Turns on test of XOR\n";
  push @ARGV, "-h";
}

$fontset = (defined $opt_fontset) ? $opt_fontset : 1;

$test_xor = (defined $opt_xor) ? $opt_xor : 0;

do 'x01.pl';
