CFLAGS=-std=c99

.PHONY : all clean

all: ia backtrack

ia: ia.c mapa.o utils.o grafo.o
	$(CC) -lm -g -o $@ $^

clean :
	$(RM) ia *.o

backtrack: backtrack.c
	$(CC) -lm -O2 -march=native -o $@ $^
