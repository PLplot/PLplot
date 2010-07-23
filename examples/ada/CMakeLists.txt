# examples/ada/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007, 2008, 2009 Alan W. Irwin
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

set(ada_STRING_INDICES
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
  "thick01"
  "thick02"
  "thick03"
  "thick04"
  "thick05"
  "thick06"
  "thick07"
  "thick08"
  "thick09"
  "thick10"
  "thick11"
  "thick12"
  "thick13"
  "thick14"
  "thick15"
  "thick16"
  "thick17"
  "thick18"
  "thick19"
  "thick20"
  "thick21"
  "thick22"
  "thick23"
  "thick24"
  "thick25"
  "thick26"
  "thick27"
  "thick28"
  "thick29"
  "thick30"
  "thick31"
  )

if(CORE_BUILD)
  set(ada_SRCS)
  foreach(STRING_INDEX ${ada_STRING_INDICES})
    set(SOURCE_FILE x${STRING_INDEX}a.adb)
    list(APPEND ada_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE})
  endforeach(STRING_INDEX ${ada_STRING_INDICES})

  install(FILES ${ada_SRCS} DESTINATION ${DATA_DIR}/examples/ada)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
    DESTINATION ${DATA_DIR}/examples/ada
    RENAME Makefile
    )

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/ada
    )
endif(CORE_BUILD)

if(BUILD_TEST)
  if(CORE_BUILD)
    include_directories(
      ${CMAKE_BINARY_DIR}/examples/ada
      ${CMAKE_SOURCE_DIR}/bindings/ada
      )
  else(CORE_BUILD)
    include_directories(${INCLUDE_DIR} ${ADA_INCLUDE_DIR})
  endif(CORE_BUILD)

  # The first -aI option in adalinkflags is required to find <TARGET>.adb
  # (see comment below). The second -aI option gives access to the
  # libplplotada spec files. The -aL option gives access to the
  # libplplotada *.ali files.  (That location currently works but is
  # probably not guaranteed indefinitely for future versions of
  # CMake.)
  
  if(CORE_BUILD)
    set(adalinkflags "-aI${CMAKE_SOURCE_DIR}/examples/ada -aI${CMAKE_SOURCE_DIR}/bindings/ada -aL${CMAKE_BINARY_DIR}/bindings/ada/CMakeFiles/plplotada${LIB_TAG}.dir")
  else(CORE_BUILD)
    set(adalinkflags "-aI${CMAKE_SOURCE_DIR}/ada -aI${ADA_INCLUDE_DIR} -aL${ADA_LIB_DIR}")
  endif(CORE_BUILD)

  foreach(STRING_INDEX ${ada_STRING_INDICES})
    set(TARGET_NAME x${STRING_INDEX}a)
    set(SOURCE_FILE x${STRING_INDEX}a.adb)
    set(adasrcfile ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE})

    # Note due to limitations of the Ada language support for CMake,
    # the nominal source file for add_executable is compiled (which is
    # why a real file has to be specified), but otherwise it is
    # ignored except for its .adb suffix which identifies the Ada
    # language.  The actual source file name for the internal gnatmake
    # command that creates the executable is constructed from
    # <TARGET>.adb.  Since no directory can be specified from this
    # construction (don't ask), you must specify the directory of
    # <TARGET>.adb with a -aI option (see above comment of adalinkflags).
    add_executable(${TARGET_NAME} ${adasrcfile})
    
    set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS ${adalinkflags})
    target_link_libraries(${TARGET_NAME} plplotada${LIB_TAG})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_ada ${TARGET_NAME})
    list(APPEND ada_extra_clean_files "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.o;${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.ali")
  endforeach(STRING_INDEX ${ada_STRING_INDICES})

  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ada_extra_clean_files}")
endif(BUILD_TEST)
