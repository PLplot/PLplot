#!/bin/bash
# This Linux-only script creates a sorted list of our API from several
# independent locations within our source tree and finds all the
# inconsistencies between them.

# This script should be run from the top-level source tree.

# Prepare API list from include/plplot.h to be compared with all others.
# Be sure to remove officially deprecated functions.
grep '^#define.*pl.*c_pl' include/plplot.h |\
grep -v plParseInternalOpts |\
tr '\t' " " |\
tr -s " " |\
cut --delimiter=" " --fields=2 |\
grep -v 'plclr$' |\
grep -v 'plcol$' |\
grep -v 'plhls$' |\
grep -v 'plpage$' |\
grep -v 'plrgb$' |\
grep -v 'plrgb1$' |\
sort \
>| /tmp/plplot_api.txt

case $1 in
  docbook)
    # Prepare API list from doc/docbook/src/api.xml
    # and compare with previous
    echo "documentation API differences (if any)"
    grep '<sect1.*id="pl.*".*renderas="sect.*">$' doc/docbook/src/api.xml |\
    cut --delimiter='"' --fields=2 |\
    sort |\
    diff -au /tmp/plplot_api.txt -
  ;;

  swig)
    # Prepare API list from bindings/swig-support/plplotcapi.i
    # and compare with previous
    echo "swig API differences (if any)"
    # The grep -v '[A-Z]' stanza gets rid of some of the non-public API that
    # is exposed for the swig-generated bindings, and similarly for the list
    # of commands that are excluded.  The grep -v stanzas also get rid of
    # deprecated API that is mentioned but excluded with #if 0 ... #endif

    grep '^pl.*(' bindings/swig-support/plplotcapi.i |\
    cut --delimiter='(' --fields=1 |\
    grep -v '[A-Z]' |\
    grep -v pl_cmd |\
    grep -v pldid2pc |\
    grep -v pldip2dc |\
    grep -v plf2eval |\
    grep -v plf2eval2 |\
    grep -v plf2evalr |\
    grep -v plfcont |\
    grep -v plfshade |\
    grep -v plgesc |\
    grep -v plgfile |\
    grep -v plsexit |\
    grep -v plsfile |\
    grep -v plsxwin |\
    grep -v pltr0f |\
    grep -v pltr2f |\
    grep -v pltr2p |\
    grep -v 'plhls$' |\
    grep -v 'plrgb$' |\
    grep -v 'plrgb1$' |\
    sort |\
    diff -au /tmp/plplot_api.txt -
  ;;

  java)
    # Prepare API list from bindings/java/PLStream.java
    # and compare with previous.
    echo "java API differences (if any)"
    # The grep -v '[A-Z]' stanza gets rid of some of the non-public API that
    # is exposed.
    # Also get rid of the deprecated plhls from the comparison.
    grep 'plplotjavac.pl.*(' bindings/java/PLStream.java |\
    cut --delimiter='(' --fields=1 |\
    cut --delimiter='.' --fields=2 |\
    sort -u |\
    grep -v '[A-Z]' |\
    grep -v 'plhls$' |\
    diff -au /tmp/plplot_api.txt -
  ;;

  f95)
    # Prepare API list from bindings/f95/plstubs.h
    # and compare with previous
    echo "f95 API differences (if any)"
    # After obtaining the basic name with paranthesis appended, we get rid of
    # that paranthesis and any trailing "_", and "7".  We then do a unique
    # sort to get rid of duplicates, and specifically exclude some added special
    # fortran functions (whose original form may have had a "7" appended).
    # We also remove the plhls, plrgb, and plrgb1 deprecated functions.
    grep 'FNAME.*,pl.*)' bindings/f95/plstubs.h |\
    cut --delimiter="," --fields=2 |\
    sed -e 's?)??' -e 's?_$??' -e 's?7$??' |\
    sort -u |\
    grep -v plclr |\
    grep -v 'plcol$' |\
    grep -v plcon0 |\
    grep -v plcon1 |\
    grep -v plcon2 |\
    grep -v plscmap1l2 |\
    grep -v plscmap1la2 |\
    grep -v plshade0 |\
    grep -v plshade2 |\
    grep -v plshades0 |\
    grep -v plshades1 |\
    grep -v plshades2 |\
    grep -v plvec0 |\
    grep -v plvec1 |\
    grep -v plvec2 |\
    grep -v 'plhls$' |\
    grep -v 'plrgb$' |\
    grep -v 'plrgb1$' |\
    diff -au /tmp/plplot_api.txt -
  ;;

  c++)
    # Prepare API list from bindings/c++/plstream.h
    # and compare with previous.
    echo "c++ API differences (if any)"
    grep 'plstream::' bindings/c++/plstream.cc |\
    grep -v '//' |\
    grep '(' |\
    cut --delimiter='(' --fields=1 |\
    cut --delimiter=':' --fields=3 |\
    sed 's/[^ ]*/pl&/' |\
    sort -u |\
    grep -v '[A-Z]' |\
    diff -au /tmp/plplot_api.txt -
  ;;

  tcl)
    # Prepare API list from bindings/tcl/plapi.tpl
    echo "tcl API differences (if any)"
    ( grep '^pltclcmd' bindings/tcl/plapi.tpl |\
      cut --delimiter=' ' --fields=2  && \
      grep '{"pl' bindings/tcl/tclAPI.c | \
      cut --delimiter='"' --fields=2 \
    ) | \
    sort -u | \
    diff -au /tmp/plplot_api.txt -
  ;;

  all)
    $0 docbook
    $0 swig
    $0 java
    $0 f95
    $0 c++
    $0 tcl
  ;;

  *)
  echo "First argument was $1"
  echo "Instead, it must be one of the following:"
  echo "docbook, swig, java, f95, c++, tcl or all"
  ;;
esac
#rm /tmp/plplot_api.txt
