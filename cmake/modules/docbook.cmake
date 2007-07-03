# cmake/modules/docbook.cmake
#
# Copyright (C) 2006  Andrew Ross
# Copyright (C) 2006  Alan W. Irwin
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
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(
  BUILD_DOC OFF CACHE INTERNAL 
  "Enable build of DocBook documentation"
  )
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")

option(PREBUILT_DOC "Assume documentation is already built and present in doc/docbooks/src, ready to be installed. This option is useful for package maintainers" OFF)

if(BUILD_DOC AND PREBUILT_DOC)
  message(FATAL_ERROR "Options BUILD_DOC and PREBUILT_DOC are logically exclusive and must not be set simultaneously. Giving up.")
endif(BUILD_DOC AND PREBUILT_DOC)

# Website installation.

set(WWW_USER "" CACHE STRING "User name at WWW host")
set(WWW_GROUP "plplot" CACHE STRING "Group name at WWW host")
set(WWW_HOST "shell1.sourceforge.net" CACHE STRING "Host name at WWW host")
set(
WWW_DIR "/home/groups/p/pl/plplot/htdocs/docbook-manual" 
CACHE STRING "Dir name at WWW host"
)
set(RSH "ssh" CACHE STRING "Remote shell command")
set(RCP "scp" CACHE STRING "Remote copy command")
# Website of the PLplot project
set(PLPLOT_WEBSITE "plplot.sourceforge.net" CACHE STRING "PLplot web site")

# Required for validation regardless of whether BUILD_DOC is set
set(XML_DECL /usr/share/xml/declaration/xml.dcl)
set(DOCBOOK_DTD_PUBID "-//OASIS//DTD DocBook XML V4.2//EN")
find_program(ONSGMLS onsgmls)

# Check for required programs and perl libraries.
if(BUILD_DOC)
  if(PERL_FOUND)
    check_perl_modules(PERL_XML_PARSER XML::Parser)
    check_perl_modules(PERL_XML_DOM XML::DOM)
  endif(PERL_FOUND)
  find_program(DB2X_TEXIXML db2x_texixml)
  if(NOT DB2X_TEXIXML)
    message(STATUS "WARNING: db2x_texixml not found")
  endif(NOT DB2X_TEXIXML)
  find_program(DB2X_XSLTPROC db2x_xsltproc)
  if(NOT DB2X_XSLTPROC)
    message(STATUS "WARNING: db2x_xsltproc not found")
  endif(NOT DB2X_XSLTPROC)
  find_program(OPENJADE openjade)
  if(NOT OPENJADE)
    message(STATUS "WARNING: openjade not found")
  endif(NOT OPENJADE)
  find_program(JADETEX jadetex)
  if(NOT JADETEX)
    message(STATUS "WARNING: jadetex not found")
  endif(NOT JADETEX)
  find_program(PDFJADETEX pdfjadetex)
  if(NOT PDFJADETEX)
    message(STATUS "WARNING: pdfjadetex not found")
  endif(NOT PDFJADETEX)
  find_program(DVIPS dvips)
  if(NOT DVIPS)
    message(STATUS "WARNING: dvips not found")
  endif(NOT DVIPS)
  find_program(MAKEINFO makeinfo)
  if(NOT MAKEINFO)
    message(STATUS "WARNING: makeinfo not found")
  endif(NOT MAKEINFO)
  # Use include style here since FindUnixCommands is a simple module and
  # clearly not meant to be an official FindXXX module.
  include(FindUnixCommands)
  find_program(MKDIR mkdir)

  # Check requirements for different doc types
  set(BUILD_INFO ON)
  set(BUILD_MAN ON)
  set(BUILD_HTML ON)
  set(BUILD_PRINT ON)

  if(NOT PERL_FOUND OR NOT DB2X_TEXIXML OR NOT DB2X_XSLTPROC OR NOT MAKEINFO)
    set(BUILD_INFO OFF)
    message(STATUS
    "WARNING: Not building info documentation - "
    "required programs are missing"
    )
  endif(NOT PERL_FOUND OR NOT DB2X_TEXIXML OR NOT DB2X_XSLTPROC OR NOT MAKEINFO)
    
  if(NOT PERL_FOUND OR NOT PERL_XML_PARSER OR NOT PERL_XML_DOM)
    set(BUILD_MAN OFF)
    message(STATUS
    "WARNING: Not building man documentation - "
    "required programs are missing"
    )
  endif(NOT PERL_FOUND OR NOT PERL_XML_PARSER OR NOT PERL_XML_DOM)

  if(NOT OPENJADE) 
    set(BUILD_HTML OFF)
    message(STATUS
    "WARNING: Not building html documentation - "
    "required programs are missing"
    )
  endif(NOT OPENJADE)

  if(NOT OPENJADE OR NOT JADETEX OR NOT PDFJADETEX OR NOT DVIPS OR NOT GZIP)
    set(BUILD_PRINT OFF)
    message(STATUS
    "WARNING: Not building print documentation - "
    "required programs are missing"
    )
  endif(NOT OPENJADE OR NOT JADETEX OR NOT PDFJADETEX OR NOT DVIPS OR NOT GZIP)
    
  set(JADELOG "jadeout.log")

  # DTD definitions
  set(DSSSL_DTD_PUBID "-//James Clark//DTD DSSSL Style Sheet//EN")
  set(DB_SS_HTML_PUBID "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN")
  set(DB_SS_PRINT_PUBID "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN")

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

  if(BUILD_PRINT)
    if(NOT HAVE_DSSSL_DTD OR NOT HAVE_PRINT_SS OR NOT HAVE_DB_DTD)
      set(BUILD_PRINT OFF)
      message(STATUS
      "WARNING: Not building print documentation - "
      "dtd files / style sheets are missing"
      )
    endif(NOT HAVE_DSSSL_DTD OR NOT HAVE_PRINT_SS OR NOT HAVE_DB_DTD)
  endif(BUILD_PRINT)

  if(BUILD_HTML)
    if(NOT HAVE_DSSSL_DTD OR NOT HAVE_HTML_SS OR NOT HAVE_DB_DTD)
      set(BUILD_HTML OFF)
      message(STATUS
      "WARNING: Not building html documentation - "
      "dtd files / style sheets are missing"
      )
    endif(NOT HAVE_DSSSL_DTD OR NOT HAVE_HTML_SS OR NOT HAVE_DB_DTD)
  endif(BUILD_HTML)

endif(BUILD_DOC)

if(BUILD_DOC OR PREBUILT_DOC)
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
endif(BUILD_DOC OR PREBUILT_DOC)
