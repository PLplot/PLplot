
dnl ### AC_INIT(src/plplotdoc.xml.in)
dnl ### AM_INIT_AUTOMAKE(plplotdoc, 0.4.3)

dnl Web site Installation

AC_ARG_WITH(www-user,
  AC_HELP_STRING([--with-www-user=NAME], [User name at WWW host]),
  [WWW_USER="$withval@"],
  [WWW_USER=""])
AC_SUBST(WWW_USER)

AC_ARG_WITH(www-group,
  AC_HELP_STRING([--with-www-group=NAME], [Group name at WWW host]),
  [WWW_GROUP="$withval"],
  [WWW_GROUP="plplot"])
AC_SUBST(WWW_GROUP)

AC_ARG_WITH(www-host,
  AC_HELP_STRING([--with-www-host=NAME], [Host name at WWW host]),
  [WWW_HOST="$withval"],
  [WWW_HOST="shell1.sourceforge.net"])
AC_SUBST(WWW_HOST)

AC_ARG_WITH(www-dir,
  AC_HELP_STRING([--with-www-dir=NAME], [Dir name at WWW host]),
  [WWW_DIR="$withval"],
  [WWW_DIR="/home/groups/p/pl/plplot/htdocs/resources/docbook-manual"])
AC_SUBST(WWW_DIR)

AC_ARG_WITH(rsh-command,
  AC_HELP_STRING([--with-rsh-command=NAME], [Remote shell command]),
  [RSH="$withval"],
  [RSH="ssh"])
AC_SUBST(RSH)

AC_ARG_WITH(rcp-command,
  AC_HELP_STRING([--with-rcp-command=NAME], [Remote copy command]),
  [RCP="$withval"],
  [RCP="scp"])
AC_SUBST(RCP)


dnl Website of the PLplot project

AC_ARG_WITH(plplot-website,
  AC_HELP_STRING([--with-plplot-website=NAME], [PLplot web site]),
  [PLPLOT_WEBSITE="$withval"],
  [PLPLOT_WEBSITE="plplot.sf.net"])
AC_SUBST(PLPLOT_WEBSITE)


dnl System wide XML declaration

XML_DECL=/usr/lib/sgml/declaration/xml.dcl
AC_ARG_WITH(xml-declaration,
  AC_HELP_STRING([--with-xml-declaration=FILE],
    [System wide file containing the SGML declaration for XML.
     Must be a absolute file name.
     Default: /usr/lib/sgml/declaration/xml.dcl]),
  [XML_DECL=$withval])
AC_SUBST(XML_DECL)


dnl Jade output log

jadelog=jadeout.log
rm -f $jadelog
JADELOG=$jadelog
AC_SUBST(JADELOG)


dnl DTD definitions.
dnl
dnl The following public identifiers should correspond to those in the
dnl SGML source files.

DSSSL_DTD_PUBID="-//James Clark//DTD DSSSL Style Sheet//EN"
DB_SS_HTML_PUBID="-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN"
DB_SS_PRINT_PUBID="-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN"
DOCBOOK_DTD_PUBID="-//OASIS//DTD DocBook XML V4.1//EN"

AC_SUBST(DSSSL_DTD_PUBID)
AC_SUBST(DB_SS_HTML_PUBID)
AC_SUBST(DB_SS_PRINT_PUBID)
AC_SUBST(DOCBOOK_DTD_PUBID)

dnl Utility macros

dnl CHECK_PROG(program, url)
AC_DEFUN(CHECK_PROG, [

pushdef([PROG], translit($1, [a-z], [A-Z]))

AC_ARG_WITH($1,
  AC_HELP_STRING([--with-$1=PATH/]PROG, [Alternative $1 program]),
  [prog_$1=$withval],
  [prog_$1=$1])
AC_CHECK_PROG(has_$1, [$prog_$1], found, no)
PROG=$prog_$1
AC_SUBST(PROG)
if test $has_$1 = no ; then
  for i in $2 ; do
    export $i=""
  done
fi
AC_OUTPUT_COMMANDS( [
if test $has_$1 = no ; then]
AC_MSG_WARN( [
Program $1 not found.
    Install the $1 program in your path or specify its
    location with the option --with-$1.
    The $1 package can be found at
    $3.])
[fi], has_$1=$has_$1)
AM_CONDITIONAL(has_$1, [test "$has_$1" = found])
popdef([PROG])
])

PRINT=print
HTML=html
INFO=info
MAN=man

