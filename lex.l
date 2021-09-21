%{
    #define EXIT_OK 0
    #define EXIT_FAIL 1

    // global variables
    // int identifiers = 0;
    // int lines = 0;
%}
%option yylineno
LETTER   [A-Za-z]
DIGIT    [0-9]
INT      ((0x[0-9a-fA-F]{8})|(-2147483648|-?(214748364[0-7]|21474836[0-3]{DIGIT}|2147483[0-5]{DIGIT}{2}|214748[0-2]{DIGIT}{3}|21474[0-7]{DIGIT}{4}|2147[0-3]{DIGIT}{5}|214[0-6]{DIGIT}{6}|21[0-3]{DIGIT}{7}|20{DIGIT}{8}|1{DIGIT}{9}|[1-9]{DIGIT}{0,8}|0))) 
FLOAT    -?(3\.[0-4]|[0-2]\.[0-9])(E[+-]?(3[0-8]|[1-2]?[0-9]))?
CHAR     \'((\\[0-7][0-9A-Fa-f])|[ !\"#$%&\'\(\)\*\+,-./0-9:;<=>\?@A-Z\[\\\]\^_`a-z\{\}\|~])\'
ID       ({LETTER}|_)({LETTER}|{DIGIT}|_)*
TYPE     int | float | char
STRUCT   struct
WS       [ \r\t\n]
EXP      \([^\)]*\)
STRING   \"[^\"]*\"
%%
{WS} {}
char {printf("TYPE char\n");}
int {printf("TYPE int\n");}
float {printf("TYPE float\n");}
struct {printf("STRUCT\n");}
if {printf("IF\n");}
else {printf("ELSE\n");}
while {printf("WHILE\n");}
return {printf("RETURN\n");}
"." {printf("DOT\n");}
";" {printf("SEMI\n");}
"," {printf("COMMA\n");}
"=" {printf("ASSIGN\n");}
"<" {printf("LT\n");}
"<=" {printf("LE\n");}
">" {printf("GT\n");}
">=" {printf("GE\n");}
"!=" {printf("NE\n");}
"==" {printf("EQ\n");}
"+" {printf("PLU\n");}
"-" {printf("MINUS\n");}
"*" {printf("MUL\n");}
"/" {printf("DIV\n");}
"&&" {printf("AND\n");}
"||" {printf("OR\n");}
"!" {printf("NOT\n");}
"(" {printf("LP\n");}
")" {printf("RP\n");}
"[" {printf("LB\n");}
"]" {printf("RB\n");}
"{" {printf("LC\n");}
"}" {printf("RC\n");}

{ID} {printf("ID %s\n", yytext);} 
{INT} {printf("INT %s\n", yytext);}
{FLOAT} {printf("FLOAT %s\n", yytext);}
{CHAR} {printf("CHAR %s\n", yytext);}
. { printf("Error type A at Line %d: Unknown characters \'%s\' \n", yylineno, yytext); }
<<EOF>> { /* printf("There are %d occurrences of valid identifiers\n", identifiers); */ yyterminate(); }

%%
int main(int argc, char **argv){
    char *file_path;
    if(argc < 2){
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAIL;
    } else if(argc == 2){
        file_path = argv[1];
        if(!(yyin = fopen(file_path, "r"))){
            perror(argv[1]);
            return EXIT_FAIL;
        }
        yylex();
        return EXIT_OK;
    } else{
        fputs("Too many arguments! Expected: 2.\n", stderr);
        return EXIT_FAIL;
    }
}
