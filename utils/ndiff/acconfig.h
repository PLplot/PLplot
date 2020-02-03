/* acconfig.h

Entries in this file provide defaults for preprocessor symbols that
autoconf does not already know about in its
/usr/local/share/autoconf/acconfig.h file.  They are inserted in the
config.h.in file by the autoheader utility.

Contrary to the autoconf 2.12 documentation in Node: Invoking
autoheader, at least one of the <at>TOP<at> or <at>BOTTOM<at> markers
must be present.  If both are omitted, then the contents of this file
are lost.
*/

@BOTTOM@

#undef HAVE_GMP
