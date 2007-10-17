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

# This is a representative generated (or prebuilt) octave documentation file.
# There are more than a hundred of these *.txt files.  If the (unlikely)
# event that the plinit command is no longer part of the PLplot API, this
# representative file would have to be changed.
set(rep_doc plinit.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_txt/${rep_doc})
  # Octave documentation has been prebuilt in the (tarball) source tree.
  # Copy it to the build tree if that tree is different from source tree.
  if(NOT CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/${rep_doc}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/bindings/octave/plplot_octave_txt
    ${CMAKE_BINARY_DIR}/bindings/octave/plplot_octave_txt
    )
  endif(NOT CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
  add_custom_target(
  make_documentation ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/${rep_doc}
  )
else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_txt/${rep_doc})
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
  if(PERL_XML_DOM AND PERL_XML_PARSER)
    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/${rep_doc}
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
  
    add_custom_target(
    make_documentation ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/${rep_doc}
    )
  else(PERL_XML_DOM AND PERL_XML_PARSER)
    message(STATUS 
    "WARNING: Perl modules XML::Parser and/or XML::DOM not available\n"
    "   so cannot generate full online help for plplot_octave"
    )
    add_custom_target(
    make_documentation ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave_txt/plplot.doc
    )
  endif(PERL_XML_DOM AND PERL_XML_PARSER)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_txt/${rep_doc})

# Generate source code for octave interface to PLplot.
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave.h.in
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.h
@ONLY
)
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/octave_version.pl.in
${CMAKE_CURRENT_BINARY_DIR}/octave_version.pl
@ONLY
)
add_custom_command(
OUTPUT 
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.cc 
${CMAKE_CURRENT_BINARY_DIR}/tmp_stub
COMMAND ${PERL_EXECUTABLE}
-I${MATWRAP_PATH} ${MATWRAP} 
-language octave -o plplot_octave.cc -stub tmp_stub
-cpp_ignore ${CMAKE_CURRENT_SOURCE_DIR} 
-cpp_ignore ${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_rej.h
-cpp ${CMAKE_C_COMPILER} -D__builtin_va_list=void -E 
-I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_BINARY_DIR} -C plplot_octave.h
DEPENDS
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.h
)

# Build octave interface.
set(octave_interface_INCLUDE_PATHS
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${OCTAVE_INCLUDE_PATH}
)
include_directories(${octave_interface_INCLUDE_PATHS})

add_library(plplot_octave MODULE ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.cc)
target_link_libraries(
plplot_octave
plplot${LIB_TAG}
"${OCTAVE_LIBRARIES}"
"${OCTINTERP_LIBRARIES}"
)

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  # (Reasonable) assumption here is that OCTAVE_LIBRARIES and
  # OCTINTERP_LIBRARIES have the same path.
  get_filename_component(OCTAVE_INSTALL_RPATH "${OCTAVE_LIBRARIES}" PATH)
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${OCTAVE_INSTALL_RPATH})
  set_target_properties(
  plplot_octave
  PROPERTIES
  PREFIX "" 
  SUFFIX ".oct"
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${OCTAVE_OCT_DIR}"
  )
else(USE_RPATH)
  set_target_properties(
  plplot_octave
  PROPERTIES
  PREFIX "" 
  SUFFIX ".oct"
  INSTALL_NAME_DIR "${OCTAVE_OCT_DIR}"
  )
endif(USE_RPATH)

# Have to be specific about permissions for some reason (probably oct suffix).
set(PERM_MODULES
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)

install(
TARGETS
plplot_octave
LIBRARY
DESTINATION ${OCTAVE_OCT_DIR}
PERMISSIONS ${PERM_MODULES}
)

# Build and install plplot_stub.m
add_executable(massage massage.c)
get_target_property(
massage_LOCATION
massage
LOCATION
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m_part2 
"# It was also massaged to add online documentation\n"
"# extracted from some PLplot distribution files\n"
"\n"
"1;\n"
"\n"
)
add_custom_command(
OUTPUT 
${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
${CMAKE_CURRENT_BINARY_DIR}/missing_help
COMMAND head -5 tmp_stub > plplot_stub.m
COMMAND
cat plplot_stub.m_part2 >> plplot_stub.m
COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_def >> plplot_stub.m
COMMAND echo >> plplot_stub.m
COMMAND 
${massage_LOCATION} >> plplot_stub.m 2> missing_help
DEPENDS
${CMAKE_CURRENT_BINARY_DIR}/tmp_stub
${CMAKE_CURRENT_SOURCE_DIR}/plplot_octave_def
${massage_LOCATION}
)

add_custom_target(
plplot_stub.m_built ALL
DEPENDS
${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
)

install(FILES 
${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
DESTINATION ${PLPLOT_OCTAVE_DIR}
)

if(never)
add_custom_command(
OUTPUT 
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.oct
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.o
COMMAND
touch
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.oct
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.o
DEPENDS
${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.cc
)

# Link octave interface.
add_custom_target(
plplot_octave-libdir.oct ALL
DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_octave.o
)
endif(never)
endif(ENABLE_octave)
