#!/usr/bin/perl -w

# This script removes all cvs-tarball-* tags from the repository
# Run it from the top_srcdir

# Written by Rafael Laboissiere, on Sun Feb 27 01:27:51 CET 2005

my $cf = "configure.ac";
my $tag_pattern = 'cvs-tarball_\d\d\d\d-\d\d-\d\d-\d\d-\d\d-\d\d';

sub remove_tag {
  my $tag = shift;
  print "Removing tag $tag\n";
  system "cvs rtag -d $_ plplot";
}

-f $cf
  or die "E: $cf not found.  Are you at the PLplot top_srcdir?\n";

(my $ac_init = `grep "AC_INIT(plplot" $cf`) =~ /plplot/
  or die "E: Current directory does not seem to have the PLplot sources.\n";

open (LOG, "cvs -f log $cf |");

remove_tag ($_)
  foreach map {
    /($tag_pattern)/;
    $1;
  } grep {
      /^\s*($tag_pattern):/;
    } <LOG>;
