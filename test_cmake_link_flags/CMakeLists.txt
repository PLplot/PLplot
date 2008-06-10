# Simple test platform for cmake_link_flags module
cmake_minimum_required(VERSION 2.6)

macro(cmake_link_flags _link_flags_out _link_flags_in)
  # Transform link flags delivered by pkg-config into the best form
  # for CMake.
  # N.B. may need revision for windows since the current assumption
  # is pkg-config delivers link flags using the -L and -l options which
  # may not be the case for windows.
  # N.B. ${_link_flags_in} must be a string and not a list.

  if("${_link_flags_in}" STREQUAL "")
    set(${_link_flags_out})
  else("${_link_flags_in}" STREQUAL ""})
    #message("(original link flags) = ${_link_flags_in}")
    # Convert link flags to a list.  Note some link flags are blank-delimited
    # (such as "-framework whatever") so preserve those by splitting into
    # separate list elements only if the next element starts with a hyphen.
    string(REGEX REPLACE " -" ";-" _link_flags_list "${_link_flags_in}")
    # Extract list of directories from -L options.
    list(LENGTH _link_flags_list _link_flags_length)
    math(EXPR _link_flags_length "${_link_flags_length} - 1")
    set(_index_list)
    set(_link_directory_list)
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-L" "" _list_element1 "${_list_element}")
      if(_list_element STREQUAL "-L${_list_element1}")
        list(APPEND _index_list ${_list_index})
        list(APPEND _link_directory_list ${_list_element1})
      endif(_list_element STREQUAL "-L${_list_element1}")
    endforeach(_list_index RANGE ${_link_flags_length})
    #message("_index_list = ${_index_list}")
    if("${_index_list}" STREQUAL "")
    else("${_index_list}" STREQUAL "")
      # Remove -L options from list.
      list(REMOVE_AT _link_flags_list ${_index_list})
    endif("${_index_list}" STREQUAL "")
    #message("_link_directory_list = ${_link_directory_list}")
    #message("_link_flags_list (without -L options) = ${_link_flags_list}")
  
    # Derive ${_link_flags_out} from _link_flags_list with -l options 
    # replaced by complete pathname of library.
    list(LENGTH _link_flags_list _link_flags_length)
    math(EXPR _link_flags_length "${_link_flags_length} - 1")
    set(${_link_flags_out})
    foreach(_list_index RANGE ${_link_flags_length})
      list(GET _link_flags_list ${_list_index} _list_element)
      string(REGEX REPLACE "^-l" "" _list_element1 "${_list_element}")
      if(_list_element STREQUAL "-l${_list_element1}")
        set(_library_pathname "_library_pathname-NOTFOUND")
        find_library(
         _library_pathname 
         ${_list_element1}
         PATHS ${_link_directory_list}
         NO_DEFAULT_PATH
        )
	# Try second time (without NO_DEFAULT_PATH) just in case pkg-config
	# specified some system libraries without corresponding -L options.
        find_library(
         _library_pathname 
         ${_list_element1}
         PATHS ${_link_directory_list}
        )
        if(NOT _library_pathname)
          message(
  	"Cannot find library corresponding to linker option ${_list_element}"
  	)
          message(
  	"original link flags delivered by pkg-config = ${_link_flags_in}"
  	)
          message(FATAL_ERROR "FATAL ERROR in cmake_link_flags macro")
        endif(NOT _library_pathname)
        list(APPEND ${_link_flags_out} ${_library_pathname})
      else(_list_element STREQUAL "-L${_list_element1}")
        # link options that are not -L or -l passed through in correct order
        # in ${_link_flags_out}.
        list(APPEND ${_link_flags_out} ${_list_element})
      endif(_list_element STREQUAL "-l${_list_element1}")
    endforeach(_list_index RANGE ${_link_flags_length})
    #message("${_link_flags_out} = ${${_link_flags_out}}")
  endif("${_link_flags_in}" STREQUAL "")
endmacro(cmake_link_flags)

# Here is where we test various input flags to make sure they work.
# Reported on list by Werner Smekal for Mac OS X, "wx-config --libs" result,
# but with Linux library flag -lIDL-2 substituted for Mac library -lwx_macud-2.8
set(flags_in "-framework QuickTime -framework IOKit -framework Carbon -framework Cocoa -framework System  -lIDL-2")
message("flags_in = ${flags_in}")
cmake_link_flags(flags_out ${flags_in})
message("flags_out = ${flags_out}")

# Actual wx-config --libs results on Linux
set(flags_in "-pthread   -lwx_gtk2u_xrc-2.6 -lwx_gtk2u_qa-2.6 -lwx_gtk2u_html-2.6 -lwx_gtk2u_adv-2.6 -lwx_gtk2u_core-2.6 -lwx_baseu_xml-2.6 -lwx_baseu_net-2.6 -lwx_baseu-2.6")
message("flags_in = ${flags_in}")
cmake_link_flags(flags_out ${flags_in})
message("flags_out = ${flags_out}")

