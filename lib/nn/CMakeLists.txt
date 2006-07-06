set(csironn_LIB_SRCS
    delaunay.c
    hash.c
    istack.c
    lpi.c
    nnai.c
    nnpi.c
    nncommon.c
   )
add_library(csironn ${csironn_LIB_SRCS})
SET_TARGET_PROPERTIES(csironn PROPERTIES SOVERSION 0 VERSION 0.0.1)
install(TARGETS csironn  DESTINATION ${LIB_INSTALL_DIR})
install(FILES README DESTINATION ${DOC_INSTALL_DIR} RENAME README.nn)
install(FILES README.1st DESTINATION ${DOC_INSTALL_DIR} RENAME README.1st.nn)
