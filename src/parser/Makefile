SHELL:=bash
.PHONY: clean

all: lex.yy.c

lex.yy.c: parser.l
	flex parser.l
clean:
	-rm -f lex.yy.c