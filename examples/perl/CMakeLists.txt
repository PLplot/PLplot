# examples/perl/CMakeLists.txt
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

set(perl_STRING_INDICES
"01"
"02"
"03"
"04"
"05"
"06"
"07"
"08"
"09"
"10"
"11"
"12"
"13"
"14"
"15"
"16"
"17"
"18"
"19"
"20"
"21"
"22"
"23"
"24"
"25"
"26"
)

set(perl_SCRIPTS)
foreach(STRING_INDEX ${perl_STRING_INDICES})
  set(perl_SCRIPTS ${perl_SCRIPTS} x${STRING_INDEX}.pl)
endforeach(STRING_INDEX ${c_STRING_INDICES})

set(PERM_SCRIPTS
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)

# These are standalone scripts which are executable and which
# should be installed in the examples/perl directory.
install(FILES ${perl_SCRIPTS}
DESTINATION ${DATA_DIR}/examples/perl
PERMISSIONS ${PERM_SCRIPTS}
)

set(perl_DATA README.perldemos)

set(PERM_DATA
OWNER_READ
OWNER_WRITE
GROUP_READ
WORLD_READ
)

# These are data files which should be installed in the examples/perl
# directory.
install(FILES ${perl_DATA}
DESTINATION ${DATA_DIR}/examples/perl
PERMISSIONS ${PERM_DATA}
)


