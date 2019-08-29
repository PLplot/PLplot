// Code for D binding of hello_world_c library that is written in C.

module hello_world;

private import std.conv;

// Get the string produced by the hello_world C routine in the hello_world_c library.
void get_hello_world_string( out string d_string )
{
    char[1024] cstring;
    hello_world( cstring.ptr );
    d_string = to!string( cstring.ptr );
}

// This means all prototypes from now on are C.
extern ( C ) :

void hello_world(char *cstring);
