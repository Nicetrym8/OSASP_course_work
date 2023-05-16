#makefile
CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -Werror -std=c11 -O3 -pedantic -lncurses -lform  
SHELL:=bash
SUBDIRS=./src/executor/ ./src/gui ./src/utility
COMPILE_LIST=./build/*.o
PARSER_PATH=./src/parser

.PHONY:clean,pack, $(SUBDIRS)


all: ./build/main ./build/parser 

./build/*.o: $(SUBDIRS)

$(SUBDIRS):
	mkdir -p ./build
	$(MAKE) -C $@
./build/main: ./src/main.c ./src/config.h ./build/*.o  Makefile
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
