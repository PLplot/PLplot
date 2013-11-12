# bindings/swig-support/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2010 Alan W. Irwin
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

# Add (Unix) custom target to check that swig_documentation.i is up to
# date with our DocBook api chapter
if(PERL_XML_DOM AND PERL_XML_PARSER)
  add_custom_target(
    check_swig_documentation
    COMMAND ${CMAKE_COMMAND} -E remove -f
    ${CMAKE_CURRENT_BINARY_DIR}/swig_documentation.i_compare
    COMMAND ${PERL_EXECUTABLE}
    ${CMAKE_SOURCE_DIR}/doc/docbook/bin/api2swigdoc.pl
    ${CMAKE_SOURCE_DIR}/doc/docbook/src/plplotdoc.xml.in
    ${CMAKE_SOURCE_DIR}/doc/docbook/src/api.xml
    ${CMAKE_CURRENT_BINARY_DIR}/swig_documentation.i_compare
    COMMAND ${CMAKE_COMMAND} -E echo "Check that swig_documentation.i is consistent with doc/docbook/src/api.xml"
    COMMAND cmp 
    ${CMAKE_CURRENT_SOURCE_DIR}/swig_documentation.i
    ${CMAKE_CURRENT_BINARY_DIR}/swig_documentation.i_compare
    )
  
  set(make_documentation_DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/${rep_doc}
    )
else(PERL_XML_DOM AND PERL_XML_PARSER)
  message(STATUS 
    "WARNING: Perl modules XML::Parser and/or XML::DOM not available\n"
    "   so cannot check that swig_documentation.i is up to date."
    )
endif(PERL_XML_DOM AND PERL_XML_PARSER)

# Add (Unix) custom target to check numerical #defines in plplotcapi.i.
add_custom_target(
  check_plplotcapi_defines
  COMMAND ${CMAKE_COMMAND} -E remove -f
  ${CMAKE_CURRENT_BINARY_DIR}/generated_plplotcapi_defines
  COMMAND grep -E "^ *#define +[^ ]+ +[()-+0-9x]+" ${CMAKE_SOURCE_DIR}/include/plplot.h | grep -v plotsh3d > ${CMAKE_CURRENT_BINARY_DIR}/generated_plplotcapi_defines
  COMMAND ${CMAKE_COMMAND} -E echo "Non-NULL diff results below (which will be accompanied by an error for this target) imply the numerical #defines section of bindings/swig-support/plplotcapi.i needs updating following the directions in that file"
  COMMAND grep -E "^ *#define +[^ ]+ +[()-+0-9x]+" ${CMAKE_CURRENT_SOURCE_DIR}/plplotcapi.i | diff -auw - ${CMAKE_CURRENT_BINARY_DIR}/generated_plplotcapi_defines
  VERBATIM
)