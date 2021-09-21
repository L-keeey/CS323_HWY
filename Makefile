CC=gcc
FLEX=flex
lex:
	$(FLEX) lex.l
	$(CC) lex.yy.c -lfl -o token.out
splc:
	@mkdir bin
	touch bin/splc
	@chmod +x bin/splc
test1:
	./token.out test1.c
test2:
	./token.out test2.c
clean:
	@rm -f lex.yy.c *.out
	@rm -rf bin/
.PHONY: splc