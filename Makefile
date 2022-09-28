.PHONY: all clean debug release updlib buildlib cplib

include CMakefile

#Compiler flags
CFLAGS=$(CFLAGS_WARNINGS) $(CFLAGS_SYNTAX)
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
#LIBDIRS=$(wildcard $(LIBDIR)/*)
EXECUTABLE=$(BUILDDIR)/cpuemu


all: dir cplib $(EXECUTABLE)


debug: CFLAGS+= $(CFLAGS_FLAGS) $(CFLAGS_DEBUG) 
debug: all
debug: MODE+=debug

release: CFLAGS+= -D NDEBUG -D NDEBUG_LOG -D NDEBUG_HASH -D NDEBUG_CANARY
release: all
release: MODE+=release


updlib:
	mkdir lib
	git clone https://github.com/maxmosk/Lovely-stack.git lib/stack

buildlib: $(LIBDIR)
	cd lib/stack; \
		make $(MODE)

cplib:
	cp $(LIBDIR)/*/build/lib*.o build


dir:
	mkdir $(BUILDDIR)

$(EXECUTABLE): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $?

$(OBJFILES): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADFILES)
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) -o $@ $<


clean:
	rm -rf $(BUILDDIR) $(LIBDIR)

