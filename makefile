CFLAGS=-std=c99

.PHONY : all clean

all: ia

ia: ia.c fronteira.o mapa.o utils.o grafo.o
	$(CC) -g -o $@ $^ -lm

clean :
	$(RM) ia *.o
