#makefile
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic -lncurses -lform  #-fsanitize=address,leak,undefined
SHELL:=bash
.PHONY:clean,pack


all: main
main: main.c main_screen.c settings.c Makefile
	$(CC) $(CFLAGS) main.c  main_screen.c about.c utility_gui_lib.c settings.c find_executor.c -o main 
test: test.c
	$(CC) $(CFLAGS) test.c -o test 
clean:
	-rm main
	-pwd | xargs basename | xargs -I{} rm ../Суязов Г.А.\ {}.tar.gz 
pack: clean
	chmod 666 ./*
	pwd | xargs basename | xargs -I{} tar -czvf ../Суязов\ Г.А.\ {}.tar.gz ../{} --transform 's,^,Суязов Г.А./,'
