CC=gcc
FLEX=flex
BISON=bison
.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d syntax.y
jp: .lex .syntax
	$(CC) syntax.tab.c -lfl -ly -o jp.out
splc:
	@mkdir bin
	touch bin/splc
	@chmod +x bin/splc

# test1:
# 	./token.out test1.c
# test2:
# 	./token.out test2.c
clean:
	@rm -f lex.yy.c *.out
	@rm -rf bin/
.PHONY: splc