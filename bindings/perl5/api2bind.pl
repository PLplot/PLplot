#!/usr/bin/perl
# 
# Call:
# ./api2bind.pl \
#   ../../doc/docbook/src/api.xml ../../tmp/plplot/plplot.h  \
#   ../../tmp/PLplot.i ../../tmp/Makefile.PL ../../tmp/PLplot.pm

use XML::Parser;
use XML::DOM;

$libs = "-L. -lplplotd -L/usr/X11R6/lib -lX11 -L/usr/lib   -lgtk -lgdk -lgmodule -lgthread -lglib -lpthread -ldl -lXi -lXext  -lm   -lgnomeui -lart_lgpl -lgdk_imlib -lSM -lICE -lgnome -lgnomesupport -lesd -laudiofile  -ldb";


sub substitute {
  my $file = shift;
  my $vars = shift;
  open (IN, "< $file.in")
    or die "Cannot open input file $file.in";
  my $sep = $/;
  $/ = undef;
  my $text = <IN>;
  $/ = $sep;
  foreach my $v (keys %$vars) {
    my $pattern = "@" . $v . "@";
    $text =~ s{$pattern}{$vars->{$v}}g;
  }
  open (OUT, "> $file")
    or die "Cannot open output file $file";
  print OUT $text;
  close (OUT);
}

sub get_cdata {
  my $e = shift;
  my $t;
  if ($e->getNodeType != TEXT_NODE) {
    $t = $e->getFirstChild;
  }
  else {
    $t = $e;
  }
  my $nt = $t->getNodeType;
  if ($nt == TEXT_NODE) {
    my $a = $t->getData;
    $a =~ s/^\s+//;
    $a =~ s/\s+$//;
    return $a;
  }
  else {
    return "";
  }
}

sub funcptr_stub {
  my $name = shift;
  my $ret = shift;
  my $argin = shift;
  $argin =~ s/^\s*\(\s*//;  
  $argin =~ s/\s*\)\s*$//;  
  my @args = split /\s*,\s*/, $argin;
  my $i = 0;
  my @arglist = map {$_ . " arg" . ++$i} @args;
  $i = 0;
  my @decls = map {
      "SV* arg" . ++$i . "_SV;\n";
    } @args;
  $i = 0;
  my @xpush = map {
    $i++;
    my $mortal = "";   
    if (/\*/) {
      if (/PLINT/) {
        $mortal = "arg" . $i . "_SV " 
        . "= sv_2mortal(newRV(sv_2mortal(newSViv ((IV) *arg" . $i . "))));";
      }
      elsif (/PLFLT/) {
        $mortal = "arg" . $i . "_SV " 
        . "= sv_2mortal(newRV(sv_2mortal(newSVnv ((double) *arg" . $i . "))));";
      }
    }   
    else {
      if (/PLINT/) {
	$mortal = "arg" . $i . "_SV "
	  . " = sv_2mortal(newSViv ((IV) arg" . $i . "));";
      }
      elsif (/PLFLT/) {
	$mortal = "arg" . $i . "_SV "
	  . "= sv_2mortal(newSVnv ((double) arg" . $i . "));";
      }
      elsif (/PLPointer/) {
	$mortal = "arg" . $i . "_SV "
	  . " = (SV*) arg" . $i . ";\n";
      }
    }
    $mortal . "\n  XPUSHs (arg" . $i . "_SV);\n";
    } @args;
  $i = 0;
  my @return = map {
      $i++;
      if (/\*/) {
	if (/PLINT/) {
	  "*arg" . $i . " = SvIV (SvRV (arg" . $i . "_SV));\n";
	}
	elsif (/PLFLT/) {
	  "*arg" . $i . " = SvNV (SvRV (arg" . $i . "_SV));\n";
	}
      }
      else {
	undef;
      }
    } @args;
  return "
\%typedef $ret (*$name) (" . join (", ", @args) . ");

\%inline \%{
static SV* $name" . "_SV;
static void
$name" . "_call (" . join (", ", @arglist) . ")
{
" . join ("  ", @decls) . "
  dSP ;

  ENTER ;
  SAVETMPS ;
  
  PUSHMARK (SP) ;

  " . join ("  ", @xpush) . "

  PUTBACK ;

  perl_call_sv ($name" . "_SV, G_DISCARD);

" . join ("  ", @return) . "
  FREETMPS ;
  LEAVE ;
}
\%}

\%typemap(perl5,in) $name {
  $name" . "_SV = \$source;
  \$target = $name" . "_call;
}

";
}

# Read API XML file

$api = "<?xml version=\"1.0\"?>
<!DOCTYPE chapter PUBLIC \"docbook\" \"\" [
]>
";

open (API, "< $ARGV[0]");
$/ = undef;
$api .= <API>;
close API;

# Collect APIprototypes 

$p = new XML::DOM::Parser;
@sections = $p->parse ($api)->getElementsByTagName ("sect1");

@funcptr = ();
@apifct = ();
%typedefs = ();
@prototypes = 
  map {
    $sect = $_;
    $title = $sect->getElementsByTagName ("title")->item (0);
    $name =
      get_cdata ($title->getElementsByTagName ("function")->item (0));
    push @apifct, "\&" . $name;
    @varlist = $sect->getElementsByTagName ("variablelist");
    if ($#varlist >= 0) {
      @entries = $varlist[0]->getElementsByTagName ("varlistentry");
      $args = join (", ", 
		    map {
		      $entry = $_;
		      @term = $entry->getElementsByTagName ("term");
		      @param = $term[0]->getElementsByTagName ("parameter");
		      $n = scalar split (",", get_cdata $param[0]);
		      $literal = $term[0]->getElementsByTagName
                                   ("literal")->item (0);
                      $_ = get_cdata ($literal);
		      if (/([^(]+)\(\*\)(.*)/) {
                         $_ = "funcptr". (scalar @funcptr);
 		         push @funcptr, funcptr_stub ($_, $1, $2);
                       }
                       else {
                         /(const\s+)*([^\s]+)\s*((\*)*)/;
                         $const = $1;
                         $type = $2;
                         $ast = $3;
                         $pp = ($ast eq "") ? "" : ("_" . ("p" x length $ast));
                         $inout = get_cdata ($literal->getNextSibling);
                         $inout =~ s/.*(output|input).*/$1/;
                         $_ = "$const $type$pp" . "_$inout $ast";
                         $typedefs{"$type$pp" . "_$inout"} = $type;
                       }
		       join (', ', ($_) x $n);
		} @entries);
    }
    else {
      $args = "void";
    }
    "extern void $name ($args);"
  } @sections;

# WriteSWIG  interface file

substitute ("$ARGV[1]", { TYPEDEFS => join ("\n",
                                            map {
                                              "typedef $typedefs{$_} $_;"
                                            } keys %typedefs),
                          FUNCPTR => join ("\n", @funcptr),
			  PROTOTYPES => join ("\n", @prototypes) });

substitute ("$ARGV[2]", { LIBS => $libs });

substitute ("$ARGV[3]", { EXPORTS => join " ", @apifct });
