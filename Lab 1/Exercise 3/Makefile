CC=gcc
CFLAGS=-Wall -g -O3
INCLUDES=state.h fringe.h searchTree.h

OBJECTS=search.o fringe.o searchTree.c

all: ${OBJECTS} ${INCLUDES}
	gcc -o search ${OBJECTS}

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
