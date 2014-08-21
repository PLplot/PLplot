# src_lib/CMakeLists.txt

set(SOURCE_LIST
hello_1.adb
hello_1.ads
)
  
# These are Ada library information files built by gnat.  I am not 
# sure whether the name suffixes correspond to the *.adb or *.ads files
# above or the union of them.  In any case, if any of the names change
# above, then this list will probably have to be changed as well.)
# N.B. the absolute location prefix of these files may have to be changed
# in the future since this is currently a CMake internal.
set(ALI_PREFIX 
${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/hello.dir
)

# This variable references the Ada library information file for the library.
# This variable is useful for installing the *.ali files (although this simple
# project does not bother with that).

set(ALI_LIST
${ALI_PREFIX}/hello_1.ali
)

# CMake assumes compilation results only in object files being generated.
# However, gnatmake generates both object files and *.ali (Ada library
# information) files so it doesn't intrinsically know how to clean those
# additional *.ali files.
# Here is a workaround for this CMake limitation.

# Add generated .ali files to the list of additional files to be
# removed by make clean
# comment out for now to test whether there is a way to get around this
# current limitation.

#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ALI_LIST}")

add_library(hello_1 ${SOURCE_LIST})

target_link_libraries(hello_1 ${GNAT_LIB})

set_target_properties(hello_1
PROPERTIES
SOVERSION 0
VERSION 0.0
OUTPUT_NAME hello
)
