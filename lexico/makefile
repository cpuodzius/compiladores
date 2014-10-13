CC?=gcc
CFLAGS=-std=c99 -Wall -pedantic -I include

lexico: src/lexico.c include/state_machine.h
	$(CC) src/$@.c -g -o bin/$@ $(CFLAGS)

all: lexico

clean:
	rm bin/* script/*.pyc	
