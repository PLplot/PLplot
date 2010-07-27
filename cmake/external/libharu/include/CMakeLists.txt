# include/CMakeLists.txt for libharu.
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2010 Alan W. Irwin
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

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

install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${INCLUDE_DIR})
