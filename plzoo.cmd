@rem #!/bin/sh
@rem #
@rem # File: plzoo
@rem # Usage: plzoo
@rem # 
@rem # Builds zoo archive of plplot files for distribution.
@rem # No binary files should be included.
@
@rem # You'll need GNU find, or workalike, to do this...
@
@rem # Build archive from parent directory.  It must also be unpacked from there.
@
cd ..
@
@rem # top level text files & scripts
@
zoo a plplot plplot/README
zoo a plplot plplot/README.1st
zoo a plplot plplot/COPYRIGHTS
zoo a plplot plplot/Changes.log
zoo a plplot plplot/ToDo
zoo a plplot plplot/pltar*
zoo a plplot plplot/plzoo*
zoo a plplot plplot/plzip*
@
@rem # Make sure zoo creates these subdirectories to make installation easier.
@
zoo a plplot plplot/tmp/.dummy
@
@rem # Font files
@
zoo a plplot plplot/lib/*.fnt
@
@rem # Source code & docs.
@
find plplot/doc      -print | zoo aI plplot
find plplot/drivers  -print | zoo aI plplot
find plplot/examples -print | zoo aI plplot
find plplot/fonts    -print | zoo aI plplot
find plplot/include  -print | zoo aI plplot
find plplot/new      -print | zoo aI plplot
find plplot/utils    -print | zoo aI plplot
find plplot/scripts  -print | zoo aI plplot
find plplot/src      -print | zoo aI plplot
find plplot/sys      -print | zoo aI plplot
