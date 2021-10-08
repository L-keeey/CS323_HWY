%{
    #define EXIT_OK 0
    #define EXIT_FAIL 1

    #include "syntax.tab.h"
    #include "stdlib.h"

    /* library inclusions */
    int yycolno = 1;
    #define YY_USER_ACTION
        yylloc.first_line = yylineno; 
        yylloc.first_column = yycolno; 
        yylloc.last_line = yylineno; 
        yylloc.last_column = yycolno + yyleng; 
        yycolno += yyleng;

%}
%option yylineno

INT      (0|[1-9][0-9]*) | 0[xX](0|[1-9a-fA-F][0-9a-fA-F]{0,7})
FLOAT    -?((0|[1-9][0-9]*)\.[0-9]+)
CHAR     \'(([ !\"#$%&\'\(\)\*\+,-./0-9:;<=>\?@A-Z\[\\\]\^_`a-z\{\}\|~]) | (\\x[0-7][0-9A-Fa-f]))\'
ID       ([a-zA-Z_])([0-9a-zA-Z_])*
TYPE     int | float | char
STRUCT   struct
WS       [ \r\t]

%%
"\n"    {yycolno = 1;}
{WS}    {}
{TYPE}  {
         yylval.string_value = yytext;
         return TYPE;
        }
struct  {return STRUCT;}
if      {return IF;}
else    {return ELSE;}
while   {return WHILE;}
return  {return RETURN;}
"."     {return DOT;}
";"     {return SEMI;}
","     {return COMMA;}
"="     {return ASSIGN;}
"<"     {return LT;}
"<="    {return LE;}
">"     {return GT;}
">="    {return GE;}
"!="    {return NE;}
"=="    {return EQ;}
"+"     {return PLU;}
"-"     {return MINUS;}
"*"     {return MUL;}
"/"     {return DIV;}
"&&"    {return AND;}
"||"    {return OR;}
"!"     {return NOT;}
"("     {return LP;}
")"     {return RP;}
"["     {return LB;}
"]"     {return RB;}
"{"     {return LC;}
"}"     {return RC;}

{ID}    {
            int res = atoi(yytext);
            if (res == 0 && yyleng >1){
                printf("Error type A at Line %d: Value out of bounds \'%s\' \n", yylineno, yytext);
            } else {
                yylval.int_value = res;
                return INT;
            }
        } 

{FLOAT} {
            float res = atof(yytext);
            if (res == 0.0 && yyleng >1){
                printf("Error type A at Line %d: Value out of bounds \'%s\' \n", yylineno, yytext);
            } else {
                yylval.float_value = res;
                return FLOAT;
            }
        }
{CHAR}  {
            yylval.string_value = yyleng > 3 ? (char) (yytext) : text;
            return CHAR;
        }

. { printf("Error type A at Line %d: Unknown characters \'%s\' \n", yylineno, yytext); }
<<EOF>> { /* printf("There are %d occurrences of valid identifiers\n", identifiers); */ yyterminate(); }

