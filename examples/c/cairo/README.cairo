
Some examples demonstrating how to use the PLplot and the Cairo driver to draw
into an externally supplied XDrawable or Cairo context.

EXTERNAL X DRAWABLE EXAMPLES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The external X drawable example is extXdrawable_demo.c.  They are not part
of the general PLplot testbed of examples, but are instead designed to
demonstrate the use of the ability of plplot's xcairo driver to use an X
drawable supplied by an application.  The example itself uses GTK but the
concept applies to any graphical framework, or even programs using libX11
directly.


EXTERNAL CAIRO CONTEXT EXAMPLES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The C program ext-cairo-test.c demonstrates how to pass in an external
Cairo Context that PLplot can use for plotting. On unix like systems you
should be able to compile this as follows:

gcc ext-cairo-test.c -o ext-cairo-test `pkg-config --cflags --libs plplotd cairo`

