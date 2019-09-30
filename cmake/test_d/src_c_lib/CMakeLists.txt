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

# Configure the hello_world_c_static_D_linking library only if the following conditions are met:
# the C library must be built statically;
# the D compiler must not be gdc;
# the C library INTERFACE_LINK_LIBRARIES property must include in the list
# "$<LINK_ONLY:Threads::Threads>"; and
# the Threads::Threads INTERFACE_LINK_LIBRARIES property must include "-pthread";
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
	# All conditions have been met to build  hello_world_c_static_D_linking the identical way
	# to hello_world_c, and then modify the INTERFACE_LINK_LIBRARIES property of that
	# otherwise identical library.
	configure_library_build(hello_world_c_static_D_linking)
	# Determine the desired INTERFACE_LINK_LIBRARIES property for this library
	# by replacing all instances of "$<LINK_ONLY:Threads::Threads>" by 
	string(REPLACE "$<LINK_ONLY:Threads::Threads>" "-Xcc=-pthread" STATIC_D_LINKING_INTERFACE_LINK_LIBRARIES ${C_LIBRARY_INTERFACE_LINK_LIBRARIES})
	#message(STATUS "DEBUG: STATIC_D_LINKING_INTERFACE_LINK_LIBRARIES ${STATIC_D_LINKING_INTERFACE_LINK_LIBRARIES}")
	set_target_properties(hello_world_c_static_D_linking
	  PROPERTIES
	  INTERFACE_LINK_LIBRARIES ${STATIC_D_LINKING_INTERFACE_LINK_LIBRARIES}
	  )
      endif("${Threads_INTERFACE_LINK_LIBRARIES}" STREQUAL "-pthread")
    endif("${FOUND_Threads_index}" GREATER_EQUAL "0")
  endif(C_LIBRARY_INTERFACE_LINK_LIBRARIES)
endif(NOT BUILD_SHARED_LIBS AND NOT CMAKE_D_COMPILER_ID STREQUAL "GNU")
