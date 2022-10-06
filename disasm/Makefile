.PHONY: all clean cleanlib debug release updlib buildlib cplib

include ../CMakefile

#Compiler flags
CFLAGS=$(CFLAGS_WARNINGS) $(CFLAGS_SYNTAX) -I$(COMMUN_INCLUDE)
LD=ld
CCLDFLAGS=$(CFLAGS_LINK)


LIBDIR=lib
SOURCEDIR=src/code
HEADERDIR=src/include
COMMUN_INCLUDE=../include
BUILDDIR=build
SOURCES=$(wildcard $(SOURCEDIR)/*.c)
HEADFILES=$(wildcard $(HEADERDIR)/*.h)
OBJFILES=$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE=$(BUILDDIR)/disasm


all: $(BUILDDIR) $(EXECUTABLE)


debug: CFLAGS+= $(CFLAGS_FLAGS) $(CFLAGS_DEBUG) 
debug: all

release: CFLAGS+= -D NDEBUG
release: all


$(BUILDDIR):
	mkdir $(BUILDDIR)

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(CCLDFLAGS) $(CFLAGS) -o $@ $(BUILDDIR)/*.o

$(OBJFILES): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADFILES)
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) -o $@ $<


clean:
	rm -rf $(BUILDDIR)

