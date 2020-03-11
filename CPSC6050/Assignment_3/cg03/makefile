## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX -std=c++11
X11FLAGS = -L/usr/X11R6/lib -lXi -lXmu
LFLAGS = -lglut -L/usr/lib -lGL -lGLU -lm

TARGETS = $(PROGFILES:.cpp=)

PROGFILES = \
        main.cpp \
        $(NULL)

targets default: $(TARGETS)

$(PROGFILES:.cpp=): main.o 
	$(CC) -o cg03 main.o ${LFLAGS}

run: $(TARGETS)
	./cg03

clean:
	rm -rf *.o cg03 *.bak

depend :
	makedepend ${PROGFILES}
# DO NOT DELETE
