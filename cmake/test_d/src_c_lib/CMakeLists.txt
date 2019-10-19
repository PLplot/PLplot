# Build hello_world C library, and its clone if necessary

# Implement the library build as a function because may have to do it twice.
function(configure_library_build target)
  add_library(${target} hello_world.c)

  if(TARGET ${tll_argument})
    target_link_libraries(${target} PRIVATE ${tll_argument})
  endif(TARGET ${tll_argument})

  set_target_properties(
    ${target}
    PROPERTIES
    INSTALL_NAME_DIR "${LIB_DIR}"
    )

  if(BUILD_SHARED_LIBS)
    set_target_properties(
      ${target}
      PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      SOVERSION 0
      VERSION 0.0
      )
  else(BUILD_SHARED_LIBS)
    set_target_properties(
      ${target}
      PROPERTIES
      # This allows static library builds to be linked by shared libraries.
      POSITION_INDEPENDENT_CODE ON
      )
  endif(BUILD_SHARED_LIBS)

  # The EXPORT export_test_d signature makes this library part of the
  # bundle of exported targets.  See src_d_lib/CMakeLists.txt for the
  # overall configuration of that bundle.

  install(TARGETS ${target}
    EXPORT export_test_d
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

endfunction(configure_library_build target)

configure_library_build(hello_world_c)

# Configure build of the hello_world_c_alt_threads library only if the following conditions are met:
# the C library must be built statically;
# the D compiler must not be gdc;
# the C library INTERFACE_LINK_LIBRARIES property must include in the list
# "$<LINK_ONLY:Threads::Threads>"; and
# the Threads::Threads INTERFACE_LINK_LIBRARIES property must include "-pthread";
# N.B. The genex expression below should handle the gdc case without issues, but
# typically we avoid the library approach below for gdc because that compiler
# understands the -pthread option without requiring the approach below.
if(NOT BUILD_SHARED_LIBS AND NOT CMAKE_D_COMPILER_ID STREQUAL "GNU")
  get_target_property(C_LIBRARY_INTERFACE_LINK_LIBRARIES hello_world_c INTERFACE_LINK_LIBRARIES)
  if(C_LIBRARY_INTERFACE_LINK_LIBRARIES)
    #message(STATUS "DEBUG: C_LIBRARY_INTERFACE_LINK_LIBRARIES = ${C_LIBRARY_INTERFACE_LINK_LIBRARIES}")
    list(FIND C_LIBRARY_INTERFACE_LINK_LIBRARIES "$<LINK_ONLY:Threads::Threads>" FOUND_Threads_index)
    #message(STATUS "DEBUG: FOUND_Threads_index = ${FOUND_Threads_index}")
    if("${FOUND_Threads_index}" GREATER_EQUAL "0")
      get_target_property(Threads_INTERFACE_LINK_LIBRARIES Threads::Threads INTERFACE_LINK_LIBRARIES)
      #message(STATUS "DEBUG: Threads_INTERFACE_LINK_LIBRARIES = ${Threads_INTERFACE_LINK_LIBRARIES}")
      if("${Threads_INTERFACE_LINK_LIBRARIES}" STREQUAL "-pthread")
	# All conditions have been met to build
	# hello_world_c_alt_threads the identical way to
	# hello_world_c, with INTERFACE_LINK_LIBRARIES property
	# updated to remove all references to THREADS::Threads and
	# -pthread, and adding an INTERFACE_LINK_OPTIONS that refers
	# to the appropriate genex for transforming (or not) -pthread
	# into a link option the relevant D compiler can understand.

	# N.B. A better solution would be simply to use this genex
	# within target_link_libraries (so no alt_threads version of the C
	# library would be necessary), but that elegant solution is
	# currently not allowed because of [CMake bug
	# 19757](https://gitlab.kitware.com/cmake/cmake/issues/19757).
	configure_library_build(hello_world_c_alt_threads)
	# Determine the desired INTERFACE_LINK_LIBRARIES property for
	# this alt_threads version of the library by removing the following items from the
	# list.  Only the first removed item is relevant for the
	# test_d project, but the other removed items might be
	# relevant in the plplot project case.
	list(REMOVE_ITEM C_LIBRARY_INTERFACE_LINK_LIBRARIES "$<LINK_ONLY:Threads::Threads>" "Threads::Threads" "-pthread")
	#message(STATUS "DEBUG: C_LIBRARY_INTERFACE_LINK_LIBRARIES = ${C_LIBRARY_INTERFACE_LINK_LIBRARIES}")
	set_target_properties(hello_world_c_alt_threads
	  PROPERTIES
	  INTERFACE_LINK_LIBRARIES "${C_LIBRARY_INTERFACE_LINK_LIBRARIES}"
	  )
	# Create an interface library to carry the required INTERFACE_LINK_OPTIONS genex
	add_library(hello_world_c_alt_interface INTERFACE)
	set_target_properties(hello_world_c_alt_interface
	  PROPERTIES
	  INTERFACE_LINK_OPTIONS $<IF:$<COMPILE_LANG_AND_ID:D,DigitalMars,LDC>,-Xcc=-pthread,-pthread>
	  )
	install(TARGETS hello_world_c_alt_interface
	  EXPORT export_test_d
	  ARCHIVE DESTINATION ${LIB_DIR}
	  LIBRARY DESTINATION ${LIB_DIR}
	  RUNTIME DESTINATION ${BIN_DIR}
	  )
      endif("${Threads_INTERFACE_LINK_LIBRARIES}" STREQUAL "-pthread")
    endif("${FOUND_Threads_index}" GREATER_EQUAL "0")
  endif(C_LIBRARY_INTERFACE_LINK_LIBRARIES)
endif(NOT BUILD_SHARED_LIBS AND NOT CMAKE_D_COMPILER_ID STREQUAL "GNU")
