# cmake/modules/wxwidgets.cmake
#
# Copyright (C) 2006 Werner Smekal
# Copyright (C) 2015-2017 Alan W. Irwin
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

# Module for determining all configuration variables related to the wxwidgets
# device driver.
# The following variables are set/modified:
# PLD_wxwidgets		  - ON means the wxwidgets device is enabled.
# wxwidgets_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile wxwidgets
# 			    device.
# wxwidgets_LINK_FLAGS	  - list of full path names of libraries and
# 			    linker flags for dynamic wxwidgets device driver.
# wxwidgets_RPATH	  - rpath (if needed) for anything linked to the
#                           wxwidgets libraries.
# DRIVERS_LINK_FLAGS  	  - list of device LINK_FLAGS for case
# 			    when ENABLE_DYNDRIVERS OFF.
# Find wxWidgets needed for driver and binding

if(PLD_wxwidgets OR PLD_wxpng)
  if(NOT CMAKE_CXX_COMPILER_WORKS)
    message(STATUS "WARNING: no working C++ compiler so "
      "setting all wxwidgets devices to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  endif(NOT CMAKE_CXX_COMPILER_WORKS)
endif(PLD_wxwidgets OR PLD_wxpng)

if(PLD_wxwidgets OR PLD_wxpng)
  if(FORCE_EXTERNAL_STATIC)
    set(wxWidgets_USE_STATIC ON)
  endif(FORCE_EXTERNAL_STATIC)
  find_package(wxWidgets COMPONENTS base core QUIET)
  message(STATUS "wxWidgets_FOUND           : ${wxWidgets_FOUND}")
  message(STATUS "wxWidgets_INCLUDE_DIRS    : ${wxWidgets_INCLUDE_DIRS}")
  message(STATUS "wxWidgets_LIBRARY_DIRS    : ${wxWidgets_LIBRARY_DIRS}")
  message(STATUS "wxWidgets_LIBRARIES       : ${wxWidgets_LIBRARIES}")
  message(STATUS "wxWidgets_CXX_FLAGS       : ${wxWidgets_CXX_FLAGS}")
  message(STATUS "wxWidgets_USE_FILE        : ${wxWidgets_USE_FILE}")
  # Convert wxWidgets_LIBRARIES to full pathname form.
  cmake_link_flags(wxwidgets_LINK_FLAGS "${wxWidgets_LIBRARIES}")
  if(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)
    message(STATUS
      "WARNING: wxWidgets or its libraries not found so "
      "setting all wxwidgets devices to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  else(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)
    # Check if stdint.h can be used from c++ (PL_HAVE_CXX_STDINT_H)
    include(TestForStdintCXX)
  endif(NOT wxWidgets_FOUND OR NOT wxwidgets_LINK_FLAGS)
endif(PLD_wxwidgets OR PLD_wxpng)

if(PLD_wxwidgets OR PLD_wxpng)
  # Determine wxwidgets version in cross-platform way.
  set(check_wxwidgets_version_source "
#include <wx/version.h>
int main(void)
{
// True if version is 3.0.0 or later....
#if  wxCHECK_VERSION(3, 0, 0)
// Return success
  return 0;
#else
// Return failure
  return 1;
#endif
}
")
  include(CheckCSourceRuns)

  cmake_push_check_state()
  list(APPEND CMAKE_REQUIRED_INCLUDES ${wxWidgets_INCLUDE_DIRS})
  message(STATUS "Checking whether wxwidgets version >= 3.0.0")
  check_c_source_runs("${check_wxwidgets_version_source}" WX_VERSION_LARGE_ENOUGH)
  cmake_pop_check_state()
  if(NOT WX_VERSION_LARGE_ENOUGH)
    message(STATUS
      "WARNING: wxWidgets version is less than 3.0.0 so "
      "setting all wxwidgets devices to OFF."
      )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
  endif(NOT WX_VERSION_LARGE_ENOUGH)
endif(PLD_wxwidgets OR PLD_wxpng)

if(PLD_wxwidgets OR PLD_wxpng)
  string(REGEX REPLACE ";" " -I"
    wxwidgets_COMPILE_FLAGS
    "-I${wxWidgets_INCLUDE_DIRS}"
    )
  string(REGEX REPLACE ";" " -D"
    wxWidgets_DEFINITIONS_wD
    "-D${wxWidgets_DEFINITIONS}"
    )
  set(wxWidgets_DEFINITIONS_DEBUG_wD "")
  if(wxWidgets_DEFINITIONS_DEBUG AND CMAKE_BUILD_TYPE MATCHES "Debug")
    string(REGEX REPLACE ";" " -D"
      wxWidgets_DEFINITIONS_DEBUG_wD
      "-D${wxWidgets_DEFINITIONS_DEBUG}"
      )
  endif(wxWidgets_DEFINITIONS_DEBUG AND CMAKE_BUILD_TYPE MATCHES "Debug")
  string(REGEX REPLACE ";" " "
    wxwidgets_COMPILE_FLAGS
    ${wxwidgets_COMPILE_FLAGS}
    " ${wxWidgets_DEFINITIONS_wD} ${wxWidgets_DEFINITIONS_DEBUG_wD}"
    )
  message(STATUS "wxwidgets_COMPILE_FLAGS = ${wxwidgets_COMPILE_FLAGS}")
  message(STATUS "wxwidgets_LINK_FLAGS = ${wxwidgets_LINK_FLAGS}")
  set(wxwidgets_RPATH ${wxWidgets_LIBRARY_DIRS})
  filter_rpath(wxwidgets_RPATH)
  if(wxwidgets_RPATH)
    message(STATUS "wxwidgets_RPATH = ${wxwidgets_RPATH}")
  endif(wxwidgets_RPATH)

  option(OLD_WXWIDGETS "Use old version of wxwidgets device driver, binding, and example" OFF)

  if(OLD_WXWIDGETS)
    if(WITH_FREETYPE)
      include(agg)
      if(HAVE_AGG)
	set(
	  wxwidgets_COMPILE_FLAGS
	  "${wxwidgets_COMPILE_FLAGS} -I${AGG_INCLUDE_DIRS}"
	  )
	set(
	  wxwidgets_LINK_FLAGS
	  ${wxwidgets_LINK_FLAGS}
	  ${AGG_LIBRARIES}
	  )
      else(HAVE_AGG)
	message(STATUS "WARNING: old wxwidgets driver and binding components depending on AGG library have been dropped.")
      endif(HAVE_AGG)
      set(
	wxwidgets_COMPILE_FLAGS
	"${wxwidgets_COMPILE_FLAGS} ${FREETYPE_INCLUDE_CFLAGS}"
	)
      set(
	wxwidgets_LINK_FLAGS
	${wxwidgets_LINK_FLAGS}
	${FREETYPE_LIBRARIES}
	)
    endif(WITH_FREETYPE)
  else(OLD_WXWIDGETS)
    # Note that both wxwidgets and wxPLViewer contains calls to shm_open
    # and shm_unlink which on most Unix systems requires an explicit link
    # with the rt library.  However, Mac OS X is a Unix system that does
    # not have the rt library and provides those functions instead as part
    # of the ordinary C libraries that do not require explicit linking.
    # So only define RT_LIB for the Unix but not Mac case.
    if(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      find_library(RT_LIB rt)
      if(RT_LIB)
	message(STATUS "RT_LIB = ${RT_LIB}")
      else(RT_LIB)
	message(STATUS "WARNING: rt library could not be found so that wxwidgets and wxPLViewer may not link correctly")
	set(RT_LIB)
      endif(RT_LIB)
    else(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      set(RT_LIB)
    endif(UNIX AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  endif(OLD_WXWIDGETS)

  set(DRIVERS_LINK_FLAGS
    ${DRIVERS_LINK_FLAGS}
    ${wxwidgets_LINK_FLAGS}
    )

endif(PLD_wxwidgets OR PLD_wxpng)

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_wxwidgets "Enable wxwidgets binding" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_wxwidgets "Enable wxwidgets binding" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_wxwidgets AND NOT ENABLE_cxx)
  # libplplotwxwidgets depends on libplplotcxx.
  message(STATUS "WARNING: C++ binding is disabled so "
    "setting ENABLE_wxwidgets to OFF."
    )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets binding" FORCE)
endif(ENABLE_wxwidgets AND NOT ENABLE_cxx)

if(ENABLE_wxwidgets AND NOT PLD_wxwidgets)
  message(STATUS
    "WARNING: PLD_wxwidgets is OFF so "
    "setting ENABLE_wxwidgets to OFF."
    )
  set(ENABLE_wxwidgets OFF CACHE BOOL "Enable wxwidgets binding" FORCE)
endif(ENABLE_wxwidgets AND NOT PLD_wxwidgets)

if(ENABLE_wxwidgets)
  set(wxwidgets_true "")

  set(wxwidgets_SOURCE)
  if(OLD_WXWIDGETS)
    set(wxdemo_name deprecated_wxPLplotDemo)
  else(OLD_WXWIDGETS)
    # These options only suitable if the new wxwidgets is enabled.
    option(PLPLOT_WX_DEBUG_OUTPUT "Enable debug output for wxwidgets device driver, binding, and example" OFF)
    option(PLPLOT_WX_NANOSEC "Experimental option (because it may lead to build failures (!)) for nanosec timestamp for wx debug output" OFF)
    # This option works well on Linux (except for problems with locate mode that also appear
    # in slightly different form when PL_WXWIDGETS_IPC3 is OFF).  So after that issue is fixed
    # and further testing occurs on Windows, the experimental "moniker" should be dropped.
    option(PL_WXWIDGETS_IPC3 "Experimental option to use the three-semaphores approach for wxwidgets IPC" OFF)
    if(PL_WXWIDGETS_IPC3 AND NOT WIN32)
      # Unnamed POSIX semaphores appear to work fine on Linux, but
      # they are an optional POSIX standard which is known to be
      # unimplemented on Mac OS X (and presumably other proprietary
      # Unix systems).  These type of semaphores are also not
      # supported on Windows.  (Well, there is the POSIX threads for Windows
      # project, but unnamed semaphores only supported in that case
      # for shared threads and not shared memory according to
      # <https://www.sourceware.org/pthreads-win32/manual/sem_init.html>
      
      if(NOT DEFINED PL_HAVE_UNNAMED_POSIX_SEMAPHORES)
	message(STATUS "Check for unnamed POSIX semaphores")
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Check_unnamed_posix_semaphores.c "
#include <semaphore.h>
// Test that sem_init and sem_destroy (API exclusively used for unnamed POSIX
// semaphores) both work.  This test is for the shared thread case (where second
// argument of sem_init must be 0) since that test is much easier to implement
// than the shared memory case.  N.B. Our build system assumes that if the shared
// threads case works, the shared memory case (which is the only
// cased used by PLplot code) will also work.

// Must be global variable for the shared threads case.
sem_t semaphore;

// main must return 1 on success and 0 on failure
int
main(void)
{
     // Return value not necessarily defined to be zero on success
     // on some POSIX platforms (ugh!), but must be defined (to -1)
     // on failure for all POSIX platforms.  So test only for failure case.
     if( sem_init(&semaphore, 0, 0) != 0 || sem_destroy(&semaphore) != 0 )
         return 0;
     else
         return 1;
}
	  ")
	try_run(RUN_RESULT COMPILE_RESULT
	  ${CMAKE_BINARY_DIR}
	  ${CMAKE_CURRENT_BINARY_DIR}/Check_unnamed_posix_semaphores.c
	  # N.B. linking this way might only work on Linux.
	  LINK_LIBRARIES "-pthread"
	  OUTPUT_VARIABLE OUTPUT
	  )
	#message(STATUS "COMPILE_RESULT = ${COMPILE_RESULT}")
	#message(STATUS "RUN_RESULT = ${RUN_RESULT}")
	if(COMPILE_RESULT AND NOT RUN_RESULT MATCHES "FAILED_TO_RUN" AND RUN_RESULT)
	  set(PL_HAVE_UNNAMED_POSIX_SEMAPHORES ON)
	else(COMPILE_RESULT AND NOT RUN_RESULT MATCHES "FAILED_TO_RUN" AND RUN_RESULT)
	  set(PL_HAVE_UNNAMED_POSIX_SEMAPHORES OFF)
	endif(COMPILE_RESULT AND NOT RUN_RESULT MATCHES "FAILED_TO_RUN" AND RUN_RESULT)

	if(PL_HAVE_UNNAMED_POSIX_SEMAPHORES)
	  message(STATUS "Check for unnamed POSIX semaphores - found")
	  file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
	    "Determining whether unnamed POSIX semaphores are supported succeeded with "
	    "the following output:\n${OUTPUT}\n\n"
	    )
	else(PL_HAVE_UNNAMED_POSIX_SEMAPHORES)
	  message(STATUS "Check for unnamed POSIX semaphores - not found")
	  file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
	    "Determining whether unnamed POSIX semaphores are supported failed with "
	    "the following output:\n${OUTPUT}\n\n"
	    )
	endif(PL_HAVE_UNNAMED_POSIX_SEMAPHORES)
      endif(NOT DEFINED PL_HAVE_UNNAMED_POSIX_SEMAPHORES)
    endif(PL_WXWIDGETS_IPC3 and NOT WIN32)
    set(wxdemo_name wxPLplotDemo)
    if((PLD_wxwidgets OR PLD_wxpng) AND PLPLOT_WX_DEBUG_OUTPUT AND PLPLOT_WX_NANOSEC)
      # This is added to later by drivers_finish with
      # the standard source code for this device driver.
      set(wxwidgets_SOURCE ${CMAKE_SOURCE_DIR}/bindings/wxwidgets/wxPLplot_nanosec.cpp)
    endif((PLD_wxwidgets OR PLD_wxpng) AND PLPLOT_WX_DEBUG_OUTPUT AND PLPLOT_WX_NANOSEC)
  endif(OLD_WXWIDGETS)

else(ENABLE_wxwidgets)
  set(wxwidgets_true "#")
  # -dev wxwidgets depends on wxPLViewer which depends on libplplotwxwidgets
  message(STATUS
    "WARNING: ENABLE_wxwidgets is OFF so "
    "setting all wxwidgets devices to OFF."
    )
    set(PLD_wxwidgets OFF CACHE BOOL "Enable wxwidgets device" FORCE)
    set(PLD_wxpng OFF CACHE BOOL "Enable wxwidgets png device" FORCE)
endif(ENABLE_wxwidgets)
