SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic -lncurses -lform
OUTFILES= 

all: ../../about.o ../../main_screen.o ../../settings.o

../../about.o: about.c about.h Makefile
	$(CC) $(CFLAGS) about.c -g -c -o ../../about.o
../../main_screen.o: main_screen.c main_screen.h Makefile
	$(CC) $(CFLAGS) main_screen.c -g -c -o ../../main_screen.o
../../settings.o: settings.c settings.h Makefile
	$(CC) $(CFLAGS) settings.c -g -c -o ../../settings.o
	