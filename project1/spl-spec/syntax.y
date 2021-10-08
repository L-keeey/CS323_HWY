%{
    #include "lex.yy.c"
    void yyerror(const char*);
%}

%union{
    int int_value;
    float float_value;
    char *string_value;
}
%token STRUCT IF ELSE WHILE RETURN  SEMI COMMA LC RC
%token <int_value> INT
%token <string_value> TYPE
%token <string_value> ID
%token <float_value> FLOAT
%token <string_value> CHAR

%right ASSIGN
%nonassoc OR
%nonassoc AND
%left LT LE GT GE NE EQ 
%left PLUS MINUS 
%left MUL DIV
%right NOT 
%left LP RP LB RB DOT
%%

/* high-level definition */
Program: ExtDefList
    ;
ExtDefList: /* to allow empty input */
    | ExtDef ExtDefList
    ;
ExtDef: Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList: VarDec
    | VarDec COMMA ExtDecList
    ;

/* specifier */
Specifier: TYPE
    | StructSpecifier
    ;
StructSpecifier: STRUCT ID LC DefList RC
    | STRUCT ID
    ;
    
/* declarator */
VarDec: ID
    | VarDec LB INT RB
    ;
FunDec: ID LP VarList RP
    | ID LP RP
    ;
VarList: ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec: Specifier VarDec
    ;

/* statement */
CompSt: LC DefList StmtList RC
    ;
StmtList: /* to allow empty input */
    : Stmt StmtList
    ;
Stmt: Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

/* local definition */
DefList: /* to allow empty input */ 
    :Def DefList
    ;
Def: Specifier DecList SEMI
    ;
DecList: Dec
    | Dec COMMA DecList
    ;
Dec: VarDec
    | VarDec ASSIGN Exp
    ;

/* Expression */
Exp: Exp ASSIGN Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp LT Exp
    | Exp LE Exp
    | Exp GT Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp EQ Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp MUL Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp %prec NOT
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    | CHAR
    ;
Args: Exp COMMA Args
    | Exp
    ;
%%



void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
int main() {
    yyparse();
}

