CC = gcc
CFLAGS = -Wall -pedantic -std=c99 #-O03

all: clean1 loop clean2

loop: logger.o loop.o argparser.o token.o lexer.o parser.o environment.o interpreter.o predecoder.o
	$(CC) $(CFLAGS) -o loop loop.o logger.o argparser.o token.o lexer.o parser.o environment.o interpreter.o predecoder.o -lm

argparser.o: src/argparser.c
	$(CC) $(CFLAGS) -c src/argparser.c

logger.o: src/util/logger.c
	$(CC) $(CFLAGS) -c src/util/logger.c

token.o: src/token.c
	$(CC) $(CFLAGS) -c src/token.c

loop.o: src/loop.c
	$(CC) $(CFLAGS) -c src/loop.c 

lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c src/lexer.c

parser.o: src/parser.c
	$(CC) $(CFLAGS) -c src/parser.c

environment.o: src/environment.c
	$(CC) $(CFLAGS) -c src/environment.c

interpreter.o: src/interpreter.c
	$(CC) $(CFLAGS) -c src/interpreter.c

predecoder.o: src/predecoder/predecoder.c
	$(CC) $(CFLAGS) -c src/predecoder/predecoder.c

clean: clean1

.PHONY: clean1 clean2
clean1:
	rm -f *.o

clean2:
	rm -f *.o