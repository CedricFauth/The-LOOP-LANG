CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -O01

all: clean1 loop clean2

argparser.o: src/util/argparser.c src/util/argparser.h src/util/logger.h
	$(CC) $(CFLAGS) -c src/util/argparser.c

logger.o: src/util/logger.c src/util/logger.h
	$(CC) $(CFLAGS) -c src/util/logger.c

token.o: src/token.c src/token.h
	$(CC) $(CFLAGS) -c src/token.c

loop.o: src/loop.c
	$(CC) $(CFLAGS) -c src/loop.c 

loop: logger.o loop.o argparser.o token.o
	$(CC) $(CFLAGS) -o loop loop.o logger.o argparser.o token.o


clean: clean1

.PHONY: clean1 clean2
clean1:
	rm -f *.o

clean2:
	rm -f *.o