## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX -std=c++11
X11FLAGS = -L/usr/X11R6/lib -lXi -lXmu
LFLAGS = -lglut -L/usr/lib -lGL -lGLU -lm

cg04: main.o square.o revolution.o vector3d.o point3d.o
	$(CC) -o cg04 main.o square.o revolution.o vector3d.o point3d.o ${LFLAGS} ${CPPFLAGS}

main.o: main.cpp square.hpp revolution.hpp point3d.hpp vector3d.cpp
	$(CC) -o main.o -c main.cpp ${CPPFLAGS}

square.o: square.cpp square.hpp
	$(CC) -o square.o -c square.cpp ${CPPFLAGS}

revolution.o: revolution.cpp revolution.hpp vector3d.hpp point3d.hpp
	$(CC) -o revolution.o -c revolution.cpp ${CPPFLAGS}

vector3d.o: vector3d.cpp vector3d.hpp
	$(CC) -o vector3d.o -c vector3d.cpp ${CPPFLAGS}

point3d.o: point3d.cpp point3d.hpp vector3d.hpp
	$(CC) -o point3d.o -c point3d.cpp ${CPPFLAGS}

run: cg04
	./cg04

clean:
	rm -rf *.o cg04

