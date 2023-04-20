#makefile
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic -lncurses -lform #-fsanitize=address,leak,undefined
SHELL:=bash
SUBDIRS=./src/executor/ ./src/gui ./src/utility
COMPILE_LIST=./*.o
PARSER_PATH=./src/parser/

.PHONY:clean,pack, build_dep $(SUBDIRS)


all: main parser 

build_dep: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
main: ./src/main.c $(SUBDIRS)  Makefile
	$(CC) $(CFLAGS) $(COMPILE_LIST) ./src/main.c  -o main  

parser: $(PARSER_PATH)/parser.l
	cd $(PARSER_PATH) && $(MAKE)
	$(CC) $(PARSER_PATH)/lex.yy.c -lfl -o parser
clean:
	 cd $(PARSER_PATH) && $(MAKE) clean
	-rm -f main parser *.o 
pack: clean
	pwd | xargs basename | xargs -I{} tar -czvf ../Суязов\ Г.А.\ {}.tar.gz ../{} --transform 's,^,Суязов Г.А./,'
