

MACRO(CheckDTD VARIABLE title docbookb_ss_dtd style_spec_use
	external_specification docbook_dtd jade_output_type
	origin_package)
IF (NOT DEFINED ${VARIABLE})
  SET(CONFTEST "testdtd")
   FILE(WRITE ${CMAKE_BINARY_DIR}/${CONFTEST}.dsl 
     "<!DOCTYPE style-sheet PUBLIC \"${DSSSL_DTD_PUBID}\" ${docbookb_ss_dtd}>
   <style-sheet>
   <style-specification ${style_spec_use}>
   <style-specification-body>
   (define %root-filename% \"${CONFTEST}\")
   </style-specification-body>
   </style-specification>
   ${external_specification}
   </style-sheet>"
   )
 FILE(WRITE ${CMAKE_BINARY_DIR}/${CONFTEST}.xml
   "<?xml version=\"1.0\"?>
   <!DOCTYPE book ${docbook_dtd}>
   <book> </book>"
   )
 EXECUTE_PROCESS( COMMAND ${OPENJADE} ${SGML_CATALOGS} -d ${CMAKE_BINARY_DIR}/${CONFTEST}.dsl -t ${jade_output_type} -o ${CMAKE_BINARY_DIR}/${CONFTEST}.out ${XML_DECL} ${CMAKE_BINARY_DIR}/${CONFTEST}.xml 
    OUTPUT_FILE "${JADELOG}.x" 
    ERROR_FILE "${JADELOG}.x"
    )
  FILE(READ "${CMAKE_BINARY_DIR}/${JADELOG}.x" RESULT)
  IF ("${RESULT}" MATCHES ":E:") 
    SET(${VARIABLE} OFF CACHE BOOL "Checking for ${title}" FORCE)
    MESSAGE("${title} not found")
  ELSE ("${RESULT}" MATCHES ":E:") 
    SET(${VARIABLE} ON CACHE BOOL "Checking for ${title}" FORCE)
    MESSAGE(STATUS "${title} found")
  ENDIF ("${RESULT}" MATCHES ":E:") 
ENDIF (NOT DEFINED ${VARIABLE})
ENDMACRO(CheckDTD)
