## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX
X11FLAGS = -L/usr/X11R6/lib -lXi -lXmu
LFLAGS = -lglut -L/usr/lib -lGL -lGLU -lm

TARGETS = $(PROGFILES:.cpp=)

PROGFILES = \
        main.cpp \
        stub.cpp \
        point3d.cpp \
        vector3d.cpp \
        $(NULL)

targets default: $(TARGETS)

$(PROGFILES:.cpp=): main.o stub.o point3d.o vector3d.o
	$(CC) -o cg02 main.o stub.o point3d.o vector3d.o ${LFLAGS}

clean:
	rm -rf *.o cg02 *.bak

depend :
	makedepend ${PROGFILES}
# DO NOT DELETE
