# - Use Module for QT4
# Sets up C and C++ to use Qt 4.  It is assumed that FindQt.cmake
# has already been loaded.  See FindQt.cmake for information on
# how to load Qt 4 into your CMake project.

# The "ndp_" prefix of this filename stands for "no directory
# property".  That is, this version of the current (CMake-2.8.12.1)
# version of UseQt4.cmake uses variables to transmit results rather
# than using directory properties (which are much too blunt an
# instrument for transmitting results since they affect all
# compilations, not just those which are Qt-specific).

# Copyright 2013 Alan W. Irwin under the license below

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:

# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.

# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.

# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# When the original_logic variable is false, add_definitions and
# include_directories calls are all replaced by collecting all -D
# options for Qt in the QT_COMPILE_DEFINITIONS list, and collecting
# all arguments to include_directories for Qt in the
# NP_INCLUDE_DIRECTORIES list.  Afterwards, NP_QT_COMPILE_DEFINITIONS
# is constructed from QT_COMPILE_DEFINITIONS by removing the -D
# prefixes for each element of that list, and QT_INCLUDE_DIRECTORIES
# is constructed by prepending either "-I" or "-isystem " to each
# element of NP_QT_INCLUDE_DIRECTORIES.  (The "NP_" prefix for variable
# names stands for "no prefix", and is the one used in general for
# CMake logic except for the pkg-config case where the
# variant of the variables without the "NP_" prefix is used instead.

set(original_logic OFF)

if(original_logic)
  add_definitions(${QT_DEFINITIONS})
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG QT_DEBUG)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO QT_NO_DEBUG)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG)
  if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
  endif()

  if(QT_INCLUDE_DIRS_NO_SYSTEM)
    include_directories(${QT_INCLUDE_DIR})
  else(QT_INCLUDE_DIRS_NO_SYSTEM)
    include_directories(SYSTEM ${QT_INCLUDE_DIR})
  endif(QT_INCLUDE_DIRS_NO_SYSTEM)
else(original_logic)
  set(QT_COMPILE_DEFINITIONS ${QT_DEFINITIONS})
  set(NP_QT_INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR})
endif(original_logic)

set(QT_LIBRARIES "")
set(QT_LIBRARIES_PLUGINS "")

