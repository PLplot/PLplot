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

find_program(ONSGMLS onsgmls)

# Check for required programs and perl libraries.
if (BUILD_DOC)
  include(FindPerl)
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
    message("Not building htmldocumentation - required programs are missing")
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

set(INFO_MANIFEST "INFO-MANIFEST")
set(BASE_INFO "${PACKAGE}-info-${VERSION}")

set(MAN_MANIFEST "MAN-MANIFEST")
set(BASE_MAN "${PACKAGE}-man-${VERSION}")

set(MANVOL "3plplot")

# DTD definitions
set(DSSSL_DTD_PUBID "-//James Clark//DTD DSSSL Style Sheet//EN")
set(DB_SS_HTML_PUBID "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN")
set(DB_SS_PRINT_PUBID "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN")
set(DOCBOOK_DTD_PUBID "-//OASIS//DTD DocBook XML V4.2//EN")

endif (BUILD_DOC OR PREBUILT_DOC)
