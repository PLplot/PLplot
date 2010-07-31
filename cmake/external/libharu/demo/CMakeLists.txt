# demo/CMakeLists.txt
#
# Copyright (C) 2008  Werner Smekal
# Copyright (C) 2010  Alan W. Irwin
#
# create demo executables

if(LIBHARU_EXAMPLES)
  # =======================================================================
  # source file names and c flags
  # =======================================================================
  set(
    demos_NAMES
    character_map
    encoding_list
    encryption
    ext_gstate_demo
    font_demo
    image_demo
    jpeg_demo
    jpfont_demo
    line_demo
    link_annotation
    make_rawimage
    outline_demo
    #outline_demo_jp
    permission
    png_demo
    slide_show_demo
    text_annotation
    ttfont_demo
    ttfont_demo_jp
    )

  set(
    demos_with_grid_NAMES
    arc_demo
    chfont_demo
    raw_image_demo
    text_demo
    text_demo2
    )

  # we need to add some definitions for win 32
  if(WIN32 AND NOT CYGWIN)
    add_definitions(-D__WIN32__)
  endif(WIN32 AND NOT CYGWIN)

  # link the examples to the shared library otherwise to the static
  if(LIBHARU_SHARED)
    set(_LIBHARU_LIB hpdf)
  else(LIBHARU_SHARED)
    set(_LIBHARU_LIB hpdf_static)
  endif(LIBHARU_SHARED)
  
  # =======================================================================
  # create demos
  # =======================================================================
  foreach(demo ${demos_NAMES})
    add_executable(${demo} ${demo}.c)
    target_link_libraries(${demo} ${_LIBHARU_LIB})
    if(USE_RPATH)
      set_target_properties(
	${demo}
	PROPERTIES 
	INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
	INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	)
    else(USE_RPATH)
      set_target_properties(
	${demo}
	PROPERTIES 
	INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	)
    endif(USE_RPATH)
  endforeach(demo)
  
  # some demos need grid_sheet.c compiled in
  foreach(demo ${demos_with_grid_NAMES})
    add_executable(${demo} ${demo}.c grid_sheet.c)
    target_link_libraries(${demo} ${_LIBHARU_LIB})
    if(USE_RPATH)
      set_target_properties(
	${demo}
	PROPERTIES 
	INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
	INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	)
    else(USE_RPATH)
      set_target_properties(
	${demo}
	PROPERTIES 
	INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	)
    endif(USE_RPATH)
  endforeach(demo)
  
  add_executable(grid_sheet grid_sheet.c)
  # the grid_sheet demo needs extra define, but cannot use add_definitions
  # for that because that command affects all source files in the directory.
  # So use set_target_properties instead.
  set_target_properties(grid_sheet PROPERTIES COMPILE_FLAGS "-DSTAND_ALONE")
  target_link_libraries(grid_sheet ${_LIBHARU_LIB})
  if(USE_RPATH)
    set_target_properties(
      grid_sheet
      PROPERTIES 
      INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
      )
  else(USE_RPATH)
    set_target_properties(
      grid_sheet
      PROPERTIES 
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
      )
  endif(USE_RPATH)

  # =======================================================================
  # install demo source, demo executables and other files
  # =======================================================================

  # comment out the rest of this until have better install tree
  # location for these files.  For example, the current location makes
  # a real mess on Unix.  In any case, demo executables should not be
  # installed.  Instead, an independent demo build and test system
  # (say, CMake-based as for PLplot) would be nice to implement for
  # the installed demos.

  #  install(
  #    TARGETS ${demos_NAMES} ${demos_with_grid_NAMES} grid_sheet
  #    DESTINATION demo
  #  )

  #  foreach(demo ${demos_NAMES} ${demos_with_grid_NAMES} grid_sheet)
  #    install(FILES ${demo}.c DESTINATION demo)
  #  endforeach(demo)

  # install demo files
  #  install(
  #    DIRECTORY images mbtext pngsuite rawimage ttfont type1
  #    DESTINATION demo
  #  )
endif(LIBHARU_EXAMPLES)
