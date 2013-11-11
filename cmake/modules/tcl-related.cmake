# cmake/modules/tcl-related.cmake
#
# Copyright (C) 2006-2013  Alan W. Irwin
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining all tcl-related configuration options not directly
# related to the tk device driver.

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_tcl "Enable Tcl bindings" OFF)
  option(ENABLE_itcl "Enable Itcl interface code" OFF)
  option(ENABLE_tk "Enable Tk interface code" OFF)
  option(ENABLE_itk "Enable Itk interface code" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_tcl "Enable Tcl bindings" ON)
  option(ENABLE_itcl "Enable Itcl interface code" ON)
  option(ENABLE_tk "Enable Tk interface code" ON)
  option(ENABLE_itk "Enable Itk interface code" ON)
endif(DEFAULT_NO_BINDINGS)

option(USE_INCRTCL_VERSION_4 "Use version 4 of Itcl and Itcl, version 4.1 of Iwidgets" OFF)

# Depending on these above options and system resources may also determine
# the following variables which are largely self-explanatory unless documented
# further.

#TCL_INCLUDE_PATH
#TCL_LIBRARY
#ITCL_INCLUDE_PATH
#ITCL_LIBRARY
#HAVE_ITCL   		(On when itcl header and library have been found.
#			 Otherwise, undefined.
#			 Used for source file configuration.)
#HAVE_ITCLDECLS_H 	(defined [actually as a path, but we don't use that]
#			 when itclDecls.h.  Otherwise, undefined.
#			 Used for source file configuration.)
#TK_INCLUDE_PATH
#TK_LIBRARY
#ITK_INCLUDE_PATH
#ITK_LIBRARY
#HAVE_ITK   		(On when itcl header and library have been found.
#			 Otherwise, undefined.
#			 Used for source file configuration.)
#PLPLOT_ITCL_VERSION     (Consistent Itcl version number found by PLplot).
#PLPLOT_ITK_VERSION      (Consistent Itk version number found by PLplot).
#PLPLOT_IWIDGETS_VERSION (Consistent Iwidgets version number found by PLplot).

if(ENABLE_tcl)
  message(STATUS "Start determining consistent system data for Tcl and friends")
  find_package(TCL QUIET)
  if(TCL_FOUND)
    message(STATUS "Looking for include paths and libraries for Tcl - found")
    message(STATUS "TCL_INCLUDE_PATH = ${TCL_INCLUDE_PATH}")
    message(STATUS "TCL_LIBRARY = ${TCL_LIBRARY}")
    get_filename_component(TCL_LIBRARY_PATH ${TCL_LIBRARY} PATH)
    message(STATUS "TCL_LIBRARY_PATH = ${TCL_LIBRARY_PATH}")

    if(TCL_TCLSH)
      message(STATUS "Looking for tclsh - found")
      message(STATUS "TCL_TCLSH = ${TCL_TCLSH}")
    else(TCL_TCLSH)
      message(STATUS "Looking for tclsh - not found")
    endif(TCL_TCLSH)
  else(TCL_FOUND)
    message(STATUS "Looking for include paths and libraries for Tcl - not found")
  endif(TCL_FOUND)

  # Initially mark various quantities as as unfound in case that turns out to be the case.
  set(HAVE_ITCL OFF)
  set(HAVE_ITK OFF)

  if(TCL_FOUND AND TCL_TCLSH)
    if(ENABLE_itcl)
      if(USE_INCRTCL_VERSION_4)
	set(SUGGESTED_ITCL_VERSION 4)
      else(USE_INCRTCL_VERSION_4)
	set(SUGGESTED_ITCL_VERSION 3)
      endif(USE_INCRTCL_VERSION_4)
      file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CheckITCL_Available.tcl "puts -nonewline [package require Itcl ${SUGGESTED_ITCL_VERSION}]; exit")
      # Refine SUGGESTED_ITCL_VERSION to exact value or fail.
      execute_process(
	COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_BINARY_DIR}/CheckITCL_Available.tcl
	RESULT_VARIABLE ITCL_RC
	OUTPUT_VARIABLE PLPLOT_ITCL_VERSION
	ERROR_VARIABLE ITCL_STDERR
	)
      if(NOT ITCL_RC)
        message(STATUS "Looking for itcl.h")
	if(NOT USE_INCRTCL_VERSION_4)
	  # Search first for epa_build install location.
          find_path(ITCL_INCLUDE_PATH itcl.h HINTS ${TCL_INCLUDE_PATH}/itcl${PLPLOT_ITCL_VERSION})
	endif(NOT USE_INCRTCL_VERSION_4)
        find_path(ITCL_INCLUDE_PATH itcl.h HINTS ${TCL_INCLUDE_PATH})

        if(ITCL_INCLUDE_PATH)
          message(STATUS "Looking for itcl.h - found")
	  message(STATUS "ITCL_INCLUDE_PATH = ${ITCL_INCLUDE_PATH}")

	  message(STATUS "Checking for Itcl header version consistency")
	  string(REGEX REPLACE "^([0-9]*)\\.[0-9]*.*$" "\\1" ITCL_MAJOR_VERSION  "${PLPLOT_ITCL_VERSION}")
	  string(REGEX REPLACE "^[0-9]*\\.([0-9]*).*$" "\\1" ITCL_MINOR_VERSION  "${PLPLOT_ITCL_VERSION}")
	  message(STATUS "ITCL_MAJOR_VERSION = ${ITCL_MAJOR_VERSION}")
	  message(STATUS "ITCL_MINOR_VERSION = ${ITCL_MINOR_VERSION}")
	  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CheckITCL_VERSION.c
	    "// #define RC_INVOKED to avoid sucking in lots of additional includes for
// efficiency and simplicity.
#define RC_INVOKED 1
#include <itcl.h>
#if ITCL_MAJOR_VERSION != ${ITCL_MAJOR_VERSION} || ITCL_MINOR_VERSION != ${ITCL_MINOR_VERSION}
  #error ITCL_VERSION mismatch
#endif
void main(void){}
"
	    )
	  try_compile(CONSISTENT_HEADER_ITCL_VERSION ${CMAKE_BINARY_DIR}
	    ${CMAKE_CURRENT_BINARY_DIR}/CheckITCL_VERSION.c
	    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${ITCL_INCLUDE_PATH};${TCL_INCLUDE_PATH}"
	    )
	  if(CONSISTENT_HEADER_ITCL_VERSION)
	    message(STATUS "Checking for Itcl header version consistency - true")
	  else(CONSISTENT_HEADER_ITCL_VERSION)
	    message(STATUS "Checking for Itcl header version consistency - false")
            message(STATUS "WARNING: Disabling Itcl interface code")
	    set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
	  endif(CONSISTENT_HEADER_ITCL_VERSION)
        else(ITCL_INCLUDE_PATH)
          message(STATUS "Looking for itcl.h - not found")
          message(STATUS "WARNING: Disabling Itcl interface code")
          set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
        endif(ITCL_INCLUDE_PATH)

        if(ENABLE_itcl)
          message(STATUS "Looking for itcl library")
	  # Search first for epa_build install location.
          find_library(ITCL_LIBRARY itcl${PLPLOT_ITCL_VERSION} HINTS ${TCL_LIBRARY_PATH}/itcl${PLPLOT_ITCL_VERSION})
          find_library(ITCL_LIBRARY itcl${PLPLOT_ITCL_VERSION} HINTS ${TCL_LIBRARY_PATH})
          if(ITCL_LIBRARY)
            message(STATUS "Looking for itcl library - found")
	    message(STATUS "ITCL_LIBRARY = ${ITCL_LIBRARY}")
            set(HAVE_ITCL ON)
            find_path(HAVE_ITCLDECLS_H itclDecls.h HINTS ${ITCL_INCLUDE_PATH})
          else(ITCL_LIBRARY)
            message(STATUS "Looking for itcl library - not found")
            message(STATUS "WARNING: Disabling Itcl interface code")
            set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
          endif(ITCL_LIBRARY)
        endif(ENABLE_itcl)
      else(NOT ITCL_RC)
        message(STATUS "Itcl not available or not compatible with current Tcl shell")
        message(STATUS "WARNING: Disabling Itcl interface code")
        set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
      endif(NOT ITCL_RC)
    endif(ENABLE_itcl)

    if(ENABLE_tk AND NOT TK_FOUND)
      message(STATUS "WARNING: Tk not found so disabling Tk interface code")
      set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
    endif(ENABLE_tk AND NOT TK_FOUND)

    if(ENABLE_tk AND NOT X11_FOUND)
      message(STATUS "WARNING: X11 not found so disabling Tk interface code")
      set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
    endif(ENABLE_tk AND NOT X11_FOUND)

    if(ENABLE_tk)
      message(STATUS "Looking for include paths and libraries for Tk - found")
      set(TK_INCLUDE_PATH ${TK_INCLUDE_PATH} ${X11_INCLUDE_DIR})
      message(STATUS "TK_INCLUDE_PATH = ${TK_INCLUDE_PATH}")
      message(STATUS "TK_LIBRARY = ${TK_LIBRARY}")

      get_filename_component(TK_LIBRARY_PATH ${TK_LIBRARY} PATH)
      message(STATUS "TK_LIBRARY_PATH = ${TK_LIBRARY_PATH}")
      if(NOT ${TK_LIBRARY_PATH} STREQUAL ${TCL_LIBRARY_PATH})
	message(STATUS "WARNING: the Tcl and Tk library locations are inconsistent so those libraries are likely not compatible")
      endif(NOT ${TK_LIBRARY_PATH} STREQUAL ${TCL_LIBRARY_PATH})
    else(ENABLE_tk)
      message(STATUS "WARNING: Because Tk is disabled must disable Itk as well")
      set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
    endif(ENABLE_tk)

    if(ENABLE_itk)
      if(USE_INCRTCL_VERSION_4)
	set(SUGGESTED_ITK_VERSION 4)
      else(USE_INCRTCL_VERSION_4)
	set(SUGGESTED_ITK_VERSION 3)
      endif(USE_INCRTCL_VERSION_4)
      file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CheckITK_Available.tcl "puts -nonewline [package require Itk ${SUGGESTED_ITK_VERSION}]; exit")
      # Refine SUGGESTED_ITK_VERSION to exact value or fail.
      execute_process(
	COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_BINARY_DIR}/CheckITK_Available.tcl
	RESULT_VARIABLE ITK_RC
	OUTPUT_VARIABLE PLPLOT_ITK_VERSION
	ERROR_VARIABLE ITK_STDERR
	)
      if(NOT ITK_RC)
	message(STATUS "Looking for itk.h")
	if(NOT USE_INCRTCL_VERSION_4)
	  # Search first for epa_build install location.
          find_path(ITK_INCLUDE_PATH itk.h HINTS ${TCL_INCLUDE_PATH}/itcl${PLPLOT_ITCL_VERSION})
	endif(NOT USE_INCRTCL_VERSION_4)
	find_path(ITK_INCLUDE_PATH itk.h HINTS ${TCL_INCLUDE_PATH})

	if(ITK_INCLUDE_PATH)
          message(STATUS "Looking for itk.h - found")
	  message(STATUS "ITK_INCLUDE_PATH = ${ITK_INCLUDE_PATH}")

	  message(STATUS "Checking for Itk header version consistency")
	  string(REGEX REPLACE "^([0-9]*)\\.[0-9]*.*$" "\\1" ITK_MAJOR_VERSION  "${PLPLOT_ITK_VERSION}")
	  string(REGEX REPLACE "^[0-9]*\\.([0-9]*).*$" "\\1" ITK_MINOR_VERSION  "${PLPLOT_ITK_VERSION}")
	  message(STATUS "ITK_MAJOR_VERSION = ${ITK_MAJOR_VERSION}")
	  message(STATUS "ITK_MINOR_VERSION = ${ITK_MINOR_VERSION}")
	  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CheckITK_VERSION.c
	    "// #define RC_INVOKED to avoid sucking in lots of additional includes for
