#!/usr/bin/perl

sub get_size {
  my $f = shift;
  my $s = 0;
  $s = `ls -sh $f | cut -d" "  -f1`;
  chomp $s;
  return $s;
}

while (<>) {
  if (/(.*)href="(.*)"(.*)/) {
    $file = $2;
    print "$1href=\"$2\"";
    $_ = $3;
    redo;
  }
  if (/(.*)\@SIZE@(.*)/) {
    if ( -f $file ) {
      $size = get_size ($file);
    }
    elsif ( -f "src/$file" ) {
      $size = get_size ("src/$file") 
    }
    else {
      die "Neither $file nor src/$file exist";
    }
    print "$1($size)$2\n";
    next;
  }
  print;
}
