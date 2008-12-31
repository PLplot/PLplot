# cmake/modules/pdl.cmake
#
#
# Copyright (C) 2006  Alan W. Irwin
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

# Module for determining configuration options related to PDL examples

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_pdl "Enable Perl/PDL examples in tests" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_pdl "Enable Perl/PDL examples in tests" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_pdl)
  if(NOT BUILD_SHARED_LIBS)
    message(STATUS "WARNING: "
    "Disabling Perl/PDL examples because BUILD_SHARED_LIBS=OFF")
    set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
  endif(NOT BUILD_SHARED_LIBS)
endif(ENABLE_pdl)

if(ENABLE_pdl)
  if(PERL_FOUND)
    execute_process(
    COMMAND ${PERL_EXECUTABLE}
    -e "use PDL"
    RESULT_VARIABLE PDL_RETURNCODE
    OUTPUT_QUIET
    ERROR_QUIET
    )
    if(NOT PDL_RETURNCODE)
      execute_process(
      COMMAND ${PERL_EXECUTABLE}
      "-e use PDL; print \"$PDL::VERSION\""
      OUTPUT_VARIABLE PDL_VERSION
      )
    message("PDL_VERSION = ${PDL_VERSION}")
      transform_version(NUMERICAL_PDL_MINIMUM_VERSION "2.4.1")
      transform_version(NUMERICAL_PDL_VERSION "${PDL_VERSION}")
      if(NUMERICAL_PDL_VERSION LESS "${NUMERICAL_PDL_MINIMUM_VERSION}")
        message(STATUS "WARNING: "
        "perl PDL version < 2.4.1. "
	"Disabling Perl/PDL examples")
        set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
      else(NUMERICAL_PDL_VERSION LESS "${NUMERICAL_PDL_MINIMUM_VERSION}")
        message(STATUS 
	   "Perl and PDL are available and PDL passes version test.\n"
	"   Enable Perl/PDL examples in tests"
	)
        execute_process(
	  COMMAND ${PERL_EXECUTABLE}
	  -MPDL::Graphics::PLplot "-e print \"$PDL::Graphics::PLplot::VERSION\""
          OUTPUT_VARIABLE PDL_PLPLOT_VERSION
          ERROR_VARIABLE PDL_PLPLOT_VERSION
          )
        #message("PDL_PLPLOT_VERSION = ${PDL_PLPLOT_VERSION}")
        transform_version(NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION "0.47.0")
        transform_version(NUMERICAL_PDL_PLPLOT_VERSION "${PDL_PLPLOT_VERSION}.0")
	#message("NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION = ${NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION}")
	#message("NUMERICAL_PDL_PLPLOT_VERSION = ${NUMERICAL_PDL_PLPLOT_VERSION}")
        if(NUMERICAL_PDL_PLPLOT_VERSION LESS "${NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION}")
          message(STATUS "WARNING: "
          "perl PDL PLplot version < 0.47 "
          "Using more limited set of Perl/PDL examples that work with old versions")
          set(HAVE_PDL_GRAPHICS_PLPLOT_40 OFF CACHE BOOL "PDL-Graphics-PLplot version is 0.47 or higher" FORCE)
        else(NUMERICAL_PDL_PLPLOT_VERSION LESS "${NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION}")
          set(HAVE_PDL_GRAPHICS_PLPLOT_40 ON CACHE BOOL "PDL-Graphics-PLplot version is 0.47 or higher" FORCE)
        endif(NUMERICAL_PDL_PLPLOT_VERSION LESS "${NUMERICAL_PDL_PLPLOT_MINIMUM_VERSION}")
      endif(NUMERICAL_PDL_VERSION LESS "${NUMERICAL_PDL_MINIMUM_VERSION}")
    else(NOT PDL_RETURNCODE)
      message(STATUS "WARNING: "
      "perl PDL module not found. Disabling Perl/PDL examples")
      set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
    endif(NOT PDL_RETURNCODE)
  else(PERL_FOUND)
    message(STATUS "WARNING: "
    "perl not found. Disabling Perl/PDL examples")
    set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
  endif(PERL_FOUND)
endif(ENABLE_pdl)
