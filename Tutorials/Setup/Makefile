## Compiler
CC=g++
## Flags
CPPFLAGS = -Wall -g -DLINUX -std=c++11
LIBFLAGS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lGLEW


main.out: main.o
	$(CC) -o main.out main.o ${LIBFLAGS}

main.o: main.cpp
	$(CC) -c main.cpp $(CPPFLAGS)

run: main.out
	./main.out

clean:
	rm -rf *.out *.o
