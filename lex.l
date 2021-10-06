%{
    #define EXIT_OK 0
    #define EXIT_FAIL 1
    #include "stdlib.h"
    #include "syntax.tab.h"

    // global variables
    // int identifiers = 0;
    // int lines = 0;

    int yycolno = 1;
    #define YY_USER_ACTION 
        yylloc.first_line = yylineno; 
        yylloc.first_column = yycolno; 
        yylloc.last_line = yylineno; 
        yylloc.last_column = yycolno + yyleng; 
        yycolno += yyleng;
%}
%option yylineno
LETTER   [A-Za-z]
DIGIT    [0-9]
INT       0|[1-9][0-9]*
/* FLOAT    -?(3\.[0-4]|[0-2]\.[0-9])(E[+-]?(3[0-8]|[1-2]?[0-9]))? */
frac    \.[0-9]+
exp     [Ee][+-]?[0-9]+
FLOAT   -?{INT}{frac}{exp}?
CHAR     \'((\\[0-7][0-9A-Fa-f])|[ !\"#$%&\'\(\)\*\+,-./0-9:;<=>\?@A-Z\[\\\]\^_`a-z\{\}\|~])\'
ID       ({LETTER}|_)({LETTER}|{DIGIT}|_)*
TYPE     (int)|(float)|(char)
/* STRUCT   struct */
WS       [ \r\t]
EXP      \([^\)]*\)
STRING   \"[^\"]*\"
%%
"\n" {yycolno = 0;}
{WS} {}
{TYPE} {
    yylval.string_value = yytext;
    return TYPE;
}
struct {return STRUCT;}
if {return IF;}
else {return ELSE;}
while {return WHILE;}
return {return RETURN;}
"." {return DOT;}
";" {return SEMI;}
"," {return COMMA;}
"=" {return ASSIGN;}
"<" {return LT;}
"<=" {return LE;}
">" {return GT;}
">=" {return GE;}
"!=" {return NE;}
"==" {return EQ;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return MUL;}
"/" {return DIV;}
"&&" {return AND;}
"||" {return OR;}
"!" {return NOT;}
"(" {return LP;}
")" {return RP;}
"[" {return LB;}
"]" {return RB;}
"{" {return LC;}
"}" {return RC;}

{ID} {
    yylval.string_value = yytext;
    return ID;
} 
{INT} {
    int res = atoi(yytext);
    int len = yyleng;
    if (res == 0 && len > 1) {
        /* return("Error type A at Line %d: Unknown characters \'%s\' \n", yylineno, yytext); */
    } else {
        yylval.int_value = res;
        return INT;
    }
}
{FLOAT} {
    float res = atof(yytext);
    int len = yyleng;
    if (res == 0.0 && len > 1) {
        /* return("Error type A at Line %d: Unknown characters \'%s\' \n", yylineno, yytext); */
    } else {
        yylval.float_value = res;
        return FLOAT;
    }
}
{CHAR} {
    if (yyleng == 3) {
        yylval.string_value = yytext;
        return CHAR;
    } else {
        /* hex type char */   
    }
}
. { return("Error type A at Line %d: Unknown characters \'%s\' \n", yylineno, yytext); }
<<EOF>> { /* return("There are %d occurrences of valid identifiers\n", identifiers); */ 
        yyterminate(); }

%%
// int main(int argc, char **argv){
//     char *file_path;
//     if(argc < 2){
//         freturn(stderr, "Usage: %s <file_path>\n", argv[0]);
//         return EXIT_FAIL;
//     } else if(argc == 2){
//         file_path = argv[1];
//         if(!(yyin = fopen(file_path, "r"))){
//             perror(argv[1]);
//             return EXIT_FAIL;
//         }
//         yylex();
//         return EXIT_OK;
//     } else{
//         fputs("Too many arguments! Expected: 2.\n", stderr);
//         return EXIT_FAIL;
//     }
// }
