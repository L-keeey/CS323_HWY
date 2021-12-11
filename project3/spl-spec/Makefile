CC=gcc
FLEX=flex
BISON=bison
CPP=g++

.comp:
	$(CPP) -c symbolTable.cpp -o symbolTable.o
	$(CPP) main.cpp -lfl -o splc
.bison:
	$(BISON) -t -d -v syntax.y

.lex:
	$(FLEX) lex.l
	
splc: .lex .bison  .comp
	@mkdir bin
	@mv splc bin/
	@chmod +x bin/splc
clean:
	@rm -f lex.yy.c *.out
	@rm -f syntax.tab.*
	@rm -rf bin/
	@rm -f a.out
.PHONY: splc