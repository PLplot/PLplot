#!/usr/bin/perl

use Getopt::Std;

getopts('o:s:x:');


print "\n********* $opt_s **********\n";
print "Creating LaTeX file... ";
open (TEX, "> TMP.tex");
print TEX "
\\documentclass{article}
\\pagestyle{empty}
\\begin{document}
$opt_s
\\end{document}";
close TEX;
print "done!\n";
print "LaTeXing it... ";
system "latex", "TMP.tex";
print "done!\n";
print "Generating EPS file... ";
system "dvips", "-E", "-o", $opt_o, "-x", $opt_x, "TMP";
print "done!\n";
