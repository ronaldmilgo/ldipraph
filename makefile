CC=gcc
CFLAGS=-Wall -pedantic -std=c17 -g3

Paths: paths.o ldigraph.o
	${CC} -o $@ ${CFLAGS} $^

ldigraph.o: ldigraph.h
paths.o: ldigraph.h
