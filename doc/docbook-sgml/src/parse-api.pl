#!/usr/bin/perl

open (NAMES, "< api.tex") or die "File api.tex not found\n";
open (TMP, "> TMP") or die "Cannot open TMP\n";

print TMP "
<chapter id=\"API\">
  <title>The PLplot Applications Programming Interface</title>
  
  <para>
    Documentation of each PLplot function.
  </para>

  <para>
    Many new ones are still missing.
  </para>";

while (<NAMES>) {
  if (/\* ([^ ]+)::\s+([^\s].*)/) {
    print TMP "

  <sect1>
    <title><function>$1</function>: $2</title>

    <para><anchor id=\"$1\">
      <funcsynopsis> 
	<funcdef><function>$1</function></funcdef>";
    open (API, "< api.tex");
    while (<API>) {
      if (/\\node $1,/) {
	while (<API>) {
	  if (/\\bname\{[^\(]+\((.*)\)\}/) {
	    $param = $1;
	    @params = split (",", $param);
	    if ( $#params < 0 ) {
	      print TMP "
        <paramdef></paramdef>";
	    }
	    else {
	      foreach $i (@params) {
		print TMP "
        <paramdef>
	  <parameter>$i</parameter>
	</paramdef>";
	      }
	    }
	    print TMP "
      </funcsynopsis>
    </para>";
	    $/ = "";
	    while (<API>) {
	      $descr = $_;
	      if (/(^\\descr\{ )/) {
		print TMP "
    <para>
      ".substr ($descr, length $1)."
    </para>";
		$start = 1;
		while (<API>) {
		  if (/^\\node /) {
		    last;
		  }
		  $descr = $_;
		  if (/^(\\argu\{ \\code\{(.*)\} \((.*), (.*)\)\}\{)/) {
		    if ( $start ) {
		      print TMP "
    <variablelist>";
		      $start = 0;
		    }
		    print TMP "
      <varlistentry>
	<term>
	  <parameter>$2</parameter>
	  (<literal>$3</literal>, $4)
	</term>
	<listitem>
	  <para>
	    ".substr ($descr, length $1)."
	  </para>
	</listitem>
      </varlistentry>";
		  }
		}
		if ( not $start ) {
		  print TMP "
    </variablelist>";
		}
		print TMP "
  </sect1>";
		last;
	      }
	    }
	    last;
	  }
	}
	last;
      }
    }
    $/ = "\n";
    close API;
  }
}


close NAMES;

print TMP "
</chapter>

<!-- Keep this comment at the end of the file
Local variables:
mode: sgml
sgml-omittag:t
sgml-shorttag:t
sgml-minimize-attributes:nil
sgml-always-quote-attributes:t
sgml-indent-step:2
sgml-indent-data:t
sgml-parent-document:nil
sgml-default-dtd-file:nil
sgml-exposed-tags:nil
sgml-local-catalogs:nil
sgml-local-ecat-files:nil
End:
-->
";

close TMP;

open (TMP, "< TMP");

while (<TMP>) {
  if (/^\\begin\{itemize\}/) {
    print "     <itemizedlist>\n";
    $start = 1;
  }
  elsif (/^\\item$/) {
    if ( not $start ) {
      print "	</para>
      </listitem>\n";
    }
    $start = 0;
    print "      <listitem>
	<para>\n";
  }
  elsif (/^\\end\{itemize\}/) {
    print "	</para>
      </listitem>
    </itemizedlist>\n";
  }
  elsif (/^\}$/) {
    print "\n";
  }
  else {
    print;
  }
}

close TMP;



art = 1;
  }
  elsif (/^\\item$/) {
    if ( not $start ) {
      print "	</para>
      </listitem>\n";
    }
    $start = 0;
    print "      <listitem>
	<para>\n";
  }
  elsif (/^\\end\{itemize\}/) {
    print "	</para>
      </listitem>
    </itemizedlist>\n";
  }
  elsif (/^\}$/) {
    print "\n";
  }
  else {
    print;
  }
}

close TMP;