if (QT_USE_QTMAIN)
  if (Q_WS_WIN)
    set(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
  endif ()
endif ()

if(QT_DONT_USE_QTGUI)
  set(QT_USE_QTGUI 0)
else()
  set(QT_USE_QTGUI 1)
endif()

if(QT_DONT_USE_QTCORE)
  set(QT_USE_QTCORE 0)
else()
  set(QT_USE_QTCORE 1)
endif()

if (QT_USE_QT3SUPPORT)
  if(original_logic)
    add_definitions(-DQT3_SUPPORT)
  else(original_logic)
    list(APPEND QT_COMPILE_DEFINITIONS -DQT3_SUPPORT)
  endif(original_logic)
endif ()

# list dependent modules, so dependent libraries are added
set(QT_QT3SUPPORT_MODULE_DEPENDS QTGUI QTSQL QTXML QTNETWORK QTCORE)
# This original_logic code contains an incorrect dependency.
#set(QT_QTSVG_MODULE_DEPENDS QTGUI QTXML QTCORE)
set(QT_QTSVG_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QTUITOOLS_MODULE_DEPENDS QTGUI QTXML QTCORE)
set(QT_QTHELP_MODULE_DEPENDS QTGUI QTSQL QTXML QTNETWORK QTCORE)
if(QT_QTDBUS_FOUND)
  set(QT_PHONON_MODULE_DEPENDS QTGUI QTDBUS QTCORE)
else()
  set(QT_PHONON_MODULE_DEPENDS QTGUI QTCORE)
endif()
set(QT_QTDBUS_MODULE_DEPENDS QTXML QTCORE)
set(QT_QTXMLPATTERNS_MODULE_DEPENDS QTNETWORK QTCORE)
set(QT_QAXCONTAINER_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QAXSERVER_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QTSCRIPTTOOLS_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QTWEBKIT_MODULE_DEPENDS QTXMLPATTERNS QTGUI QTCORE)
set(QT_QTDECLARATIVE_MODULE_DEPENDS QTSCRIPT QTSVG QTSQL QTXMLPATTERNS QTGUI QTCORE)
set(QT_QTMULTIMEDIA_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QTOPENGL_MODULE_DEPENDS QTGUI QTCORE)
set(QT_QTSCRIPT_MODULE_DEPENDS QTCORE)
set(QT_QTGUI_MODULE_DEPENDS QTCORE)
set(QT_QTTEST_MODULE_DEPENDS QTCORE)
set(QT_QTXML_MODULE_DEPENDS QTCORE)
set(QT_QTSQL_MODULE_DEPENDS QTCORE)
set(QT_QTNETWORK_MODULE_DEPENDS QTCORE)

# Qt modules  (in order of dependence)
foreach(module QT3SUPPORT QTOPENGL QTASSISTANT QTDESIGNER QTMOTIF QTNSPLUGIN
               QAXSERVER QAXCONTAINER QTDECLARATIVE QTSCRIPT QTSVG QTUITOOLS QTHELP
               QTWEBKIT PHONON QTSCRIPTTOOLS QTMULTIMEDIA QTXMLPATTERNS QTGUI QTTEST
               QTDBUS QTXML QTSQL QTNETWORK QTCORE)

  if (QT_USE_${module} OR QT_USE_${module}_DEPENDS)
    if (QT_${module}_FOUND)
      if(QT_USE_${module})
        string(REPLACE "QT" "" qt_module_def "${module}")
	if(original_logic)
          add_definitions(-DQT_${qt_module_def}_LIB)
	else(original_logic)
	  list(APPEND QT_COMPILE_DEFINITIONS -DQT_${qt_module_def}_LIB)
	endif(original_logic)
	if(original_logic)
          if(QT_INCLUDE_DIRS_NO_SYSTEM)
            include_directories(${QT_${module}_INCLUDE_DIR})
          else(QT_INCLUDE_DIRS_NO_SYSTEM)
            include_directories(SYSTEM ${QT_${module}_INCLUDE_DIR})
          endif(QT_INCLUDE_DIRS_NO_SYSTEM)
	else(original_logic)
	  list(APPEND NP_QT_INCLUDE_DIRECTORIES ${QT_${module}_INCLUDE_DIR})
	endif(original_logic)
      endif()

      # For the case when QT_USE_${module} is false but
      # QT_USE_${module}_DEPENDS is true, the following logic
      # implies transitive linking.  But we let that go....
      set(QT_LIBRARIES ${QT_LIBRARIES} ${QT_${module}_LIBRARY})
      set(QT_LIBRARIES_PLUGINS ${QT_LIBRARIES_PLUGINS} ${QT_${module}_PLUGINS})

      if(QT_IS_STATIC)
        set(QT_LIBRARIES ${QT_LIBRARIES} ${QT_${module}_LIB_DEPENDENCIES})
      endif()

      foreach(depend_module ${QT_${module}_MODULE_DEPENDS})
        set(QT_USE_${depend_module}_DEPENDS 1)
      endforeach()
    else ()
      message("Qt ${module} library not found.")
    endif ()
  endif ()

endforeach()

if(NOT original_logic)
  # QT_COMPILE_DEFINITIONS is only used for the pkg-config case.
  # NP_QT_COMPILE_DEFINITIONS is equivalent to QT_COMPILE_DEFINITIONS with
  # the -D prefixes removed.  Also, check the assumption that
  # QT_COMPILE_DEFINITIONS contains nothing but -D... options.
  set(NP_QT_COMPILE_DEFINITIONS)
  foreach(definition ${QT_COMPILE_DEFINITIONS})
    if(definition MATCHES "^-D")
      string(REGEX REPLACE "^-D" "" new_definition ${definition})
      list(APPEND NP_QT_COMPILE_DEFINITIONS ${new_definition})
    else(definition MATCHES "^-D")
      message(STATUS "QT_DEFINITIONS = ${QT_DEFINITIONS}")
      message(FATAL_ERROR "An element of QT_DEFINITIONS does not start with \"-D\"")
    endif(definition MATCHES "^-D")
  endforeach(definition ${QT_COMPILE_DEFINITIONS})
  message(STATUS "NP_QT_COMPILE_DEFINITIONS = ${NP_QT_COMPILE_DEFINITIONS}")
  message(STATUS "QT_COMPILE_DEFINITIONS = ${QT_COMPILE_DEFINITIONS}")

  # QT_INCLUDE_DIRECTORIES is only used for the pkg-config case.
  # It is equivalent to NP_QT_INCLUDE_DIRECTORIES with "-I"
  # or "-isystem " suffixes prepended.

  set(QT_INCLUDE_DIRECTORIES)
  foreach(directory ${NP_QT_INCLUDE_DIRECTORIES})
    if(QT_INCLUDE_DIRS_NO_SYSTEM)
      list(APPEND QT_INCLUDE_DIRECTORIES "-I${directory}")
    else(QT_INCLUDE_DIRS_NO_SYSTEM)
      list(APPEND QT_INCLUDE_DIRECTORIES "-isystem ${directory}")
    endif(QT_INCLUDE_DIRS_NO_SYSTEM)
  endforeach(directory ${QT_INCLUDE_DIR})

  message(STATUS "NP_QT_INCLUDE_DIRECTORIES = ${NP_QT_INCLUDE_DIRECTORIES}")
  message(STATUS "QT_INCLUDE_DIRECTORIES = ${QT_INCLUDE_DIRECTORIES}")

  # Function to set appropriate target properties for Qt-related build targets.
  function(set_qt_target_properties target)
    # Actual target argument is just a string so only one level of
    # indirection required to determine that string.
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS ${NP_QT_COMPILE_DEFINITIONS})
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG ${NP_QT_COMPILE_DEFINITIONS} QT_DEBUG)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE ${NP_QT_COMPILE_DEFINITIONS} QT_NO_DEBUG)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO ${NP_QT_COMPILE_DEFINITIONS} QT_NO_DEBUG)
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL ${NP_QT_COMPILE_DEFINITIONS} QT_NO_DEBUG)
    if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
      set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS QT_NO_DEBUG)
    endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    # The only method of setting system versions of the include
    # directories for the compiler to use to build a target is the
    # target_include_directories function (outside of directory
    # properties which which is a method we want to avoid since it
    # affects every compilation in the directory and all
    # subdirectories).  However, target_include_directories was first
    # available only for 2.8.12.1.  So for previous versions (or if
    # QT_INCLUDE_DIRS_NO_SYSTEM is true) drop the system qualifier for
    # the include directories the compiler is to use, and specify
    # those directories with set_property which is available for
    # earlier versions of cmake.
    if(QT_INCLUDE_DIRS_NO_SYSTEM OR ${CMAKE_VERSION} VERSION_LESS 2.8.12.1)
      set_property(TARGET ${target} APPEND PROPERTY INCLUDE_DIRECTORIES ${NP_QT_INCLUDE_DIRECTORIES})
    else(QT_INCLUDE_DIRS_NO_SYSTEM OR ${CMAKE_VERSION} VERSION_LESS 2.8.12.1)
      target_include_directories(${target} SYSTEM PRIVATE ${NP_QT_INCLUDE_DIRECTORIES})
    endif(QT_INCLUDE_DIRS_NO_SYSTEM OR ${CMAKE_VERSION} VERSION_LESS 2.8.12.1)
  endfunction(set_qt_target_properties target)
endif(NOT original_logic)
