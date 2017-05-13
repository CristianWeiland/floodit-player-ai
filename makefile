CFLAGS=-std=c99

.PHONY : all clean

all: ia backtrack

ia: ia.c mapa.o utils.o
	$(CC) -g -o $@ $^

clean :
	$(RM) ia *.o

backtrack: backtrack.c
	$(CC) -O2 -march=native -o $@ $^
