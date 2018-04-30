CC=gcc
CFLAGS=-Wall -pedantic
INCLUDES=heap.h

OBJECTS=idknight.o heap.o

%.o: %.c ${OBJECTS}
	${CC} -c -o $@ $< ${CFLAGS}

make: ${OBJECTS} ${INCLUDES}
	gcc ${CFLAGS} ${OBJECTS} -lm
