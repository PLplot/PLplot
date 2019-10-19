# src_d_lib/CMakeLists.txt

set(SOURCE_LIST
${CMAKE_CURRENT_SOURCE_DIR}/hello_world.d
)

add_library(hello_world_d ${D_LIBRARY_TYPE} ${SOURCE_LIST})

if(TARGET hello_world_c_alt_threads)
  target_link_libraries(hello_world_d PRIVATE hello_world_c_alt_threads)
else(TARGET hello_world_c_alt_threads)
  target_link_libraries(hello_world_d PRIVATE hello_world_c)
endif(TARGET hello_world_c_alt_threads)

if(BUILD_SHARED_LIBS)
  if(USE_RPATH)
    set_target_properties(
      hello_world_d
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  else(USE_RPATH)
    set_target_properties(
      hello_world_d
      PROPERTIES
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  set_target_properties(
    hello_world_d
    PROPERTIES
    SOVERSION 0
    VERSION 0.0
    )

else(BUILD_SHARED_LIBS)
  set_target_properties(
    hello_world_d
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    )
endif(BUILD_SHARED_LIBS)

install(FILES hello_world.d DESTINATION ${SOURCE_CODE_DIR})

install(TARGETS hello_world_d
  EXPORT export_test_d
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )

# This exports information for every target for the whole build that
# has the EXPORT export_test_d signature for the
# install(TARGETS ...) command.  The only two such targets in this test_d
# project is the hello_world_d target above, and hello_world_c.
install(EXPORT export_test_d DESTINATION ${LIB_DIR}/cmake/test_d)

# Install overall configuration file describing the above export
install(FILES test_dConfig.cmake DESTINATION ${LIB_DIR}/cmake/test_d)
