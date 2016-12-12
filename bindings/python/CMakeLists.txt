# bindings/python/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2016 Alan W. Irwin
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

if(ENABLE_python)

  # This is currently the include list for swig, the C wrapper and the
  # the Python headers. Not particular pretty...
  if(ENABLE_tkX)
    set(python_interface_INCLUDE_PATHS
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/lib/qsastime
      ${CMAKE_SOURCE_DIR}/bindings/tcl
      ${CMAKE_SOURCE_DIR}/bindings/tk
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/include
      ${CMAKE_CURRENT_BINARY_DIR}
      ${TCL_INCLUDE_PATH}
      ${TK_INCLUDE_PATH}
      ${PYTHON_INCLUDE_PATH}
      ${NUMPY_INCLUDE_PATH}
      ${CMAKE_SOURCE_DIR}/bindings/swig-support
      )
  else(ENABLE_tkX)
    set(python_interface_INCLUDE_PATHS
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/lib/qsastime
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/include
      ${CMAKE_CURRENT_BINARY_DIR}
      ${PYTHON_INCLUDE_PATH}
      ${NUMPY_INCLUDE_PATH}
      ${CMAKE_SOURCE_DIR}/bindings/swig-support
      )
  endif(ENABLE_tkX)
  include_directories(${python_interface_INCLUDE_PATHS})

  if(PL_DOUBLE)
    set(CMAKE_SWIG_FLAGS -DPL_DOUBLE_INTERFACE -DSWIG_PYTHON)
  else(PL_DOUBLE)
    set(CMAKE_SWIG_FLAGS -DSWIG_PYTHON)
  endif(PL_DOUBLE)

  if(PYTHON_HAVE_PYBUFFER)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DPYTHON_HAVE_PYBUFFER)
  endif(PYTHON_HAVE_PYBUFFER)

  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

  set(SWIG_MODULE_plplotcmodule_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/swig_documentation.i
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i
    )

  set_source_files_properties(plplotcmodule.i
    PROPERTIES SWIG_MODULE_NAME plplotc
    )

  # Set up swig + c wrapper.
  # N.B. the python target has an underscore prepended automatically.
  swig_add_module(plplotcmodule python plplotcmodule.i)

  swig_link_libraries(plplotcmodule plplot ${PYTHON_LIBRARIES})

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    set_target_properties(
      _plplotcmodule
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  if(WIN32_AND_NOT_CYGWIN)
    set_target_properties(
      _plplotcmodule
      PROPERTIES
      SUFFIX ".pyd"
      OUTPUT_NAME "_plplotc"
      )
  elseif(CYGWIN)
    set_target_properties(
      _plplotcmodule
      PROPERTIES
      SUFFIX ".dll"
      OUTPUT_NAME "_plplotc"
      )
  endif(WIN32_AND_NOT_CYGWIN)

  # Fixup required to deal with UTF-8 help strings.  Note the DEPENDS
  # only brings in a file dependency on the shared object and
  # not ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py that is also built
  # by swig.  Therefore, when the _plplotcmodule_fixed target is run the
  # following command is only executed if
  # ${CMAKE_CURRENT_BINARY_DIR}/plplotc_fixed.py is outdated relative
  # to the shared object and it ignores the dating of
  # ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py.  Thus, that file
  # can be overwritten by the command below without affecting whether
  # the custom command is run or not.
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplotc_fixed.py
    COMMAND ${SED_EXECUTABLE} -e "1s?^\\(.*\\)\$?\\1  It uses coding: utf-8?" < ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py > ${CMAKE_CURRENT_BINARY_DIR}/plplotc_fixed.py
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/plplotc_fixed.py ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py
    DEPENDS _plplotcmodule
    VERBATIM
    )
  add_custom_target(
    _plplotcmodule_fixed ALL
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplotc_fixed.py
    )
  add_dependencies(_plplotcmodule_fixed _plplotcmodule)

  # AND (ENABLE_DYNDRIVERS OR PLD_tk) Boolean logic required to
  # insure correct code to support plplot_widgetmodule is in
  # libplplot if ENABLE_DYNDRIVERS is OFF.
  if(ENABLE_tkX AND (ENABLE_DYNDRIVERS OR PLD_tk))
    add_library(plplot_widgetmodule MODULE plplot_widgetmodule.c)
    if(BUILD_SHARED_LIBS)
      set_target_properties(plplot_widgetmodule PROPERTIES
	COMPILE_DEFINITIONS "USINGDLL"
	)
    endif(BUILD_SHARED_LIBS)
    set_target_properties(plplot_widgetmodule PROPERTIES PREFIX "")

    if(ENABLE_DYNDRIVERS)
      target_link_libraries( plplot_widgetmodule
	plplot
	plplottcltk
	${PYTHON_LIBRARIES}
	)
    else(ENABLE_DYNDRIVERS)
      # Don't link to plplottcltk if ENABLE_tkX is false since all of
      # the plplottcltk code is already in plplot.  Therefore this
      # rule avoids redundant linking.
      target_link_libraries( plplot_widgetmodule
	plplot
	${PYTHON_LIBRARIES}
	)
    endif(ENABLE_DYNDRIVERS)

    if(USE_RPATH)
      set_target_properties(
	plplot_widgetmodule
	PROPERTIES
	INSTALL_RPATH "${LIB_INSTALL_RPATH}"
	)
    else(USE_RPATH)
      set_target_properties(
	plplot_widgetmodule
	PROPERTIES
	INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
	)
    endif(USE_RPATH)

    install(
      TARGETS plplot_widgetmodule _plplotcmodule
      EXPORT export_plplot
      LIBRARY
      DESTINATION ${PYTHON_INSTDIR}
      )
  endif(ENABLE_tkX AND (ENABLE_DYNDRIVERS OR PLD_tk))

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
    FILES plplot.py ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py
    DESTINATION ${PYTHON_INSTDIR}
    )
  if(ENABLE_tkX)
    install(
      FILES
      Plframe.py TclSup.py
      DESTINATION ${PYTHON_INSTDIR}
      )
  endif(ENABLE_tkX)

endif(ENABLE_python)
