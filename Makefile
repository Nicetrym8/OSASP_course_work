#makefile
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -lncurses -lform  -fsanitize=address
SHELL:=bash
.PHONY:clean,pack


all: main
main: main.c screen.c config.h find_executor.h  Makefile
	$(CC) $(CFLAGS) main.c screen.c find_executor.c -o main 
test: test.c
	$(CC) $(CFLAGS) test.c -o test 
clean:
	-rm main
	-pwd | xargs basename | xargs -I{} rm ../Суязов Г.А.\ {}.tar.gz 
pack: clean
	chmod 666 ./*
	pwd | xargs basename | xargs -I{} tar -czvf ../Суязов\ Г.А.\ {}.tar.gz ../{} --transform 's,^,Суязов Г.А./,'
