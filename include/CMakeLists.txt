configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plConfig.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/plConfig.h
)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plDevs.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/plDevs.h
)

########### next target ###############

set(plhershey-unicode-gen_SRCS
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode-gen.c
)

add_executable(plhershey-unicode-gen ${plhershey-unicode-gen_SRCS})

add_custom_target(
plhershey-unicode.h ALL
./plhershey-unicode-gen
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
plhershey-unicode.h
DEPENDS
plhershey-unicode-gen
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
)

########### header files ###############

set(include_INSTALLED_HEADERS
disptab.h
drivers.h
pdf.h
pldebug.h
plevent.h
plplot.h
plplotP.h
plstrm.h
plxwd.h
gcw.h
plplotcanvas.h
plConfig.h
plDevs.h
)
install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${HEADER_INSTALL_DIR})
