# bindings/python/CMakeLists.txt
# Copyright (C) 2006-2018 Alan W. Irwin
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
      ${CMAKE_BINARY_DIR}/lib/qsastime
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
      ${CMAKE_BINARY_DIR}/lib/qsastime
      ${CMAKE_CURRENT_BINARY_DIR}
      ${PYTHON_INCLUDE_PATH}
      ${NUMPY_INCLUDE_PATH}
      ${CMAKE_SOURCE_DIR}/bindings/swig-support
      )
  endif(ENABLE_tkX)
  include_directories(${python_interface_INCLUDE_PATHS})

  # Implement _Pltk_init swig-generated Python extension module
  # that allows calling our Tcl/Tk code from python.  This result
  # works both for Python 2 and Python 3 and
  # replaces the old unmaintable plplot_widget hand-crafted extension
  # module that was not compatible with Python 3.

  # AND (ENABLE_DYNDRIVERS OR PLD_tk) Boolean logic required to
  # insure correct code to support _Pltk_init extension module is in
  # libplplot if ENABLE_DYNDRIVERS is OFF.
  if(ENABLE_tkX AND (ENABLE_DYNDRIVERS OR PLD_tk))

    # Configure build of swig-generated C wrapper for pltk_init module.
    if(ENABLE_DYNDRIVERS)
      set(pltk_init_tll_list
	PLPLOT::plplot
	PLPLOT::plplottcltk
	${PYTHON_LIBRARIES}
	)
    else(ENABLE_DYNDRIVERS)
      # Don't link to plplottcltk if ENABLE_DYNDRIVERS is false since all of
      # the plplottcltk code is already in plplot.  Therefore this
      # rule avoids redundant linking.
      set(pltk_init_tll_list
	PLPLOT::plplot
	${PYTHON_LIBRARIES}
	)
    endif(ENABLE_DYNDRIVERS)

    if(LIBRARY_TARGET_PREFIX)
      # For this experimental configuration need to specify the -module option
      # directly to swig.
      set(CMAKE_SWIG_FLAGS -module ${LIBRARY_TARGET_PREFIX}Pltk_init)
    endif(LIBRARY_TARGET_PREFIX)

    configure_library_build(Pltk_init python Pltk_init.i "${pltk_init_tll_list}")

    if(USE_RPATH)
      get_target_property(LIB_INSTALL_RPATH PLPLOT::plplot INSTALL_RPATH)
      set_target_properties(
	${SWIG_MODULE_Pltk_init_REAL_NAME}
	PROPERTIES
	INSTALL_RPATH "${LIB_INSTALL_RPATH}"
	)
    else(USE_RPATH)
      set_target_properties(
	${SWIG_MODULE_Pltk_init_REAL_NAME}
	PROPERTIES
	INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
	)
    endif(USE_RPATH)

    if(LIBRARY_TARGET_PREFIX)
      # For this experimental configuration copy the generated
      # ${LIBRARY_TARGET_PREFIX}Pltk_init.py file to the Pltk_init.py
      # name required by examples/python/pytkdemo.
      add_custom_command(TARGET ${SWIG_MODULE_Pltk_init_REAL_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_PREFIX}Pltk_init.py ${CMAKE_CURRENT_BINARY_DIR}/Pltk_init.py
	VERBATIM
	)
    endif(LIBRARY_TARGET_PREFIX)

    install(
      TARGETS ${SWIG_MODULE_Pltk_init_REAL_NAME}
      EXPORT export_plplot
      LIBRARY
      DESTINATION ${PYTHON_INSTDIR}
      )
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/Pltk_init.py
      DESTINATION ${PYTHON_INSTDIR}
    )
  endif(ENABLE_tkX AND (ENABLE_DYNDRIVERS OR PLD_tk))

  # Configure build of swig-generated C wrapper for plplotc module
  if(PL_DOUBLE)
    set(CMAKE_SWIG_FLAGS -DPL_DOUBLE_INTERFACE -DSWIG_PYTHON)
  else(PL_DOUBLE)
    set(CMAKE_SWIG_FLAGS -DSWIG_PYTHON)
  endif(PL_DOUBLE)

  if(LIBRARY_TARGET_PREFIX)
    # For this experimental configuration need to specify the -module option
    # directly to swig.
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -module ${LIBRARY_TARGET_PREFIX}plplotc)
  endif(LIBRARY_TARGET_PREFIX)

  if(PYTHON_HAVE_PYBUFFER)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DPYTHON_HAVE_PYBUFFER)
  endif(PYTHON_HAVE_PYBUFFER)

  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

  set(SWIG_MODULE_plplotc_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/swig_documentation.i
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i
    )

  set(plotc_tll_list
    PLPLOT::plplot
    ${PYTHON_LIBRARIES}
    )
  configure_library_build(plplotc python plplotc.i "${plotc_tll_list}")

  if(USE_RPATH)
    set_target_properties(
      ${SWIG_MODULE_plplotc_REAL_NAME}
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  if(LIBRARY_TARGET_PREFIX)
    # For this experimental configuration copy the generated
    # ${SWIG_MODULE_plplotc_REAL_NAME}.py file to the plplotc.py name
    # required by plplot.py.
    add_custom_command(TARGET ${SWIG_MODULE_plplotc_REAL_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_PREFIX}plplotc.py ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py
      VERBATIM
    )
  endif(LIBRARY_TARGET_PREFIX)

  if(${PYTHON_VERSION} VERSION_LESS "3.0.0")
    # Fix up the swig-generated plplotc.py code to allow UTF-8 help strings for the python2 case.
    add_custom_command(TARGET ${SWIG_MODULE_plplotc_REAL_NAME} POST_BUILD
      COMMAND ${SED_EXECUTABLE} -i -e "1s?^\\(.*\\)\$?\\1  It uses coding: utf-8?" ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py
      VERBATIM
    )
  endif(${PYTHON_VERSION} VERSION_LESS "3.0.0")

  install(
    TARGETS ${SWIG_MODULE_plplotc_REAL_NAME}
    EXPORT export_plplot
    LIBRARY
    DESTINATION ${PYTHON_INSTDIR}
    )

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