dnl CHECK_DTD(title, cache-id, dsssl_dtd, docbookb_ss_dtd, style_spec_use,
dnl           external_specification, docbook_dtd, jade_output_type,
dnl           origin_package)
dnl ### CONFTEST=conftest
CONFTEST=doc/docbook/conftest
AC_SUBST(CONFTEST)
AC_DEFUN(CHECK_DTD, [
AC_CACHE_CHECK($1, pldb_cv_$2, [
echo -e "\nChecking for $1" >> $jadelog
sed -e "s|@DSSSL_DTD@|\"$DSSSL_DTD_PUBID\"|" -e "s|@DB_STYLE_SHEET@|$3|" \
  -e "s|@STYLE_SPEC_USE@|$4|" -e "s|@EXTERNAL_SPECIFICATION@|$5|" \
dnl ###  -e "s|@CONFTEST@|$CONFTEST|" < cnf/test.dsl.in > $CONFTEST.dsl
  -e "s|@CONFTEST@|$CONFTEST|" < doc/docbook/cnf/test.dsl.in > $CONFTEST.dsl
dnl ### sed "s|@DOCBOOK_DTD@|$6|" < cnf/test.xml.in > $CONFTEST.xml
sed "s|@DOCBOOK_DTD@|$6|" < doc/docbook/cnf/test.xml.in > $CONFTEST.xml

$prog_openjade $SGML_CATALOGS -d $CONFTEST.dsl -t $7 -o $CONFTEST.out \
   $XML_DECL $CONFTEST.xml > $jadelog.x 2>&1
jade_result=`egrep ":E:" $jadelog.x`
cat $jadelog.x >> $jadelog
rm -f $jadelog.x
if test "$jade_result" = "" ; then
  pldb_cv_$2=found
else
  pldb_cv_$2=no
fi
])
if test $pldb_cv_$2 = no ; then
  for i in $11 ; do
    export $i=""
  done
fi
AC_OUTPUT_COMMANDS([
if test $pldb_cv_$2 = no ; then]
  AC_MSG_WARN([Could not find $1.
    The following SGML public identifier could not be found in any of
    the default SGML catalogs:
        $dtd_$2
    Its system identifier[,] i.e. a file usually called
        \"$9\"
    is distributed with the $10 package.  Install it in your
    system and put the correct entry in the catalog file.
    You might also use the configure option --with-sgml-catalog to specify
    alternative SGML catalogs.])
[fi
], [
pldb_cv_$2=$pldb_cv_$2
dtd_$2="$8"
])
])

dnl SGML catalogs

AC_ARG_WITH(sgml-catalogs,
  AC_HELP_STRING([--with-sgml-catalogs=CATALOGS],
    [SGML catalogs in a colon (:) separated list.
     Must contain only existent files.]),
  [SGML_CATALOGS=$withval],
  [SGML_CATALOGS=""])

[
for i in `echo $SGML_CATALOGS | sed "s/:/ /g"` ; do
  if test ! -f $i ; then ]
AC_MSG_ERROR([Catalog file $i is not valid.
    Specify only existent files with option --with-sgml-catalogs.])[
  fi
done

if test ! "$SGML_CATALOGS" = "" ;then
  SGML_CATALOGS="-c `echo $SGML_CATALOGS | sed 's/:/ -c /g'`"
fi
]

AC_SUBST(SGML_CATALOGS)

dnl File extensions

AC_DEFUN([FILE_EXT], [
pushdef([FILE], translit($1, [a-z], [A-Z]))
FILE[_EXT=$1]
AC_ARG_WITH($1-extension,
  AC_HELP_STRING([--with-$1-extension=EXT],
    [File extension for the generated FILE files. (Default value: $1)]),
  [FILE[_EXT=$withval]])
AC_SUBST(FILE[_EXT])
popdef([FILE])
])

FILE_EXT(html)


dnl Check programs

