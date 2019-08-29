# Build hello_world C library.
add_library(hello_world_c hello_world.c)

set_target_properties(
  hello_world_c
  PROPERTIES
  INSTALL_NAME_DIR "${LIB_DIR}"
  )

if(BUILD_SHARED_LIBS)
  set_target_properties(
    hello_world_c
    PROPERTIES
    COMPILE_DEFINITIONS "USINGDLL"
    SOVERSION 0
    VERSION 0.0
    )
else(BUILD_SHARED_LIBS)
  set_target_properties(
    hello_world_c
    PROPERTIES
    # This allows static library builds to be linked by shared libraries.
    POSITION_INDEPENDENT_CODE ON
    )
endif(BUILD_SHARED_LIBS)

# The EXPORT export_test_d signature makes this library part of the
# bundle of exported targets.  See src_d_lib/CMakeLists.txt for the
# overall configuration of that bundle.

install(TARGETS hello_world_c
  EXPORT export_test_d
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )
