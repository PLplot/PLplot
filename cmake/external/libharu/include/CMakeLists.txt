# include/CMakeLists.txt for libharu.
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2010 Alan W. Irwin

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/hpdf_config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/hpdf_config.h
  )

set(include_INSTALLED_HEADERS
  hpdf.h
  hpdf_annotation.h
  hpdf_catalog.h
  hpdf_conf.h
  ${CMAKE_CURRENT_BINARY_DIR}/hpdf_config.h
  hpdf_consts.h
  hpdf_destination.h
  hpdf_doc.h
  hpdf_encoder.h
  hpdf_encrypt.h
  hpdf_encryptdict.h
  hpdf_error.h
  hpdf_ext_gstate.h
  hpdf_font.h
  hpdf_fontdef.h
  hpdf_gstate.h
  hpdf_image.h
  hpdf_info.h
  hpdf_list.h
  hpdf_mmgr.h
  hpdf_objects.h
  hpdf_outline.h
  hpdf_page_label.h
  hpdf_pages.h
  hpdf_streams.h
  hpdf_types.h
  hpdf_u3d.h
  hpdf_utils.h
  hpdf_version.h
  )

install(FILES ${include_INSTALLED_HEADERS} DESTINATION include/hpdf)
