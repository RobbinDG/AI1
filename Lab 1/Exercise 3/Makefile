CC=gcc
CFLAGS=-Wall -g -O3
INCLUDES=state.h fringe.h

OBJECTS=search.o fringe.o

all: ${OBJECTS} ${INCLUDES}
	gcc -o search ${OBJECTS}

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
