module hello;
import std.stdio;
// This refers to the module name of the D code for the hello_world_d library.
import hello_world;

void main() {
  string hello_string;
  get_hello_world_string(hello_string);
  // writef writes just the formatted string
  writef("%s\n", hello_string);
}
