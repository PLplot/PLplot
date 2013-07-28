cmake_policy(SET CMP0007 NEW)
file(STRINGS ${FILENAME} lines)
list(LENGTH lines nlines)
# There are currently groups of 9 lines for each
# gtk software package specifying the required
# build-configuration information.
set(group_size 9)
set(remove_indices 0 1 2 3 4 5 6 7 8)
math(EXPR max_index "(${nlines}/${group_size}) - 1")
foreach(index RANGE 0 ${max_index})
  list(GET lines 0 package)
  list(GET lines 1 config_type)
  list(GET lines 2 config_arguments)
  list(GET lines 3 download_hash_type)
  list(GET lines 4 download_hash)
  list(GET lines 5 download_href)
  list(GET lines 6 dependencies)
  list(GET lines 7 suggests)
  list(GET lines 8 after)
  list(REMOVE_AT lines ${remove_indices})

  # Turn colon-separated lists into official CMake lists.
  string(REGEX REPLACE ":" ";" dependencies "${dependencies}")
  string(REGEX REPLACE ":" ";" suggests "${suggests}")
  string(REGEX REPLACE ":" ";" after "${after}")

  # For now, ignore "after" and treat "dependencies" and "suggests" the same
  if(suggests)
    list(APPEND dependencies "${suggests}")
  endif(suggests)

  # Configure the package file using the appropriate template file.
  if(config_type STREQUAL "autotools" OR config_type STREQUAL "tarball" OR config_type STREQUAL "cmake")
    set(template_file template.bp.in)
  else(config_type STREQUAL "autotools" OR config_type STREQUAL "tarball" OR config_type STREQUAL "cmake")
    message(FATAL_ERROR "incorrect config_type = ${config_type}")
  endif(config_type STREQUAL "autotools" OR config_type STREQUAL "tarball" OR config_type STREQUAL "cmake")
  
  if(NOT package)
    message(FATAL_ERROR "bad package = ${package}")
  endif(NOT package)
  
  file(MAKE_DIRECTORY ${package})
  configure_file(${template_file} ${package}/bp.cmake @ONLY)
  
endforeach(index RANGE 0 ${max_index})
