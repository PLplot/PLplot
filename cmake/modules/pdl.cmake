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
  option(ENABLE_pdl "Enable Perl/PDL examples in tests" OFF)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_pdl)
  if(NOT BUILD_SHARED_LIBS)
    message(STATUS "WARNING: "
      "Disabling Perl/PDL examples because BUILD_SHARED_LIBS=OFF")
    set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
  endif(NOT BUILD_SHARED_LIBS)
endif(ENABLE_pdl)

if(ENABLE_pdl)
  option(SKIP_PDL_CHECK "SKIP all PDL checks because libplplotd not yet available at cmake time" OFF)
  if(SKIP_PDL_CHECK)
    message(STATUS "Assume PDL exists and has version >= 2.4.7")
    message(STATUS "Assume PDL-Graphics-PLplot exists and has version >= 0.55")
  else(SKIP_PDL_CHECK)
    # N.B. Some of the checks below only work if the correct version
    # of libplplotd.so (currently libplplotd.so.10) is accessible
    # somewhere on the disk, and LD_LIBRARY_PATH
    # points to that directory location.
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
	message(STATUS "PDL_VERSION = ${PDL_VERSION}")
	if(${PDL_VERSION} VERSION_LESS "2.4.7")
          message(STATUS "WARNING: "
            "PDL version < 2.4.7. Disabling Perl/PDL examples in tests")
          set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
	else(${PDL_VERSION} VERSION_LESS "2.4.7")
          execute_process(
	    COMMAND ${PERL_EXECUTABLE}
	    -MPDL::Graphics::PLplot "-e print \"$PDL::Graphics::PLplot::VERSION\""
	    RESULT_VARIABLE PDL_GRAPHICS_PLPLOT_RETURN_CODE
            OUTPUT_QUIET
            ERROR_QUIET
            )
	  if(NOT PDL_GRAPHICS_PLPLOT_RETURN_CODE)
            message(STATUS
	      "PDL version >= 2.4.7 and  PDL::Graphics::PLplot found."
	      )
            execute_process(
	      COMMAND ${PERL_EXECUTABLE}
	      -MPDL::Graphics::PLplot "-e print \"$PDL::Graphics::PLplot::VERSION\""
              OUTPUT_VARIABLE PDL_PLPLOT_VERSION
              ERROR_VARIABLE PDL_PLPLOT_VERSION
              )
            message(STATUS "PDL_PLPLOT_VERSION = ${PDL_PLPLOT_VERSION}")
            if(${PDL_PLPLOT_VERSION} VERSION_LESS "0.55")
              message(STATUS "WARNING: "
		"perl PDL PLplot version < 0.55\n"
		"   Drop testing of Perl/PDL examples.")
	      set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
            else(${PDL_PLPLOT_VERSION} VERSION_LESS "0.55")
              message(STATUS
		"perl PDL PLplot version >= 0.55\n"
		"   Test Perl/PDL examples")
            endif(${PDL_PLPLOT_VERSION} VERSION_LESS "0.55")
	  else(NOT PDL_GRAPHICS_PLPLOT_RETURN_CODE)
	    message(STATUS "WARNING: "
	      "perl PDL::GRAPHICS_PLplot module not found. Disabling Perl/PDL examples")
	    set(ENABLE_pdl OFF CACHE BOOL "Enable Perl/PDL examples in tests" FORCE)
	  endif(NOT PDL_GRAPHICS_PLPLOT_RETURN_CODE)
	endif(${PDL_VERSION} VERSION_LESS "2.4.7")
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
  endif(SKIP_PDL_CHECK)
endif(ENABLE_pdl)
