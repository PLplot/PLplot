# Add executable called "helloDemo" that is built from the source file
# "demo.f".
add_executable (helloDemo demo.f) 
 
# Link the executable to the Hello library. 
target_link_libraries (helloDemo Hello)
