# src/CMakeLists.txt
#
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
# create library
# =======================================================================
# libharu source files
# =======================================================================
set(
  libharu_SRCS
  hpdf_annotation.c
  hpdf_array.c
  hpdf_binary.c
  hpdf_boolean.c
  hpdf_catalog.c
  hpdf_destination.c
  hpdf_dict.c
  hpdf_doc_png.c
  hpdf_doc.c
  hpdf_encoder_cns.c
  hpdf_encoder_cnt.c
  hpdf_encoder_jp.c
  hpdf_encoder_kr.c
  hpdf_encoder.c
  hpdf_encrypt.c
  hpdf_encryptdict.c
  hpdf_error.c
  hpdf_ext_gstate.c
  hpdf_font_cid.c
  hpdf_font_tt.c
  hpdf_font_type1.c
  hpdf_font.c
  hpdf_fontdef_base14.c
  hpdf_fontdef_cid.c
  hpdf_fontdef_cns.c
  hpdf_fontdef_cnt.c
  hpdf_fontdef_jp.c
  hpdf_fontdef_kr.c
  hpdf_fontdef_tt.c
  hpdf_fontdef_type1.c
  hpdf_fontdef.c
  hpdf_gstate.c
  hpdf_image_png.c
  hpdf_image.c
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
option(POST_2.1.0 "Version after 2.1.0?" ON)
# Source code has one additional file in git version after 2.1.0 was released.
if(POST_2.1.0)
  list(APPEND libharu_SRCS
    hpdf_namedict.c
    )
endif(POST_2.1.0)

# =======================================================================
# create static and shared haru library
# =======================================================================
if(LIBHARU_STATIC)
  add_library(hpdf_static STATIC ${libharu_SRCS})
  target_link_libraries(hpdf_static ${ADDITIONAL_LIBRARIES})
  set_target_properties(
    hpdf_static
    PROPERTIES
    OUTPUT_NAME hpdf
    )
  install(
    TARGETS hpdf_static
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )
  if(WIN32 AND NOT CYGWIN)
    foreach(addlib ${ADDITIONAL_LIBRARIES})
      install(FILES ${addlib} DESTINATION lib)
    endforeach(addlib)
  endif(WIN32 AND NOT CYGWIN)
endif(LIBHARU_STATIC)
if(LIBHARU_SHARED)
  add_library(hpdf SHARED ${libharu_SRCS})
  target_link_libraries(hpdf ${ADDITIONAL_LIBRARIES})
  if(USE_RPATH)
    set_target_properties(
      hpdf
      PROPERTIES 
      SOVERSION ${hpdf_SOVERSION}
      VERSION ${hpdf_VERSION}
      INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
      )
  else(USE_RPATH)
    set_target_properties(
      hpdf
      PROPERTIES
      SOVERSION ${hpdf_SOVERSION}
      VERSION ${hpdf_VERSION}
      INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
      )
  endif(USE_RPATH)

  install(
    TARGETS hpdf
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )
endif(LIBHARU_SHARED)
