SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -Werror -std=c11 -O3 -pedantic -lncurses -lform -c
OUTFILES= 

all: ../../build/about.o ../../build/main_screen.o ../../build/settings.o

../../build/about.o: about.c ../config.h about.h Makefile
	$(CC) $(CFLAGS) about.c  -o ../../build/about.o
../../build/main_screen.o: main_screen.c ../config.h main_screen.h Makefile
	$(CC) $(CFLAGS) main_screen.c  -o ../../build/main_screen.o
../../build/settings.o: settings.c ../config.h settings.h Makefile
	$(CC) $(CFLAGS) settings.c  -o ../../build/settings.o
	