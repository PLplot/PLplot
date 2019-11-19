# utils/CMakeLists.txt for PLplot
# Copyright (C) 2006-2019 Alan W. Irwin
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

# plrender and pltek both depend on the plplot library.
set(LIB_INSTALL_RPATH ${LIB_DIR})

if(PLD_plm)
  configure_executable_build(plrender "plrender.c" "PLPLOT::plplot" "${LIB_INSTALL_RPATH}" ${BIN_DIR})
endif(PLD_plm)

configure_executable_build(pltek "pltek.c" "PLPLOT::plplot" "${LIB_INSTALL_RPATH}" ${BIN_DIR})

# Build simple executable to check parity bits of all characters read
# by stdin.
add_executable(parity_bit_check parity_bit_check.c)
