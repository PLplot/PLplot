set(core_manpages plm2gif.1 plpr.1 pltek.1)

if(PLD_pstex)
   set(core_manpages ${core_manpages} pstex2eps.1)
endif(PLD_pstex)

if(PLD_plmeta)
  set(core_manpages ${core_manpages} plrender.1)
endif(PLD_plmeta)

set(tcl_manpages plserver.1 pltcl.1)

set(manpages ${core_manpages})
if (ENABLE_tcl)
  set(manpages ${manpages} ${tcl_manpages})
endif (ENABLE_tcl)

add_subdirectory( docbook ) 

install(FILES ${manpages} DESTINATION ${MAN_DIR}/man1)
