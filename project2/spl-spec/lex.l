%{
    #define EXIT_OK 0
    #define EXIT_FAIL 1

    #include "syntax.tab.h"
    #include "stdlib.h"
    #include "parseTree.h"


    /* library inclusions */
    int yycolno = 1;
    int error_num;
    #define YY_USER_ACTION yylloc.first_line = yylineno; \
        yylloc.first_column = yycolno; \
        yylloc.last_line = yylineno; \
        yylloc.last_column = yycolno + yyleng; \
        yycolno += yyleng;
    

%}

%option yylineno
INT      (0|[1-9][0-9]*)
HEX      0[xX](0|[1-9a-fA-F][0-9a-fA-F]*)
FLOAT    -?((0|[1-9][0-9]*)\.[0-9]+)
CHAR     \'(([!\"#$%&\'\(\)\*\+,-./0-9:;<=>\?@A-Z\[\\\]\^_`a-z\{\}\|~])|(\\x[0-7][0-9A-Fa-f]))\'
ERROR_CHAR \'.*\'
ID       ([a-zA-Z_])([0-9a-zA-Z_])*
TYPE     int|float|char
STRUCT   struct
WS       [ \r\t]*


%%
"//"[^\n]* {}
\/\*([^*"]*|".*"|\*+[^/]|"\n")*\*\/ {} 

\n    {yycolno = 1;}
{WS}    {}
"int"   {
            yylval.Node_value = new_Node_s("TYPE", "int");
            return TYPE;
        }
"float"   {
            yylval.Node_value = new_Node_s("TYPE", "float");
            return TYPE;
        }
"char"   {
            yylval.Node_value = new_Node_s("TYPE", "char");
            return TYPE;
        }
struct  {   
            yylval.Node_value = new_Node("STRUCT");
            return STRUCT;
        }
if      {
            yylval.Node_value = new_Node("IF");
            return IF;
        }
else    {
            yylval.Node_value = new_Node("ELSE");
            return ELSE;
        }
while   {
            yylval.Node_value = new_Node("WHILE");
            return WHILE;
        }
return  {
            yylval.Node_value = new_Node("RETURN");
            return RETURN;
        }
"."     {
            yylval.Node_value = new_Node("DOT");
            return DOT;
        }
";"     {
            yylval.Node_value = new_Node("SEMI");
            return SEMI;
        }
","     {
            yylval.Node_value = new_Node("COMMA");
            return COMMA;}
"="     {
            yylval.Node_value = new_Node("ASSIGN");
            return ASSIGN;
        }
"<"     {
            yylval.Node_value = new_Node("LT");
            return LT;
        }
"<="    {
            yylval.Node_value = new_Node("LE");
            return LE;
        }
">"     {
            yylval.Node_value = new_Node("GT");
            return GT;
        }
">="    {
            yylval.Node_value = new_Node("GE");
            return GE;
        }
"!="    {
            yylval.Node_value = new_Node("NE");
            return NE;           
        }
"=="    {
            yylval.Node_value = new_Node("EQ");
            return EQ;
        }
"+"     {
            yylval.Node_value = new_Node("PLUS");
            return PLUS;
        }
"-"     {
            yylval.Node_value = new_Node("MINUS");
            return MINUS;
        }
"*"     {
            yylval.Node_value = new_Node("MUL");
            return MUL;
        }
"/"     {
            yylval.Node_value = new_Node("DIV");
            return DIV;
        }
"&&"    {
            yylval.Node_value = new_Node("AND");
            return AND;
        }
"||"    {
            yylval.Node_value = new_Node("OR");
            return OR;
        }
"!"     {
            yylval.Node_value = new_Node("NOT");
            return NOT;
        }
"("     {
            yylval.Node_value = new_Node("LP");
            return LP;
        }
")"     {
            yylval.Node_value = new_Node("RP");
            return RP;
        }
"["     {
            yylval.Node_value = new_Node("LB");
            return LB;
        }
"]"     {
            yylval.Node_value = new_Node("RB");
            return RB;
        }
"{"     {
            yylval.Node_value = new_Node("LC");
            return LC;
        }
"}"     {
            yylval.Node_value = new_Node("RC");
            return RC;
        }

{ID}    {
            yylval.Node_value = new_Node_s("ID", yytext);
            return ID;
        }

{HEX}   {
            char* rubbish[10];
            int res = (int) strtol(yytext,rubbish, 16);
            if (res == 0 && yyleng > 1){
                printf("Error type A at Line %d: Value out of bounds \'%s\' \n", yylineno, yytext);
            } else {
                yylval.Node_value = new_Node_h("INT", res);
                return INT;
            }
        }

{INT}   {
            int res = atoi(yytext);
            if (res == 0 && yyleng > 1){
                printf("Error type A at Line %d: Value out of bounds \'%s\' \n", yylineno, yytext);
            } else {
                yylval.Node_value = new_Node_i("INT", res, yytext);
                return INT;
            }
        } 

{FLOAT} {
            float res = atof(yytext);
            if (res == 0.0 && yyleng > 3){
                printf("Error type A at Line %d: Value out of bounds \'%s\' \n", yylineno, yytext);
            } else {
                yylval.Node_value = new_Node_f("FLOAT", res, yytext);
                return FLOAT;
            }
        }
{CHAR}  {
            yylval.Node_value = new_Node_s("CHAR", yytext);
            return CHAR;
        }
[0-9]+{ID} {
    printf("Error type A at Line %d: unknown lexeme %s \n", yylineno, yytext); 
    error_num += 1;
    return ERROR;
}

{ERROR_CHAR} {
    printf("Error type A at Line %d: Invalid char %s \n", yylineno, yytext); 
    error_num += 1;
    return ERROR;
}

. { printf("Error type A at Line %d: unknown lexeme %s \n", yylineno, yytext); 
error_num += 1;
    return ERROR;
}
<<EOF>> { /* printf("There are %d occurrences of valid identifiers\n", identifiers); */ yyterminate(); }

