@rem #!/bin/sh
@rem #
@rem # File: pltar
@rem # Usage: pltar
@rem # 
@rem # Builds tar archive of plplot files for distribution.
@rem # No binary files should be included.
@
@rem # Build archive from parent directory.  It must also be unpacked from there.
@
cd ..
@
@rem # top level text files & scripts
@
tar cf plplot.tar plplot/README
tar rf plplot.tar plplot/README.1st
tar rf plplot.tar plplot/COPYRIGHTS
tar rf plplot.tar plplot/Installation
tar rf plplot.tar plplot/Changes
tar rf plplot.tar plplot/pltar*
tar rf plplot.tar plplot/plzoo*
@
@rem # Make sure tar creates these subdirectories to make installation easier.
@
tar rf plplot.tar plplot/tmp/.dummy
tar rf plplot.tar plplot/lib/.dummy
@
@rem # Source code & docs.
@
tar rf plplot.tar plplot/doc 
tar rf plplot.tar plplot/drivers
tar rf plplot.tar plplot/examples
tar rf plplot.tar plplot/fonts
tar rf plplot.tar plplot/include
tar rf plplot.tar plplot/new
tar rf plplot.tar plplot/utils
tar rf plplot.tar plplot/src
tar rf plplot.tar plplot/sys
