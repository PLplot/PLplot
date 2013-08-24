# doc/docbook/src/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
# Copyright (C) 2006 Alan W. Irwin
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

set(BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(PLPLOTDOC plplotdoc)
if(NOT DEFINED HTML_EXT)
  set(HTML_EXT "html")
endif(NOT DEFINED HTML_EXT)
set(MANVOL "3plplot")

foreach(suffix info html print)
  if(suffix STREQUAL "print" AND DOCBOOK_XML_BACKEND)
    set(inline_entities ${CMAKE_CURRENT_SOURCE_DIR}/inline-${suffix}_xsl.ent)
  elseif(suffix STREQUAL "print" AND NOT DOCBOOK_XML_BACKEND)
    set(inline_entities ${CMAKE_CURRENT_SOURCE_DIR}/inline-${suffix}_dsl.ent)
  else(suffix STREQUAL "print" AND DOCBOOK_XML_BACKEND)
    set(inline_entities ${CMAKE_CURRENT_SOURCE_DIR}/inline-${suffix}.ent)
  endif(suffix STREQUAL "print" AND DOCBOOK_XML_BACKEND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLPLOTDOC}.xml.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-${suffix}.xml
    )
endforeach(suffix info html print)

set(
  SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/ada.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/advanced.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-c.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-compat.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-fortran77.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-fortran95.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-internal.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/api-obsolete.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/bibliography.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/c.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/deploying.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/fortran77.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/fortran95.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/cplus.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/drivers.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/gui.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/intro.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/ocaml.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/os_notes.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/perl.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/python.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/simple.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/tcl.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/wish.xml
  ${CMAKE_CURRENT_SOURCE_DIR}/math.ent
  )

execute_process(
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different 
  ${CMAKE_CURRENT_SOURCE_DIR}/jadetex.cfg
  ${CMAKE_CURRENT_BINARY_DIR}/jadetex.cfg
  )

