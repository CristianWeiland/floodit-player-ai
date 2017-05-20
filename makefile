CFLAGS=-std=c99

.PHONY : all clean

all: ia backtrack

ia: ia.c mapa.o utils.o grafo.o
	$(CC) -g -o $@ $^ -lm

clean :
	$(RM) ia *.o

backtrack: backtrack.c
	$(CC) -lm -O2 -march=native -o $@ $^
