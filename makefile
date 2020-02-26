CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -O03

all: clean1 loop clean2

loop: logger.o loop.o argparser.o token.o lexer.o
	$(CC) $(CFLAGS) -o loop loop.o logger.o argparser.o token.o lexer.o -lm

argparser.o: src/util/argparser.c
	$(CC) $(CFLAGS) -c src/util/argparser.c

logger.o: src/util/logger.c
	$(CC) $(CFLAGS) -c src/util/logger.c

token.o: src/token.c
	$(CC) $(CFLAGS) -c src/token.c

loop.o: src/loop.c
	$(CC) $(CFLAGS) -c src/loop.c 

lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c src/lexer.c

clean: clean1

.PHONY: clean1 clean2
clean1:
	rm -f *.o

clean2:
	rm -f *.o