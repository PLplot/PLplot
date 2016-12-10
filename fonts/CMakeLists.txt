# fonts/CMakeLists.txt
# N.B. the plhershey-unicode-gen stuff is taken care of by
# ../include/CMakeLists.txt.  This file only used to optionally build the
# Hershey fonts.

option(BUILD_HERSHEY_FONTS "Build Hershey fonts?" OFF)

if(BUILD_HERSHEY_FONTS)

  # Build Hershey fonts.

  # Create "convenience static library" to help build Hershey fonts.

  set(libhershey_SRC
    font01.c
    font02.c
    font03.c
    font04.c
    font05.c
    font06.c
    font07.c
    font08.c
    font09.c
    font10.c
    font11.c
    )

  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )
  add_library(hershey STATIC ${libhershey_SRC})

  add_executable(generate_stndfonts stndfont.c)
  if(BUILD_SHARED_LIBS)
    set_target_properties(generate_stndfonts PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  target_link_libraries(generate_stndfonts hershey plplot)

  add_executable(generate_xtndfonts xtndfont.c)
  if(BUILD_SHARED_LIBS)
    set_target_properties(generate_xtndfonts PROPERTIES
      COMPILE_DEFINITIONS "USINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  target_link_libraries(generate_xtndfonts hershey plplot)

endif(BUILD_HERSHEY_FONTS)
