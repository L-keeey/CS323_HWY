CC=gcc
FLEX=flex
BISON=bison
.syntax: syntax.y
	$(BISON) -t -d syntax.y
.lex: lex.l
	$(FLEX) lex.l
project: .lex .syntax
	$(CC) syntax.tab.c -lfl -ly -o a.out
clean:
	@rm -rf bin/
.PHONY: splc