SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic

all: ../../find_executor.o

../../find_executor.o: find_executor.c find_executor.h Makefile
	$(CC) $(CFLAGS) find_executor.c -g -c -o ../../find_executor.o
	