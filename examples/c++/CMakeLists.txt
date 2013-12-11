# examples/c++/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
# Copyright (C) 2006, 2007, 2008, 2009 Alan W. Irwin
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

set(cxx_STRING_INDICES
  "01cc"
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
  "33"
  )

if(CORE_BUILD)
  set(cxx_SRCS plc++demos.h)
  foreach(STRING_INDEX ${cxx_STRING_INDICES})
    set(cxx_SRCS ${cxx_SRCS} x${STRING_INDEX}.cc)
  endforeach(STRING_INDEX ${cxx_STRING_INDICES})

  if(ENABLE_wxwidgets)
    set(wxPLplotDemo_SRCS wxPLplotDemo.cpp)
  endif(ENABLE_wxwidgets)

  if(PLD_extqt)
    set(qt_INSTALLED_FILES qt_PlotWindow.cpp qt_PlotWindow.h qt_example.cpp README.qt_example)
  endif(PLD_extqt)

  install(FILES ${cxx_SRCS} ${wxPLplotDemo_SRCS} ${qt_INSTALLED_FILES} DESTINATION ${DATA_DIR}/examples/c++)

  set(CXX ${CMAKE_CXX_COMPILER})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
    DESTINATION ${DATA_DIR}/examples/c++
    RENAME Makefile
    )

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/c++
    )
endif(CORE_BUILD)

if(BUILD_TEST)
  if(CORE_BUILD)
    include_directories(
      ${CMAKE_SOURCE_DIR}/bindings/c++
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_BINARY_DIR}/include
      ${CMAKE_SOURCE_DIR}/lib/qsastime
      )
  else(CORE_BUILD)
    include_directories(${INCLUDE_DIR})
  endif(CORE_BUILD)
endif(BUILD_TEST)

if(BUILD_TEST)
  set_property(GLOBAL PROPERTY TARGETS_examples_cxx)
  foreach(STRING_INDEX ${cxx_STRING_INDICES})
    if(CORE_BUILD AND BUILD_SHARED_LIBS)
      SET_SOURCE_FILES_PROPERTIES(x${STRING_INDEX}.cc
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL")
    endif(CORE_BUILD AND BUILD_SHARED_LIBS)
    add_executable(x${STRING_INDEX} x${STRING_INDEX}.cc)
    target_link_libraries(x${STRING_INDEX} plplotcxx${LIB_TAG} ${MATH_LIB})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_cxx x${STRING_INDEX})
  endforeach(STRING_INDEX ${cxx_STRING_INDICES})

  if(ENABLE_wxwidgets)
    # Build wxwidgets applications with same wxwidgets compile and link flags
    # as used with the PLplot wxwidgets device driver.
    if(CORE_BUILD)
      if(BUILD_SHARED_LIBS)
	set_source_files_properties(wxPLplotDemo.cpp
          PROPERTIES COMPILE_FLAGS "-DUSINGDLL ${wxwidgets_COMPILE_FLAGS}")
      else(BUILD_SHARED_LIBS)
	set_source_files_properties(wxPLplotDemo.cpp
          PROPERTIES COMPILE_FLAGS "${wxwidgets_COMPILE_FLAGS}")
      endif(BUILD_SHARED_LIBS)
      include_directories(
	${CMAKE_SOURCE_DIR}/bindings/wxwidgets
	${CMAKE_BINARY_DIR}/bindings/wxwidgets
	)
    else(CORE_BUILD)
      set_source_files_properties(wxPLplotDemo.cpp
        PROPERTIES COMPILE_FLAGS "${wxwidgets_COMPILE_FLAGS}")
    endif(CORE_BUILD)
    add_executable(wxPLplotDemo WIN32 wxPLplotDemo.cpp)
    target_link_libraries(wxPLplotDemo plplotwxwidgets${LIB_TAG} plplotcxx${LIB_TAG} ${wxwidgets_LINK_FLAGS} ${MATH_LIB})
  endif(ENABLE_wxwidgets)

  if(ENABLE_qt)
    if(CORE_BUILD)
      set(BUILD_qt_example ON)
      if(NOT QT_LIBRARIES)
        message(FATAL_ERROR "include(${QT_USE_FILE}) failed in core build examples/c++ subdirectory.")
      endif(NOT QT_LIBRARIES)
    else(CORE_BUILD)
      # Must find Qt4 all over again here rather than simply copying variables
      # from the core build to this installed examples build so that we can
      # access the qt4_wrap_cpp macro here.
      find_package(Qt4 4.8.2 REQUIRED QtCore QtGui QtSvg)
      # To avoid Qt4 version mismatch make sure the Qt version matches that
      # used by the core build.
      if(QT4_FOUND AND QT_VERSION_MAJOR STREQUAL "${CORE_QT_VERSION_MAJOR}" AND QT_VERSION_MINOR STREQUAL "${CORE_QT_VERSION_MINOR}" AND QT_VERSION_PATCH STREQUAL "${CORE_QT_VERSION_PATCH}")
	include(ndp_UseQt4)
	if(NOT QT_LIBRARIES)
          message(FATAL_ERROR "include(${QT_USE_FILE}) failed in installed examples build examples/c++ subdirectory.")
	endif(NOT QT_LIBRARIES)
	set(BUILD_qt_example ON)
      else(QT4_FOUND AND QT_VERSION_MAJOR STREQUAL "${CORE_QT_VERSION_MAJOR}" AND QT_VERSION_MINOR STREQUAL "${CORE_QT_VERSION_MINOR}" AND QT_VERSION_PATCH STREQUAL "${CORE_QT_VERSION_PATCH}")
	message(STATUS "WARNING: Qt4 not found or wrong version.  qt_example not built")
	set(BUILD_qt_example OFF)
      endif(QT4_FOUND AND QT_VERSION_MAJOR STREQUAL "${CORE_QT_VERSION_MAJOR}" AND QT_VERSION_MINOR STREQUAL "${CORE_QT_VERSION_MINOR}" AND QT_VERSION_PATCH STREQUAL "${CORE_QT_VERSION_PATCH}")
    endif(CORE_BUILD)
    if(BUILD_qt_example)
      qt4_wrap_cpp(
	QT_MOC_OUTFILES
	${CMAKE_CURRENT_SOURCE_DIR}/qt_PlotWindow.h
	)
      # message("QT_MOC_OUTFILES = ${QT_MOC_OUTFILES}")
      set_source_files_properties(
	${QT_MOC_OUTFILES}
	PROPERTIES GENERATED "ON"
	)
      add_executable(qt_example qt_example.cpp qt_PlotWindow.cpp ${QT_MOC_OUTFILES})
      target_link_libraries(qt_example ${QT_LIBRARIES} plplotqt${LIB_TAG} plplot${LIB_TAG})
      # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
      set_qt_target_properties(qt_example)
    endif(BUILD_qt_example)
  endif(ENABLE_qt)
endif(BUILD_TEST)
