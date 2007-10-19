# bindings/gnome2/python/CMakeLists.txt
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

if(ENABLE_pygcw)

execute_process(
COMMAND
${PKG_CONFIG_EXECUTABLE}
pygtk-2.0 --variable=defsdir
OUTPUT_VARIABLE PYGTK_DEFS
OUTPUT_STRIP_TRAILING_WHITESPACE
)
#message("PYGTK_DEFS = ${PYGTK_DEFS}")

### Prepare gcwmodule_SRCS.
# Use sed to rename PLINT, PLFLT and PLUNICODE so that h2def.py knows what to do.
add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gcw.defs
COMMAND ${PYTHON_EXECUTABLE}
${codegen}/h2def.py
${CMAKE_SOURCE_DIR}/include/gcw.h
| sed -e s/PLINT/gint32/ -e s/PLFLT/double/ -e s/PLUNICODE/guint32/
> ${CMAKE_CURRENT_BINARY_DIR}/gcw.defs
)

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gcw.c
COMMAND pygtk-codegen-2.0
--prefix gcw --override 
${CMAKE_CURRENT_SOURCE_DIR}/gcw.override
${CMAKE_CURRENT_BINARY_DIR}/gcw.defs
| sed -e s/\\"gcw_/\\"/ 
> ${CMAKE_CURRENT_BINARY_DIR}/gcw.c
DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/gcw.override
${CMAKE_CURRENT_BINARY_DIR}/gcw.defs
)

set(gcwmodule_SRCS
${CMAKE_CURRENT_BINARY_DIR}/gcw.c
gcwmodule.c
)
### gcwmodule_SRCS is now prepared.

### Prepare cplplotcanvasmodule_SRCS
# Use sed to rename PLINT, PLFLT and PLUNICODE so that h2def.py knows what to do.
add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas.defs
COMMAND ${PYTHON_EXECUTABLE}
${codegen}/h2def.py
${CMAKE_SOURCE_DIR}/include/plplotcanvas.h
| sed -e s/PLINT/gint32/ -e s/PLFLT/double/ -e s/PLUNICODE/guint32/
> ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas.defs
)

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cplplotcanvas.c
COMMAND pygtk-codegen-2.0
--prefix cplplotcanvas
--register ${PYGTK_DEFS}/gdk.defs
--register ${PYGTK_DEFS}/gtk.defs
--register ${PYGTK_DEFS}/gnome.defs
--register ${PYGTK_DEFS}/canvas.defs
--override ${CMAKE_CURRENT_SOURCE_DIR}/cplplotcanvas.override
${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas.defs
> ${CMAKE_CURRENT_BINARY_DIR}/cplplotcanvas.c
DEPENDS 
${CMAKE_CURRENT_SOURCE_DIR}/cplplotcanvas.override
${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas.defs
)

set(cplplotcanvasmodule_SRCS
${CMAKE_CURRENT_BINARY_DIR}/cplplotcanvas.c
cplplotcanvasmodule.c
)
### cplplotcanvasmodule_SRCS now prepared.

set_source_files_properties(
${gcwmodule_SRCS} ${cplplotcanvasmodule_SRCS}
PROPERTIES COMPILE_FLAGS 
"${gcw_COMPILE_FLAGS} ${PYGCW_CFLAGS}"
)

include_directories(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${PYTHON_INCLUDE_PATH}
)

### Build and install the gcw module
add_library(gcwmodule MODULE ${gcwmodule_SRCS})

target_link_libraries(
gcwmodule
plplot${LIB_TAG}
plplotgnome2${LIB_TAG}
${gcw_LINK_FLAGS}
${PYGCW_LIBS}
)

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(
  gcwmodule
  PROPERTIES 
  PREFIX ""
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  )
else(USE_RPATH)
  set_target_properties(
  gcwmodule
  PROPERTIES 
  PREFIX ""
  )
endif(USE_RPATH)

install(
TARGETS gcwmodule
LIBRARY 
DESTINATION ${PYTHON_INSTDIR}
)
### gcw module has now been built and installed

### Build and install the cplplotcanvas module
add_library(cplplotcanvasmodule MODULE ${cplplotcanvasmodule_SRCS})

target_link_libraries(
cplplotcanvasmodule
plplot${LIB_TAG}
plplotgnome2${LIB_TAG}
${gcw_LINK_FLAGS}
${PYGCW_LIBS}
)

if(USE_RPATH)
  set_target_properties(
  cplplotcanvasmodule
  PROPERTIES 
  PREFIX ""
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  )
else(USE_RPATH)
  set_target_properties(
  cplplotcanvasmodule
  PROPERTIES 
  PREFIX ""
  )
endif(USE_RPATH)

install(
TARGETS cplplotcanvasmodule
LIBRARY 
DESTINATION ${PYTHON_INSTDIR}
)
### cplplotcanvas module has now been built and installed

set(PERM_SCRIPTS
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)
install(
FILES plplotcanvas.py
DESTINATION ${PYTHON_INSTDIR}
)

endif(ENABLE_pygcw)
