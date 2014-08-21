# src_executable/CMakeLists.txt

include_directories(
${CMAKE_SOURCE_DIR}/src_lib
${CMAKE_SOURCE_DIR}/src_executable
)

# Note due to limitations of the Ada language support for CMake, the the
# nominal source file for add_executable is compiled (which is why a
# real file has to be specified), but otherwise it is ignored except for
# its .adb suffix which identifies the Ada language.  The actual source
# file name for the internal gnatmake command that creates the
# executable is constructed from <TARGET>.adb.  Since no directory can
# be specified from this construction (don't ask), you must specify the
# directory of <TARGET>.adb with a -aI option (see below).
add_executable(hello hello.adb)
  
# The first -aI option is required to find <TARGET>.adb (see above
# comment). The second -aI option gives access to the libplplotada spec
# files. The -aL option gives access to the libplplotada *.ali files.
# (That location currently works but is probably not guaranteed
# indefinitely for future versions of CMake.)
 
set_target_properties(
hello
PROPERTIES
LINK_FLAGS
"-aI${CMAKE_SOURCE_DIR}/src_executable -aI${CMAKE_SOURCE_DIR}/src_lib -aL${CMAKE_BINARY_DIR}/src_lib/CMakeFiles/hello_1.dir"
)
target_link_libraries(hello hello_1)

# Comment this out in anticipation another solution will be found for
# the "make clean" problem
#set_directory_properties(
#PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
#"${CMAKE_CURRENT_BINARY_DIR}/hello.o;${CMAKE_CURRENT_BINARY_DIR}/hello.ali"
#)
