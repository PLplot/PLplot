include $(TOPDIR)/Makefile.glob

SRC = x01c.c  x03c.c  x05c.c  x07c.c  x09c.c  x11c.c  x13c.c  \
	x15c.c x17c.c  x19c.c x02c.c  x04c.c  x06c.c  x08c.c  x10c.c \
	 x12c.c  x14c.c  x16c.c  x18c.c

OBJ = x01c  x03c  x05c  x07c  x09c  x11c  x13c  \
	x15c x17c  x19c x02c  x04c  x06c  x08c  x10c \
	 x12c  x14c  x16c  x18c

CFLAGS += -DDOUBLE -I. -I/usr/X11R6/include -I/usr/local/include
LOADLIBES += -L/usr/local/lib -L/usr/X11R6/lib -lplplotdtk -lMatrix \
	$(TK_LIB) -ltcl8.0 -lX11 $(SOCKET_LIB) -lm

all:: $(OBJ)

$(OBJ): $(SRC)
	$(C) $(CFLAGS) $(LOADLIBES) $< -o $@  

clean::
	-$(RM) $(OBJ) *.o core

