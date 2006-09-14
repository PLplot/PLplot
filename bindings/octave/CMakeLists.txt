# bindings/octave/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Build and install octave interface to PLplot and octave PLplot examples.
if(ENABLE_octave)

add_subdirectory(PLplot)
add_subdirectory(demos)
add_subdirectory(misc)

# Install renamed documentation files.
set(docfiles
BUGS
FGA
INSTALL
README
ToDo
USAGE
)
foreach(doc ${docfiles})
  install(FILES ${doc} DESTINATION ${DOC_DIR} RENAME ${doc}.octave)
endforeach(doc ${docfiles})

# Generate full on-line help for plplot_octave
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt)
add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/plplot.doc
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_CURRENT_SOURCE_DIR}/etc/plplot.doc
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/plplot.doc
DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/etc/plplot.doc
)
if(PERL_FOUND AND PERL_XML_DOM AND PERL_XML_PARSER)
  add_custom_target(
  make_documentation ALL
  COMMAND ${PERL_EXECUTABLE}
  ${CMAKE_SOURCE_DIR}/doc/docbook/bin/api2text.pl
  ${CMAKE_SOURCE_DIR}/doc/docbook/src/plplotdoc.xml.in
  ${CMAKE_SOURCE_DIR}/doc/docbook/src/api.xml
  DEPENDS 
  ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/plplot.doc
  ${CMAKE_SOURCE_DIR}/doc/docbook/bin/api2text.pl
  ${CMAKE_SOURCE_DIR}/doc/docbook/src/plplotdoc.xml.in
  ${CMAKE_SOURCE_DIR}/doc/docbook/src/api.xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt
  )
else(PERL_FOUND AND PERL_XML_DOM AND PERL_XML_PARSER)
  message(STATUS 
  "WARNING: Perl modules XML::Parser and/or XML::DOM not available\n"
  "   so cannot generate full online help for plplot_octave"
  )
  add_custom_target(
  make_documentation ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/plplot.doc
  )
endif(PERL_FOUND AND PERL_XML_DOM AND PERL_XML_PARSER)

endif(ENABLE_octave)