if(
    PREBUILT_DOC
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
    )

  # Copy pre-built documentation (whose filenames are contained in the
  # files "static_built_files", ${INFO_MANIFEST}, ${MAN_MANIFEST}, and
  # ${HTML_MANIFEST}) in the source tree to the build tree just as if
  # it had been created there by the alternative BUILD_DOC=ON case.
  if(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    add_custom_target(copy_prebuiltdoc ALL)
  else(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
      COMMAND cp
      `cat static_built_files ${INFO_MANIFEST} ${MAN_MANIFEST} ${HTML_MANIFEST}`
      ${CMAKE_BINARY_DIR}/doc/docbook/src
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/docbook/src
      )
    add_custom_target(
      copy_prebuiltdoc ALL
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
      )
  endif(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
  
  if(ONSGMLS)
    set(VALIDATE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/validate.out)

    # Custom target for validating xml file
    add_custom_target(validate
      COMMAND ${ONSGMLS} ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml > ${VALIDATE_OUTPUT}
      )
    add_dependencies(validate copy_prebuiltdoc)
  endif(ONSGMLS)
else(
    PREBUILT_DOC
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
    )

  # Include this file in what is copied back from build tree to source tree
  # for the custom top-level prebuild_dist target.
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/static_built_files 
    "static_built_files\n"
    )

  # The following files are defined as entities in the various forms
  # of the configured ${PLPLOTDOC}-*.xml files and are
  # used to help build the documentation.  Of course, these files are
  # required for validation as well.
  # N.B. if PERL_FOUND is false, then BUILD_DOC is false.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt
    COMMAND ${PERL_EXECUTABLE}
    ${BINDIR}/get-library-symbols.pl > ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt ${CMAKE_BINARY_DIR}/src/libplplot${LIB_TAG}.so ${CMAKE_SOURCE_DIR}/include/plplot.h 3
    DEPENDS ${CMAKE_SOURCE_DIR}/include/plplot.h
    )
  # For parallel builds to work must have all file dependency chains of
  # targets independent of each other.  Thus, since several targets
  # need plplot-symbols.txt, must create the target 
  # docbook_plplot-symbols_txt and have the targets depend on that target
  # rather than have each target file depend directly on 
  # ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt.
  add_custom_target(
    docbook_plplot-symbols_txt
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-symbols.txt
    )
  # libplplot should be created before plplot-symbols.txt since
  # get-library-symbols.pl depends upon libplplot.
  add_dependencies(docbook_plplot-symbols_txt plplot${LIB_TAG})
  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "plplot-symbols.txt\n"
    )

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt
    COMMAND ${PERL_EXECUTABLE}
    ${BINDIR}/get-library-structs.pl > ${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt ${CMAKE_SOURCE_DIR}/include/plplot.h
    DEPENDS ${CMAKE_SOURCE_DIR}/include/plplot.h
    )
  # For parallel builds to work must have all file dependency chains of
  # targets independent of each other.  Thus, since several targets
  # need plplot-structs.txt, must use target dependencies rather than
  # file dependencies to build this file.
  add_custom_target(
    docbook_plplot-structs_txt
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-structs.txt
    )
  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
    "plplot-structs.txt\n"
    )
  
  # Custom command for validating xml file
  # This one different from alternative above used in PREBUILT_DOC case
  # because this one has dependencies which have to be built first.
  if(ONSGMLS)

    set(VALIDATE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/validate.out)

    add_custom_target(validate
      COMMAND ${ONSGMLS} ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml > ${VALIDATE_OUTPUT}
      )
    add_dependencies(validate
      docbook_plplot-symbols_txt
      docbook_plplot-structs_txt
      )
  else(ONSGMLS)
    add_custom_target(validate
      ${CMAKE_COMMAND} -E echo "Unable to validate xml file - missing dependencies"
      )
  endif(ONSGMLS)

  # N.B. if PERL_FOUND is false, then BUILD_DOC is false.
  if(BUILD_DOC)

    if(BUILD_INFO)
      # Custom commands for building info version of documentation
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-clean.xml
	COMMAND ${PERL_EXECUTABLE} 
	${BINDIR}/info-clean.pl ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-info.xml > ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-clean.xml
	DEPENDS
	${SOURCE_FILES} 
	${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-info.xml
	${CMAKE_CURRENT_SOURCE_DIR}/inline-info.ent
	)

      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.txml
	COMMAND ${CMAKE_COMMAND} -E echo "Note that no information is lost when \"too deep\" warning messages indicate"
	COMMAND ${CMAKE_COMMAND} -E echo "where flat organization has replaced hierarchical organization."
	COMMAND ${DB2X_XSLTPROC}
	-s texi -o ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.txml ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-clean.xml
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-clean.xml
	VERBATIM
	)

      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi
	COMMAND ${DB2X_TEXIXML} --encoding=utf-8 ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.txml
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi_sed
	COMMAND sed "s?The PLplot Plotting Library: (${PLPLOTDOC})?PLplot: (${PLPLOTDOC})?" < ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi > ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi_sed
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi
	COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi_sed ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.txml
	VERBATIM
	)

      set(INFO_TMPDIR ${CMAKE_CURRENT_BINARY_DIR}/info)

      file(MAKE_DIRECTORY ${INFO_TMPDIR})

      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
	COMMAND ${CMAKE_COMMAND}
	-E remove -f ${INFO_TMPDIR}/*
	COMMAND ${MAKEINFO} 
	--force -o ${INFO_TMPDIR}/${PLPLOTDOC}.info ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi
	COMMAND ${CMAKE_COMMAND}
	-E chdir ${INFO_TMPDIR} ls |cat > ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
	COMMAND mv
	${INFO_TMPDIR}/* ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND rmdir ${INFO_TMPDIR}
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}.texi
	)

      add_custom_target(
	info ALL 
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
	)
      # These files used for more than one target.  Therefore must
      # use target dependencies rather than file dependencies to build
      # them.  Otherwise, parallel builds won't work.
      add_dependencies(info
	docbook_plplot-symbols_txt
	docbook_plplot-structs_txt
	)
    else(BUILD_INFO)
      add_custom_target(
	info ALL
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
	COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST}
	)
    endif(BUILD_INFO)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
      "${INFO_MANIFEST}\n"
      )

    if(BUILD_MAN)
      # Custom commands for building man version of documentation

      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
	COMMAND ${PERL_EXECUTABLE}
	${CMAKE_CURRENT_BINARY_DIR}/../bin/api2man.pl ${PLPLOTDOC}-info.xml ${CMAKE_CURRENT_SOURCE_DIR}/api.xml ${MANVOL} > ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
	DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-info.xml
	${CMAKE_CURRENT_SOURCE_DIR}/inline-info.ent
	${CMAKE_CURRENT_SOURCE_DIR}/api.xml
	)

      add_custom_target(
	man ALL 
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
	)
    else(BUILD_MAN)
      add_custom_target(
	man ALL
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
	COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST}
	)
    endif(BUILD_MAN)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
      "${MAN_MANIFEST}\n"
      )

    if(DOCBOOK_XML_BACKEND)
      if(BUILD_HTML)
	# Custom commands for building html version of documentation
	set(STYLESHEET_HTML ${PLPLOTDOC}-html.xsl)
	set(STYLESHEET_HTML_IN ${STYLESHEET_HTML}.in)
	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/${STYLESHEET_HTML_IN}
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_HTML}
	  @ONLY
	  )

	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/stylesheet.css.xsl.in
	  ${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css
	  )

	set(HTML_TMPDIR ${CMAKE_CURRENT_BINARY_DIR}/html)
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  COMMAND ${CMAKE_COMMAND} -E remove_directory ${HTML_TMPDIR}
	  COMMAND ${XMLTO} -vv -x ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_HTML} -o ${HTML_TMPDIR} html ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml
	  COMMAND ${CMAKE_COMMAND}
	  -E chdir ${HTML_TMPDIR} ls |cat > ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  COMMAND mv ${HTML_TMPDIR}/* ${CMAKE_CURRENT_BINARY_DIR}
	  COMMAND ${CMAKE_COMMAND} -E remove_directory ${HTML_TMPDIR}
	  DEPENDS
	  ${SOURCE_FILES} 
	  ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml
	  ${CMAKE_CURRENT_SOURCE_DIR}/inline-html.ent
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_HTML}
	  )
	add_custom_target(
	  html ALL
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(html
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )
      else(BUILD_HTML)
	add_custom_target(
	  html ALL
	  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  )
      endif(BUILD_HTML)
      file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	"${HTML_MANIFEST}\n"
	)

      if(BUILD_PRINT)
	# Custom commands for building html version of documentation
	set(STYLESHEET_PRINT ${PLPLOTDOC}-print.xsl)
	set(STYLESHEET_PRINT_IN ${STYLESHEET_PRINT}.in)
	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/${STYLESHEET_PRINT_IN}
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT}
	  @ONLY
	  )

	if(BUILD_DVI)
	  add_custom_command(
	    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
	    # Drop -x ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT} option
            # since it doesn't appear to work with --with-dblatex
	    COMMAND ${XMLTO} -vv --with-dblatex dvi ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc-print.dvi ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
	    DEPENDS
	    ${SOURCE_FILES} 
	    ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	    ${CMAKE_CURRENT_SOURCE_DIR}/inline-print_xsl.ent
	    )
	  add_custom_target(
	    dvi_target ALL 
	    DEPENDS 
	    ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
	    )
	  # These files used for more than one target.  Therefore must
	  # use target dependencies rather than file dependencies to build
	  # them.  Otherwise, parallel builds won't work.
	  add_dependencies(dvi_target
	    docbook_plplot-symbols_txt
	    docbook_plplot-structs_txt
	    )

	  file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	    "${BASE}.ps.gz\n"
	    )

	endif(BUILD_DVI)

	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
	  COMMAND ${XMLTO} -vv --with-fop -x ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT} ps ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc-print.ps ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps
	  COMMAND ${GZIP} -f ${BASE}.ps
	  DEPENDS
	  ${SOURCE_FILES} 
	  ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  ${CMAKE_CURRENT_SOURCE_DIR}/inline-print_xsl.ent
	  )
	add_custom_target(
	  ps_target ALL 
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(ps_target
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	  "${BASE}.ps.gz\n"
	  )

	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
	  COMMAND ${XMLTO} -vv --with-fop -x ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT} pdf ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/plplotdoc-print.pdf ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
	  DEPENDS
	  ${SOURCE_FILES} 
	  ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  ${CMAKE_CURRENT_SOURCE_DIR}/inline-print_xsl.ent
	  )
	add_custom_target(
	  pdf_target ALL 
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(pdf_target
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	  "${BASE}.pdf\n"
	  )
	add_custom_target(
	  print ALL 
	  )
	add_dependencies(print ps_target pdf_target)
	if(BUILD_DVI)
	  add_dependencies(print dvi_target)
	endif(BUILD_DVI)
      endif(BUILD_PRINT)
    else(DOCBOOK_XML_BACKEND)
      if(BUILD_HTML)
	# Custom commands for building html version of documentation
	set(STYLESHEET_HTML ${PLPLOTDOC}-html.dsl)
	set(STYLESHEET_HTML_IN ${STYLESHEET_HTML}.in)
	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/${STYLESHEET_HTML_IN}
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_HTML}
	  )

	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/stylesheet.css.dsssl.in
	  ${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css
	  )

	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  COMMAND ${OPENJADE} -b utf-8
	  ${SGML_CATALOGS} -d ${STYLESHEET_HTML} -t sgml ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml
	  COMMAND ${PERL_EXECUTABLE}
	  -pi -e 's/&\#13;//g' `cat ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}`
	  DEPENDS
	  ${SOURCE_FILES} 
	  ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-html.xml
	  ${CMAKE_CURRENT_SOURCE_DIR}/inline-html.ent
	  )

	add_custom_target(
	  html ALL
	  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(html
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )
      else(BUILD_HTML)
	add_custom_target(
	  html ALL
	  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST}
	  )
      endif(BUILD_HTML)
      file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	"${HTML_MANIFEST}\n"
	)

      if(BUILD_PRINT)
	set(STYLESHEET_PRINT ${PLPLOTDOC}-print.dsl)
	set(STYLESHEET_PRINT_IN ${STYLESHEET_PRINT}.in)

	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/${STYLESHEET_PRINT_IN}
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT}
	  )

	configure_file(
	  ${CMAKE_CURRENT_SOURCE_DIR}/pdftex.map.in
	  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.map
	  )

	# Custom commands for building print version of documentation
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex
	  COMMAND ${OPENJADE} -b utf-8
	  ${SGML_CATALOGS} -d ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT} 
	  -t tex -o ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex 
	  ${XML_DECL} ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  DEPENDS
	  ${SOURCE_FILES}
	  ${CMAKE_CURRENT_BINARY_DIR}/${PLPLOTDOC}-print.xml
	  ${CMAKE_CURRENT_BINARY_DIR}/${STYLESHEET_PRINT}
	  ${CMAKE_CURRENT_SOURCE_DIR}/inline-print_xsl.ent
	  )

	# Following approach used in our ABS, use shell commands to recurse
	# ${PDFJADETEX} ${BASE}.jadetex five times or until the *.aux file that is
	# produced doesn't change.
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
	  COMMAND ${PDFJADETEX} ${BASE}.jadetex
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-pdf.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-pdf.aux \; 
	  ${PDFJADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-pdf.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-pdf.aux \; 
	  ${PDFJADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-pdf.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-pdf.aux \; 
	  ${PDFJADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-pdf.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-pdf.aux \; 
	  ${PDFJADETEX} ${BASE}.jadetex \; 
	  fi
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex 
	  ${CMAKE_CURRENT_SOURCE_DIR}/jadetex.cfg 
	  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.map
	  )

	# Following approach used in our ABS, use shell commands to recurse
	# ${JADETEX} ${BASE}.jadetex five times or until the *.aux file that is
	# produced doesn't change.
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
	  COMMAND ${JADETEX} ${BASE}.jadetex
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-dvi.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-dvi.aux \; 
	  ${JADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-dvi.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-dvi.aux \; 
	  ${JADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-dvi.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-dvi.aux \; 
	  ${JADETEX} ${BASE}.jadetex \; 
	  fi
	  COMMAND 
	  if ! cmp ${BASE}.aux prior-dvi.aux 2>/dev/null \; then 
	  cp -pf ${BASE}.aux prior-dvi.aux \; 
	  ${JADETEX} ${BASE}.jadetex \; 
	  fi
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.jadetex 
	  ${CMAKE_CURRENT_SOURCE_DIR}/jadetex.cfg 
	  ${CMAKE_CURRENT_BINARY_DIR}/pdftex.map
	  )

	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
	  COMMAND ${DVIPS} ${BASE}.dvi
	  COMMAND ${GZIP} -f ${BASE}.ps
	  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi 
	  )

	add_custom_target(
	  dvi_target ALL 
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(dvi_target
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	  "${BASE}.dvi\n"
	  )
	add_custom_target(
	  ps_target ALL 
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(ps_target
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )

	# dvi_target must complete before ps_target is executed for parallel builds.
	add_dependencies(ps_target dvi_target)

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	  "${BASE}.ps.gz\n"
	  )
	add_custom_target(
	  pdf_target ALL 
	  DEPENDS 
	  ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf
	  )
	# These files used for more than one target.  Therefore must
	# use target dependencies rather than file dependencies to build
	# them.  Otherwise, parallel builds won't work.
	add_dependencies(pdf_target
	  docbook_plplot-symbols_txt
	  docbook_plplot-structs_txt
	  )

	# dvi_target must complete before pdf_target is executed for parallel builds.
	add_dependencies(pdf_target dvi_target)

	file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/static_built_files
	  "${BASE}.pdf\n"
	  )
	add_custom_target(
	  print ALL 
	  )
	add_dependencies(print dvi_target ps_target pdf_target)

      endif(BUILD_PRINT)
    endif(DOCBOOK_XML_BACKEND)
  endif(BUILD_DOC)
endif(
  PREBUILT_DOC
  AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
  AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
  AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
  AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
  )

########### install files ###############
# Where appropriate, use contents of manifest files rather than
# file(GLOB... for best consistency.

if(BUILD_INFO OR PREBUILT_DOC)
  install(CODE "
# Suppress empty element warning.
cmake_policy(SET CMP0007 NEW)
file(READ ${CMAKE_CURRENT_BINARY_DIR}/${INFO_MANIFEST} INFO_FILES)
# Convert to list with trailing empty element
string(REGEX REPLACE \"\n\" \";\" INFO_FILES \"\${INFO_FILES}\")
# Delete trailing empty element
list(REMOVE_ITEM INFO_FILES \"\")
foreach(file \${INFO_FILES})
  # INSTALL_DESTINATION signature automatically honors DESTDIR.
  file(INSTALL DESTINATION ${INFO_DIR} TYPE FILE FILES \"\${file}\")
endforeach (file \"\${INFO_FILES}\")
"
    )
endif(BUILD_INFO OR PREBUILT_DOC)

if(BUILD_MAN OR PREBUILT_DOC)
  install(CODE "
# Suppress empty element warning.
cmake_policy(SET CMP0007 NEW)
file(READ ${CMAKE_CURRENT_BINARY_DIR}/${MAN_MANIFEST} MAN_FILES)
# Convert to list with trailing empty element
string(REGEX REPLACE \"\n\" \";\" MAN_FILES \"\${MAN_FILES}\")
# Delete trailing empty element
list(REMOVE_ITEM MAN_FILES \"\")
foreach(file \${MAN_FILES})
  # INSTALL_DESTINATION signature automatically honors DESTDIR.
  file(INSTALL DESTINATION ${MAN_DIR}/man3 TYPE FILE FILES \"\${file}\")
endforeach (file \"\${MAN_FILES}\")
"
    )
endif(BUILD_MAN OR PREBUILT_DOC)

if(BUILD_HTML OR PREBUILT_DOC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST} ${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css DESTINATION ${DOC_DIR}/html )
  install(CODE "
# Suppress empty element warning.
cmake_policy(SET CMP0007 NEW)
file(READ ${CMAKE_CURRENT_BINARY_DIR}/${HTML_MANIFEST} HTML_FILES)
# Convert to list with trailing empty element
string(REGEX REPLACE \"\n\" \";\" HTML_FILES \"\${HTML_FILES}\")
# Delete trailing empty element
list(REMOVE_ITEM HTML_FILES \"\")
foreach(file \${HTML_FILES})
  # INSTALL_DESTINATION signature automatically honors DESTDIR.
  file(INSTALL DESTINATION ${DOC_DIR}/html TYPE FILE FILES \"\${file}\")
endforeach (file \"\${HTML_FILES}\")
"
    )
endif(BUILD_HTML OR PREBUILT_DOC)

if(BUILD_PRINT OR PREBUILT_DOC)
  if(BUILD_DVI)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.dvi DESTINATION ${DOC_DIR})
  endif(BUILD_DVI)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.pdf ${CMAKE_CURRENT_BINARY_DIR}/${BASE}.ps.gz DESTINATION ${DOC_DIR})
endif(BUILD_PRINT OR PREBUILT_DOC)

# Everything below here has to do with the custom target www-install
if((BUILD_DOC OR PREBUILT_DOC) AND (WWW_USER AND WWW_GROUP AND WWW_HOST AND WWW_DIR))

  set(TARBALL tarball.tgz)
  set(HTML_TARBALL ${BASE_HTML}.tar.gz)
  set(INFO_TARBALL ${BASE_INFO}.tar.gz)
  set(MAN_TARBALL ${BASE_MAN}.tar.gz)

  set(WWW_USERAT ${WWW_USER}@)
  set(RCP_PREFIX ${WWW_USERAT}${WWW_HOST}:)
  set(RSH_COMMAND ${RSH} ${WWW_USERAT}${WWW_HOST})

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${INFO_TARBALL}
    COMMAND rm -rf ${BASE_INFO}
    COMMAND mkdir ${BASE_INFO}
    COMMAND cp `cat ${INFO_MANIFEST}` ${BASE_INFO}
    COMMAND tar cfz ${INFO_TARBALL} ${BASE_INFO}
    COMMAND rm -rf ${BASE_INFO}
    DEPENDS ${INFO_MANIFEST}
    )

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MAN_TARBALL}
    COMMAND rm -rf ${BASE_MAN}
    COMMAND mkdir ${BASE_MAN}
    COMMAND cp `cat ${MAN_MANIFEST}` ${BASE_MAN}
    COMMAND tar cfz ${MAN_TARBALL} ${BASE_MAN}
    COMMAND rm -rf ${BASE_MAN}
    DEPENDS ${MAN_MANIFEST}
    )

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${HTML_TARBALL}
    COMMAND rm -rf ${BASE_HTML}
    COMMAND mkdir ${BASE_HTML}
    COMMAND cp `cat ${HTML_MANIFEST}` ${BASE_HTML}
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css ${BASE_HTML}
    COMMAND tar cfz ${HTML_TARBALL} ${BASE_HTML}
    COMMAND rm -rf ${BASE_HTML}
    DEPENDS
    ${HTML_MANIFEST}
    ${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css
    )

  set(tarball_list
    ${INFO_TARBALL}
    ${MAN_TARBALL}
    ${HTML_TARBALL}
    )

  set(tarball_depends_list
    ${CMAKE_CURRENT_BINARY_DIR}/${HTML_TARBALL}
    ${CMAKE_CURRENT_BINARY_DIR}/${INFO_TARBALL}
    ${CMAKE_CURRENT_BINARY_DIR}/${MAN_TARBALL}
    )

  if(BUILD_PRINT)
    list(APPEND tarball_list
      ${BASE}.ps.gz
      ${BASE}.pdf
      )
    list(APPEND tarball_depends_list
      ${BASE}.ps.gz
      ${BASE}.pdf
      )
    if(BUILD_DVI)
      list(APPEND tarball_list ${BASE}.dvi)
      list(APPEND tarball_depends_list ${BASE}.dvi)
    endif(BUILD_DVI)
  endif(BUILD_PRINT)

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARBALL}
    COMMAND tar cfz ${TARBALL}
    ${tarball_list}
    -C ${CMAKE_CURRENT_SOURCE_DIR}/.. COPYING
    DEPENDS
    ${tarball_depends_list}
    ${CMAKE_CURRENT_SOURCE_DIR}/../COPYING
    )

  add_custom_target(www-uninstall 
    COMMAND ${RSH_COMMAND} rm -rf ${WWW_DIR}/htdocs/docbook-manual
    )

  add_custom_target(
    www-install
    COMMAND ${RSH_COMMAND} mkdir -p ${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RCP} ${CMAKE_CURRENT_BINARY_DIR}/${TARBALL} ${RCP_PREFIX}${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RSH_COMMAND} tar xfz ${WWW_DIR}/htdocs/docbook-manual/${TARBALL} -C ${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RSH_COMMAND} tar xfz ${WWW_DIR}/htdocs/docbook-manual/${HTML_TARBALL} -C ${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RSH_COMMAND} chgrp -R ${WWW_GROUP} ${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RSH_COMMAND} chmod -R g=u ${WWW_DIR}/htdocs/docbook-manual
    COMMAND ${RSH_COMMAND} rm -f ${WWW_DIR}/htdocs/docbook-manual/${TARBALL}
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/${TARBALL}
    )

  if(
      PREBUILT_DOC
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
      )
    add_dependencies(www-install copy_prebuiltdoc www-uninstall)
  else(
      PREBUILT_DOC
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
      AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
      )
    add_dependencies(www-install www-uninstall)
  endif(
    PREBUILT_DOC
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/static_built_files
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INFO_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${MAN_MANIFEST}
    AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HTML_MANIFEST}
    )
endif((BUILD_DOC OR PREBUILT_DOC) AND (WWW_USER AND WWW_GROUP AND WWW_HOST AND WWW_DIR))
