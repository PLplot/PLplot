# data/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

set(data_DATAFILES
plstnd5.fnt
plxtnd5.fnt
cmap0_default.pal
cmap0_alternate.pal
cmap0_white_bg.pal
cmap0_black_on_white.pal
cmap1_default.pal
cmap1_blue_yellow.pal
cmap1_blue_red.pal
cmap1_gray.pal
cmap1_radar.pal
cmap1_highfreq.pal
cmap1_lowfreq.pal
)

if(HAVE_SHAPELIB)
  set(data_DATAFILES
  ${data_DATAFILES}
  cglobe.shx
  cglobe.shp
  globe.shx
  globe.shp
  usa.shx
  usa.shp
  usaglobe.shx
  usaglobe.shp
  )
  set(data_DOCFILES
  README.shapefiles
  )
else(HAVE_SHAPELIB)
  set(data_DATAFILES
  ${data_DATAFILES}
  cglobe.map
  globe.map
  usa.map
  usaglobe.map
  )
endif(HAVE_SHAPELIB)

install(FILES ${data_DATAFILES} DESTINATION ${DATA_DIR})

if(HAVE_SHAPELIB)
  install(FILES ${data_DOCFILES} DESTINATION ${DOC_DIR})
endif(HAVE_SHAPELIB)
