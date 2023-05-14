SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic -c

all: ../../build/executor.o

../../build/executor.o: executor.c executor.h Makefile
	$(CC) $(CFLAGS) executor.c  -o ../../build/executor.o
	