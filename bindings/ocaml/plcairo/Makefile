# The target library's name
LIBRARY = plcairo

# Commands to use for ocamlbuild and ocamlfind (in case they are not in $PATH)
OCAMLBUILD = ocamlbuild -tag debug
OCAMLFIND = ocamlfind

# Where ocamlbuild put the build files
BUILD_DIR = _build/

# Default to building bytecoode and native code libraries
all: byte opt

byte:
	$(OCAMLBUILD) $(LIBRARY).cma

opt:
	$(OCAMLBUILD) $(LIBRARY).cmxa

mli:
	$(OCAMLBUILD) $(LIBRARY).inferred.mli

# (Un)Installation using ocamlfind
install:
	$(OCAMLFIND) install $(LIBRARY) \
	    META \
	    $(BUILD_DIR)*plcairo.cmi \
	    $(BUILD_DIR)*plcairo.cma \
	    $(BUILD_DIR)*plcairo.cmxa \
	    $(BUILD_DIR)*plcairo_stubs.so \
	    $(BUILD_DIR)*plcairo_stubs.a \
	    $(BUILD_DIR)*plcairo.a

uninstall:
	$(OCAMLFIND) remove $(LIBRARY)

# Clean up the build process using ocamlbuild
clean:
	$(OCAMLBUILD) -clean