CHECK_PROG(openjade, [HTML PRINT], http://www.openjade.org)
CHECK_PROG(jadetex, PRINT, http://jadetex.sourceforge.net/)
CHECK_PROG(pdfjadetex, PRINT, http://jadetex.sourceforge.net/)
CHECK_PROG(dvips, PRINT, http://www.ctan.org)
CHECK_PROG(perl, [INFO MAN], http://www.perl.com)
CHECK_PROG(onsgmls, [], http://www.openjade.org)

dnl Check public identifiers

CHECK_DTD(
  [DSSSL Style Sheet DTD],
  [dsssl_dtd],
  [],
  [],
  [],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [sgml],
  [\"$DSSSL_DTD_PUBID\"],
  [style-sheet.dtd],
  [jade],
  [PRINT HTML])

CHECK_DTD(
  [DocBook HTML Stylesheet],
  [html_ss],
  [[[<!ENTITY dbstyle PUBLIC \"$DB_SS_HTML_PUBID\" CDATA DSSSL>]]],
  [use="docbook"],
  [<external-specification id="docbook" document="dbstyle">],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [sgml],
  [\"$DB_SS_HTML_PUBID\"],
  [html/docbook.dsl],
  [docbook-stylesheets],
  [HTML])

CHECK_DTD(
  [DocBook Print Stylesheet],
  [print_ss],
  [[[<!ENTITY dbstyle PUBLIC \"$DB_SS_PRINT_PUBID\" CDATA DSSSL>]]],
  [use="docbook"],
  [<external-specification id="docbook" document="dbstyle">],
  [[[<!ELEMENT book - O (#PCDATA)>]]],
  [tex],
  [\"$DB_SS_PRINT_PUBID\"],
  [print/docbook.dsl],
  [docbook-stylesheets],
  [PRINT])

CHECK_DTD(
  [DocBook DTD],
  [db_dtd],
  [],
  [],
  [],
  [PUBLIC \"$DOCBOOK_DTD_PUBID\"],
  [sgml],
  [\"$DOCBOOK_DTD_PUBID\"],
  [docbookx.dtd],
  [docbook-xml (DTD version 3.1.3)],
  [PRINT HTML])

dnl Info building

AC_DEFUN(CHECK_PERL_SCRIPT, [

pushdef([PROG], translit($1, [a-z], [A-Z]))

AC_ARG_WITH($1,
  AC_HELP_STRING([--with-$1=PATH/]PROG, [Alternative $1 Perl script]),
  [prog_$1=$withval],
  [prog_$1=/usr/bin/$1])
AC_MSG_CHECKING([for Perl script $1])
if test -x $prog_$1 ; then
  has_$1=found
else
  has_$1=no
fi
AC_MSG_RESULT([$has_$1])
PROG=$prog_$1
AC_SUBST(PROG)
if test $has_$1 = no ; then
  for i in $2 ; do
    export $i=""
  done
fi
AC_OUTPUT_COMMANDS( [
if test $has_$1 = no ; then]
AC_MSG_WARN( [
Program $1 not found.
    Install the perl script $1 program in your system or specify its
    location with the option --with-$1.
    The $1 package can be found at
    $3.])
[fi], has_$1=$has_$1)

popdef([PROG])
])

DOCBOOK2TEXIXML=`pwd`/doc/docbook/perl/docbook2texixml
AC_SUBST(DOCBOOK2TEXIXML)
TEXI_XML=`pwd`/doc/docbook/perl/db2x_texixml
AC_SUBST(TEXI_XML)

CHECK_PROG(makeinfo, [INFO], [http://www.gnu.org])

AC_DEFUN(CHECK_PM, [
AC_CACHE_CHECK( $1, pldb_cv_$2, [
cat > conftest.pl << EOF
use $1;
EOF
if test -n "$prog_perl" && $prog_perl conftest.pl >/dev/null 2>&1 ; then
  pldb_cv_$2=yes
else
  pldb_cv_$2=no
fi])
if test $pldb_cv_$2 = no ; then
  for i in $3; do
    export $i=""
  done
fi
AC_OUTPUT_COMMANDS([
if test $pldb_cv_$2 = no ; then]
  AC_MSG_WARN([
Could not find Perl module $1.
    Install it in your system at a place where Perl can find
    it (through @INC).])
[fi], [
pldb_cv_$2=$pldb_cv_$2
])
])

DOCBOOK2X_INC=-I`pwd`/doc/docbook/perl
AC_SUBST(DOCBOOK2X_INC)

CHECK_PM(XML::DOM, xml_dom, [INFO])
CHECK_PM(XML::Parser::PerlSAX, xml_parser_perlsax, [INFO])
CHECK_PM(XML::Writer, xml_writer, [INFO])

dnl Output commands

AC_OUTPUT_COMMANDS( [
if test "$print" = "" ; then
  echo
  echo '  ******************************************************************'
  echo '  * Due to some lacking dependencies (see warning messages above), *'
  echo '  * it will not be possible to build a printed version of the      *'
  echo '  * documentation.                                                 *'
  echo '  ******************************************************************'
fi
if test "$html" = "" ; then
  echo
  echo '  ******************************************************************'
  echo '  * Due to some lacking dependencies (see warning messages above), *'
  echo '  * it will not be possible to build a HTML version of the         *'
  echo '  * documentation.                                                 *'
  echo '  ******************************************************************'
fi
if test "$info" = "" ; then
  echo
  echo '  ******************************************************************'
  echo '  * Due to some lacking dependencies (see warning messages above), *'
  echo '  * it will not be possible to build an info version of the        *'
  echo '  * documentation.                                                 *'
  echo '  ******************************************************************'
fi
if test "$man" = "" ; then
  echo
  echo '  ******************************************************************'
  echo '  * Due to some lacking dependencies (see warning messages above), *'
  echo '  * it will not be possible to build the man pages of the API      *'
  echo '  * chapter.                                                       *'
  echo '  ******************************************************************'
fi], [
print=$PRINT; html=$HTML; info=$INFO; man=$MAN
])


dnl Control build of man and info pages

AC_ARG_ENABLE(info,
  AC_HELP_STRING([--disable-info], [Disable build of info pages]),
  [if test "$enable_info" = "no" ; then
    INFO=""
    AC_MSG_WARN([
Info pages will not be build at your request.])
   fi])
AC_ARG_ENABLE(man,
  AC_HELP_STRING([--disable-man], [Disable build of man pages]),
  [if test "$enable_man" = "no" ; then
    MAN=""
    AC_MSG_WARN([
Man pages will not be build at your request.])
    fi])

TARGETS="$PRINT $HTML $INFO $MAN"
AC_SUBST(TARGETS)


dnl ### AC_OUTPUT([Makefile
dnl ###          src/Makefile src/plplotdoc.xml src/plplotdoc-html.dsl
dnl ###          src/plplotdoc-print.dsl
dnl ###          bin/Makefile bin/api2man.pl
dnl ###          www/index.html.in])
