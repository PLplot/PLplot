# include/CMakeLists.txt for libharu.
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008  Werner Smekal
# Copyright (C) 2010  Alan W. Irwin
#
# This software is provided 'as-is', without any express or implied warranty.
#
# In no event will the authors be held liable for any damages arising from the
# use of this software.
# 
# Permission is granted to anyone to use this software for any purpose,including
# commercial applications, and to alter it and redistribute it freely, subject
# to the following restrictions:
# 
#  1. The origin of this software must not be misrepresented; you must not claim
#     that you wrote the original software. If you use this software in a
#     product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution. 
#

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
