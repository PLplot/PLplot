# examples/c/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2015 Alan W. Irwin
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

# N.B. This file is used for both the core build (which installs the examples
# and optionally [depending on BUILD_TEST} builds them) and the installed
# examples build.  The core build has BUILD_TEST OFF or ON at user option
# and CORE_BUILD always ON.  The installed examples build always has
# BUILD_TEST ON and CORE_BUILD OFF.

set(c_STRING_INDICES
  "00"
  "01"
  "02"
  "03"
  "04"
  "05"
  "06"
  "07"
  "08"
  "09"
  "10"
  "11"
  "12"
  "13"
  "14"
  "15"
  "16"
  "17"
  "18"
  "19"
  "20"
  "21"
  "22"
  "23"
  "24"
  "25"
  "26"
  "27"
  "28"
  "29"
  "30"
  "31"
  "32"
  "33"
  "34"
  )

if(CORE_BUILD)
  set(c_SRCS
    plcdemos.h
    tutor.c
    test_plend.c
    test_plbuf.c
    )
  foreach(STRING_INDEX ${c_STRING_INDICES})
    set(c_SRCS ${c_SRCS} x${STRING_INDEX}c.c)
  endforeach(STRING_INDEX ${c_STRING_INDICES})

  if(PLD_xcairo)
    set(extXdrawable_SRC extXdrawable_demo.c)
  endif(PLD_xcairo)

  if(PLD_extcairo)
    set(extcairo_SRC ext-cairo-test.c)
  endif(PLD_extcairo)

  if(PLD_xcairo OR PLD_extcairo)
    set(cairo_DOCS README.cairo)
  endif(PLD_xcairo OR PLD_extcairo)

  if(ENABLE_gnome2)
    set(plplotcanvas_SRCS
      plplotcanvas_demo.c
      plplotcanvas_animation.c
      )

    set(plplotcanvas_DOCS
      README.plplotcanvas
      )
    set(gcw_true "")
    set(gcw_false "#")
  else(ENABLE_gnome2)
    set(gcw_true "#")
    set(gcw_false "")
  endif(ENABLE_gnome2)

  install(FILES ${c_SRCS} ${plplotcanvas_SRCS} ${plplotcanvas_DOCS} ${extXdrawable_SRC} ${extcairo_SRC} ${cairo_DOCS}
    DESTINATION ${DATA_DIR}/examples/c
    )

  install(FILES CMakeLists.txt 
    DESTINATION ${DATA_DIR}/examples/c
    )

  if(ENABLE_cxx AND NOT ENABLE_DYNDRIVERS)
    # For this case must link with C++ compiler for
    # the traditional build of the examples since
    # core PLplot library contains C++ code.
    set(CC ${CMAKE_CXX_COMPILER})
  else(ENABLE_cxx AND NOT ENABLE_DYNDRIVERS)
    set(CC ${CMAKE_C_COMPILER})
  endif(ENABLE_cxx AND NOT ENABLE_DYNDRIVERS)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    DESTINATION ${DATA_DIR}/examples/c
    RENAME Makefile
    )
endif(CORE_BUILD)

if(BUILD_TEST)
  if(CORE_BUILD)
    include_directories(
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_BINARY_DIR}/include
      ${CMAKE_SOURCE_DIR}/lib/qsastime
      )
  else(CORE_BUILD)
    include_directories(${INCLUDE_DIR})
  endif(CORE_BUILD)

  set_property(GLOBAL PROPERTY TARGETS_examples_c)
  foreach(STRING_INDEX ${c_STRING_INDICES})
    if(BUILD_SHARED_LIBS)
      set_source_files_properties(x${STRING_INDEX}c.c
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
    endif(BUILD_SHARED_LIBS)
    add_executable(x${STRING_INDEX}c x${STRING_INDEX}c.c)
    target_link_libraries(x${STRING_INDEX}c plplot ${MATH_LIB})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_c x${STRING_INDEX}c)
  endforeach(STRING_INDEX ${c_STRING_INDICES})
  
  # Build simple test routines which nothing else depends on.
  if(BUILD_SHARED_LIBS)
    set_source_files_properties(test_plend.c
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
  endif(BUILD_SHARED_LIBS)
  add_executable(test_plend test_plend.c)
  target_link_libraries(test_plend plplot ${MATH_LIB})
  
  # Build test routines that test the plot buffer functionality
  if(BUILD_SHARED_LIBS)
    set_source_files_properties(test_plbuf.c
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
  endif(BUILD_SHARED_LIBS)
  add_executable(test_plbuf test_plbuf.c)
  target_link_libraries(test_plbuf plplot ${MATH_LIB})

  if(PKG_CONFIG_EXECUTABLE)
    if(ENABLE_gnome2)
      # plplotcanvas_demo.c and plplotcanvas_animation.c both
      # contain "#include <gtk/gtk.h>" which occurs in the
      # /usr/include/gtk-2.0 directory.  They also contain 
      # "#include <plplotcanvas.h>".  These issues are both taken care of
      # by the libgnomeprintui-2.2 pkg-config module.
      message(STATUS "Determine compile and link flags for plotcanvas_demo")
      pkg_check_pkgconfig(libgnomeprintui-2.2 includedir libdir linkflags1 cflags1 version1 _GNOMEPRINTUI)
      if(linkflags1)
	# Requires blank-delimited list.
	string(REGEX REPLACE ";" " " GNOMEPRINTUI_COMPILE_FLAGS "${cflags1}")
	if(BUILD_SHARED_LIBS)
	  set(GNOMEPRINTUI_COMPILE_FLAGS "-DUSINGDLL ${GNOMEPRINTUI_COMPILE_FLAGS}")
	endif(BUILD_SHARED_LIBS)
	set(GNOMEPRINTUI_LINK_FLAGS "${linkflags1}")
	set_source_files_properties(
	  plplotcanvas_demo.c
	  PROPERTIES COMPILE_FLAGS "${GNOMEPRINTUI_COMPILE_FLAGS}"
	  )
	add_executable(plplotcanvas_demo plplotcanvas_demo.c)
	target_link_libraries(plplotcanvas_demo ${GNOMEPRINTUI_LINK_FLAGS} plplotgnome2)

	# plplotcanvas_animation.c contains a reference to gthread_init and also
	# contains '#include <glib.h>'.  Analysis shows both these issues are
	# taken care of by the gthread-2.0 pkg-config module.
        message(STATUS "Determine compile and link flags for plotcanvas_animation")
	pkg_check_pkgconfig(gthread-2.0 includedir libdir linkflags2 cflags2 version2 _GTHREAD_2.0)
	if(linkflags2)
	  # Requires blank-delimited list.
	  string(REGEX REPLACE ";" " " GTHREAD_2.0_COMPILE_FLAGS "${cflags2}")
	  set(GTHREAD_2.0_LINK_FLAGS "${linkflags2}")
	  set_source_files_properties(
	    plplotcanvas_animation.c
	    PROPERTIES COMPILE_FLAGS "${GNOMEPRINTUI_COMPILE_FLAGS} ${GTHREAD_2.0_COMPILE_FLAGS}"
	    )
	  add_executable(plplotcanvas_animation plplotcanvas_animation.c)
	  target_link_libraries(plplotcanvas_animation ${GNOMEPRINTUI_LINK_FLAGS} ${GTHREAD_2.0_LINK_FLAGS} plplotgnome2 ${MATH_LIB})
	else(linkflags2)
	  message(STATUS "WARNING: gthread-2.0 not found.  plplotcanvas_animation not built")
	endif(linkflags2)
      else(linkflags1)
	message(STATUS "WARNING: libgnomeprintui-2.2 not found.  plplotcanvas_demo and plplotcanvas_animation not built.")
      endif(linkflags1)
    endif(ENABLE_gnome2)
    
    if(PLD_xcairo)
      message(STATUS "Determine compile and link flags for extXdrawable_demo")
      pkg_check_pkgconfig(gtk+-x11-2.0 includedir libdir linkflags3 cflags3 version3 _GTK_X11)
      if(linkflags3)
	# Requires blank-delimited list.
	string(REGEX REPLACE ";" " " GTK_X11_COMPILE_FLAGS "${cflags3}")
	if(BUILD_SHARED_LIBS)
	  set(GTK_X11_COMPILE_FLAGS "-DUSINGDLL ${GTK_X11_COMPILE_FLAGS}")
	endif(BUILD_SHARED_LIBS)
	set(GTK_X11_LINK_FLAGS "${linkflags3}")
	set_source_files_properties(
	  extXdrawable_demo.c
	  PROPERTIES COMPILE_FLAGS "${GTK_X11_COMPILE_FLAGS}"
	  )
	add_executable(extXdrawable_demo extXdrawable_demo.c)
	target_link_libraries(extXdrawable_demo ${GTK_X11_LINK_FLAGS} plplot)
      else(linkflags3)
	message(STATUS "WARNING: gtk+-x11-2.0 not found.  extXdrawable_demo not built.")
      endif(linkflags3)
    endif(PLD_xcairo)

    if(PLD_extcairo)
      message(STATUS "Determine compile and link flags for ext-cairo-test")
      pkg_check_pkgconfig(cairo includedir libdir linkflags4 cflags4 version4 _CAIRO)
      if(linkflags4)
	# Requires blank-delimited list.
	string(REGEX REPLACE ";" " " CAIRO_COMPILE_FLAGS "${cflags4}")
	if(BUILD_SHARED_LIBS)
	  set(CAIRO_COMPILE_FLAGS "-DUSINGDLL ${CAIRO_COMPILE_FLAGS}")
	endif(BUILD_SHARED_LIBS)
	set(CAIRO_LINK_FLAGS "${linkflags4}")
	set_source_files_properties(
	  ext-cairo-test.c
	  PROPERTIES COMPILE_FLAGS "${CAIRO_COMPILE_FLAGS}"
	  )
	add_executable(ext-cairo-test ext-cairo-test.c)
	target_link_libraries(ext-cairo-test ${CAIRO_LINK_FLAGS} plplot)
      else(linkflags4)
	message(STATUS "WARNING: cairo not found.  ext-cairo-test not built.")
      endif(linkflags4)
    endif(PLD_extcairo)
  else(PKG_CONFIG_EXECUTABLE)
    message(STATUS "WARNING: pkg-config not found so plplotcanvas_demo, plplotcanvas_animation,  extXdrawable_demo, and ext-cairo-test not built.")
  endif(PKG_CONFIG_EXECUTABLE)
endif(BUILD_TEST)
