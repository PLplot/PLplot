# cmake/modules/docbook.cmake
#
# Copyright (C) 2006  Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Configuration for the building of DocBook documentation 

option(BUILD_DOC "Enable build of DocBook documentation" OFF)
option(PREBUILT_DOC "Assume documentation is already built and present in doc/docbooks/src, ready to be installed. This option is useful for package maintainers" OFF)

if (BUILD_DOC AND PREBUILT_DOC)
  message(FATAL_ERROR "Options BUILD_DOC and PREBUILT_DOC are logically exclusive and must not be set simultaneously. Giving up.")
endif (BUILD_DOC AND PREBUILT_DOC)

# Website configuration
if (NOT DEFINED PLPLOT_WEBSITE)
  set(PLPLOT_WEBSITE "plplot.sf.net")
endif (NOT DEFINED PLPLOT_WEBSITE)

# Required for validation regardless of whether BUILD_DOC is set
find_program(ONSGMLS onsgmls)

# Check for required programs and perl libraries.
if (BUILD_DOC)
  if (PERL_FOUND)
    include(CheckPerlModules)
    check_perl_modules(PERL_XML_SAX XML::SAX::Expat)
    check_perl_modules(PERL_XML_PARSER XML::Parser)
    check_perl_modules(PERL_XML_DOM XML::DOM)
  else (PERL_FOUND)
    set(PERL_XML_SAX OFF)
    set(PERL_XML_PARSER OFF)
    set(PERL_XML_DOM OFF)
  endif (PERL_FOUND)
  find_program(DB2X_TEXIXML db2x_texixml)
  find_program(DB2X_XSLTPROC db2x_xsltproc)
  find_program(OPENJADE openjade)
  find_program(JADETEX jadetex)
  find_program(PDFJADETEX pdfjadetex)
  find_program(DVIPS dvips)
  find_program(MAKEINFO makeinfo)
  # Use include style here since FindUnixCommands is a simple module and
  # clearly not meant to be an official FindXXX module.
  include(FindUnixCommands)
  find_program(MKDIR mkdir)

  # Check requirements for different doc types
  set(BUILD_INFO ON)
  set(BUILD_MAN ON)
  set(BUILD_HTML ON)
  set(BUILD_PRINT ON)

  if (NOT PERL_FOUND OR NOT PERL_XML_SAX OR NOT DB2X_TEXIXML OR NOT DB2X_XSLTPROC OR NOT MAKEINFO)
    set(BUILD_INFO OFF)
    message("Not building info documentation - required programs are missing")
  endif (NOT PERL_FOUND OR NOT PERL_XML_SAX OR NOT DB2X_TEXIXML OR NOT DB2X_XSLTPROC OR NOT MAKEINFO)
    
  if (NOT PERL_FOUND OR NOT PERL_XML_PARSER OR NOT PERL_XML_DOM)
    set(BUILD_MAN OFF)
    message("Not building man documentation - required programs are missing")
  endif (NOT PERL_FOUND OR NOT PERL_XML_PARSER OR NOT PERL_XML_DOM)

  if (NOT OPENJADE) 
    set(BUILD_HTML OFF)
    message("Not building html documentation - required programs are missing")
  endif (NOT OPENJADE)

  if (NOT OPENJADE OR NOT JADETEX OR NOT PDFJADETEX OR NOT DVIPS OR NOT GZIP)
    set(BUILD_PRINT OFF)
    message("Not building print documentation - required programs are missing")
  endif (NOT OPENJADE OR NOT JADETEX OR NOT PDFJADETEX OR NOT DVIPS OR NOT GZIP)
    
endif (BUILD_DOC)

if (BUILD_DOC OR PREBUILT_DOC)
set(XML_DECL /usr/share/xml/declaration/xml.dcl)

set(BASE "${PACKAGE}-${VERSION}")

set(HTML_MANIFEST "HTML-MANIFEST")
set(BASE_HTML "${PACKAGE}-html-${VERSION}")

if(NOT DEFINED HTML_EXT)
  set(HTML_EXT "html")
endif(NOT DEFINED HTML_EXT)

set(INFO_MANIFEST "INFO-MANIFEST")
set(BASE_INFO "${PACKAGE}-info-${VERSION}")

set(MAN_MANIFEST "MAN-MANIFEST")
set(BASE_MAN "${PACKAGE}-man-${VERSION}")

set(MANVOL "3plplot")

set(JADELOG "jadeout.log")

# DTD definitions
set(DSSSL_DTD_PUBID "-//James Clark//DTD DSSSL Style Sheet//EN")
set(DB_SS_HTML_PUBID "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN")
set(DB_SS_PRINT_PUBID "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN")
set(DOCBOOK_DTD_PUBID "-//OASIS//DTD DocBook XML V4.2//EN")

# Check public identifiers
include(CheckDTD)

CheckDTD(HAVE_DSSSL_DTD  
  "DSSSL Style Sheet DTD" 
  "" 
  "" 
  "" 
  "[<!ELEMENT book - O (#PCDATA)>]" 
  "sgml" 
  "\"${DSSSL_DTD_PUBID}\"" 
  "style-sheet.dtd" 
  "jade"
  )

CheckDTD(HAVE_HTML_SS
  "DocBook HTML Stylesheet"
  "[<!ENTITY dbstyle PUBLIC \"${DB_SS_HTML_PUBID}\" CDATA DSSSL>]"
  "use=\"docbook\""
  "<external-specification id=\"docbook\" document=\"dbstyle\">"
  "[<!ELEMENT book - O (#PCDATA)>]"
  "sgml"
  "\"${DB_SS_HTML_PUBID}\""
  "html/docbook.dsl"
  "docbook-stylesheets"
  )

CheckDTD(HAVE_PRINT_SS
  "DocBook Print Stylesheet"
  "[<!ENTITY dbstyle PUBLIC \"${DB_SS_PRINT_PUBID}\" CDATA DSSSL>]"
  "use=\"docbook\""
  "<external-specification id=\"docbook\" document=\"dbstyle\">"
  "[<!ELEMENT book - O (#PCDATA)>]"
  "tex"
  "\"${DB_SS_PRINT_PUBID}\""
  "print/docbook.dsl"
  "docbook-stylesheets"
  )

CheckDTD(HAVE_DB_DTD
  "DocBook DTD"
  ""
  ""
  ""
  "PUBLIC \"${DOCBOOK_DTD_PUBID}\""
  "sgml"
  "\"${DOCBOOK_DTD_PUBID}\""
  "docbookx.dtd"
  "docbook-xml (DTD version 3.1.3)"
  )

if (BUILD_PRINT)
  if(NOT HAVE_DSSSL_DTD OR NOT HAVE_PRINT_SS OR NOT HAVE_DB_DTD)
    set (BUILD_PRINT OFF)
    message("Not building print documentation - dtd files / style sheets are missing")
  endif (NOT HAVE_DSSSL_DTD OR NOT HAVE_PRINT_SS OR NOT HAVE_DB_DTD)
endif (BUILD_PRINT)

if (BUILD_HTML)
  if (NOT HAVE_DSSSL_DTD OR NOT HAVE_HTML_SS OR NOT HAVE_DB_DTD)
    set (BUILD_HTML OFF)
    message("Not building html documentation - dtd files / style sheets are missing")
  endif (NOT HAVE_DSSSL_DTD OR NOT HAVE_HTML_SS OR NOT HAVE_DB_DTD)
endif (BUILD_HTML)

endif (BUILD_DOC OR PREBUILT_DOC)
