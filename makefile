CFLAGS=-std=c99

.PHONY : all clean

all: ia

ia: ia.c mapa.o utils.o
	$(CC) -g -o $@ $^

clean :
	$(RM) ia *.o
