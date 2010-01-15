# Create a Fortran library called "Hello" which includes the source file
# "hello.f". 
add_library (Hello hello.f)
SET_TARGET_PROPERTIES(Hello
PROPERTIES SOVERSION 0.0 VERSION 0.0.1
)
