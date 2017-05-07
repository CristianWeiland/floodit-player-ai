CFLAGS=-std=c99

.PHONY : all clean

all: ia

ia: ia.c utils.o
	$(CC) -g -o $@ $^

clean :
	$(RM) ia *.o
