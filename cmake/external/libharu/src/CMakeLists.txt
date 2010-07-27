# src/CMakeLists.txt for libharu.
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

set(hpdf_LIB_SRCS
  hpdf_annotation.c
  hpdf_array.c
  hpdf_binary.c
  hpdf_boolean.c
  hpdf_catalog.c
  hpdf_destination.c
  hpdf_dict.c
  hpdf_doc.c
  hpdf_doc_png.c
  hpdf_encoder.c
  hpdf_encoder_cns.c
  hpdf_encoder_cnt.c
  hpdf_encoder_jp.c
  hpdf_encoder_kr.c
  hpdf_encrypt.c
  hpdf_encryptdict.c
  hpdf_error.c
  hpdf_ext_gstate.c
  hpdf_font.c
  hpdf_font_cid.c
  hpdf_font_tt.c
  hpdf_font_type1.c
  hpdf_fontdef.c
  hpdf_fontdef_base14.c
  hpdf_fontdef_cid.c
  hpdf_fontdef_cns.c
  hpdf_fontdef_cnt.c
  hpdf_fontdef_jp.c
  hpdf_fontdef_kr.c
  hpdf_fontdef_tt.c
  hpdf_fontdef_type1.c
  hpdf_gstate.c
  hpdf_image.c
  hpdf_image_png.c
  hpdf_info.c
  hpdf_list.c
  hpdf_mmgr.c
  hpdf_name.c
  hpdf_null.c
  hpdf_number.c
  hpdf_objects.c
  hpdf_outline.c
  hpdf_page_label.c
  hpdf_page_operator.c
  hpdf_pages.c
  hpdf_real.c
  hpdf_streams.c
  hpdf_string.c
  hpdf_u3d.c
  hpdf_utils.c
  hpdf_xref.c
  )

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
  ${ZLIB_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIR}
  )

add_library(hpdf ${hpdf_LIB_SRCS})

target_link_libraries(hpdf ${PNG_LIBRARIES})

if(USE_RPATH)
  set_target_properties(
    hpdf
    PROPERTIES 
    SOVERSION ${hpdf_SOVERSION}
    VERSION ${hpdf_VERSION}
    INSTALL_RPATH "${LIB_DIR}"
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
else(USE_RPATH)
  set_target_properties(
    hpdf
    PROPERTIES 
    SOVERSION ${hpdf_SOVERSION}
    VERSION ${hpdf_VERSION}
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
endif(USE_RPATH)

install(TARGETS hpdf
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )
