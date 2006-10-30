# doc/docbook/src/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software

set(MASTER_XML plplotdoc.xml)
set(MASTER_XML_IN ${MASTER_XML}.in)

set(BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/${MASTER_XML_IN}
${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML}
)

set(
SOURCE_FILES
advanced.xml
api.xml
api-c.xml
api-compat.xml
api-fortran77.xml
api-fortran95.xml
api-obsolete.xml
bibliography.xml
c.xml
deploying.xml
fortran77.xml
fortran95.xml
cplus.xml
drivers.xml
gui.xml
inline-html.ent
inline-print.ent
intro.xml
os_notes.xml
perl.xml
python.xml
simple.xml
tcl.xml
wish.xml
math.ent
)

set(
INCLUDED_FILES
${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt
${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt
)

set(HTML_TARBALL ${BASE_HTML}.tar.gz)
set(INFO_TARBALL ${BASE_INFO}.tar.gz)
set(MAN_TARBALL ${BASE_MAN}.tar.gz)
set(INDEX_HTML ../www/index.html)

# Create empty file so that appends below will work.
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/static_built_files "")

# The following files are included in plplotdoc.xml and are
# required for validation
if(PERL_FOUND)
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt
  COMMAND ${PERL_EXECUTABLE}
  ARGS ${BINDIR}/get-library-symbols.pl > ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt ${CMAKE_BINARY_DIR}/src/libplplot${LIB_TAG}.so ${CMAKE_SOURCE_DIR}/include/plplot.h 3
  DEPENDS ${CMAKE_SOURCE_DIR}/include/plplot.h
  )
  
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt
  COMMAND ${PERL_EXECUTABLE}
  ARGS ${BINDIR}/get-library-structs.pl > ${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt ${CMAKE_SOURCE_DIR}/include/plplot.h
  DEPENDS ${CMAKE_SOURCE_DIR}/include/plplot.h
  )
  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
  "plplot-symbols.txt\n"
  "plplot-structs.txt\n"
  )
endif(PERL_FOUND)
  
# Custom command for validating xml file
if(PERL_FOUND AND ONSGMLS)

  set(VALIDATE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/validate.out)

  add_custom_target(validate
  ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${CMAKE_COMMAND} -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}/inline-html.ent
  ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${ONSGMLS}  ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml > ${VALIDATE_OUTPUT}
  DEPENDS 
  ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml
  ${INCLUDED_FILES}
  )
else(PERL_FOUND AND ONSGMLS)
  add_custom_target(validate
  ${CMAKE COMMAND} -E echo "Unable to validate xml file - missing dependencies"
  )
endif(PERL_FOUND AND ONSGMLS)

