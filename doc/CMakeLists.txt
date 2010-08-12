# Process documentation for the PLplot project.

# Process man pages in this subdirectory.

set(core_manpages pltek.1)

if(PLD_pstex)
  set(core_manpages ${core_manpages} pstex2eps.1)
endif(PLD_pstex)

if(PLD_plmeta)
  set(core_manpages ${core_manpages} plrender.1 plm2gif.1 plpr.1)
endif(PLD_plmeta)

set(tcl_manpages plserver.1 pltcl.1)

set(manpages ${core_manpages})
if (ENABLE_tcl)
  set(manpages ${manpages} ${tcl_manpages})
endif (ENABLE_tcl)

install(FILES ${manpages} DESTINATION ${MAN_DIR}/man1)

# Process the docbook documentation.
add_subdirectory(docbook) 

# Process the doxygen documentation.

if(BUILD_DOX_DOC)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
    )

  # Notes: OUTPUT file is a directory so this insures "make clean"
  # removes that directory.  This command depends on all sourcefiles,
  # but do not keep track of those dependencies.  Instead, use a
  # dependency on the Doxyfile configured above simply to make sure
  # this command is only run once (initially or after "make clean").
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/doxygen
    COMMAND ${DOXYGEN_EXECUTABLE}
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
  add_custom_target(doxygen_built ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen
    )
endif(BUILD_DOX_DOC)
