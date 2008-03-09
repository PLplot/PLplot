#!/bin/bash
# This Linux-only script creates a sorted list of our API from several
# independent locations within our source tree and finds all the 
# inconsistencies between them.

# This script should be run from the top-level source tree.

# Prepare API list from include/plplot.h to be compared with all others.
grep '^#define.*pl.*c_pl' include/plplot.h |\
grep -v plParseInternalOpts |\
tr '\t' " " |\
tr -s " " |\
cut --delimiter=" " --fields=2 |\
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
    # of commands that are excluded.
    
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
    sort |\
    diff -au /tmp/plplot_api.txt -
  ;;

  octave)
    # Prepare API list from bindings/octave/plplot_octave.h.in
    # and compare with previous.
    echo "octave API differences (if any)"
    grep '^#define.*pl.*c_pl' bindings/octave/plplot_octave.h.in |\
    grep -v plParseInternalOpts |\
    tr '\t' " " |\
    tr -s " " |\
    cut --delimiter=" " --fields=2 |\
    sort |\
    diff -au /tmp/plplot_api.txt -
  ;;
  
  f77)
    # Prepare API list from bindings/f77/plstubs.h
    # and compare with previous
    echo "f77 API differences (if any)"
    # After obtaining the basic name with paranthesis appended, we get rid of
    # that paranthesis and any trailing underscores or "7"s.  We then do a unique
    # sort to get rid of duplicates, and specifically exclude some added special
    # fortran functions (whose original form may have had a "7" appended).
    grep 'FNAME.*,pl.*)' bindings/f77/plstubs.h |\
    cut --delimiter="," --fields=2 |\
    sed -e 's?)??' -e 's?_$??' -e 's?7$??' |\
    sort -u |\
    grep -v plclr |\
    grep -v 'plcol$' |\
    grep -v plcon0 |\
    grep -v plcon1 |\
    grep -v plcon2 |\
    grep -v plshade0 |\
    grep -v plshade2 |\
    grep -v plshades0 |\
    grep -v plshades1 |\
    grep -v plshades2 |\
    grep -v plvec0 |\
    grep -v plvec1 |\
    grep -v plvec2 |\
    diff -au /tmp/plplot_api.txt - 
  ;;

  f95)
    # Prepare API list from bindings/f95/plstubs.h
    # and compare with previous
    echo "f95 API differences (if any)"
    # After obtaining the basic name with paranthesis appended, we get rid of
    # that paranthesis and any trailing "_", "f77", and "7".  We then do a unique
    # sort to get rid of duplicates, and specifically exclude some added special
    # fortran functions (whose original form may have had a "7" appended).
    grep 'FNAME.*,pl.*)' bindings/f95/plstubs.h |\
    cut --delimiter="," --fields=2 |\
    sed -e 's?)??' -e 's?_$??' -e 's?f77$??' -e 's?7$??' |\
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
    diff -au /tmp/plplot_api.txt - 
  ;;
  
  all)
    $0 docbook
    $0 swig
    $0 octave
    $0 f77
    $0 f95
  ;;

  *)
  echo "First argument was $1"
  echo "Instead, it must be one of the following:"
  echo "docbook, swig, octave, f77, f95, or all"
  ;;
esac
#rm /tmp/plplot_api.txt
