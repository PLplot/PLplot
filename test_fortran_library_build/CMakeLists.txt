# Test Fortran linking of an executable (built in Demo) with a library
# (built in Hello).
cmake_minimum_required (VERSION 2.6) 
project (HELLO Fortran) 

add_subdirectory (Hello) 
add_subdirectory (Demo)
