set(core_manpages plm2gif.1 plpr.1 pltek.1 pstex2eps.1)
set(tcl_manpages plrender.1 plserver.1 pltcl.1)

set(manpages ${core_manpages})
if (ENABLE_tcl)
  set(manpages ${manpages} ${tcl_manpages})
endif (ENABLE_tcl)

add_subdirectory( docbook ) 

install(FILES ${manpages} DESTINATION ${MAN_DIR}/man1)
