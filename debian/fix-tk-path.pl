#!/usr/bin/perl
# File: fix-tk-path.pl
# Written by Rafael Laboissiere <rafael@icp.inpg.fr> 15 Jun 1998 22:03:41 +0200
# Fix path for Tk scripts after initial `#!'

$done = 0;
while(<>){
  $line = $_;
  if ((not $done) && /^\#\![ ]*(.*) (.*)$/ ) {
    if ( $1 eq "plserver" ) {
      print "#!/usr/bin/plserver $2\n";
    }
    else {
      print "#!/usr/doc/plplot/examples/tk/$1 $2\n";
    }
    $done = 1;
  }
  else {
    print $line;
  }
} 