if (BUILD_DOC)

  if(NOT CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    foreach(file ${SOURCE_FILES})
      if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${file})
        exec_program(
	${CMAKE_COMMAND}
	ARGS -E create_symlink 
	${CMAKE_CURRENT_SOURCE_DIR}/${file}
	${CMAKE_CURRENT_BINARY_DIR}/${file}
	)
      endif(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${file})
    endforeach(file ${SOURCE_FILES})
  endif(NOT CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")

  set(STYLESHEET_HTML plplotdoc-html.dsl)
  set(STYLESHEET_PRINT plplotdoc-print.dsl)
  set(STYLESHEET_HTML_IN ${STYLESHEET_HTML}.in)
  set(STYLESHEET_PRINT_IN ${STYLESHEET_PRINT}.in)

  configure_file(${STYLESHEET_HTML_IN} ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_HTML})
  configure_file(${STYLESHEET_PRINT_IN} ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT})

  # Custom commands for building print version of documentation
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E remove ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}/inline-print.ent
  ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${OPENJADE} 
  ARGS ${SGML_CATALOGS} -d ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT} -t tex -o ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML}
  )

  # The following trick with tex auxillary file ensures that
  # jadetex is run 3 times to correctly generated the documentation with
  # references. Taken from the CMake FAQ.
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.aux
  COMMAND ${PDFJADETEX} 
  ARGS ${BASE}.jadetex 
  DEPENDS 
  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex 
  jadetex.cfg 
  ${INCLUDED_FILES}
  )

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.log
  COMMAND ${PDFJADETEX} 
  ARGS ${BASE}.jadetex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.aux 
  )

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
  COMMAND ${PDFJADETEX} 
  ARGS ${BASE}.jadetex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.log 
  )

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
  COMMAND ${JADETEX} 
  ARGS ${BASE}.jadetex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.log 
  )
  
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
  COMMAND ${DVIPS} 
  ARGS ${BASE}.dvi
  COMMAND ${GZIP}
  ARGS -f ${BASE}.ps
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi 
  )

  if(BUILD_PRINT)
    add_custom_target(
    print ALL 
    DEPENDS 
    ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
    )
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "${BASE}.jadetex\n"
    "${BASE}.dvi\n"
    "${BASE}.ps.gz\n"
    "${BASE}.pdf\n"
    )
  endif(BUILD_PRINT)

  # Custom commands for building info version of documentation
  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml-info
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E remove ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}/inline-info.ent
  ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${PERL_EXECUTABLE} 
  ARGS ${BINDIR}/info-clean.pl ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML} > ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml-info
  DEPENDS
  ${SOURCE_FILES} 
  ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML} 
  ${INCLUDED_FILES}
  inline-info.ent
  )

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.txml
  COMMAND ${DB2X_XSLTPROC}
  ARGS -s texi -o ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.txml ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml-info
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.xml-info
  )

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.texi
  COMMAND ${DB2X_TEXIXML}
  ARGS ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.txml
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.txml
  )

  set(INFO_TMPDIR ${CMAKE_CURRENT_BINARY_DIR}/info)

  file(MAKE_DIRECTORY ${INFO_TMPDIR})

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
  COMMAND ${CMAKE_COMMAND}
  ARGS -E remove -f ${INFO_TMPDIR}/*
  COMMAND ${MAKEINFO} 
  ARGS --force -o ${INFO_TMPDIR}/plplotdoc.info ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.texi
  COMMAND ${CMAKE_COMMAND}
  ARGS -E chdir ${INFO_TMPDIR} ls |cat > ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
  COMMAND ${MV}
  ARGS ${INFO_TMPDIR}/* ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc.texi
  )

  if(BUILD_INFO)
    add_custom_target(
    info ALL 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "plplotdoc.xml-info\n"
    "plplotdoc.txml\n"
    "plplotdoc.texi\n"
    "${INFO_MANIFEST}\n"
    )
  endif(BUILD_INFO)

  # Custom commands for building man version of documentation

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
  COMMAND ${PERL_EXECUTABLE}
  ARGS ${CMAKE_CURRENT_BINARY_DIR}/../bin/api2man.pl ${MASTER_XML} api.xml ${MANVOL} > ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML} api.xml
  )

  if(BUILD_MAN)
    add_custom_target(
    man ALL 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "${MAN_MANIFEST}\n"
    )
  endif(BUILD_MAN)

  # Custom commands for building html version of documentation

  add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E remove ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${CMAKE_COMMAND} 
  ARGS -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}/inline-html.ent 
  ${CMAKE_CURRENT_BINARY_DIR}/inline.ent
  COMMAND ${OPENJADE}
  ARGS ${SGML_CATALOGS} -d ${STYLESHEET_HTML} -t sgml ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML}
  COMMAND ${PERL_EXECUTABLE}
  ARGS -pi -e 's/&\#13;//g' `cat ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}`
  DEPENDS
  ${SOURCE_FILES} 
  ${CMAKE_CURRENT_BINARY_DIR}/${MASTER_XML}
  ${INCLUDED_FILES}
  inline-html.ent
  )

  if(BUILD_HTML)
    add_custom_target(
    html ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "${HTML_MANIFEST}\n"
    )
  endif(BUILD_HTML)

endif(BUILD_DOC)

########### install files ###############

# We use file(GLOB ...) to generate lists of html, info and man files 
# to install. These need to be generated at install time, not at 
# Makefile creation time and so we also need to use the install(CODE ...)
# command. This makes for slightly ugly looking code. 

if(BUILD_HTML OR PREBUILT_DOC)
  install(CODE "file(GLOB HTML_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.${HTML_EXT})\n foreach(file \${HTML_FILES}) \n file(INSTALL DESTINATION ${DOC_DIR}/html TYPE FILE FILES \"\${file}\") \n endforeach (file \"\${HTML_FILES}\")")
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST} stylesheet.css DESTINATION ${DOC_DIR}/html )
endif(BUILD_HTML OR PREBUILT_DOC)

if(BUILD_PRINT OR PREBUILT_DOC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz DESTINATION ${DOC_DIR})
endif(BUILD_PRINT OR PREBUILT_DOC)

if(BUILD_MAN OR PREBUILT_DOC)
  install(CODE "file(GLOB MAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.${MANVOL})\n foreach(file \${MAN_FILES}) \n file(INSTALL DESTINATION ${MAN_DIR}/man3 TYPE FILE FILES \"\${file}\") \n endforeach (file \"\${MAN_FILES}\")")
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST} DESTINATION ${MAN_DIR}/man3 )
endif(BUILD_MAN OR PREBUILT_DOC)

if(BUILD_INFO OR PREBUILT_DOC)
  install(CODE "file(GLOB INFO_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.info*)\n foreach(file \${INFO_FILES}) \n file(INSTALL DESTINATION ${INFO_DIR} TYPE FILE FILES \"\${file}\") \n endforeach (file \"\${INFO_FILES}\")")
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST} DESTINATION ${INFO_DIR} )
endif(BUILD_INFO OR PREBUILT_DOC)
