.PHONY: all clean cleanlib debug release updlib buildlib cplib

include ../CMakefile

#Compiler flags
CFLAGS=$(CFLAGS_WARNINGS) $(CFLAGS_SYNTAX) -I$(LIBDIR)/stack/src/include -I$(LIBDIR)/text/src/head
LD=ld
LDFLAGS=-relocatable
CCLDFLAGS=$(CFLAGS_LINK)


MODE=
LIBDIR=lib
SOURCEDIR=src/code
HEADERDIR=src/include
BUILDDIR=build
SOURCES=$(wildcard $(SOURCEDIR)/*.c)
HEADFILES=$(wildcard $(HEADERDIR)/*.h)
OBJFILES=$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE=$(BUILDDIR)/cpuemu


all: $(BUILDDIR) cplib $(EXECUTABLE)


debug: CFLAGS+= $(CFLAGS_FLAGS) $(CFLAGS_DEBUG) 
debug: all
debug: MODE+=debug

release: CFLAGS+= -D NDEBUG -D NDEBUG_LOG -D NDEBUG_HASH -D NDEBUG_CANARY
release: all
release: MODE+=release


updlib:
	mkdir lib
	git clone https://github.com/maxmosk/Lovely-stack.git lib/stack
	git clone https://github.com/maxmosk/Evgeny-Onegin.git lib/text

buildlib: $(LIBDIR)
	cd lib/stack; 		\
		make clean;		\
		make $(MODE)
	cd lib/text; 		\
		make clean;		\
		make $(MODE)

cplib: $(BUILDDIR) buildlib
	cp $(LIBDIR)/*/build/lib*.o $(BUILDDIR)


$(BUILDDIR):
	mkdir $(BUILDDIR)

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(CCLDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)/*.o

$(OBJFILES): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADFILES)
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) -o $@ $<


clean:
	rm -rf $(BUILDDIR)

cleanlib:
	rm -rf $(LIBDIR)

