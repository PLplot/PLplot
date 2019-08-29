#
# CMakeD - CMake module for D Language
#
# Copyright (c) 2007, Selman Ulug <selman.ulug@gmail.com>
#                     Tim Burrell <tim.burrell@gmail.com>
#
# All rights reserved.
#
# See LICENSE for details.
#
# Modified from CMake 2.6.5 gcc.cmake
# See http://www.cmake.org/HTML/Copyright.html for details
#

set(CMAKE_D_DASH_O "-o")
set(CMAKE_BASE_NAME gdc)

if(CMAKE_D_USE_TANGO)
  set(DSTDLIB_FLAGS "-fversion=Tango")
endif()
if(CMAKE_D_USE_PHOBOS)
  set(DSTDLIB_FLAGS "-fversion=Phobos")
endif()
if(CMAKE_D_BUILD_DOCS)
  set(DDOC_FLAGS "-fdoc -fdoc-dir=documentation")
  foreach(item ${CMAKE_D_DDOC_FILES})
    set(DDOC_FLAGS "${DDOC_FLAGS} -fdoc-inc=${item}")
  endforeach()
endif()

# SET(CMAKE_D_FLAGS_INIT "-fversion=Posix -fversion=${CMAKE_BUILD_TYPE}Build ${DSTDLIB_FLAGS}")
set(CMAKE_D_FLAGS_INIT "")
set(CMAKE_D_FLAGS_DEBUG_INIT "-g ${DDOC_FLAGS}")
set(CMAKE_D_FLAGS_MINSIZEREL_INIT "-Os ${DDOC_FLAGS}")
set(CMAKE_D_FLAGS_RELEASE_INIT "-O3 -fomit-frame-pointer -fweb -frelease -finline-functions ${DDOC_FLAGS}")
set(CMAKE_D_FLAGS_RELWITHDEBINFO_INIT "-O2 -g ${DDOC_FLAGS}")
# set(CMAKE_D_CREATE_PREPROCESSED_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -E <SOURCE> > <PREPROCESSED_SOURCE>")
set(CMAKE_D_CREATE_ASSEMBLY_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE>")
# set(CMAKE_INCLUDE_SYSTEM_FLAG_D "-isystem ")

# Options normally supplied by ../Compiler/GNU.cmake for officially supported languages.
set(CMAKE_D_COMPILE_OPTIONS_PIC "-fPIC")
set(CMAKE_SHARED_LIBRARY_D_FLAGS "-fPIC")
set(CMAKE_SHARED_LIBRARY_CREATE_D_FLAGS "-shared")
set(CMAKE_SHARED_LIBRARY_SONAME_D_FLAG "-Wl,-soname,")

# C version of these flags normally set by Platform/GNU.cmake
set(CMAKE_SHARED_LIBRARY_RUNTIME_D_FLAG "-Wl,-rpath,")
set(CMAKE_SHARED_LIBRARY_RPATH_LINK_D_FLAG "-Wl,-rpath-link,")



set(CMAKE_INCLUDE_FLAG_D "-I")       # -I
