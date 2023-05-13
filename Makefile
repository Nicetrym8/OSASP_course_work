#makefile
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -O3 -pedantic -lncurses -lform -fsanitize=address,leak,undefined
SHELL:=bash
SUBDIRS=./src/executor/ ./src/gui ./src/utility
COMPILE_LIST=./build/*.o
PARSER_PATH=./src/parser

.PHONY:clean,pack, build_dep $(SUBDIRS)


all: ./build/main ./build/parser 

build_dep: $(SUBDIRS)

$(SUBDIRS):
	mkdir -p ./build
	$(MAKE) -C $@
./build/main: ./src/main.c $(SUBDIRS)  Makefile
	$(CC) $(CFLAGS) $(COMPILE_LIST) ./src/main.c  -o ./build/main
./build/parser: $(PARSER_PATH)/parser.l
	cd $(PARSER_PATH) && $(MAKE)
	$(CC) $(PARSER_PATH)/lex.yy.c -lfl -o ./build/parser
clean:
	 cd $(PARSER_PATH) && $(MAKE) clean
	 -cd ./build && rm -f *
pack: clean
	-rmdir -p build
	pwd | xargs basename | xargs -I{} tar -czvf ../Суязов\ Г.А.\ {}.tar.gz ../{} --transform 's,^,Суязов Г.А./,'