// efficiency and simplicity and also to avoid the header file disaster that has long
// been a problem for version 3.3 of itk.h
#define RC_INVOKED 1
#include <itk.h>
#if ITK_MAJOR_VERSION != ${ITK_MAJOR_VERSION} || ITK_MINOR_VERSION != ${ITK_MINOR_VERSION}
  #error ITK_VERSION mismatch
#endif
void main(void){}
"
	    )
	  try_compile(CONSISTENT_HEADER_ITK_VERSION ${CMAKE_BINARY_DIR}
	    ${CMAKE_CURRENT_BINARY_DIR}/CheckITK_VERSION.c
	    CMAKE_FLAGS -DINCLUDE_DIRECTORIES:PATH=${ITK_INCLUDE_PATH}
	    )
	  if(CONSISTENT_HEADER_ITK_VERSION)
	    message(STATUS "Checking for Itk header version consistency - true")
	  else(CONSISTENT_HEADER_ITK_VERSION)
	    message(STATUS "Checking for Itk header version consistency - false")
            message(STATUS "WARNING: Disabling Itk interface code")
	    set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
	  endif(CONSISTENT_HEADER_ITK_VERSION)
	else(ITK_INCLUDE_PATH)
          message(STATUS "Looking for itk.h - not found")
          message(STATUS "WARNING: Disabling Itk interface code")
          set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
	endif(ITK_INCLUDE_PATH)

	if(ENABLE_itk)
          message(STATUS "Looking for itk library")
	  # Search first for epa_build install location.
          find_library(ITK_LIBRARY itk${PLPLOT_ITK_VERSION} HINTS ${TCL_LIBRARY_PATH}/itk${PLPLOT_ITK_VERSION})
          find_library(ITK_LIBRARY itk${PLPLOT_ITK_VERSION} HINTS ${TCL_LIBRARY_PATH})
          if(ITK_LIBRARY)
            message(STATUS "Looking for itk library - found")
	    message(STATUS "ITK_LIBRARY = ${ITK_LIBRARY}")
	    set(HAVE_ITK ON)

	    # Test version consistency between iwidgets, itk, and itcl.
	    if(USE_INCRTCL_VERSION_4)
	      set(SUGGESTED_IWIDGETS_VERSION 4.1)
	      file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/determine_versions.tcl "puts -nonewline \"[package require Iwidgets ${SUGGESTED_IWIDGETS_VERSION}];[package require Itk];[package require Itcl]\";exit")
	      execute_process(
		COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_BINARY_DIR}/determine_versions.tcl
		RESULT_VARIABLE IWIDGETS_RC
		OUTPUT_VARIABLE IWIDGETS_VERSIONS_LIST
		ERROR_VARIABLE IWIDGETS_STDERR
		)
	    else(USE_INCRTCL_VERSION_4)
	      # Find maximum 4.0 version of iwidgets that is
	      # available.  4.0.1 (released in 2002) is the largest
	      # version of iwidgets4.0 (as opposed to the recently
	      # developed iwidgets4.1) I have found on the web in
	      # tarball form, but I have seen one reference to 4.0.2
	      # which may have been publicly released somewhere
	      # inaccessible to my google searches or privately made
	      # available before all development of iwidgets4.0
	      # stopped. So add 4.0.2 to the list just in case.
	      foreach(SUGGESTED_IWIDGETS_VERSION 4.0.2 4.0.1 4.0.0)
		file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/determine_versions.tcl "puts -nonewline \"[package require -exact Iwidgets ${SUGGESTED_IWIDGETS_VERSION}];[package require Itk];[package require Itcl]\";exit")
		execute_process(
		  COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_BINARY_DIR}/determine_versions.tcl
		  RESULT_VARIABLE IWIDGETS_RC
		  OUTPUT_VARIABLE IWIDGETS_VERSIONS_LIST
		  ERROR_VARIABLE IWIDGETS_STDERR
		  )
		if(NOT IWIDGETS_RC)
		  break()
		endif(NOT IWIDGETS_RC)
	      endforeach(SUGGESTED_IWIDGETS_VERSION 4.0.2 4.0.1 4.0.0)
	    endif(USE_INCRTCL_VERSION_4)
	    if(NOT IWIDGETS_RC)
	      list(GET IWIDGETS_VERSIONS_LIST 0 IWIDGETS_VERSION)
	      list(GET IWIDGETS_VERSIONS_LIST 1 CONSISTENT_ITK_VERSION)
	      list(GET IWIDGETS_VERSIONS_LIST 2 CONSISTENT_ITCL_VERSION)
	      message(STATUS "Checking that the Iwidgets, Itk, and Itcl packages are consistent")
	      if(CONSISTENT_ITCL_VERSION STREQUAL PLPLOT_ITCL_VERSION AND CONSISTENT_ITK_VERSION STREQUAL PLPLOT_ITK_VERSION)
		message(STATUS "Checking that the Iwidgets, Itk, and Itcl packages are consistent - true")
	      else(CONSISTENT_ITCL_VERSION STREQUAL PLPLOT_ITCL_VERSION AND CONSISTENT_ITK_VERSION STREQUAL PLPLOT_ITK_VERSION)
		message(STATUS "IWIDGETS_VERSION = ${IWIDGETS_VERSION}")
		message(STATUS "PLPLOT_ITK_VERSION = ${PLPLOT_ITK_VERSION}")
		message(STATUS "CONSISTENT_ITK_VERSION = ${CONSISTENT_ITK_VERSION}")
		message(STATUS "PLPLOT_ITCL_VERSION = ${PLPLOT_ITCL_VERSION}")
		message(STATUS "CONSISTENT_ITCL_VERSION = ${CONSISTENT_ITCL_VERSION}")
		message(STATUS "Checking that the Iwidgets, Itk, and Itcl packages are consistent - false")
		message(STATUS "WARNING: Consistent combination of Iwidgets, Itk, and Itcl not found so disabling Itcl and Itk")
		set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
		set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
	      endif(CONSISTENT_ITCL_VERSION STREQUAL PLPLOT_ITCL_VERSION AND CONSISTENT_ITK_VERSION STREQUAL PLPLOT_ITK_VERSION)
	    else(NOT IWIDGETS_RC)
	      message(STATUS "WARNING: Iwidgets could not be found so disabling Itcl and Itk")
	      set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
	      set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
	    endif(NOT IWIDGETS_RC)
          else(ITK_LIBRARY)
            message(STATUS "Looking for itk library - not found")
            message(STATUS "WARNING: Disabling Itk interface code")
            set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
          endif(ITK_LIBRARY)
	endif(ENABLE_itk)
      else(NOT ITK_RC)
        message(STATUS "Itk not available or not compatible with current Tcl shell")
        message(STATUS "WARNING: Disabling Itk interface code")
        set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
      endif(NOT ITK_RC)
    endif(ENABLE_itk)

    # ------------------------------------------------------------------------
    # figure out how to build tclIndex
    # ------------------------------------------------------------------------
    set(MKTCLINDEX ${CMAKE_SOURCE_DIR}/scripts/mktclIndex)
    if(ENABLE_itcl)
      set(MKTCLINDEX_ARGS "-itcl")
    else(ENABLE_itcl)
      set(MKTCLINDEX_ARGS "-tcl")
    endif(ENABLE_itcl)

  else(TCL_FOUND AND TCL_TCLSH)
    message(STATUS "Tcl not found OR tclsh not found")
    message(STATUS "WARNING: Disabling everything that is Tcl/Tk related")
    set(ENABLE_tcl OFF CACHE BOOL "Enable Tcl bindings" FORCE)
    set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
    set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
    set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
  endif(TCL_FOUND AND TCL_TCLSH)
  message(STATUS "Finished determining consistent system data for Tcl and friends")
else(ENABLE_tcl)
  message(STATUS "WARNING: ENABLE_tcl is OFF so disabling everything else that is Tcl/Tk related")
  set(ENABLE_itcl OFF CACHE BOOL "Enable Itcl interface code" FORCE)
  set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
  set(ENABLE_itk OFF CACHE BOOL "Enable Itk interface code" FORCE)
endif(ENABLE_tcl)

if(NOT ENABLE_itcl)
  set(PLPLOT_ITCL_VERSION 0.0.0)
endif(NOT ENABLE_itcl)

if(NOT ENABLE_itk)
  set(itk_true "#")
  set(PLPLOT_ITK_VERSION 0.0.0)
  set(PLPLOT_IWIDGETS_VERSION 0.0.0)
endif(NOT ENABLE_itk)
