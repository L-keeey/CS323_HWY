CC=gcc
FLEX=flex
BISON=bison

.bison:
	$(BISON) -t -d -v syntax.y

.lex:
	$(FLEX) lex.l
	$(CC) syntax.tab.c -lfl -ly -o splc
splc: .bison .lex
	@mkdir bin
	@mv splc bin/
	@chmod +x bin/splc
# test1:
# 	./token.out test1.c
# test2:
# 	./token.out test2.c
clean:
	@rm -f lex.yy.c *.out
	@rm -f syntax.tab.*
	@rm -rf bin/
	@rm -f a.out
.PHONY: splc