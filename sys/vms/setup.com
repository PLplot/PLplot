$ x = f$env("PROCEDURE")
$ dir = f$parse(x,,,"DEVICE")+f$parse(x,,,"DIRECTORY")
$ dir = dir-"]"+".LIB]"
$ define PLPLOT_LIB 'dir'
