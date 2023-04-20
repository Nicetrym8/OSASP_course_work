SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic

all: ../../utility_gui_lib.o

../../utility_gui_lib.o: ./* Makefile
	$(CC) $(CFLAGS) utility_gui_lib.c -g -c -o ../../utility_gui_lib.o
	