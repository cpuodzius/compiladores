CC?=gcc
CFLAGS=-std=c99 -Wall -pedantic -I include
COMPILADOR_OBJS=bin/util.o bin/lexico.o bin/sintatico.o

all: compilador

util:
	$(CC) src/$@.c -c -g -o bin/$@.o $(CFLAGS)

lexico: src/lexico.c include/state_machine.h
	$(CC) src/$@.c -c -g -o bin/$@.o -DPALAVRAS_RESERVADAS $(CFLAGS)

sintatico: src/sintatico.c
	make util
	$(CC) src/$@.c -c -g -o bin/$@.o $(CFLAGS)

compilador: src/compilador.c
	make lexico
	make sintatico
	$(CC) src/$@.c -g -o bin/$@ $(COMPILADOR_OBJS) $(CFLAGS)

clean:
	rm bin/* script/*.pyc	
