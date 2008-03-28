Instructions to create the plplot module
========================================

* Download the D compiler (version 1.028) from http://www.digitalmars.com/d/download.html
* Download the digitalmars C/C++ compiler from http://www.digitalmars.com/download/freecompiler.html
* Unzip them and set the PATH variable accordingly
* Download the htod tool from http://www.digitalmars.com/d/2.0/htod.html and put it in the dm/bin directory
* cd into the plplot/include directory
* edit the plplot.h file:
  - remove the first 136 lines
  - add
      #define PLDLLIMPEXP
      #define PLDLLIMPEXP_DATA(type) type
    before the rest of the header file
  - remove the plgfile(FILE **p_file); and plsfile(FILE *file); declarations (around line 1600)
* run "htod plplot.h plplot.d -hc -DDOUBLE"
* remove
    alias DBL_MAX PLFLT_MAX;
    alias DBL_MIN PLFLT_MIN;
  from the plplot.d file (right at the top).
  
Compile examples
================
* Download and install the gdc compiler (only tested with this compiler)
  - Linux: http://dgcc.sourceforge.net/
  - Ubuntu: sudo apt-get install gdc
  - Windows: http://gdcwin.sourceforge.net/ (You need the MinGW compiler 3.4.5 installed as well)
  - Mac OS X: http://gdcmac.sourceforge.net/
* cd into the plplot/examples/d directory
* MinGW: gdc x07d.d -I ..\..\bindings\d path_to_build_directory\dll\libplplotd.dll.a -o x07d
