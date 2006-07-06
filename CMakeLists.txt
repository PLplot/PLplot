project(PLPLOT)

#libraries are all shared for now.
set(BUILD_SHARED_LIBS ON)

#relative path of installed libraries.
set(LIB_INSTALL_DIR lib)

#relative path of installed documentation.
set(DOC_INSTALL_DIR share/doc/plplot)

add_subdirectory(lib)
