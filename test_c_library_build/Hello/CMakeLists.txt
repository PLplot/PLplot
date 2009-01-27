# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 
add_library (Hello SHARED hello.c)
SET_TARGET_PROPERTIES(Hello
PROPERTIES SOVERSION 0.0 VERSION 0.0.1
)
