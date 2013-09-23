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
if(NOT (PERL_FOUND AND CMAKE_SYSTEM_NAME STREQUAL "Linux"))
  set(
  BUILD_DOC OFF CACHE INTERNAL 
  "Enable build of DocBook documentation"
  )
endif(NOT (PERL_FOUND AND CMAKE_SYSTEM_NAME STREQUAL "Linux"))

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
set(XML_DECL "/usr/share/xml/declaration/xml.dcl" CACHE STRING "Full path to xml.dcl file.")
set(DOCBOOK_DTD_PUBID "-//OASIS//DTD DocBook XML V4.5//EN")
set(DOCBOOK_DTD_SYSID "http://www.oasis-open.org/docbook/xml/4.5/docbookx.dtd")

find_program(ONSGMLS onsgmls)

if(ONSGMLS )
  message(STATUS "validate target will be available to check for syntax issues in the PLplot DocBook documentation using ${ONSGMLS}.")
else(ONSGMLS )
  message(STATUS "WARNING: validate target will not be available to check for syntax issues in the PLplot DocBook documentation because onsgmls was not found.")
endif(ONSGMLS )

# Check for required programs and perl libraries.
if(BUILD_DOC)

  option(DOCBOOK_XML_BACKEND "Use DocBook XML/XSLT backend tools to generate our documentation from DocBook source" YES)

  # The info backend is implemented only with perl and XML/XSLT regardless of
  # DOCBOOK_XML_BACKEND.
  find_program(DB2X_TEXIXML db2x_texixml)
  if(NOT DB2X_TEXIXML)
    message(STATUS "WARNING: db2x_texixml not found")
  endif(NOT DB2X_TEXIXML)
  find_program(DB2X_XSLTPROC db2x_xsltproc)
  if(NOT DB2X_XSLTPROC)
    message(STATUS "WARNING: db2x_xsltproc not found")
  endif(NOT DB2X_XSLTPROC)
  find_program(MAKEINFO makeinfo)
  if(NOT MAKEINFO)
    message(STATUS "WARNING: makeinfo not found")
  endif(NOT MAKEINFO)
  if(PERL_FOUND AND DB2X_TEXIXML AND DB2X_XSLTPROC AND MAKEINFO)
    set(BUILD_INFO ON)
  else(PERL_FOUND AND DB2X_TEXIXML AND DB2X_XSLTPROC AND MAKEINFO)
    set(BUILD_INFO OFF)
    message(STATUS
    "WARNING: Not building info documentation - "
    "required programs are missing"
    )
  endif(PERL_FOUND AND DB2X_TEXIXML AND DB2X_XSLTPROC AND MAKEINFO)

  # The man backend is implemented only with perl and XML regardless of
  # DOCBOOK_XML_BACKEND.
  if(PERL_FOUND AND PERL_XML_PARSER AND PERL_XML_DOM)
    set(BUILD_MAN ON)
  else(PERL_FOUND AND PERL_XML_PARSER AND PERL_XML_DOM)
    set(BUILD_MAN OFF)
    message(STATUS
    "WARNING: Not building man documentation - "
    "required programs are missing"
    )
  endif(PERL_FOUND AND PERL_XML_PARSER AND PERL_XML_DOM)

  if(DOCBOOK_XML_BACKEND)
    # For this case only use xmlto.  That tool does its own warnings at
    # run time when components are missing so don't bother with looking
    # for missing components at cmake time.
    find_program(XMLTO xmlto)
    if(NOT XMLTO)
      message(STATUS "WARNING: xmlto not found")
    endif(NOT XMLTO)
    find_program(GZIP gzip)
    if(NOT GZIP)
      message(STATUS "WARNING: gzip not found")
    endif(NOT GZIP)

    if(XMLTO)
      set(BUILD_HTML ON)
      if(GZIP)
	set(BUILD_PRINT ON)
	set(BUILD_DVI OFF CACHE BOOL "Build dvi form of documentation" FORCE)
      else(GZIP)
	set(BUILD_PRINT OFF)
	message(STATUS
	  "WARNING: Not building print documentation - "
	  "gzip is not installed"
	  )
      endif(GZIP)
    else(XMLTO)
      set(BUILD_HTML OFF)
      set(BUILD_PRINT OFF)
      message(STATUS
	"WARNING: Not building html or print documentation - "
	"xmlto script is not installed"
	)
    endif(XMLTO)


  else(DOCBOOK_XML_BACKEND)
    # Deprecated SGML/DSSSL backends to generate html and print documentation.

    # These JADELOG and EC_PDFTEX variables used during course of
    # SGML/DSSSL builds
    set(JADELOG "jadeout.log")

    # EC_PDFTEX needed to configure pdftex.map which is used in general for
    # the documentation build.  The specific location in
    # /usr/share/texmf/dvips/base is used in Debian sarge and may cover other
    # distributions which still use tetex.
    find_file(EC_ENC_NAME EC.enc /usr/share/texmf/dvips/base)
    if(EC_ENC_NAME)
      # Value appropriate for tetex
      set(EC_PDFTEX EC)
    else(EC_ENC_NAME)
      # Value appropriate for texlive
      set(EC_PDFTEX ec)
    endif(EC_ENC_NAME)

    # Find programmes required for SGML/DSSSL builds.
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
    find_program(GZIP gzip)
    if(NOT GZIP)
      message(STATUS "WARNING: gzip not found")
    endif(NOT GZIP)

    # These DSSSL stylesheets needed for SGML/DSSSL builds.
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

    if(OPENJADE) 
      if(HAVE_DSSSL_DTD AND HAVE_HTML_SS AND HAVE_DB_DTD)
	set(BUILD_HTML ON)
      else(HAVE_DSSSL_DTD AND HAVE_HTML_SS AND HAVE_DB_DTD)
	set(BUILD_HTML OFF)
	message(STATUS
	  "WARNING: Not building html documentation - "
	  "dtd files / style sheets are missing"
	  )
      endif(HAVE_DSSSL_DTD AND HAVE_HTML_SS AND HAVE_DB_DTD)
    else(OPENJADE) 
      set(BUILD_HTML OFF)
      message(STATUS
	"WARNING: Not building html documentation - "
	"required programs are missing"
	)
    endif(OPENJADE)

    if(OPENJADE AND JADETEX AND PDFJADETEX AND DVIPS AND GZIP)
      if(HAVE_DSSSL_DTD AND HAVE_PRINT_SS AND HAVE_DB_DTD)
	set(BUILD_PRINT ON)
	set(BUILD_DVI ON CACHE BOOL "Build dvi form of documentation" FORCE)
      else(HAVE_DSSSL_DTD AND HAVE_PRINT_SS AND HAVE_DB_DTD)
	set(BUILD_PRINT OFF)
	message(STATUS
	  "WARNING: Not building print documentation - "
	  "dtd files / style sheets are missing"
	  )
      endif(HAVE_DSSSL_DTD AND HAVE_PRINT_SS AND HAVE_DB_DTD)
    else(OPENJADE AND JADETEX AND PDFJADETEX AND DVIPS AND GZIP)
      set(BUILD_PRINT OFF)
      message(STATUS
	"WARNING: Not building print documentation - "
	"required programs are missing"
	)
    endif(OPENJADE AND JADETEX AND PDFJADETEX AND DVIPS AND GZIP)

  endif(DOCBOOK_XML_BACKEND)
endif(BUILD_DOC)

# The "BASE" variables needed for www/documentation.php.in configuration and
# in doc/docbook/src/CMakeLists.txt.
# The "MANIFEST" variables needed in top-level CMakeLists.txt and
# in doc/docbook/src/CMakeLists.txt.
if(BUILD_DOC OR PREBUILT_DOC)
  set(BASE "${PACKAGE}-${VERSION}")
  set(INFO_MANIFEST "INFO-MANIFEST")
  set(BASE_INFO "${PACKAGE}-info-${VERSION}")
  set(MAN_MANIFEST "MAN-MANIFEST")
  set(BASE_MAN "${PACKAGE}-man-${VERSION}")
  set(HTML_MANIFEST "HTML-MANIFEST")
  set(BASE_HTML "${PACKAGE}-html-${VERSION}")
endif(BUILD_DOC OR PREBUILT_DOC)
