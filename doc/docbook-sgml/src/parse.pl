#!/usr/bin/perl

package MyParser;
use lib '/usr/lib/perl5/perlsgml/';
use SGML::Parser;
@ISA = qw( SGML::Parser );

$indent = 0;

sub start_tag { 
  my $this = shift;
  my $gi = lc shift;
  my $attr_spec = shift;

  for ($i=0;$i<$indent;$i++){print "  ";}
  print $gi."\n";
  $indent++;
}

sub end_tag {
  $indent--;
}
  
$myparser = new MyParser;
$myparser->parse_data(\*STDIN);
  

