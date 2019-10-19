# src_d_executable/CMakeLists.txt

if(BUILD_TEST)
  add_executable(hello hello.d)

  if(CORE_BUILD)
    set_source_files_properties(
      hello.d
      PROPERTIES
      INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src_d_lib
      )
  else(CORE_BUILD)
    set_source_files_properties(
      hello.d
      PROPERTIES
      INCLUDE_DIRECTORIES ${SOURCE_CODE_DIR}
      )
  endif(CORE_BUILD)
  if(TARGET hello_world_c_alt_interface)
    # Need explicit link to that library to pick up INTERFACE_LINK_OPTIONS from it
    target_link_libraries(hello hello_world_d hello_world_c_alt_interface)
  else(TARGET hello_world_c_alt_interface)
    target_link_libraries(hello hello_world_d)
  endif(TARGET hello_world_c_alt_interface)

  # configure file to compare with output of hello executable.
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/compare_hello.out
    "hello, world\n"
    )

  # Add test_noninteractive custom_target to compare hello executable output
  # with file configure above.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/hello.out
    COMMAND
    ./hello > ${CMAKE_CURRENT_BINARY_DIR}/hello.out
    COMMAND
    ${CMP_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/hello.out ${CMAKE_CURRENT_BINARY_DIR}/compare_hello.out
    DEPENDS
    hello
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
    )
  add_custom_target(test_noninteractive
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/hello.out
    )

endif(BUILD_TEST)

if(CORE_BUILD)
  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/hello.d
    DESTINATION ${DATA_DIR}/examples/d
    )
endif(CORE_BUILD)
