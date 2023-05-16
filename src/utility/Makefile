SHELL:=bash
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -Werror -std=c11 -O3 -pedantic -c

all: ../../build/utility_gui_lib.o

../../build/utility_gui_lib.o: ./* ../config.h Makefile
	$(CC) $(CFLAGS) utility_gui_lib.c  -o ../../build/utility_gui_lib.o
	